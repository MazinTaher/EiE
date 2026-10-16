#include <zephyr/smf.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#include "LED.h"
#include "BTN.h"
#include "my_state_machine.h"

#include <stdbool.h>
#include <string.h>


/*--------------------------------------------------------
 * FUNCTION PROTOTYPES
 *-------------------------------------------------------*/
void first_char_entry(void* o);
static enum smf_state_result first_char_run(void* o);
void first_char_exit(void* o);

void second_char_entry(void* o);
static enum smf_state_result second_char_run(void* o);
void second_char_exit(void* o);

void ready_to_send_entry(void* o);
static enum smf_state_result ready_to_send_run(void* o);
void ready_to_send_exit(void* o);

void ready_to_send_entry(void* o);
static enum smf_state_result ready_to_send_run(void* o);
void ready_to_send_exit(void* o);


enum ascii_state_machine_states{
    FIRST_CHAR,
    MORE_CHARS,
    READY_TO_SEND,
    STANDBY
};

