/*-
 * Copyright (c) 2009, Kohsuke Ohtani
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

#ifndef _mmu_h_
#define _mmu_h_

#include <sys/types.h>

typedef uint32_t *pgd_t;                // Page directory.

typedef struct pte {
  uint32_t pte_hi;
  uint32_t pte_lo;
} *pte_t;                               // Page table entry.

// High word:
#define PTE_VALID       0x00000001      // Valid.
#define PTE_VSID        0x7fffff80      // Virtual segment.
#define PTE_HID         0x00000040      // Hash.
#define PTE_API         0x0000003f      // Abbreviated page index.

// Low word:
#define PTE_RPN         0xfffff000      // Real page number.
#define PTE_REF         0x00000100      // Reference.
#define PTE_CHG         0x00000080      // Change.
#define PTE_WIMG        0x00000078      // Memory/cache control.
#define PTE_PP          0x00000003      // Page protection.

// Virtual and physical address translation.
#define PAGE_DIR(virt)
#define PAGE_TABLE(virt)
#define pte_present(pgd, virt)
#define page_present(pte, virt)
#define vtopte(pgd, virt)
#define ptetopg(pte, virt)

#endif // !_mmu_h_
