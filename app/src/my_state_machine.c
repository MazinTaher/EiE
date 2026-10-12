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

#include <stdbool.h>
#include <string.h>

/*--------------------------------------------------------------------
* Function Prototypes
*------------------------------------------------------------------*/
static void locked_state_entry(void* o);
static enum smf_state_result locked_state_run(void* o);
static void locked_state_exit(void* o);

static void pEntry_state_entry(void* o);
static enum smf_state_result pEntry_state_run(void* o);
static void pEntry_state_exit(void* o);

static void waiting_state_entry(void* o);
static enum smf_state_result waiting_state_run(void* o);
static void waiting_state_exit(void* o);

/*--------------------------------------------------------------------
* Typedefs
*------------------------------------------------------------------*/
enum sX_states {
    LOCKED,
    WAITING,
    ENTRY
};

typedef struct {
    struct smf_ctx ctx;

    uint16_t count;
} password_object_t;


/*--------------------------------------------------------------------
* Variables / Alias's
*------------------------------------------------------------------*/
#define PASSWORD_LEN 4
#define THREE_SECOND_COUNT 3000
static char password[5] = "";
static char user_password[5] = "";
static char default_password[5] = "0120";
static bool new_password = false;
static int i = 0;
static int j = 0;

/*--------------------------------------------------------------------
 * State table
 *------------------------------------------------------------------*/
static const struct smf_state password_states_table[] = {
    [LOCKED] = SMF_CREATE_STATE(
        locked_state_entry, 
        locked_state_run, 
        locked_state_exit, 
        NULL, NULL
    ),

   [WAITING] = SMF_CREATE_STATE(
        waiting_state_entry, 
        waiting_state_run, 
        waiting_state_exit,
        NULL, NULL
    ),

    [ENTRY] = SMF_CREATE_STATE(
        pEntry_state_entry, 
        pEntry_state_run, 
        pEntry_state_exit, 
        NULL, NULL
    ),
    
};

static password_object_t password_object;

/*--------------------------------------------------------------------
 * LOCKED state
 *------------------------------------------------------------------*/
static void locked_state_entry(void* o){
    printk("Entered state LOCKED\n");
    LED_set(LED0, LED_ON);
    LED_set(LED1, LED_OFF);
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_OFF);

    BTN_check_clear_pressed(BTN0);
    BTN_check_clear_pressed(BTN1);
    BTN_check_clear_pressed(BTN2);
    BTN_check_clear_pressed(BTN3);
}
static enum smf_state_result locked_state_run(void* o){

    if(password_object.count <= THREE_SECOND_COUNT){
        LED_set(LED3, LED_ON);
        password_object.count ++;
    }
    else{
        LED_set(LED3, LED_OFF);
    }

    if(password_object.count <= THREE_SECOND_COUNT && BTN_check_clear_pressed(BTN3)){
        smf_set_state(SMF_CTX(&password_object), &password_states_table[ENTRY]);
    }
    if(BTN_check_clear_pressed(BTN0) && i < PASSWORD_LEN){
        user_password[i] = '0';
        i++;
        user_password[i] = '\0';
        printk("Password: %s\n", user_password);
    }
    else if(BTN_check_clear_pressed(BTN1) && i < PASSWORD_LEN){
        user_password[i] = '1';
        i++;
        user_password[i] = '\0';
        printk("Password: %s\n", user_password);
    }
    else if(BTN_check_clear_pressed(BTN2) && i < PASSWORD_LEN){
        user_password[i] = '2';
        i++;
        user_password[i] = '\0';
        printk("Password: %s\n", user_password);
    }
    else if(BTN_check_clear_pressed(BTN3)){
       if(!new_password && strcmp(user_password,default_password) == 0){
        printk("Correct!\n");
       }
       else if(new_password && strcmp(user_password, password) == 0){
        printk("Correct!\n");
       }
       else{
        printk("Incorrect!\n");
       }
       smf_set_state(SMF_CTX(&password_object), &password_states_table[WAITING]);
    }
    return SMF_EVENT_HANDLED;
}

static void locked_state_exit(void* o){
    printk("Exiting state LOCKED\n");
    LED_set(LED0, LED_OFF);
    LED_set(LED1, LED_OFF);
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_OFF);
}
 
/*--------------------------------------------------------------------
 * WAITING state
 *------------------------------------------------------------------*/

static void waiting_state_entry(void* o){
    printk("Entered state WAITING\n");
    user_password[0] = '\0';
    i = 0;

    LED_set(LED0, LED_OFF);
    LED_set(LED1, LED_OFF);
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_OFF);

    BTN_check_clear_pressed(BTN0);
    BTN_check_clear_pressed(BTN1);
    BTN_check_clear_pressed(BTN2);
    BTN_check_clear_pressed(BTN3);
}
static enum smf_state_result waiting_state_run(void* o){
   
    if(BTN_check_clear_pressed(BTN0) ||
       BTN_check_clear_pressed(BTN1) ||
       BTN_check_clear_pressed(BTN2) ||
       BTN_check_clear_pressed(BTN3)) {
       smf_set_state(SMF_CTX(&password_object), &password_states_table[LOCKED]);
    }
    return SMF_EVENT_HANDLED;
}

static void waiting_state_exit(void* o){
    printk("Exiting state WAITING\n");
    LED_set(LED0, LED_OFF);
    LED_set(LED1, LED_OFF);
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_OFF);
}

/*--------------------------------------------------------------------
 * ENTRY state
 *------------------------------------------------------------------*/

static void pEntry_state_entry(void* o){
    printk("Entered state ENTRY\n");
    LED_blink(LED3, LED_4HZ);
    password_object.count = 3001;
    password[0] = '\0';
    j = 0;

    BTN_check_clear_pressed(BTN0);
    BTN_check_clear_pressed(BTN1);
    BTN_check_clear_pressed(BTN2);
    BTN_check_clear_pressed(BTN3);
}
static enum smf_state_result pEntry_state_run(void* o){
    if(BTN_check_clear_pressed(BTN3)){
        smf_set_state(SMF_CTX(&password_object), &password_states_table[LOCKED]);
    }

    if(BTN_check_clear_pressed(BTN0) && j < PASSWORD_LEN){
        password[j] = '0';
        j++;
        password[j] = '\0';
        new_password = true;
        printk("Password selection: %s\n", password);
    }
    else if(BTN_check_clear_pressed(BTN1) && j < PASSWORD_LEN){
        password[j] = '1';
        j++;
        password[j] = '\0';
        new_password = true;
        printk("Password selection: %s\n", password);
    }
    else if(BTN_check_clear_pressed(BTN2) && j < PASSWORD_LEN){
        password[j] = '2';
        j++;
        password[j] = '\0';
        new_password = true;
        printk("Password selection: %s\n", password);
    }
    return SMF_EVENT_HANDLED;
}

static void pEntry_state_exit(void* o){
    printk("Exiting state ENTRY\n");
    LED_set(LED0, LED_OFF);
    LED_set(LED1, LED_OFF);
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_OFF);
}

/*--------------------------------------------------------------------
Public functions
*------------------------------------------------------------------*/
void state_machine_init() {
    password_object.count = 0;
    smf_set_initial(SMF_CTX(&password_object), &password_states_table[LOCKED]);
}

int state_machine_run() {
    return smf_run_state(SMF_CTX(&password_object));
}