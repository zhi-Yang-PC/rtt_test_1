

/*---------------2025--2--11-------------*/

 Q: ���� rt_vsnprintf_full�������� ((thread->stat & RT_THREAD_STAT_MASK) == RT_THREAD_SUSPEND) 
    assertion failed at function:_thread_timeout, line number:135 
  
  
  A: ������ rt_thread_mdelay(500);�����ʱ����ֻ���� �߳��е��� ������main�е���-----------��main�м����� rt_thread_mdelay(500); �ֿ�����
  A1:������ main�߳� & app_led_control�߳� ��ջ�ռ� 






/*---------------2025--1--19-------------*/

///-add
ha_inner_temperature.c
app_sys_useAge.c




///-mdfc_0928
  1.��ֲ rtt
  
  2.����heap��  ͨ��ϵͳ���ٿռ�
    #define RT_USING_HEAP
	#define RT_USING_MEMHEAP_AS_HEAP
	#define RT_USING_MEMHEAP

/*---------------2024--9--28-------------*/














