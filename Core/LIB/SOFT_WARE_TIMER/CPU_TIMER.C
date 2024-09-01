#include "main.h"
#include "cpu_timer.h"

struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];
struct cpu_timer_basic_10bit tb_g2[def_num_tb_g2];
struct cpu_timer_8bit_reset_contorol_Seconds tbrc_s1[def_num_tbrc_s1];

extern struct counter c;

void ini_cpu_timer(){

	uint16_t counter_t=0;
	
	for(counter_t=0; counter_t<def_num_tbr_g1; counter_t++ ){
		
		tbr_g1[counter_t].C_set_time=0;
		tbr_g1[counter_t].EN =0;
		tbr_g1[counter_t].F_end=0;
		tbr_g1[counter_t].I_time=0;	
	}
	
	for(counter_t=0; counter_t<def_num_tb_g2; counter_t++ ){
		
		tb_g2[counter_t].EN =0;
		tb_g2[counter_t].F_end=0;
		tb_g2[counter_t].I_time=0;	
	}	
	
	for(counter_t=0; counter_t<def_num_tbrc_s1; counter_t++ ){
		
		tbrc_s1[counter_t].EN =0;
		tbrc_s1[counter_t].C_set_time=0;
		tbrc_s1[counter_t].AUTO=0;
		tbrc_s1[counter_t].F_end=0;
		tbrc_s1[counter_t].I_time=0;	
	}	

}

void timer_second_manager(){
	
	uint16_t counter_t=0;
	
	for(counter_t=0; counter_t<def_num_tbrc_s1; counter_t++ ){
		
		if( tbrc_s1[counter_t].EN == 1 ){  
			tbrc_s1[counter_t].I_time++; 
			if( tbrc_s1[counter_t].I_time > tbrc_s1[counter_t].C_set_time ){
				if( tbrc_s1[counter_t].AUTO == 0 )tbrc_s1[counter_t].EN=0; 
				tbrc_s1[counter_t].I_time=0;
				tbrc_s1[counter_t].F_end=1; 		
			}	
		}
		else tbrc_s1[counter_t].I_time=0; 
	}
	
}

void timer_basic_manager(){
	
	volatile int count_timer;
	for(count_timer=0; count_timer<def_num_tbr_g1; count_timer++ ){
		
		if( tbr_g1[count_timer].EN == 1 ){  
			tbr_g1[count_timer].I_time++; 
			if( tbr_g1[count_timer].I_time > tbr_g1[count_timer].C_set_time ){ 
				 tbr_g1[count_timer].I_time=0;
				 tbr_g1[count_timer].F_end=1; 		
			}	
		}
		else tbr_g1[count_timer].I_time=0; 
	}
	
	
	for(count_timer=0; count_timer<def_num_tb_g2; count_timer++ ){
		
		if( tb_g2[count_timer].EN == 1 ){ 
			
			if( tb_g2[count_timer].I_time > 0 ){ tb_g2[count_timer].I_time--; }
			else{
				tb_g2[count_timer].EN=0;
				tb_g2[count_timer].F_end=1; 	
			}	
		}
		else tb_g2[count_timer].I_time=0; 
	}	
		
}