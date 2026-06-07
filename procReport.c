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
    int numPages;
    int totalPages = 0;
    struct task_struct* task;
    
    printk(KERN_INFO "pid,name,total_pages\n");
    
    for_each_process(task) {
    	numPages = 0;
        if (task_pid_nr(task) > 650 && task->mm && task->mm->mmap) {
            for (vma = task->mm->mmap; vma; vma = vma->vm_next) {
                for (vpage = vma->vm_start; vpage < vma->vm_end; vpage += PAGE_SIZE) {
                    unsigned long physical_page_addr = virt2phys(pgd_offset(task->mm, vpage), vpage);
                    if (physical_page_addr != 0) {
                    	numPages++;
                    }
                }
            }
            printk(KERN_INFO "%d,%s,%d\n", task_pid_nr(task), task->comm, numPages);
        	totalPages += numPages;
        }
    }
    printk(KERN_INFO "Total pages: %d\n", totalPages);
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
