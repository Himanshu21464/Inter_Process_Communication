#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include<linux/string.h>
#include <linux/path.h>


int id=1;

module_param(id,int,S_IRUGO);



static int tasks_init(void){
    struct task_struct *process; 
    struct path *path;
    char *command_path;

    pr_info("%s: ENtering into KERNEL_MODULE for Assignment_3 \n", __func__);
    
    for_each_process(process) {

        
        if(process->pid==id){
        //get_task_comm(command_path, task);
        //get_task_exe_path(task, &path);
        //d_path(&path->dentry, &path->mnt, command_path, PATH_MAX);
        printk(KERN_INFO "Process ID       : %d",process->pid);
        printk(KERN_INFO "User ID          : %d",process->cred->uid);
        printk(KERN_INFO "Process Group ID : %d",process->cred->gid);
        printk(KERN_INFO "Command Path     : %s",process->comm);
        
        printk(KERN_INFO "\n");
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
