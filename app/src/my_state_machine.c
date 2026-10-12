/**
 * @file my_state_machine.c
 */

#include <zephyr/smf.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#include "LED.h"
#include "BTN.h"
#include "my_state_machine.h"

/*--------------------------------------------------------------------
* Function Prototypes
*------------------------------------------------------------------*/
static void s0_state_entry(void* o);
static enum smf_state_result s0_state_run(void* o);
static void s0_state_exit(void* o);

static void s1_state_entry(void* o);
static enum smf_state_result s1_state_run(void* o);
static void s1_state_exit(void* o);

static void s2_state_entry(void* o);
static enum smf_state_result s2_state_run(void* o);
static void s2_state_exit(void* o);

static void s3_state_entry(void* o);
static enum smf_state_result s3_state_run(void* o);
static void s3_state_exit(void* o);

static void s4_state_entry(void* o);
static enum smf_state_result s4_state_run(void* o);
static void s4_state_exit(void* o);

/*--------------------------------------------------------------------
* Typedefs
*------------------------------------------------------------------*/
enum sX_states {
    S0,
    S1,
    S2,
    S3,
    S4
};

typedef struct {
    // Context variable used by zephyr to track state machine state. Must be first
    struct smf_ctx ctx;

    uint16_t count;
} state_object_t;

/*--------------------------------------------------------------------
 * State table
 *------------------------------------------------------------------*/
static const struct smf_state sX_states_table[] = {
    [S0] = SMF_CREATE_STATE(
        s0_state_entry, 
        s0_state_run, 
        s0_state_exit, 
        NULL, NULL
    ),

   [S1] = SMF_CREATE_STATE(
        s1_state_entry, 
        s1_state_run, 
        s1_state_exit,
        NULL, NULL
    ),

    [S2] = SMF_CREATE_STATE(
        s2_state_entry, 
        s2_state_run, 
        s2_state_exit, 
        NULL, NULL
    ),

    [S3] = SMF_CREATE_STATE(
        s3_state_entry, 
        s3_state_run, 
        s3_state_exit, 
        NULL, NULL
    ),

    [S4] = SMF_CREATE_STATE(
        s4_state_entry, 
        s4_state_run, 
        s4_state_exit, 
        NULL, NULL
    ),
};

static state_object_t sX_state_object;

/*--------------------------------------------------------------------
 * S0 state
 *------------------------------------------------------------------*/
static void s0_state_entry(void* o){
    printk("Entered state s0\n");
    LED_set(LED0, LED_OFF);
    LED_set(LED1, LED_OFF);
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_OFF);

    BTN_check_clear_pressed(BTN0);
    BTN_check_clear_pressed(BTN1);
    BTN_check_clear_pressed(BTN2);
    BTN_check_clear_pressed(BTN3);
}
static enum smf_state_result s0_state_run(void* o){
    if(BTN_check_clear_pressed(BTN0)){
        smf_set_state(SMF_CTX(&sX_state_object), &sX_states_table[S1]);
    }
    return SMF_EVENT_HANDLED;
}

static void s0_state_exit(void* o){
    printk("Exiting state s0\n");
    LED_set(LED0, LED_OFF);
    LED_set(LED1, LED_OFF);
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_OFF);
}

/*--------------------------------------------------------------------
 * S1 state
 *------------------------------------------------------------------*/
static void s1_state_entry(void* o){
    printk("Entered state s1\n");
    LED_blink(LED0, LED_4HZ);

    BTN_check_clear_pressed(BTN0);
    BTN_check_clear_pressed(BTN1);
    BTN_check_clear_pressed(BTN2);
    BTN_check_clear_pressed(BTN3);
}
static enum smf_state_result s1_state_run(void* o){
    if(BTN_check_clear_pressed(BTN1)){
        smf_set_state(SMF_CTX(&sX_state_object), &sX_states_table[S2]);
    }

    else if(BTN_check_clear_pressed(BTN2)){
        smf_set_state(SMF_CTX(&sX_state_object), &sX_states_table[S4]);
    }

    else if(BTN_check_clear_pressed(BTN3)){
        smf_set_state(SMF_CTX(&sX_state_object), &sX_states_table[S0]);
    }
    return SMF_EVENT_HANDLED;
}

static void s1_state_exit(void* o){
    printk("Exiting state s1\n");
    LED_set(LED0, LED_OFF);
    LED_set(LED1, LED_OFF);
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_OFF);
}

/*--------------------------------------------------------------------
 * S2 state
 *------------------------------------------------------------------*/
static void s2_state_entry(void* o){
    sX_state_object.count = 0;
    printk("Entered state s2\n");
    LED_set(LED0, LED_ON);
    LED_set(LED1, LED_OFF);
    LED_set(LED2, LED_ON);
    LED_set(LED3, LED_OFF);

    BTN_check_clear_pressed(BTN0);
    BTN_check_clear_pressed(BTN1);
    BTN_check_clear_pressed(BTN2);
    BTN_check_clear_pressed(BTN3);
}
static enum smf_state_result s2_state_run(void* o){
    sX_state_object.count++;

    if(BTN_check_clear_pressed(BTN3)){
        smf_set_state(SMF_CTX(&sX_state_object), &sX_states_table[S0]);
    }

    else if(sX_state_object.count > 1000){
        smf_set_state(SMF_CTX(&sX_state_object), &sX_states_table[S3]);
    }

    return SMF_EVENT_HANDLED;
}
static void s2_state_exit(void* o){
    printk("Exiting state s2\n");
    LED_set(LED0, LED_OFF);
    LED_set(LED1, LED_OFF);
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_OFF);
}

/*--------------------------------------------------------------------
 * S3 state
 *------------------------------------------------------------------*/
static void s3_state_entry(void* o){
    sX_state_object.count = 0;
    printk("Entered state s3\n");
    LED_set(LED0, LED_OFF);
    LED_set(LED1, LED_ON);
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_ON);

    BTN_check_clear_pressed(BTN0);
    BTN_check_clear_pressed(BTN1);
    BTN_check_clear_pressed(BTN2);
    BTN_check_clear_pressed(BTN3);
}
static enum smf_state_result s3_state_run(void* o){
    sX_state_object.count++;

    if(BTN_check_clear_pressed(BTN3)){
        smf_set_state(SMF_CTX(&sX_state_object), &sX_states_table[S0]);
    }

    else if(sX_state_object.count > 2000){
        smf_set_state(SMF_CTX(&sX_state_object), &sX_states_table[S2]);
    }

    return SMF_EVENT_HANDLED;
}
static void s3_state_exit(void* o){
    printk("Exiting state s3\n");
    LED_set(LED0, LED_OFF);
    LED_set(LED1, LED_OFF);
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_OFF);
}

/*--------------------------------------------------------------------
 * S4 state
 *------------------------------------------------------------------*/
static void s4_state_entry(void* o){
    printk("Entered state s4\n");
    LED_blink(LED0, LED_16HZ);
    LED_blink(LED1, LED_16HZ);
    LED_blink(LED2, LED_16HZ);
    LED_blink(LED3, LED_16HZ);

    BTN_check_clear_pressed(BTN0);
    BTN_check_clear_pressed(BTN1);
    BTN_check_clear_pressed(BTN2);
    BTN_check_clear_pressed(BTN3);
}
static enum smf_state_result s4_state_run(void* o){
    if(BTN_check_clear_pressed(BTN3)){
        smf_set_state(SMF_CTX(&sX_state_object), &sX_states_table[S0]);
    }
    return SMF_EVENT_HANDLED;
}
static void s4_state_exit(void* o){
    printk("Exiting state s4\n");
    LED_set(LED0, LED_OFF);
    LED_set(LED1, LED_OFF);
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_OFF);
}

/*--------------------------------------------------------------------
Public functions
*------------------------------------------------------------------*/
void state_machine_init() {
    sX_state_object.count = 0;
    smf_set_initial(SMF_CTX(&sX_state_object), &sX_states_table[S0]);
}

int state_machine_run() {
    return smf_run_state(SMF_CTX(&sX_state_object));
}