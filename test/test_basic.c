#ifdef TEST

#include "unity.h"

#include "fsm.h"

static uint32_t state_1_calls_counter = 0;
static uint32_t state_2_calls_counter = 0;
static uint32_t state_3_calls_counter = 0;

void setUp(void) 
{
	state_1_calls_counter = 0;
	state_2_calls_counter = 0;
	state_3_calls_counter = 0;
}   

void tearDown(void) {}

static void state_1(void)
{
	state_1_calls_counter++;
}

static void state_2(void)
{
	state_2_calls_counter++;
}

static void state_3(void)
{
	state_3_calls_counter++;
}

#define STATE_1 {NULL, state_1, NULL, "STATE_1"}
#define STATE_2 {NULL, state_2, NULL, "STATE_2"}
#define STATE_3 {NULL, state_3, NULL, "STATE_3"}

static const State_transition fsm_states[] = {
											 {STATE_1, 1, STATE_1, NULL},
                                        	 {STATE_1, 2, STATE_2, NULL},
                                        	 {STATE_2, 3, STATE_2, NULL},
                                        	 {STATE_2, 4, STATE_3, NULL},
                                        	 {STATE_1, 5, STATE_1, NULL}
                                        	 };     

void test_fsm_create_use_delete(void)
{
	Fsm_state_t init_state = {0};
	init_state = *(Fsm_state_t []){STATE_1};
	int handle = fsm_create((State_transition *) fsm_states, init_state, sizeof(fsm_states)/sizeof(State_transition));
	TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, handle);

	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 1));
	fsm_execute(handle);

	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 2));
	void* dest = fsm_execute(handle);
	TEST_ASSERT_EQUAL(state_2, dest);

	TEST_ASSERT_EQUAL(1, state_1_calls_counter);
	TEST_ASSERT_EQUAL(1, state_2_calls_counter);

	TEST_ASSERT_EQUAL(0, fsm_delete(handle));
}

void test_fsm_create_delete(void)
{
	Fsm_state_t init_state = {0};
	init_state = *(Fsm_state_t []){STATE_1};
	int handle = fsm_create((State_transition *) fsm_states, init_state, sizeof(fsm_states)/sizeof(State_transition));
	TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, handle);
	TEST_ASSERT_EQUAL(0, fsm_delete(handle));
}

#endif // TEST
