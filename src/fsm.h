#ifndef FSM_H_
#define FSM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#define MAX_FSMS           1
#define FSM_INVALID_HANDLE (-1)
#define FSM_NO_EVENT       0

    typedef struct
    {
        void (*on_enter_state)(void);
        void (*state)(void);
        void (*on_exit_state)(void);
        char name[25];
    } Fsm_state_t;

    typedef struct
    {
        Fsm_state_t *current_state;
        int event;
        Fsm_state_t *next_state;
        void (*on_transition)(void);
    } State_transition;

    int fsm_create(State_transition *states_table_pointer, Fsm_state_t *initial_state, unsigned int max_states);
    int fsm_delete(int handle);
    Fsm_state_t *fsm_execute_event(int handle, int event);

    /**
     * @brief Get pointer on current fsm state.
     * Use it only for tests
     *
     * @param handle number of fsm. You got it from fsm_create()
     * @return Fsm_state_t* pointer on current state or NULL for invalid handle
     */
    Fsm_state_t *fsm_get_current_state(int handle);

    /**
     * @brief Set current state for fsm.
     * Use it only for tests
     *
     * @param handle number of fsm. You got it from fsm_create()
     * @param new_state pointer for new state
     * @return true Success
     * @return false Fail, check your handle
     */
    bool fsm_set_current_state(int handle, Fsm_state_t *new_state);

#ifdef __cplusplus
}
#endif

#endif /* FSM_H_ */
