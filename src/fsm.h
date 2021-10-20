#ifndef FSM_H_
#define FSM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define FSM_INVALID_HANDLE (-1)
#define FSM_NO_EVENT  0

typedef struct
{
    void (*on_enter_state)(void);
    void (*on_state)(void);
    void (*on_exit_state)(void);
    char name[25];
}state_t;

typedef struct
{
    state_t const *current;
    int event;
    state_t const *next;
    void (*on_transition)(void);
}transition_t;


/*
* Returns zero at SUCCESS case
* Returns non-zero at FAIL case
*/
int fsm_create(transition_t const *transition_table, unsigned int transition_table_size, state_t const *initial_state);
int fsm_delete(int handle);
int fsm_set_event(int handle, int event);

/*
* Returns pointer to state_t after event execution
*/
state_t const *fsm_execute(int handle);


/*
* Returns pointer to current state_t
*/
state_t const *fsm_get_current_state(int handle);

#ifdef __cplusplus
}
#endif

#endif /* FSM_H_ */
