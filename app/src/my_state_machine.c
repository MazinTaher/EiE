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
static void first_char_entry(void* o);
static enum smf_state_result first_char_run(void* o);
static void first_char_exit(void* o);

static void more_chars_entry(void* o);
static enum smf_state_result more_chars_run(void* o);
static void more_chars_exit(void* o);

static void ready_to_send_entry(void* o);
static enum smf_state_result ready_to_send_run(void* o);
static void ready_to_send_exit(void* o);

static void standby_entry(void* o);
static enum smf_state_result standby_run(void* o);
static void standby_exit(void* o);

/*--------------------------------------------------------
 * Typedefs
 *-------------------------------------------------------*/

enum ascii_state_machine_states{
    FIRST_CHAR,
    MORE_CHARS,
    READY_TO_SEND,
    STANDBY
};

typedef struct{
    struct smf_ctx ctx;

    uint16_t count;
} ascii_object_t;

/*--------------------------------------------------------
 * Variables / Alias's
 *-------------------------------------------------------*/
#define THREE_SECOND_COUNT 3000
static uint8_t cuurent_duty_cycle = 10;
static bool up_pulse = true;
static bool down_pulse = false;
 /*--------------------------------------------------------
 * State table
 *-------------------------------------------------------*/

static const struct smf_state ascii_states[] = {
    [FIRST_CHAR] = SMF_CREATE_STATE(first_char_entry, first_char_run, first_char_exit, NULL, NULL),
    [MORE_CHARS] = SMF_CREATE_STATE(more_chars_entry, more_chars_run, more_chars_exit, NULL, NULL),
    [READY_TO_SEND] = SMF_CREATE_STATE(ready_to_send_entry, ready_to_send_run, ready_to_send_exit, NULL, NULL),
    [STANDBY] = SMF_CREATE_STATE(standby_entry, standby_run, standby_exit, NULL, NULL)
};

static ascii_object_t ascii_object;

/*--------------------------------------------------------
 * FIRST_CHAR STATE
 *-------------------------------------------------------*/
static void first_char_entry(void* o){
    printk("Entered state FIRST_CHAR\n");
}
static enum smf_state_result first_char_run(void* o){
    if(ascii_object.count >= THREE_SECOND_COUNT){
    if (BTN_is_pressed(BTN0) && BTN_is_pressed(BTN1)){
        smf_set_state(SMF_CTX(&ascii_object), &ascii_states[STANDBY]);
        ascii_object.count = 0;
        }
    }       
    ascii_object.count++;
    return SMF_EVENT_HANDLED;
}
static void first_char_exit(void* o){
    // TODO: implement
}

/*--------------------------------------------------------
 * MORE_CHARS STATE 
 *-------------------------------------------------------*/
static void more_chars_entry(void* o){
    // TODO: implement
}
static enum smf_state_result more_chars_run(void* o){
    // TODO: implement
    return 0;
}
static void more_chars_exit(void* o){
    // TODO: implement
}

/*--------------------------------------------------------
 * READY_TO_SEND STATE 
 *-------------------------------------------------------*/
static void ready_to_send_entry(void* o){
    // TODO: implement
}
static enum smf_state_result ready_to_send_run(void* o){
    // TODO: implement
    return 0;
}
static void ready_to_send_exit(void* o){
    // TODO: implement
}
/*--------------------------------------------------------
 * STANDBY STATE 
 *-------------------------------------------------------*/

    static void standby_entry(void* o){
        printk("Entered state STANDBY\n");
        ascii_object.count = 0;

    }
    static enum smf_state_result standby_run(void* o){
        if(cuurent_duty_cycle >= 100){
            up_pulse = false;
            down_pulse = true;
        }

        if(cuurent_duty_cycle <= 0){
            up_pulse = true;
            down_pulse = false;
        }

        if(ascii_object.count >= 1000){
        if(up_pulse && cuurent_duty_cycle < 100){
        cuurent_duty_cycle = cuurent_duty_cycle + 10;
       }

        else if(cuurent_duty_cycle > 0){
        cuurent_duty_cycle = cuurent_duty_cycle - 10;
            }
        }
        else{
            ascii_object.count++;
        }

        LED_pwm(LED0, cuurent_duty_cycle);
        /*
        if(BTN_check_clear_pressed(BTN0) || 
           BTN_check_clear_pressed(BTN1) ||
           BTN_check_clear_pressed(BTN2) ||
           BTN_check_clear_pressed(BTN3)){
            smf_set_state(SMF_CTX(&ascii_object), &ascii_states[FIRST_CHAR]);
           }
            */
        return SMF_EVENT_HANDLED;
    }
    static void standby_exit(void* o){
        printk("Exiting state STANDBY\n");

        LED_set(LED0, LED_OFF);
        LED_set(LED1, LED_OFF);
        LED_set(LED2, LED_OFF);
        LED_set(LED3, LED_OFF);
    }

/*--------------------------------------------------------
 * Public functions
 *-------------------------------------------------------*/
void state_machine_init(){
        ascii_object.count = 0;
        smf_set_initial(SMF_CTX(&ascii_object), &ascii_states[FIRST_CHAR]);
}

int state_machine_run(){
        return smf_run_state(SMF_CTX(&ascii_object));
}