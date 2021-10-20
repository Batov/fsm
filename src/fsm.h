#ifndef FSM_H_
#define FSM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * Typical usage:
 *
 * #define LONG_PRESS  1
 * #define NO_ACTIONS  2
 * #define SHORT_PRESS 3
 * 
 * static void on_off(void) {}
 * static void on_on(void) {}
 * static void on_sleep(void) {}
 * 
 * static const state_t off = {NULL, on_off, NULL, "OFF"};
 * static const state_t on = {NULL, on_on, NULL, "ON"};
 * static const state_t sleep = {NULL, on_sleep, NULL, "SLEEP"};
 * 
 * static const transition_t table[] = {
 *									    {&off,   LONG_PRESS,  &on, NULL},
 *                                      {&on,    LONG_PRESS,  &off, NULL},
 *                                      {&on,    NO_ACTIONS,  &sleep, NULL},
 *                                      {&sleep, LONG_PRESS,  &on, NULL},
 *                                      {&sleep, SHORT_PRESS, &on, NULL}
 *                                     };
 * 
 * void main(void)
 * {
 *     int handle = fsm_create(table, sizeof(table)/sizeof(transition_t), &off);
 *     fsm_execute_event(handle, LONG_PRESS);
 *     fsm_execute_event(handle, NO_ACTIONS);
 * }
 * 
 */

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
state_t const *fsm_execute_event(int handle, int event);

/*
 * fsm_set_event(handle, BUTTON_LONG_PRESS_EVENT);
 * fsm_execute(handle);
 * 
 *  is equal
 * 
 * fsm_execute_event(handle, BUTTON_LONG_PRESS_EVENT);
 */


/*
* Returns pointer to current state_t
*/
state_t const *fsm_get_current_state(int handle);


#ifdef __cplusplus
}
#endif

#endif /* FSM_H_ */
