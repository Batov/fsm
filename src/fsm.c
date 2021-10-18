#include "fsm.h"
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "fsm_config.h"

/**************************************** Local defines ***********************************************/
#define HANDLE_IS_VALID    0
#define HANDLE_IS_INVALID -1

typedef struct
{
    Fsm_state_t *state;
    unsigned int event;
    unsigned int max_states;
    State_transition *states_table_pointer;
}Fsm;

static Fsm fsm_table[MAX_FSMS];
static int32_t fsm_handle = FSM_INVALID_HANDLE;

static int validate_handle(int handle)
{
    int valid = HANDLE_IS_INVALID;

    if(handle >= 0 && handle < MAX_FSMS)
    {
        if(fsm_table[handle].states_table_pointer != NULL)
        {
            valid = HANDLE_IS_VALID;
        }
    }

    return valid;
}


int fsm_create(State_transition const *states_table_pointer, Fsm_state_t const *initial_state, unsigned int max_states)
{
    int handle = FSM_INVALID_HANDLE;

    for(int i = 0; i < MAX_FSMS; i++)
    {
        if(fsm_table[i].states_table_pointer == NULL)
        {
            fsm_table[i].state = (Fsm_state_t *) initial_state;
            fsm_table[i].states_table_pointer = (State_transition *) states_table_pointer;
            fsm_table[i].max_states = max_states;
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
        fsm_table[handle].states_table_pointer = NULL;
        ret_val = 0;
    }

    return ret_val;
}

Fsm_state_t *fsm_execute(int handle)
{
    Fsm *fsm = &fsm_table[handle];
    State_transition *states_table = (&fsm_table[handle])->states_table_pointer;
    State_transition transition;

    if(validate_handle(handle) == HANDLE_IS_VALID)
    {
      for(int i = 0; i < fsm->max_states; i++)
      {
        if(fsm->state->state == states_table[i].current_state->state &&
           fsm->event == states_table[i].event)
        {
            transition = states_table[i];
            fsm->event = FSM_NO_EVENT;

            // Exiting from the current state
            if(transition.current_state->on_exit_state != NULL &&
               transition.current_state->state != transition.next_state->state)
            {
                transition.current_state->on_exit_state();
            }
            
            // Call transition callback
            if (transition.on_transition != NULL)
            {
              transition.on_transition();
            }

            // Entering to the new state
            if(transition.next_state->on_enter_state != NULL &&
               transition.current_state->state != transition.next_state->state)
            {
                transition.next_state->on_enter_state();
            }
            
            assert(transition.next_state->state);
            transition.next_state->state();
            fsm->state = transition.next_state;
            
            break;
          }
      }
    }
    return fsm->state;
}


int fsm_set_event(int handle, int event)
{
    Fsm *fsm = &fsm_table[handle];

    if(validate_handle(handle) == HANDLE_IS_VALID)
    {
        fsm->event = event;
    }
    else
        return -1;

    return 0;
}

Fsm_state_t *fsm_get_current_state(int handle)
{
  Fsm *fsm = &fsm_table[handle];
  return fsm->state;
}
