#ifndef FSM_H_
#define FSM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define FSM_NO_STATE {NULL, NULL, NULL, ""}
#define FSM_INVALID_HANDLE (-1)
#define FSM_NO_EVENT  0

typedef struct
{
    void (*on_enter_state)(void);
    void (*state)(void);
    void (*on_exit_state)(void);
    char name[25];
}Fsm_state_t;

typedef struct
{
    Fsm_state_t current_state;
    int event;
    Fsm_state_t next_state;
    void (*on_transition)(void);
}State_transition;


/*
* Returns zero at SUCCESS case
* Returns non-zero at FAIL case
*/
int fsm_create(State_transition *states_table_pointer, Fsm_state_t initial_state, unsigned int max_states);
int fsm_delete(int handle);
int fsm_set_event(int handle, int event);

/*
* Returns pointer to state after event execution
*/
void *fsm_execute(int handle);


/*
* Returns pointer to current state
*/
void *fsm_get_current_state(int handle);

#ifdef __cplusplus
}
#endif

#endif /* FSM_H_ */
