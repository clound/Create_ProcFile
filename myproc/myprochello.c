#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/utsname.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/rtc.h>
#define IS_LEAP_YEAR(year) (!(year%100)&&!(year%400)||(year%100)&&!(year%4))//判断闰年
int mon[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
struct timeval tv;	//struct timeval 为设定时间或获取时间时使用的结构体，tv_sec 变量把当前时间换算为秒，tv_usec 值指定或获取 tv_usec 无法表示的 us 单位经过的时间。
	struct rtc_time tm;
	int year,month,day,hour;
	int carry = 0;
static int hello_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m,"GMT time :%d-%d-%d %d:%d:%d\n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);//格林尼治时间
	seq_printf(m,"BeiJing time :%d-%d-%d %d:%d:%d\n",year,month,day,hour,tm.tm_min,tm.tm_sec);//北京时间
	return 0;
}

static int hello_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, hello_proc_show, NULL);
}

static const struct file_operations hello_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= hello_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init hello_proc_init(void)
{
	
	do_gettimeofday(&tv);//此函数获取从1970-1-1 0:0:0到现在的时间值，存在timeval的结构体里边。
	rtc_time_to_tm(tv.tv_sec,&tm);//此内核函数将系统实时时钟时间转换为格林尼治标准时间（GMT）。如果要得到北京时间需。需要将此时间处理（年份加上1900，月份加上1，小时加上8）。具体参看内核中rtc.h中rtc_time_to_tm代码实现。
	printk("GMT time :%d-%d-%d %d:%d:%d\n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);//格林尼治时间
	hour = (tm.tm_hour + 8)%24;
	carry = (tm.tm_hour + 8)/24;
	if((IS_LEAP_YEAR(tm.tm_year)) && (tm.tm_mon + 1 == 2)){
		day = (tm.tm_mday + carry)%(mon[tm.tm_mon] + 2);
		carry = (tm.tm_mday + carry)/(mon[tm.tm_mon] + 2);
	}
	else{
		day = (tm.tm_mday + carry)%(mon[tm.tm_mon]+1);
		carry = (tm.tm_mday + carry)/(mon[tm.tm_mon]+1);
	}
	month = (tm.tm_mon + carry)%12 + 1;
	carry = (tm.tm_mon + carry)/12;
	year = tm.tm_year + 1900 + carry;
	proc_create("myproc", 0, NULL, &hello_proc_fops);	
	return 0;
}
static int __exit hello_proc_exit(void)
{
	remove_proc_entry("myproc",NULL);
}
MODULE_LICENSE("GPL");
module_init(hello_proc_init);
module_exit(hello_proc_exit);
