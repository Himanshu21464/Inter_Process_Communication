#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include "syscall_module.h"


MODULE_LICENCE("GPL");
long syscall_module_syscall(int pid) {
    struct task_struct *task;
    struct path *path;
    char *buf;
    int ret;

    // Find the task_struct for the given pid
    for_each_process(task) {
        if (task->pid == pid) {
            break;
        }
    }

    // Print the values of the desired fields
    printk(KERN_INFO "pid: %d\n", task->pid);
    printk(KERN_INFO "user id: %d\n", task->cred->uid);
    printk(KERN_INFO "process group id: %d\n", task->cred->gid);

    // Get the command path
    path = &task->mm->exe_file->f_path;
    buf = (char *) kmalloc(PATH_MAX, GFP_KERNEL);
    d_path(path, buf, PATH_MAX);
    printk(KERN_INFO "command path: %s\n", buf);

    kfree(buf);

    return 0;
}
