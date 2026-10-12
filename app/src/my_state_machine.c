
/**
 * @file my_state_machine.c
 */

#include <zephyr/smf.h>
#include <zephyr/sys/printk.h>
#include <stdint.h>

#include "my_state_machine.h"

/*--------------------------------------------------------------------
 * States
 *------------------------------------------------------------------*/
enum vending_states {
    WAITING,
    DISPENSING
};

/*--------------------------------------------------------------------
 * State machine context
 *------------------------------------------------------------------*/
typedef struct {
    struct smf_ctx ctx;
    uint32_t count;
} vending_object_t;

static vending_object_t vending_machine;

/*--------------------------------------------------------------------
 * Function prototypes
 *------------------------------------------------------------------*/
static void waiting_entry(void *o);
static enum smf_state_result waiting_run_state(void *o);

static void dispensing_entry(void *o);
static enum smf_state_result dispensing_run_state(void *o);

/*--------------------------------------------------------------------
 * State table
 *------------------------------------------------------------------*/
static const struct smf_state vending_states_table[] = {
    [WAITING] = SMF_CREATE_STATE(
        waiting_entry,
        waiting_run_state,
        NULL,
        NULL,
        NULL
    ),

    [DISPENSING] = SMF_CREATE_STATE(
        dispensing_entry,
        dispensing_run_state,
        NULL,
        NULL,
        NULL
    )
};

/*--------------------------------------------------------------------
 * WAITING state
 *------------------------------------------------------------------*/
static void waiting_entry(void *o)
{
    printk("WAITING: Waiting for coin\n");
}

static enum smf_state_result waiting_run_state(void *o)
{
    vending_machine.count++;

    /* Simulate a coin being inserted after enough iterations. */
    if (vending_machine.count >= 100) {
        vending_machine.count = 0;

        smf_set_state(
            SMF_CTX(&vending_machine),
            &vending_states_table[DISPENSING]
        );
    }

    return SMF_EVENT_HANDLED;
}

/*--------------------------------------------------------------------
 * DISPENSING state
 *------------------------------------------------------------------*/
static void dispensing_entry(void *o)
{
    printk("DISPENSING: Dispensing snack\n");
}

static enum smf_state_result dispensing_run_state(void *o)
{
    vending_machine.count++;

    /* Simulate finishing dispensing the snack. */
    if (vending_machine.count >= 100) {
        vending_machine.count = 0;

        smf_set_state(
            SMF_CTX(&vending_machine),
            &vending_states_table[WAITING]
        );
    }

    return SMF_EVENT_HANDLED;
}

/*--------------------------------------------------------------------
 * Public functions
 *------------------------------------------------------------------*/
void state_machine_init(void)
{
    vending_machine.count = 0;

    smf_set_initial(
        SMF_CTX(&vending_machine),
        &vending_states_table[WAITING]
    );
}

int state_machine_run(void)
{
    return smf_run_state(SMF_CTX(&vending_machine));
}
