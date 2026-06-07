#include <linux/sched/signal.h>
#include <linux/pid_namespace.h>
#include <asm/io.h>

int proc_init(void) {
    printk(KERN_INFO "pageTableWalker: kernel module initialized");
    return 0;
}

void proc_exit(void) {
    printk(KERN_INFO "pageTableWalker: kernel module exiting");
}

MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_exit);
