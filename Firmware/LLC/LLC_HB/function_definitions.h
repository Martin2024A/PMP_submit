//###########################################################################
//
// FILE:    function_definitions.h
//
// TITLE:
//
// NOTES:
//  1)
//###########################################################################
//
//  Ver  | dd mmm yyyy | Who  		| Description of changes
// ======|=============|============|========================================
//  00 	   08 28 2015 	 HPCS
//
//  Texas Instruments, Inc
//  Copyright Texas Instruments 2008. All rights reserved.
//###########################################################################

#ifndef FUNCTION_DEFINITIONS_H
#define FUNCTION_DEFINITIONS_H

#include "pmbus_topology.h"


void clear_program_flash(void);
void clear_program_flash_0(void);
void clear_program_flash_1(void);
void clear_program_flash_2(void);
void clear_program_flash_3(void);
void clear_program_flashes(void);
void zero_out_integrity_word_0(void);
void zero_out_integrity_word_1(void);
void zero_out_integrity_word_2(void);
void zero_out_integrity_word_3(void);
void zero_out_integrity_double_word(void);
void zero_out_integrity_word(void);
void UART_auto_cal(void);


void init_DTC(void);
void char_out(char data);
void init_dpwm0(void);
void init_dpwm1(void);
void init_dpwm2(void);
void init_dpwm3(void);
void init_fault_mux(void);
void init_front_end0(void);
void configure_cc_dac_value(void);
void init_front_end2(void);
void init_loop_mux(void);
void init_uart0(void);
void init_uart1(void);
void init_watchdog(void);
void global_disable(void);
void start_up_reset(void);
void init_filter0_states(void);
void handle_regulation_state(void);
void zero_out_integrity_word(void);
void watchdog_reset(void);
void init_interrupt(void);
void init_gpio(void);
void configure_burst_mode(void);
void init_sample_trigger(void);
void init_filter0(void);
void init_filter1(void);
void init_sec_ocp(void);
void init_ovp(void);
void configure_ovp(void);
void init_pri_ocp(void);
void init_adc12(void);
void xn_histogram(void);
void poll_adc(void);
void clear_faults(void);
void uart_transmit_data(void);
void translate_raw_to_text(void);
void uart_process_rx_data(void);
char translate_nybble_out(char byte,char * pchar2 );
char translate_nybble_in(char nyb);
void translate_text_to_raw(void);
void uart_receive_data(void);
void configure_constant_power(void);
void init_filter1_states(void);
void gpio_sr_off(void);
void send_string(const Uint8 string_to_send[], Uint8 num_bytes);
Uint8 start_erase_task(const void* dest_ptr, Uint16 byte_count);
Uint32 calculate_dflash_checksum(Uint8 *start_addr, Uint8 *end_addr);
inline Uint8 calc_flash_segments(const void* dest_ptr, Uint16 byte_count, Uint8* first_segment);
void clear_program_flash(void);
void write_program_flash_word(int * address, int data);
void look_for_interrupted_dflash_erase(void);
Uint8 update_data_flash(void* dest_ptr, const void* src_ptr, Uint16 byte_count);
void erase_task(void);
void erase_one_section(int first_segment, int byte_count);
void copy_coefficients_to_filter(volatile struct FILTER_REGS *dest, const FILTER_PMBUS_REGS *source);
void copy_coefficients_to_ram(volatile FILTER_PMBUS_REGS *dest, volatile struct FILTER_REGS *source);
void configure_vout_ramp_rate(void);
void configure_ton_rise(void);
void handle_faults(void);
void handle_warnings(void);
void handle_pgood(void);
void transition_to_idle_state(void);
void control_sr_on_off(void);
void cccp_dac_adjust(void);

void configure_iout_ocp(void);

struct qnote qnote_scale_int32 (struct qnote x, int32 y);
struct qnote qnote_scale (struct qnote x, int16 y);
struct qnote linear11_to_qnote (int16 linear11);

void configure_dpwm_timing(void);

struct qnote qnote_subtract(struct qnote x, struct qnote y);
int16 qnote_to_linear11 (struct qnote x);

struct qnote qnote_add(struct qnote x, struct qnote y);

void configure_ipri_cycle_by_cycle(Uint16 fault_limit);

void enable_current_sharing(void);
void disable_current_sharing(void);
void handle_current_sharing_average(void);
void handle_current_sharing_master_slave(void);

void set_vout(void);
void init_cpcc(void);

void configure_iin_ocp(void);

void init_variables(void);
void init_ipri_cycle_by_cycle(void);
void init_front_end1(void);

void handle_ramp_up_state(void);
void handle_idle_state(void);
void handle_vout_transition_state(void);
void handle_fault_state(void);
void handle_standard_interrupt_global_tasks(void);
void handle_cpcc_state(void);
void handle_light_load_state(void);
void handle_ramp_down_state(void);

void gpio_dpwm_on(void);
void gpio_dpwm_off(void);
void global_enable(void);
void gpio_sr_on(void);

void stop_filter_states(void);
void start_filter_states(void);

void restore_default_all(void);
void configure_filter_parameters(void);

void init_dpwms(void);

Uint32 qnote_linear11_multiply_fit(struct qnote x, int16 linear11, Uint32 max_value);
Uint32 qnote_linear16_multiply_fit(struct qnote x, Uint16 linear16_mantissa, int8 vout_mode, Uint32 max_value);

void burst_mode_enable(void);
void burst_mode_disable(void);

void configure_vin_on_off_thresholds(void);
void configure_fault_levels(void);
void configure_warning_levels(void);
void configure_pgood_levels(void);
void configure_cs_limits(void);
void configure_vout_timing(void);
void configure_vout_cmd(void);
void rom_back_door(void);
void configure_dpwm_timing_deadtime_update(void);
void configure_uvp(void);
void init_uvp(void);
void init_fault_dpwms(void);

void configure_frequency_switch(void);

void configure_new_compensation(volatile FILTER_PMBUS_REGS *source);
void handle_hiccup_state(void);
void cpcc_adjustment(void);

void init_filter2(void);

void init_dcomp(void);

inline void burst_mode_handler(void);
inline void cc_handler(void);
//flash.c
void load_filter_from_flash(Uint8 loop_index, Uint8 bank_index);
void load_filter_to_cla_gains_in_ram(Uint8 loop_index, Uint8 bank_index);



//interrupts.c
void undefined_instruction_exception(void);
void software_interrupt(Uint32 arg1, Uint32 arg2, Uint32 arg3, Uint8 swi_number);
void abort_prefetch_exception(void);
void abort_data_fetch_exception(void);
void fast_interrupt(void);


//main.c
void main();
void c_int00(void);


//standard_interrupt.c
void average_adc_readings(void);
void standard_interrupt(void);

int32 pmbus_read_two_byte_handler(Uint16 value);
int32 pmbus_read_one_byte_handler(Uint8 value);

#endif //FUNCTION_DEFINITIONS_H

