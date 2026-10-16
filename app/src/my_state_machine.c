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


enum ascii_state_machine_states{
    FIRST_CHAR,
    MORE_CHARS,
    READY_TO_SEND,
    STANDBY
};

typedef struct{
    struct smf_ctx ctx;

} ascii_object_t;

static const struct smf_state ascii_states[] = {
    [FIRST_CHAR] = SMF_CREATE_STATE(first_char_entry, first_char_run, first_char_exit, NULL, NULL),
    [MORE_CHARS] = SMF_CREATE_STATE(more_chars_entry, more_chars_run, more_chars_exit, NULL, NULL),
    [READY_TO_SEND] = SMF_CREATE_STATE(ready_to_send_entry, ready_to_send_run, ready_to_send_exit, NULL, NULL),
    [STANDBY] = SMF_CREATE_STATE(standby_entry, standby_run, standby_exit, NULL, NULL)
};

static ascii_object_t ascii_object;
void state_machine_init(){
        smf_set_initial(SMF_CTX(&ascii_object), &ascii_states[FIRST_CHAR]);
}

int state_machine_run(){
        return smf_run_state(SMF_CTX(&ascii_object));
}