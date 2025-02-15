

/*---------------2025--2--11-------------*/

 Q: 新增 rt_vsnprintf_full函数导致 ((thread->stat & RT_THREAD_STAT_MASK) == RT_THREAD_SUSPEND) 
    assertion failed at function:_thread_timeout, line number:135 
  
  
  A: 可能是 rt_thread_mdelay(500);这个延时函数只能在 线程中调用 不能在main中调用-----------在main中加入了 rt_thread_mdelay(500); 又可以了
  A1:调整了 main线程 & app_led_control线程 的栈空间 






/*---------------2025--1--19-------------*/

///-add
ha_inner_temperature.c
app_sys_useAge.c




///-mdfc_0928
  1.移植 rtt
  
  2.配置heap宏  通过系统开辟空间
    #define RT_USING_HEAP
	#define RT_USING_MEMHEAP_AS_HEAP
	#define RT_USING_MEMHEAP

/*---------------2024--9--28-------------*/














