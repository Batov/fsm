/*!
  @file    fsm.h
  @author  Zinoviev Sergey
  @author  Batov Nikita
  @version 
  @date    Oct 30, 2017
  @brief   

  @verbatim
    
  @endverbatim
*/
/*!
  @addtogroup _default_group_
  @{
*/
      
/*!
  @defgroup 
  @brief    
  @{
*/
#ifndef FSM_H_
#define FSM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "test.h"
#define FSM_NO_STATE {NULL, NULL, NULL, "FSM_NO_STATE"}

#define MAX_FSMS 1
#define FSM_INVALID_HANDLE (-1)
#define FSM_NO_EVENT  0

typedef struct
{
    void (*on_enter_state)(void);
    void (*state)(void);
    void (*on_exit_state)(void);
    char end_state[25];
}Fsm_state_t;

typedef struct
{
    Fsm_state_t current_state;
    int event;
    Fsm_state_t next_state;
    void (*on_transition)(void);
}State_transition;

int fsm_create(State_transition *states_table_pointer, Fsm_state_t initial_state, unsigned int max_states);
int fsm_delete(int handle);

#if TEST
char *fsm_execute(int handle);
void fsm_set_current_id(int handle, Fsm_state_t state);
#else
bool fsm_execute(int handle);
#endif
int fsm_set_event(int handle, int event);

// For only test purpuses
uint32_t fsm_get_current_id(int handle);

#ifdef __cplusplus
}
#endif

#endif /* FSM_H_ */

/*!
  @}
*/

/*!
  @}
*/
