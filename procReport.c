#include <linux/module.h>
#include <linux/sched/signal.h>
#include <linux/pid_namespace.h>
#include <asm/io.h>

int virt2phys(pgd_t* offset, unsigned long vpage) {
    pgd_t* pgd;
    p4d_t* p4d;
    pud_t* pud;
    pmd_t* pmd;
    pte_t* pte;

    struct page* page;

    pgd = offset;

    if (pgd_none(*pgd) || pgd_bad(*pgd))
        return 0;

    p4d = p4d_offset(pgd, vpage);

    if (p4d_none(*p4d) || p4d_bad(*p4d))
        return 0;

    pud = pud_offset(p4d, vpage);

    if (pud_none(*pud) || pud_bad(*pud))
        return 0;

    pmd = pmd_offset(pud, vpage);

    if (pmd_none(*pmd) || pmd_bad(*pmd))
        return 0;

    if (!(pte = pte_offset_map(pmd, vpage)))
        return 0;

    if (!(page = pte_page(*pte)))
        return 0;
    
    unsigned long physical_page_addr;
    physical_page_addr = page_to_phys(page);
    pte_unmap(pte);

    if (physical_page_addr == 70368744173568)
        return 0;

    return physical_page_addr;
}

void walker(void) {
    struct vm_area_struct* vma = 0;
    unsigned long vpage;
    int numPages, non_contigpages, contigpages;
    int totalPages = 0, totalContigPages = 0, totalNonContigPages = 0;
    struct task_struct* task;
    
    printk(KERN_INFO "pid,name,contigpages,non_contigpages,total_pages\n");
    
    for_each_process(task) {
        unsigned long last_physpage = 0; 
        numPages = contigpages = non_contigpages = 0;

        if (task_pid_nr(task) > 650 && task->mm && task->mm->mmap) {
            for (vma = task->mm->mmap; vma; vma = vma->vm_next) {
                for (vpage = vma->vm_start; vpage < vma->vm_end; vpage += PAGE_SIZE) {
                    unsigned long physical_page_addr = virt2phys(pgd_offset(task->mm, vpage), vpage);

                    // do we know this address is for this process?
                    if (physical_page_addr != 0) { // this will always set the last physical page to something other than 0
                        // could be the first page we see
                        if (last_physpage != 0) {
                            // this new address is one of two possibilities
                            if (last_physpage + PAGE_SIZE != physical_page_addr) {
                                non_contigpages++;
                                totalNonContigPages++;
                            } else {
                                contigpages++;
                                totalContigPages++;
                            }

                            if (numPages == 1) { // edge case, is the first page contig or not?
                                if (totalNonContigPages == 1) { // if we found one non-contig, first one must be non-contig
                                    non_contigpages++;
                                    totalNonContigPages++;
                                } else { // same reason for contig...
                                    contigpages++;
                                    totalContigPages++;
                                }
                            }
                        }

                        numPages++;
                        last_physpage = physical_page_addr;
                    }
                }
            }
            printk(KERN_INFO "%d,%s,%d,%d,%d\n", task_pid_nr(task), task->comm, contigpages, non_contigpages, numPages);
            totalPages += numPages;
        }
    }
    printk(KERN_INFO "TOTALS,,%d,%d,%d\n", totalContigPages, totalNonContigPages, totalPages);
}

int proc_init(void) {
    printk(KERN_INFO "procReport: kernel module initialized");
    walker();
    return 0;
}

void proc_exit(void) {
    printk(KERN_INFO "procReport: kernel module exiting");
}

MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_exit);
