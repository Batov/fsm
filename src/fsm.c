#include "fsm.h"
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

/**************************************** Local defines ***********************************************/
#define HANDLE_IS_VALID    0
#define HANDLE_IS_INVALID -1

typedef struct
{
    Fsm_state_t state;
    unsigned int event;
    unsigned int max_states;
    State_transition *states_table_pointer;
}Fsm;

static Fsm fsm_table[MAX_FSMS];

/*************************************** Local variables **********************************************/
static int32_t fsm_handle = FSM_INVALID_HANDLE;
/*************************************** Local functions **********************************************/
static int validate_handle(int handle);

/***************************************** Code block *************************************************/
int fsm_create(State_transition *states_table_pointer, Fsm_state_t initial_state, unsigned int max_states)
{
    int handle = FSM_INVALID_HANDLE;

    for(int i = 0; i < MAX_FSMS; i++)
    {
        if(fsm_table[i].states_table_pointer == NULL)
        {
            fsm_table[i].state = initial_state;
            fsm_table[i].states_table_pointer = states_table_pointer;
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

bool fsm_execute(int handle)
{
    Fsm *fsm = &fsm_table[handle];
    State_transition *states_table = (&fsm_table[handle])->states_table_pointer;
    State_transition transition;
    bool event_processed = false;

    if(validate_handle(handle) == HANDLE_IS_VALID)
    {
          for(int i = 0; i < fsm->max_states; i++)
          {
              if(fsm->state.state == states_table[i].current_state.state &&
                 fsm->event == states_table[i].event)
              {
                  event_processed = true;
                  transition = states_table[i];
                  fsm->event = FSM_NO_EVENT;

                  // Exiting from the current state
                  if(transition.current_state.on_exit_state != NULL &&
                     transition.current_state.state != transition.next_state.state)
                  {
                      transition.current_state.on_exit_state();
                  }
                  
                  // Call transition callback
                  if (transition.on_transition != NULL)
                  {
                    transition.on_transition();
                  }

                  // Entering to the new state
                  if(transition.next_state.on_enter_state != NULL &&
                     transition.current_state.state != transition.next_state.state)
                  {
                      transition.next_state.on_enter_state();
                  }
                  
                  assert(transition.next_state.state);
                  transition.next_state.state();
                  fsm->state = transition.next_state;
                  
                  break;
                }
          }
    }
    return event_processed;
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

uint32_t fsm_get_current_id(int handle)
{
  Fsm *fsm = &fsm_table[handle];
  return (uint32_t) fsm->state.state;
}

/*!
  @}
*/

/*!
  @}
*/

/*---------------------------------------------------UNITTEST--------------------------------------------------*/
#if TEST

void fsm_set_current_id(int handle, Fsm_state_t state)
{
  Fsm *fsm = &fsm_table[handle];
  Fsm_state_t init_state = {NULL, 0, NULL, "INIT"};
  init_state = *(Fsm_state_t []){state};
  fsm->state = init_state;
}

#if TEST_POS
#endif
#if TEST_NEG
static const char test_tag_neg[] = "[components/fsm_neg]";
TEST_CASE("fsm_create", test_tag_neg)
{
    static State_transition fsm_states[] = {{FSM_NO_STATE, 0, FSM_NO_STATE, NULL}};
    int32_t max_states = INT32_MAX;
    TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, fsm_create(fsm_states, *(Fsm_state_t []){NULL}, max_states));
}
TEST_CASE("fsm_execute", test_tag_neg)
{
    fsm_handle = INT32_MAX;
    fsm_execute(fsm_handle);
    TEST_FAIL();
}
TEST_CASE("fsm_set_event", test_tag_neg)
{
    fsm_handle = INT32_MAX;
    int32_t event = INT32_MAX;
    TEST_ASSERT_EQUAL(-1, fsm_set_event(fsm_handle, event));
}
TEST_CASE("fsm_delete", test_tag_neg)
{
    fsm_handle = INT32_MAX;
    TEST_ASSERT_EQUAL(-1, fsm_delete(fsm_handle));
}
#endif
#endif
