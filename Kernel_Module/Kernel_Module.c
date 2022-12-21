#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include<linux/string.h>


char *process_name="systemd";

module_param(process_name,charp,S_IRUGO);



static int tasks_init(void){
    struct task_struct *process;    
    pr_info("%s: ENtering into KERNEL_MODULE for Assignment_3 \n", __func__);
    
    for_each_process(process) {
        
        if(strcmp(process->comm,process_name)==0){
        printk(KERN_INFO "Process ID       : %d",process->pid);
        printk(KERN_INFO "User ID          : %d",process->cred->uid);
        printk(KERN_INFO "Process Group ID : %d",process->cred->gid);
        printk(KERN_INFO "\n");
        //printk(KERN_INFO "Command Path     : %s",get_cmdline(process_name));
        return 0;
        }
         
    }
    return 0;
}

static void tasks_exit(void){
    pr_info("%s: Exiting module ....\n", __func__);
}

MODULE_LICENSE("GPL");
module_init(tasks_init);
module_exit(tasks_exit);
