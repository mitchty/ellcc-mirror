#ifndef _PPC_LINUX_ASM_UNISTD_H_
#define _PPC_LINUX_ASM_UNISTD_H_

#define __NR_SYSCALL_BASE 0

#define __NR_restart_syscall (__NR_SYSCALL_BASE + 0)
#define __NR_exit (__NR_SYSCALL_BASE + 1)
#define __NR_fork (__NR_SYSCALL_BASE + 2)
#define __NR_read (__NR_SYSCALL_BASE + 3)
#define __NR_write (__NR_SYSCALL_BASE + 4)
#define __NR_open (__NR_SYSCALL_BASE + 5)
#define __NR_close (__NR_SYSCALL_BASE + 6)
#define __NR_waitpid (__NR_SYSCALL_BASE + 7)
#define __NR_creat (__NR_SYSCALL_BASE + 8)
#define __NR_link (__NR_SYSCALL_BASE + 9)
#define __NR_unlink (__NR_SYSCALL_BASE + 10)
#define __NR_execve (__NR_SYSCALL_BASE + 11)
#define __NR_chdir (__NR_SYSCALL_BASE + 12)
#define __NR_time (__NR_SYSCALL_BASE + 13)
#define __NR_mknod (__NR_SYSCALL_BASE + 14)
#define __NR_chmod (__NR_SYSCALL_BASE + 15)
#define __NR_lchown (__NR_SYSCALL_BASE + 16)
#define __NR_break (__NR_SYSCALL_BASE + 17)
#define __NR_oldstat (__NR_SYSCALL_BASE + 18)
#define __NR_lseek (__NR_SYSCALL_BASE + 19)
#define __NR_getpid (__NR_SYSCALL_BASE + 20)
#define __NR_mount (__NR_SYSCALL_BASE + 21)
#define __NR_umount (__NR_SYSCALL_BASE + 22)
#define __NR_setuid (__NR_SYSCALL_BASE + 23)
#define __NR_getuid (__NR_SYSCALL_BASE + 24)
#define __NR_stime (__NR_SYSCALL_BASE + 25)
#define __NR_ptrace (__NR_SYSCALL_BASE + 26)
#define __NR_alarm (__NR_SYSCALL_BASE + 27)
#define __NR_oldfstat (__NR_SYSCALL_BASE + 28)
#define __NR_pause (__NR_SYSCALL_BASE + 29)
#define __NR_utime (__NR_SYSCALL_BASE + 30)
#define __NR_stty (__NR_SYSCALL_BASE + 31)
#define __NR_gtty (__NR_SYSCALL_BASE + 32)
#define __NR_access (__NR_SYSCALL_BASE + 33)
#define __NR_nice (__NR_SYSCALL_BASE + 34)
#define __NR_ftime (__NR_SYSCALL_BASE + 35)
#define __NR_sync (__NR_SYSCALL_BASE + 36)
#define __NR_kill (__NR_SYSCALL_BASE + 37)
#define __NR_rename (__NR_SYSCALL_BASE + 38)
#define __NR_mkdir (__NR_SYSCALL_BASE + 39)
#define __NR_rmdir (__NR_SYSCALL_BASE + 40)
#define __NR_dup (__NR_SYSCALL_BASE + 41)
#define __NR_pipe (__NR_SYSCALL_BASE + 42)
#define __NR_times (__NR_SYSCALL_BASE + 43)
#define __NR_prof (__NR_SYSCALL_BASE + 44)
#define __NR_brk (__NR_SYSCALL_BASE + 45)
#define __NR_setgid (__NR_SYSCALL_BASE + 46)
#define __NR_getgid (__NR_SYSCALL_BASE + 47)
#define __NR_signal (__NR_SYSCALL_BASE + 48)
#define __NR_geteuid (__NR_SYSCALL_BASE + 49)
#define __NR_getegid (__NR_SYSCALL_BASE + 50)
#define __NR_acct (__NR_SYSCALL_BASE + 51)
#define __NR_umount2 (__NR_SYSCALL_BASE + 52)
#define __NR_lock (__NR_SYSCALL_BASE + 53)
#define __NR_ioctl (__NR_SYSCALL_BASE + 54)
#define __NR_fcntl (__NR_SYSCALL_BASE + 55)
#define __NR_mpx (__NR_SYSCALL_BASE + 56)
#define __NR_setpgid (__NR_SYSCALL_BASE + 57)
#define __NR_ulimit (__NR_SYSCALL_BASE + 58)
#define __NR_oldolduname (__NR_SYSCALL_BASE + 59)
#define __NR_umask (__NR_SYSCALL_BASE + 60)
#define __NR_chroot (__NR_SYSCALL_BASE + 61)
#define __NR_ustat (__NR_SYSCALL_BASE + 62)
#define __NR_dup2 (__NR_SYSCALL_BASE + 63)
#define __NR_getppid (__NR_SYSCALL_BASE + 64)
#define __NR_getpgrp (__NR_SYSCALL_BASE + 65)
#define __NR_setsid (__NR_SYSCALL_BASE + 66)
#define __NR_sigaction (__NR_SYSCALL_BASE + 67)
#define __NR_sgetmask (__NR_SYSCALL_BASE + 68)
#define __NR_ssetmask (__NR_SYSCALL_BASE + 69)
#define __NR_setreuid (__NR_SYSCALL_BASE + 70)
#define __NR_setregid (__NR_SYSCALL_BASE + 71)
#define __NR_sigsuspend (__NR_SYSCALL_BASE + 72)
#define __NR_sigpending (__NR_SYSCALL_BASE + 73)
#define __NR_sethostname (__NR_SYSCALL_BASE + 74)
#define __NR_setrlimit (__NR_SYSCALL_BASE + 75)
#define __NR_getrlimit (__NR_SYSCALL_BASE + 76)
#define __NR_getrusage (__NR_SYSCALL_BASE + 77)
#define __NR_gettimeofday (__NR_SYSCALL_BASE + 78)
#define __NR_settimeofday (__NR_SYSCALL_BASE + 79)
#define __NR_getgroups (__NR_SYSCALL_BASE + 80)
#define __NR_setgroups (__NR_SYSCALL_BASE + 81)
#define __NR_select (__NR_SYSCALL_BASE + 82)
#define __NR_symlink (__NR_SYSCALL_BASE + 83)
#define __NR_oldlstat (__NR_SYSCALL_BASE + 84)
#define __NR_readlink (__NR_SYSCALL_BASE + 85)
#define __NR_uselib (__NR_SYSCALL_BASE + 86)
#define __NR_swapon (__NR_SYSCALL_BASE + 87)
#define __NR_reboot (__NR_SYSCALL_BASE + 88)
#define __NR_readdir (__NR_SYSCALL_BASE + 89)
#define __NR_mmap (__NR_SYSCALL_BASE + 90)
#define __NR_munmap (__NR_SYSCALL_BASE + 91)
#define __NR_truncate (__NR_SYSCALL_BASE + 92)
#define __NR_ftruncate (__NR_SYSCALL_BASE + 93)
#define __NR_fchmod (__NR_SYSCALL_BASE + 94)
#define __NR_fchown (__NR_SYSCALL_BASE + 95)
#define __NR_getpriority (__NR_SYSCALL_BASE + 96)
#define __NR_setpriority (__NR_SYSCALL_BASE + 97)
#define __NR_profil (__NR_SYSCALL_BASE + 98)
#define __NR_statfs (__NR_SYSCALL_BASE + 99)
#define __NR_fstatfs (__NR_SYSCALL_BASE + 100)
#define __NR_ioperm (__NR_SYSCALL_BASE + 101)
#define __NR_socketcall (__NR_SYSCALL_BASE + 102)
#define __NR_syslog (__NR_SYSCALL_BASE + 103)
#define __NR_setitimer (__NR_SYSCALL_BASE + 104)
#define __NR_getitimer (__NR_SYSCALL_BASE + 105)
#define __NR_stat (__NR_SYSCALL_BASE + 106)
#define __NR_lstat (__NR_SYSCALL_BASE + 107)
#define __NR_fstat (__NR_SYSCALL_BASE + 108)
#define __NR_olduname (__NR_SYSCALL_BASE + 109)
#define __NR_iopl (__NR_SYSCALL_BASE + 110)
#define __NR_vhangup (__NR_SYSCALL_BASE + 111)
#define __NR_idle (__NR_SYSCALL_BASE + 112)
#define __NR_vm86 (__NR_SYSCALL_BASE + 113)
#define __NR_wait4 (__NR_SYSCALL_BASE + 114)
#define __NR_swapoff (__NR_SYSCALL_BASE + 115)
#define __NR_sysinfo (__NR_SYSCALL_BASE + 116)
#define __NR_ipc (__NR_SYSCALL_BASE + 117)
#define __NR_fsync (__NR_SYSCALL_BASE + 118)
#define __NR_sigreturn (__NR_SYSCALL_BASE + 119)
#define __NR_clone (__NR_SYSCALL_BASE + 120)
#define __NR_setdomainname (__NR_SYSCALL_BASE + 121)
#define __NR_uname (__NR_SYSCALL_BASE + 122)
#define __NR_modify_ldt (__NR_SYSCALL_BASE + 123)
#define __NR_adjtimex (__NR_SYSCALL_BASE + 124)
#define __NR_mprotect (__NR_SYSCALL_BASE + 125)
#define __NR_sigprocmask (__NR_SYSCALL_BASE + 126)
#define __NR_create_module (__NR_SYSCALL_BASE + 127)
#define __NR_init_module (__NR_SYSCALL_BASE + 128)
#define __NR_delete_module (__NR_SYSCALL_BASE + 129)
#define __NR_get_kernel_syms (__NR_SYSCALL_BASE + 130)
#define __NR_quotactl (__NR_SYSCALL_BASE + 131)
#define __NR_getpgid (__NR_SYSCALL_BASE + 132)
#define __NR_fchdir (__NR_SYSCALL_BASE + 133)
#define __NR_bdflush (__NR_SYSCALL_BASE + 134)
#define __NR_sysfs (__NR_SYSCALL_BASE + 135)
#define __NR_personality (__NR_SYSCALL_BASE + 136)
#define __NR_afs_syscall (__NR_SYSCALL_BASE + 137)
#define __NR_setfsuid (__NR_SYSCALL_BASE + 138)
#define __NR_setfsgid (__NR_SYSCALL_BASE + 139)
#define __NR__llseek (__NR_SYSCALL_BASE + 140)
#define __NR_getdents (__NR_SYSCALL_BASE + 141)
#define __NR__newselect (__NR_SYSCALL_BASE + 142)
#define __NR_flock (__NR_SYSCALL_BASE + 143)
#define __NR_msync (__NR_SYSCALL_BASE + 144)
#define __NR_readv (__NR_SYSCALL_BASE + 145)
#define __NR_writev (__NR_SYSCALL_BASE + 146)
#define __NR_getsid (__NR_SYSCALL_BASE + 147)
#define __NR_fdatasync (__NR_SYSCALL_BASE + 148)
#define __NR__sysctl (__NR_SYSCALL_BASE + 149)
#define __NR_mlock (__NR_SYSCALL_BASE + 150)
#define __NR_munlock (__NR_SYSCALL_BASE + 151)
#define __NR_mlockall (__NR_SYSCALL_BASE + 152)
#define __NR_munlockall (__NR_SYSCALL_BASE + 153)
#define __NR_sched_setparam (__NR_SYSCALL_BASE + 154)
#define __NR_sched_getparam (__NR_SYSCALL_BASE + 155)
#define __NR_sched_setscheduler (__NR_SYSCALL_BASE + 156)
#define __NR_sched_getscheduler (__NR_SYSCALL_BASE + 157)
#define __NR_sched_yield (__NR_SYSCALL_BASE + 158)
#define __NR_sched_get_priority_max (__NR_SYSCALL_BASE + 159)
#define __NR_sched_get_priority_min (__NR_SYSCALL_BASE + 160)
#define __NR_sched_rr_get_interval (__NR_SYSCALL_BASE + 161)
#define __NR_nanosleep (__NR_SYSCALL_BASE + 162)
#define __NR_mremap (__NR_SYSCALL_BASE + 163)
#define __NR_setresuid (__NR_SYSCALL_BASE + 164)
#define __NR_getresuid (__NR_SYSCALL_BASE + 165)
#define __NR_query_module (__NR_SYSCALL_BASE + 166)
#define __NR_poll (__NR_SYSCALL_BASE + 167)
#define __NR_nfsservctl (__NR_SYSCALL_BASE + 168)
#define __NR_setresgid (__NR_SYSCALL_BASE + 169)
#define __NR_getresgid (__NR_SYSCALL_BASE + 170)
#define __NR_prctl (__NR_SYSCALL_BASE + 171)
#define __NR_rt_sigreturn (__NR_SYSCALL_BASE + 172)
#define __NR_rt_sigaction (__NR_SYSCALL_BASE + 173)
#define __NR_rt_sigprocmask (__NR_SYSCALL_BASE + 174)
#define __NR_rt_sigpending (__NR_SYSCALL_BASE + 175)
#define __NR_rt_sigtimedwait (__NR_SYSCALL_BASE + 176)
#define __NR_rt_sigqueueinfo (__NR_SYSCALL_BASE + 177)
#define __NR_rt_sigsuspend (__NR_SYSCALL_BASE + 178)
#define __NR_pread64 (__NR_SYSCALL_BASE + 179)
#define __NR_pwrite64 (__NR_SYSCALL_BASE + 180)
#define __NR_chown (__NR_SYSCALL_BASE + 181)
#define __NR_getcwd (__NR_SYSCALL_BASE + 182)
#define __NR_capget (__NR_SYSCALL_BASE + 183)
#define __NR_capset (__NR_SYSCALL_BASE + 184)
#define __NR_sigaltstack (__NR_SYSCALL_BASE + 185)
#define __NR_sendfile (__NR_SYSCALL_BASE + 186)
#define __NR_getpmsg (__NR_SYSCALL_BASE + 187)
#define __NR_putpmsg (__NR_SYSCALL_BASE + 188)
#define __NR_vfork (__NR_SYSCALL_BASE + 189)
#define __NR_ugetrlimit (__NR_SYSCALL_BASE + 190)
#define __NR_readahead (__NR_SYSCALL_BASE + 191)
#define __NR_mmap2 (__NR_SYSCALL_BASE + 192)
#define __NR_truncate64 (__NR_SYSCALL_BASE + 193)
#define __NR_ftruncate64 (__NR_SYSCALL_BASE + 194)
#define __NR_stat64 (__NR_SYSCALL_BASE + 195)
#define __NR_lstat64 (__NR_SYSCALL_BASE + 196)
#define __NR_fstat64 (__NR_SYSCALL_BASE + 197)
#define __NR_pciconfig_read (__NR_SYSCALL_BASE + 198)
#define __NR_pciconfig_write (__NR_SYSCALL_BASE + 199)
#define __NR_pciconfig_iobase (__NR_SYSCALL_BASE + 200)
#define __NR_multiplexer (__NR_SYSCALL_BASE + 201)
#define __NR_getdents64 (__NR_SYSCALL_BASE + 202)
#define __NR_pivot_root (__NR_SYSCALL_BASE + 203)
#define __NR_fcntl64 (__NR_SYSCALL_BASE + 204)
#define __NR_madvise (__NR_SYSCALL_BASE + 205)
#define __NR_mincore (__NR_SYSCALL_BASE + 206)
#define __NR_gettid (__NR_SYSCALL_BASE + 207)
#define __NR_tkill (__NR_SYSCALL_BASE + 208)
#define __NR_setxattr (__NR_SYSCALL_BASE + 209)
#define __NR_lsetxattr (__NR_SYSCALL_BASE + 210)
#define __NR_fsetxattr (__NR_SYSCALL_BASE + 211)
#define __NR_getxattr (__NR_SYSCALL_BASE + 212)
#define __NR_lgetxattr (__NR_SYSCALL_BASE + 213)
#define __NR_fgetxattr (__NR_SYSCALL_BASE + 214)
#define __NR_listxattr (__NR_SYSCALL_BASE + 215)
#define __NR_llistxattr (__NR_SYSCALL_BASE + 216)
#define __NR_flistxattr (__NR_SYSCALL_BASE + 217)
#define __NR_removexattr (__NR_SYSCALL_BASE + 218)
#define __NR_lremovexattr (__NR_SYSCALL_BASE + 219)
#define __NR_fremovexattr (__NR_SYSCALL_BASE + 220)
#define __NR_futex (__NR_SYSCALL_BASE + 221)
#define __NR_sched_setaffinity (__NR_SYSCALL_BASE + 222)
#define __NR_sched_getaffinity (__NR_SYSCALL_BASE + 223)
#define __NR_tuxcall (__NR_SYSCALL_BASE + 225)
#define __NR_sendfile64 (__NR_SYSCALL_BASE + 226)
#define __NR_io_setup (__NR_SYSCALL_BASE + 227)
#define __NR_io_destroy (__NR_SYSCALL_BASE + 228)
#define __NR_io_getevents (__NR_SYSCALL_BASE + 229)
#define __NR_io_submit (__NR_SYSCALL_BASE + 230)
#define __NR_io_cancel (__NR_SYSCALL_BASE + 231)
#define __NR_set_tid_address (__NR_SYSCALL_BASE + 232)
#define __NR_fadvise64 (__NR_SYSCALL_BASE + 233)
#define __NR_exit_group (__NR_SYSCALL_BASE + 234)
#define __NR_lookup_dcookie (__NR_SYSCALL_BASE + 235)
#define __NR_epoll_create (__NR_SYSCALL_BASE + 236)
#define __NR_epoll_ctl (__NR_SYSCALL_BASE + 237)
#define __NR_epoll_wait (__NR_SYSCALL_BASE + 238)
#define __NR_remap_file_pages (__NR_SYSCALL_BASE + 239)
#define __NR_timer_create (__NR_SYSCALL_BASE + 240)
#define __NR_timer_settime (__NR_SYSCALL_BASE + 241)
#define __NR_timer_gettime (__NR_SYSCALL_BASE + 242)
#define __NR_timer_getoverrun (__NR_SYSCALL_BASE + 243)
#define __NR_timer_delete (__NR_SYSCALL_BASE + 244)
#define __NR_clock_settime (__NR_SYSCALL_BASE + 245)
#define __NR_clock_gettime (__NR_SYSCALL_BASE + 246)
#define __NR_clock_getres (__NR_SYSCALL_BASE + 247)
#define __NR_clock_nanosleep (__NR_SYSCALL_BASE + 248)
#define __NR_swapcontext (__NR_SYSCALL_BASE + 249)
#define __NR_tgkill (__NR_SYSCALL_BASE + 250)
#define __NR_utimes (__NR_SYSCALL_BASE + 251)
#define __NR_statfs64 (__NR_SYSCALL_BASE + 252)
#define __NR_fstatfs64 (__NR_SYSCALL_BASE + 253)
#define __NR_fadvise64_64 (__NR_SYSCALL_BASE + 254)
#define __NR_rtas (__NR_SYSCALL_BASE + 255)
#define __NR_sys_debug_setcontext (__NR_SYSCALL_BASE + 256)
#define __NR_migrate_pages (__NR_SYSCALL_BASE + 258)
#define __NR_mbind (__NR_SYSCALL_BASE + 259)
#define __NR_get_mempolicy (__NR_SYSCALL_BASE + 260)
#define __NR_set_mempolicy (__NR_SYSCALL_BASE + 261)
#define __NR_mq_open (__NR_SYSCALL_BASE + 262)
#define __NR_mq_unlink (__NR_SYSCALL_BASE + 263)
#define __NR_mq_timedsend (__NR_SYSCALL_BASE + 264)
#define __NR_mq_timedreceive (__NR_SYSCALL_BASE + 265)
#define __NR_mq_notify (__NR_SYSCALL_BASE + 266)
#define __NR_mq_getsetattr (__NR_SYSCALL_BASE + 267)
#define __NR_kexec_load (__NR_SYSCALL_BASE + 268)
#define __NR_add_key (__NR_SYSCALL_BASE + 269)
#define __NR_request_key (__NR_SYSCALL_BASE + 270)
#define __NR_keyctl (__NR_SYSCALL_BASE + 271)
#define __NR_waitid (__NR_SYSCALL_BASE + 272)
#define __NR_ioprio_set (__NR_SYSCALL_BASE + 273)
#define __NR_ioprio_get (__NR_SYSCALL_BASE + 274)
#define __NR_inotify_init (__NR_SYSCALL_BASE + 275)
#define __NR_inotify_add_watch (__NR_SYSCALL_BASE + 276)
#define __NR_inotify_rm_watch (__NR_SYSCALL_BASE + 277)
#define __NR_spu_run (__NR_SYSCALL_BASE + 278)
#define __NR_spu_create (__NR_SYSCALL_BASE + 279)
#define __NR_pselect6 (__NR_SYSCALL_BASE + 280)
#define __NR_ppoll (__NR_SYSCALL_BASE + 281)
#define __NR_unshare (__NR_SYSCALL_BASE + 282)
#define __NR_splice (__NR_SYSCALL_BASE + 283)
#define __NR_tee (__NR_SYSCALL_BASE + 284)
#define __NR_vmsplice (__NR_SYSCALL_BASE + 285)
#define __NR_openat (__NR_SYSCALL_BASE + 286)
#define __NR_mkdirat (__NR_SYSCALL_BASE + 287)
#define __NR_mknodat (__NR_SYSCALL_BASE + 288)
#define __NR_fchownat (__NR_SYSCALL_BASE + 289)
#define __NR_futimesat (__NR_SYSCALL_BASE + 290)
#define __NR_fstatat64 (__NR_SYSCALL_BASE + 291)
#define __NR_unlinkat (__NR_SYSCALL_BASE + 292)
#define __NR_renameat (__NR_SYSCALL_BASE + 293)
#define __NR_linkat (__NR_SYSCALL_BASE + 294)
#define __NR_symlinkat (__NR_SYSCALL_BASE + 295)
#define __NR_readlinkat (__NR_SYSCALL_BASE + 296)
#define __NR_fchmodat (__NR_SYSCALL_BASE + 297)
#define __NR_faccessat (__NR_SYSCALL_BASE + 298)
#define __NR_get_robust_list (__NR_SYSCALL_BASE + 299)
#define __NR_set_robust_list (__NR_SYSCALL_BASE + 300)
#define __NR_move_pages (__NR_SYSCALL_BASE + 301)
#define __NR_getcpu (__NR_SYSCALL_BASE + 302)
#define __NR_epoll_pwait (__NR_SYSCALL_BASE + 303)
#define __NR_utimensat (__NR_SYSCALL_BASE + 304)
#define __NR_signalfd (__NR_SYSCALL_BASE + 305)
#define __NR_timerfd_create (__NR_SYSCALL_BASE + 306)
#define __NR_eventfd (__NR_SYSCALL_BASE + 307)
#define __NR_sync_file_range2 (__NR_SYSCALL_BASE + 308)
#define __NR_fallocate (__NR_SYSCALL_BASE + 309)
#define __NR_subpage_prot (__NR_SYSCALL_BASE + 310)
#define __NR_timerfd_settime (__NR_SYSCALL_BASE + 311)
#define __NR_timerfd_gettime (__NR_SYSCALL_BASE + 312)
#define __NR_signalfd4 (__NR_SYSCALL_BASE + 313)
#define __NR_eventfd2 (__NR_SYSCALL_BASE + 314)
#define __NR_epoll_create1 (__NR_SYSCALL_BASE + 315)
#define __NR_dup3 (__NR_SYSCALL_BASE + 316)
#define __NR_pipe2 (__NR_SYSCALL_BASE + 317)
#define __NR_inotify_init1 (__NR_SYSCALL_BASE + 318)
#define __NR_perf_event_open (__NR_SYSCALL_BASE + 319)
#define __NR_preadv (__NR_SYSCALL_BASE + 320)
#define __NR_pwritev (__NR_SYSCALL_BASE + 321)
#define __NR_rt_tgsigqueueinfo (__NR_SYSCALL_BASE + 322)

#endif // _PPC_LINUX_ASM_UNISTD_H_