/*
 * Copyright (c) 2005-2007, Kohsuke Ohtani
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * vfs_mount.c - mount operations
 */

#include <sys/stat.h>
#include <dirent.h>

#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include "syscalls.h"           // For syscall numbers.
#include "list.h"
#include "buf.h"
#include "vnode.h"
#include "file.h"
#include "device.h"
#include "mount.h"
#include "vfs.h"
#include "command.h"

/*
 * List for VFS mount points.
 */
static struct list mount_list = LIST_INIT(mount_list);

/*
 * Global lock to access mount point.
 */
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#define MOUNT_LOCK_INIT() do { \
  pthread_mutexattr_t attr; \
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); \
  pthread_mutex_init(&mutex, &attr); \
  } while (0)

#define MOUNT_LOCK()  pthread_mutex_lock(&mutex)
#define MOUNT_UNLOCK()  pthread_mutex_unlock(&mutex)

/*
 * Lookup file system.
 */
static const struct vfssw *fs_getfs(char *name)
{
  int i;
  for (i = 0; vfssw[i].vs_name; ++i) {
    if (!strncmp(name, vfssw[i].vs_name, FSMAXNAMES))
      break;
  }

  return &vfssw[i];
}

/*
 * Compare two path strings. Return matched length.
 * @path: target path.
 * @root: vfs root path as mount point.
 */
static size_t count_match(char *path, char *mount_root)
{
  size_t len = 0;

  while (*path && *mount_root) {
    if (*path++ != *mount_root++)
      break;
    len++;
  }
  if (*mount_root != '\0')
    return 0;

  if (len == 1 && *(path - 1) == '/')
    return 1;

  if (*path == '\0' || *path == '/')
    return len;
  return 0;
}

/*
 * Get the root directory and mount point for specified path.
 * @path: full path.
 * @mp: mount point to return.
 * @root: pointer to root directory in path.
 */
int vfs_findroot(char *path, mount_t *mp, char **root)
{
  mount_t m, tmp;
  list_t head, n;
  size_t len, max_len = 0;

  if (!path)
    return -1;

  /* Find mount point from nearest path */
  MOUNT_LOCK();
  m = NULL;
  head = &mount_list;
  for (n = list_first(head); n != head; n = list_next(n)) {
    tmp = list_entry(n, struct mount, m_link);
    len = count_match(path, tmp->m_path);
    if (len > max_len) {
      max_len = len;
      m = tmp;
    }
  }
  MOUNT_UNLOCK();
  if (m == NULL)
    return -1;
  *root = (char *)(path + max_len);
  if (**root == '/')
    (*root)++;
  *mp = m;
  return 0;
}

/*
 * Mark a mount point as busy.
 */
void vfs_busy(mount_t mp)
{

  MOUNT_LOCK();
  mp->m_count++;
  MOUNT_UNLOCK();
}

/*
 * Mark a mount point as busy.
 */
void vfs_unbusy(mount_t mp)
{

  MOUNT_LOCK();
  mp->m_count--;
  MOUNT_UNLOCK();
}

int vfs_nullop(void)
{
  return 0;
}

int vfs_einval(void)
{
  return EINVAL;
}

static int sys_mount(char *dev, char *dir, char *fsname, int flags,
                     void *data)
{
  const struct vfssw *fs;
  mount_t mp;
  list_t head, n;
  device_t device;
  vnode_t vp, vp_covered;
  int error;

#ifdef DEBUG
  dprintf("VFS: mounting %s at %s\n", fsname, dir);
#endif

  if (!dir || *dir == '\0')
    return -ENOENT;

  /* Find a file system. */
  if (!(fs = fs_getfs(fsname)))
    return -ENODEV; /* No such file system */

  /* Open device. NULL can be specified as a device. */
  device = 0;
  if (*dev != '\0') {
    if (strncmp(dev, "/dev/", 5))
      return ENOTBLK;
    if ((error = device_open(dev + 5, DO_RDWR, &device)) != 0)
      return error;
  }

  MOUNT_LOCK();

  /* Check if device or directory has already been mounted. */
  head = &mount_list;
  for (n = list_first(head); n != head; n = list_next(n)) {
    mp = list_entry(n, struct mount, m_link);
    if (!strcmp(mp->m_path, dir) ||
        (device && mp->m_dev == (dev_t)device)) {
      error = -EBUSY; /* Already mounted */
      goto err1;
    }
  }
  /*
   * Create VFS mount entry.
   */
  if (!(mp = malloc(sizeof(struct mount)))) {
    error = -ENOMEM;
    goto err1;
  }
  mp->m_count = 0;
  mp->m_op = fs->vs_op;
  mp->m_flags = flags;
  mp->m_dev = (dev_t)device;
  strlcpy(mp->m_path, dir, sizeof(mp->m_path));

  /*
   * Get vnode to be covered in the upper file system.
   */
  if (*dir == '/' && *(dir + 1) == '\0') {
    /* Ignore if it mounts to global root directory. */
    vp_covered = NULL;
  } else {
    if ((error = namei(dir, &vp_covered)) != 0) {

      error = -ENOENT;
      goto err2;
    }
    if (vp_covered->v_type != VDIR) {
      error = -ENOTDIR;
      goto err3;
    }
  }
  mp->m_covered = vp_covered;

  /*
   * Create a root vnode for this file system.
   */
  if ((vp = vget(mp, "/")) == NULL) {
    error = -ENOMEM;
    goto err3;
  }
  vp->v_type = VDIR;
  vp->v_flags = VROOT;
  vp->v_mode = S_IFDIR | S_IRUSR | S_IWUSR | S_IXUSR;
  mp->m_root = vp;

  /*
   * Call a file system specific routine.
   */
  if ((error = VFS_MOUNT(mp, dev, flags, data)) != 0)
    goto err4;

  if (mp->m_flags & MNT_RDONLY)
    vp->v_mode &=~S_IWUSR;

  /*
   * Keep reference count for root/covered vnode.
   */
  vn_unlock(vp);
  if (vp_covered)
    vn_unlock(vp_covered);

  /*
   * Insert to mount list
   */
  list_insert(&mount_list, &mp->m_link);
  MOUNT_UNLOCK();

  return 0;  /* success */
 err4:
  vput(vp);
 err3:
  if (vp_covered)
    vput(vp_covered);
 err2:
  free(mp);
 err1:
  device_close(device);

  MOUNT_UNLOCK();
  return error;
}

// RICH: use flags.
static int sys_umount2(char *path, int flags)
{
  mount_t mp;
  list_t head, n;
  int error;

  DPRINTF(VFSDB_SYSCALL, ("sys_umount2: path=%s\n", path));

  MOUNT_LOCK();

  /* Get mount entry */
  head = &mount_list;
  for (n = list_first(head); n != head; n = list_next(n)) {
    mp = list_entry(n, struct mount, m_link);
    if (!strcmp(path, mp->m_path))
      break;
  }
  if (n == head) {
    error = EINVAL;
    goto out;
  }
  /*
   * Root fs can not be unmounted.
   */
  if (mp->m_covered == NULL) {
    error = EINVAL;
    goto out;
  }
  if ((error = VFS_UNMOUNT(mp)) != 0)
    goto out;
  list_remove(&mp->m_link);

  /* Decrement referece count of root vnode */
  vrele(mp->m_covered);

  /* Release all vnodes */
  vflush(mp);

  /* Flush all buffers */
  binval(mp->m_dev);

  if (mp->m_dev)
    device_close((device_t)mp->m_dev);
  free(mp);
 out:
  MOUNT_UNLOCK();
  return error;
}

static int sys_sync(void)
{
  mount_t mp;
  list_t head, n;

  /* Call each mounted file system. */
  MOUNT_LOCK();
  head = &mount_list;
  for (n = list_first(head); n != head; n = list_next(n)) {
    mp = list_entry(n, struct mount, m_link);
    VFS_SYNC(mp);
  }
  MOUNT_UNLOCK();
  bio_sync();
  return 0;
}

#if defined(VFS_COMMANDS)
/*
 * Dump mounts.
 */
static int mountCommand(int argc, char **argv)
{
  if (argc <= 0) {
    printf("dump all mounted file systems.\n");
    return COMMAND_OK;
  }

  list_t head, n;
  mount_t mp;

  MOUNT_LOCK();

  printf("dev      count root\n");
  printf("-------- ----- --------\n");
  head = &mount_list;
  for (n = list_first(head); n != head; n = list_next(n)) {
    mp = list_entry(n, struct mount, m_link);
    printf("%8llx %5d %s\n", (long long)mp->m_dev, mp->m_count, mp->m_path);
  }

  MOUNT_UNLOCK();
  return COMMAND_OK;
}
#endif

ELK_CONSTRUCTOR()
{
  SYSCALL(mount);
  SYSCALL(umount2);
  SYSCALL(sync);

#if defined(VFS_COMMANDS)
  command_insert("mount", mountCommand);
#endif
}

C_CONSTRUCTOR()
{
  MOUNT_LOCK_INIT();
}
