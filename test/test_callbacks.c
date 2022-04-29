#ifdef TEST

    #include "unity.h"

    #include "fsm.h"

static uint32_t transition_1_calls_counter = 0;
static uint32_t transition_2_calls_counter = 0;
static uint32_t transition_3_calls_counter = 0;

static uint32_t state_1_on_enter_calls_counter = 0;
static uint32_t state_1_on_exit_calls_counter  = 0;
static uint32_t state_2_on_enter_calls_counter = 0;
static uint32_t state_2_on_exit_calls_counter  = 0;

void setUp(void)
{
    transition_1_calls_counter = 0;
    transition_2_calls_counter = 0;
    transition_3_calls_counter = 0;

    state_1_on_enter_calls_counter = 0;
    state_1_on_exit_calls_counter  = 0;
    state_2_on_enter_calls_counter = 0;
    state_2_on_exit_calls_counter  = 0;
}

void tearDown(void)
{
}

static void on_state_1(void)
{
}
static void on_state_1_enter(void)
{
    state_1_on_enter_calls_counter++;
}

static void on_state_1_exit(void)
{
    state_1_on_exit_calls_counter++;
}

static void on_state_2(void)
{
}
static void on_state_2_enter(void)
{
    state_2_on_enter_calls_counter++;
}

static void on_state_2_exit(void)
{
    state_2_on_exit_calls_counter++;
}

static void on_transition_1(void)
{
    transition_1_calls_counter++;
}

static void on_transition_2(void)
{
    transition_2_calls_counter++;
}

static void on_transition_3(void)
{
    transition_3_calls_counter++;
}

static const Fsm_state_t state_1 = {on_state_1_enter, on_state_1, on_state_1_exit, "STATE_1"};
static const Fsm_state_t state_2 = {on_state_2_enter, on_state_2, on_state_2_exit, "STATE_2"};

static const State_transition fsm_states[] = {
    {(Fsm_state_t *)&state_1, 1, (Fsm_state_t *)&state_1, on_transition_1},
    {(Fsm_state_t *)&state_1, 2, (Fsm_state_t *)&state_2, NULL},
    {(Fsm_state_t *)&state_2, 3, (Fsm_state_t *)&state_2, on_transition_2},
    {(Fsm_state_t *)&state_2, 4, (Fsm_state_t *)&state_1, on_transition_3},
};

void test_fsm_on_transitions_callbacks(void)
{
    int handle = fsm_create((State_transition *)fsm_states, (Fsm_state_t *)&state_1,
                            sizeof(fsm_states) / sizeof(State_transition));
    TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, handle);

    fsm_execute_event(handle, 1);
    fsm_execute_event(handle, 1);
    fsm_execute_event(handle, 1);
    fsm_execute_event(handle, 2);
    fsm_execute_event(handle, 2);
    fsm_execute_event(handle, 2);
    fsm_execute_event(handle, 2);
    fsm_execute_event(handle, 3);
    fsm_execute_event(handle, 4);
    fsm_execute_event(handle, 4);

    TEST_ASSERT_EQUAL(3, transition_1_calls_counter);
    TEST_ASSERT_EQUAL(1, transition_2_calls_counter);
    TEST_ASSERT_EQUAL(1, transition_3_calls_counter);

    TEST_ASSERT_EQUAL(0, fsm_delete(handle));
}

void test_fsm_on_enter_exit_callbacks(void)
{
    int handle = fsm_create((State_transition *)fsm_states, (Fsm_state_t *)&state_1,
                            sizeof(fsm_states) / sizeof(State_transition));
    TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, handle);

    fsm_execute_event(handle, 1);
    fsm_execute_event(handle, 1);
    fsm_execute_event(handle, 1);
    fsm_execute_event(handle, 2);
    fsm_execute_event(handle, 2);
    fsm_execute_event(handle, 2);
    fsm_execute_event(handle, 2);
    fsm_execute_event(handle, 3);
    fsm_execute_event(handle, 4);
    fsm_execute_event(handle, 4);

    TEST_ASSERT_EQUAL(1, state_1_on_enter_calls_counter);
    TEST_ASSERT_EQUAL(1, state_1_on_exit_calls_counter);
    TEST_ASSERT_EQUAL(1, state_2_on_enter_calls_counter);
    TEST_ASSERT_EQUAL(1, state_2_on_exit_calls_counter);

    TEST_ASSERT_EQUAL(0, fsm_delete(handle));
}

#endif    // TEST
