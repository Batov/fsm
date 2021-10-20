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

static void on_state_1(void)
{
	state_1_calls_counter++;
}

static void on_state_2(void)
{
	state_2_calls_counter++;
}

static void on_state_3(void)
{
	state_3_calls_counter++;
}

static const state_t state_1 = {NULL, on_state_1, NULL, "STATE_1"};
static const state_t state_2 = {NULL, on_state_2, NULL, "STATE_2"};
static const state_t state_3 = {NULL, on_state_3, NULL, "STATE_3"};

static const transition_t fsm_states[] = {
											 {&state_1, 1, &state_1, NULL},
                                        	 {&state_1, 2, &state_2, NULL},
                                        	 {&state_2, 3, &state_2, NULL},
                                        	 {&state_2, 4, &state_3, NULL},
                                        	 {&state_1, 5, &state_1, NULL}
                                        	 };     

void test_fsm_create_use_delete(void)
{
	int handle = fsm_create(fsm_states, sizeof(fsm_states)/sizeof(transition_t), &state_1);
	TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, handle);

	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 1));
	fsm_execute(handle);

	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 2));
	state_t const * dest = fsm_execute(handle);
	TEST_ASSERT_EQUAL(&state_2, dest);

	TEST_ASSERT_EQUAL(1, state_1_calls_counter);
	TEST_ASSERT_EQUAL(1, state_2_calls_counter);

	TEST_ASSERT_EQUAL(0, fsm_delete(handle));
}

void test_fsm_create_delete(void)
{
	int handle = fsm_create(fsm_states, sizeof(fsm_states)/sizeof(transition_t), &state_1);
	TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, handle);
	TEST_ASSERT_EQUAL(0, fsm_delete(handle));
}

void test_fsm_create_more_use_delete(void)
{
	int handle = fsm_create(fsm_states, sizeof(fsm_states)/sizeof(transition_t), &state_1);
	TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, handle);

	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 1));
	fsm_execute(handle);

	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 2));
	state_t const * dest = fsm_execute(handle);
	TEST_ASSERT_EQUAL(&state_2, dest);

	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 2));
	fsm_execute(handle);
	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 2));
	fsm_execute(handle);
	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 2));
	fsm_execute(handle);
	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 2));
	fsm_execute(handle);
	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 3));
	fsm_execute(handle);
	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 3));
	fsm_execute(handle);
	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 3));
	fsm_execute(handle);
	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 3));
	fsm_execute(handle);
	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 3));
	fsm_execute(handle);

	TEST_ASSERT_EQUAL(1, state_1_calls_counter);
	TEST_ASSERT_EQUAL(6, state_2_calls_counter);

	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 4));
	fsm_execute(handle);
	TEST_ASSERT_EQUAL(0, fsm_set_event(handle, 5));
	fsm_execute(handle);

	dest = fsm_execute(handle);
	TEST_ASSERT_EQUAL(&state_3, dest);

	TEST_ASSERT_EQUAL(0, fsm_delete(handle));
}


#endif // TEST
