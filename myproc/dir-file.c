#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/timer.h>
#include <linux/rtc.h>
struct timeval tv;//struct timeval 为设定时间或获取时间时使用的结构体
struct rtc_time tm;
struct proc_dir_entry *myproc,*student,*date;
static int a = 0;
static int b = 0;
module_param(a,int,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);//模块传参
MODULE_PARM_DESC(a,"An invisiable int under sysfs");
module_param(b,int,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
MODULE_PARM_DESC(b,"An visiable int under sysfs");
static int task_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m,"Task!\n");//写入proc文件中
	return 0;
}
static int member_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m,"Member!\n");
	return 0;
}
static int day_proc_show(struct seq_file *m, void *v)
{
	switch(a){
		case 1:		
			seq_printf(m,"BJ time :%d-%d-%d\n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday);		
			break;
		case 2:
			seq_printf(m,"BJ time :%d:%d:%d\n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday);
			break;
		default:
			seq_printf(m,"BJ time :%d-%d-%d %d:%d:%d\n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
			break;
		}	
	return 0;
}
static int time_proc_show(struct seq_file *m, void *v)
{
	switch(b){
		case 1:		
			seq_printf(m,"BJ time :%d-%d-%d\n",tm.tm_hour,tm.tm_min,tm.tm_sec);		
			break;
		case 2:
			seq_printf(m,"BJ time :%d:%d:%d\n",tm.tm_hour,tm.tm_min,tm.tm_sec);
			break;
		default:
			seq_printf(m,"BJ time :%d-%d-%d %d:%d:%d\n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
			break;
		}
	return 0;
}

static int hello_proc_open(struct inode *inode, struct file *file)
{
	if(!strncmp("task",file->f_dentry->d_name.name,4))	
		return single_open(file, task_proc_show, NULL);//调用show函数
	if(!strncmp("member",file->f_dentry->d_name.name,6))	
		return single_open(file, member_proc_show, NULL);
	if(!strncmp("day",file->f_dentry->d_name.name,3))	
		return single_open(file, day_proc_show, NULL);
	if(!strncmp("time",file->f_dentry->d_name.name,3))	
		return single_open(file, time_proc_show, NULL);
}

static const struct file_operations hello_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= hello_proc_open,//打开文件
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
static int __init init_procfs(void)
{
	do_gettimeofday(&tv);//此函数获取从1970-1-1 0:0:0到现在的时间值，存在timeval的结构体里边
	rtc_time_to_tm(tv.tv_sec+8*3600,&tm);//此内核函数将系统实时时钟时间转换为北京时间
	myproc = proc_mkdir("myproc",NULL);//创建proc下目录文件
	student = proc_mkdir("student",myproc);
	date = proc_mkdir("date",myproc);
	if(myproc == NULL){
		return -ENOMEM;	
	}
	if(student == NULL){
		return -ENOMEM;	
	}
	if(date == NULL){
		return -ENOMEM;	
	}
	proc_create("task", 0, myproc, &hello_proc_fops);//创建proc下文件
	proc_create("member", 0, student, &hello_proc_fops);
	proc_create("day", 0, date, &hello_proc_fops);
	proc_create("time", 0, date, &hello_proc_fops);
	printk("hello kernel\n");
	return 0;
}
static void __exit exit_procfs(void)
{
	proc_remove(myproc);//删除创建在proc下的目录文件
	proc_remove(student);
	proc_remove(date);	
	printk("GoodBye kernel\n");
}
module_init(init_procfs);
module_exit(exit_procfs);
MODULE_LICENSE("GPL");


