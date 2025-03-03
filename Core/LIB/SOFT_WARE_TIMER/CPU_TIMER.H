#ifndef _CPU_TIMER_H
#define _CPU_TIMER_H

//-------------------------------------------CPU_TIMER

#define tbr_g1_STATUS_LEDS 0 
#define tbr_g1_SEC_MAN 1 
#define tbr_g1_SW_READ 2 
#define tbr_g1_TEST 3
#define tbr_g1_MODBUS_SLAVE_RX 4
#define tbr_g1_ESP_RANDOM_CONNECT 5
#define tbr_g1_SIM_UART_GET 6

#define tbrc_s1_ESP_RANDOM_CONNET 0
#define tbrc_s1_ADVANCE_CONNECTION_TEST 1
#define tbrc_s1_MC60_CONECTION_TEST 2
#define tbrc_s1_ESP_SERVER_CHECK_TX 3
#define tbrc_s1_ESP_SERVER_CHECK_RX 4
#define tbrc_s1_ESP_EN_USER 5
#define tbrc_s1_MC60_SERVER_CHECK_TX 6
#define tbrc_s1_MC60_SERVER_CHECK_RX 7

//-------------------------------------------CPU_TIMER_CONFIG

#define def_num_tbrc_s1 10
#define def_num_tbr_g1 10
#define def_num_tb_g2 10

//-------------------------------------------

	void ini_cpu_timer();
	void timer_second_manager();
	void timer_basic_manager();

	struct cpu_timer_basic_10bit_auto_reset	 
	{
		volatile unsigned char F_end : 1;
		volatile unsigned char EN : 1;
		volatile unsigned int I_time : 10;
		volatile unsigned int C_set_time : 10;		
	};
	
	struct  cpu_timer_basic_10bit
	{
		volatile unsigned char F_end :1;
		volatile unsigned char EN :1;
		volatile unsigned int I_time :10;		
	};
	
	struct  cpu_timer_8bit_reset_contorol_Seconds
	{
		volatile unsigned char AUTO : 1;
		volatile unsigned char F_end : 1;
		volatile unsigned char EN : 1;
		volatile unsigned int I_time : 10;
		volatile unsigned int C_set_time : 10;		
	};
	
#endif
