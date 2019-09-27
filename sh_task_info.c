#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <uapi/asm-generic/errno-base.h>
#include <asm/uaccess.h>

#include "taskinfo.h"

asmlinkage long sys_sh_task_info(int pid , char* filename)
{
	if(pid <= 0)
		return -EINVAL;
	if(pid > 32768)
		return -EINVAL;

	struct task_struct *task;
	struct file *file;
	int fileOpen , xVariable = 0;

	mm_segment_t old_fs = get_fs();
	set_fs(KERNEL_DS);

	fileOpen = sys_open(filename , O_WRONLY|O_CREAT , 0644);

	char data[700] , temp[700];
	int lines = 15;

	for_each_process(task)
	{
		if((int)task->pid == pid)
		{
			xVariable = 3;

			printk("Process Name: %s\n" , task->comm);
			strcpy(data , "Process Name: ");
			strcat(data , task->comm);
			strcat(data , "\n");

			lines = lines + 1;

			printk("Process PID: %ld\n" , (long)task->pid);
			strcat(data , "Process PID: ");
			sprintf(temp , "%ld\n" , (long)task->pid);
			strcat(data , temp);

			lines = lines + 2;

			printk("Process Priority: %ld\n" , (long)task->prio);
			strcat(data , "Process Priority: ");
			sprintf(temp , "%ld\n"  , (long)task->prio);
			strcat(data , temp);

			lines = lines + 3;

			printk("Process Parent Name: %s\n" , task->parent->comm);
			strcat(data , "Process Parent Name: ");
			sprintf(temp , "%s\n"  , task->parent->comm);
			strcat(data , temp);


			lines = lines + 5;

			printk("Process Exit State : %ld\n" , (long)task->exit_code);
			strcat(data , "Process Exit State: ");
			sprintf(temp , "%ld\n"  , (long)task->exit_code);
			strcat(data , temp);

			lines = lines + 7;

			if(fileOpen < 0)
				return -EISDIR;
			file = fget(fileOpen);
			file->f_op->write(file , data , strlen(data) , &file->f_pos);
		}
	}
	set_fs(old_fs);
	return 0;
}
