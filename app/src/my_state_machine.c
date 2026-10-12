/**
 * @file my_state_machine.c
 */

#include <zephyr/smf.h>

#include "LED.h"
#include "my_state_machine.h"

enum vending_states{
    WAITING,
    DISPENSING
};

typedef struct{
    struct smf_ctx ctx;

} vending_object_t;

static vending_object_t vending_machine;

static void waiting_entry(void* o);
static void dispensing_entry(void* o);

static enum smf_state_result waiting_run_state(void* o);
static enum smf_state_result dispensing_run_state(void* o);

static const struct smf_state vending_states_table[] = {
    [WAITING] = SMF_CREATE_STATE(waiting_entry, waiting_run_state, NULL, NULL, NULL),
    [DISPENSING] = SMF_CREATE_STATE(dispensing_entry, dispensing_run_state, NULL, NULL, NULL)
};

void state_machine_init(){}

int state_machine_run(){}