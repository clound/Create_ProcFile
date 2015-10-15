# Linux Proc模块
###创建Proc文件模块
    关键函数:
> struct timeval tv;	//struct timeval 为设定时间或获取时间时使用的结构体，tv_sec 变量把当前时间换算为秒，tv_usec 值指定或获取 tv_usec 无法表示的 us 单位经过的时间。  
	struct rtc_time tm;  
	do_gettimeofday(&tv);//此函数获取从1970-1-1 0:0:0到现在的时间值，存在timeval的结构体里边。  
	rtc_time_to_tm(tv.tv_sec+8*3600,&tm);//此内核函数将系统实时时钟时间转换为格林尼治标准时间（GMT）。如果要得到北京时间需。需要将此时间处理（年份加上1900，月份加上1，小时加上8）。具体参看内核中rtc.h中rtc_time_to_tm代码实现。  
	myproc = proc_mkdir("myproc",NULL);//创建proc下目录文件  
	proc_create("task", 0, myproc, &hello_proc_fops);//创建proc下文件  
	proc_remove(myproc);//删除创建在proc下的目录文件  
	seq_printf(m,"Task!\n");//写入proc文件中  
	module_param(a,int,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);//模块传参  
  MODULE_PARM_DESC(a,"An invisiable int under sysfs");
