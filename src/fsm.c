#include "fsm.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "fsm_config.h"

#define HANDLE_IS_VALID    0
#define HANDLE_IS_INVALID -1

typedef struct
{
    state_t const * current_state;
    unsigned int event;
    unsigned int transition_table_size;
    transition_t const * transition_table;
}fsm_t;

static fsm_t fsm_table[MAX_FSMS];
static int32_t fsm_handle = FSM_INVALID_HANDLE;

static int validate_handle(int handle)
{
    int valid = HANDLE_IS_INVALID;

    if(handle >= 0 && handle < MAX_FSMS)
    {
        if(fsm_table[handle].transition_table != NULL)
        {
            valid = HANDLE_IS_VALID;
        }
    }

    return valid;
}

int fsm_create(transition_t const *transition_table, unsigned int transition_table_size, state_t const *initial_state)
{
    int handle = FSM_INVALID_HANDLE;

    for(int i = 0; i < MAX_FSMS; i++)
    {
        if(fsm_table[i].transition_table == NULL)
        {
            fsm_table[i].current_state = initial_state;
            fsm_table[i].transition_table = transition_table;
            fsm_table[i].transition_table_size = transition_table_size;
            fsm_table[i].event = FSM_NO_EVENT;
            handle = i;
            break;
        }
    }

    return handle;
}

int fsm_delete(int handle)
{
    int ret_val = -1;

    if(validate_handle(handle) == HANDLE_IS_VALID)
    {
        fsm_table[handle].transition_table = NULL;
        ret_val = 0;
    }

    return ret_val;
}

state_t const *fsm_execute(int handle)
{
    fsm_t *fsm = &fsm_table[handle];
    transition_t const * transition_table = (&fsm_table[handle])->transition_table;
    transition_t transition;

    if(validate_handle(handle) == HANDLE_IS_VALID)
    {
        for(int i = 0; i < fsm->transition_table_size; i++)
        {
            if(fsm->current_state == transition_table[i].current &&
                fsm->event == transition_table[i].event)
            {
                transition = transition_table[i];
                fsm->event = FSM_NO_EVENT;

                // Exiting from the current state
                if(transition.current->on_exit_state != NULL &&
                    transition.current != transition.next)
                {
                    transition.current->on_exit_state();
                }
                
                // Call transition callback
                if (transition.on_transition != NULL)
                {
                    transition.on_transition();
                }

                // Entering to the new state
                if(transition.next->on_enter_state != NULL &&
                    transition.current != transition.next)
                {
                    transition.next->on_enter_state();
                }
                
                transition.next->on_state();
                fsm->current_state = transition.next;
                
                break;
            }
        }
    }
    return fsm->current_state;
}


int fsm_set_event(int handle, int event)
{
    fsm_t *fsm = &fsm_table[handle];

    if(validate_handle(handle) == HANDLE_IS_VALID)
    {
        fsm->event = event;
    }
    else
    {
        return -1;
    } 

    return 0;
}

state_t const *fsm_get_current_state(int handle)
{
    fsm_t *fsm = &fsm_table[handle];
    return fsm->current_state;
}
