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

void tearDown(void)
{
}

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

static const Fsm_state_t state_1 = {NULL, on_state_1, NULL, "STATE_1"};
static const Fsm_state_t state_2 = {NULL, on_state_2, NULL, "STATE_2"};
static const Fsm_state_t state_3 = {NULL, on_state_3, NULL, "STATE_3"};

static const State_transition fsm_states[] = {{(Fsm_state_t *)&state_1, 1, (Fsm_state_t *)&state_1, NULL},
                                              {(Fsm_state_t *)&state_1, 2, (Fsm_state_t *)&state_2, NULL},
                                              {(Fsm_state_t *)&state_2, 3, (Fsm_state_t *)&state_2, NULL},
                                              {(Fsm_state_t *)&state_2, 4, (Fsm_state_t *)&state_3, NULL},
                                              {(Fsm_state_t *)&state_3, 5, (Fsm_state_t *)&state_1, NULL}};

void test_fsm_create_use_delete(void)
{
    int handle = fsm_create((State_transition *)fsm_states, (Fsm_state_t *)&state_1,
                            sizeof(fsm_states) / sizeof(State_transition));
    TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, handle);

    Fsm_state_t *dest = fsm_execute_event(handle, 1);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_1, sizeof(Fsm_state_t));

    dest = fsm_execute_event(handle, 2);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_2, sizeof(Fsm_state_t));

    TEST_ASSERT_EQUAL(1, state_1_calls_counter);
    TEST_ASSERT_EQUAL(1, state_2_calls_counter);

    TEST_ASSERT_EQUAL(0, fsm_delete(handle));
}

void test_fsm_create_delete(void)
{
    int handle = fsm_create((State_transition *)fsm_states, (Fsm_state_t *)&state_1,
                            sizeof(fsm_states) / sizeof(State_transition));
    TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, handle);
    TEST_ASSERT_EQUAL(0, fsm_delete(handle));
}

void test_fsm_create_more_use_delete(void)
{
    int handle = fsm_create((State_transition *)fsm_states, (Fsm_state_t *)&state_1,
                            sizeof(fsm_states) / sizeof(State_transition));
    TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, handle);

    Fsm_state_t *dest = fsm_execute_event(handle, 1);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_1, sizeof(Fsm_state_t));

    dest = fsm_execute_event(handle, 2);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_2, sizeof(Fsm_state_t));

    dest = fsm_execute_event(handle, 2);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_2, sizeof(Fsm_state_t));
    dest = fsm_execute_event(handle, 2);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_2, sizeof(Fsm_state_t));
    dest = fsm_execute_event(handle, 2);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_2, sizeof(Fsm_state_t));
    dest = fsm_execute_event(handle, 2);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_2, sizeof(Fsm_state_t));
    dest = fsm_execute_event(handle, 3);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_2, sizeof(Fsm_state_t));
    dest = fsm_execute_event(handle, 3);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_2, sizeof(Fsm_state_t));
    dest = fsm_execute_event(handle, 3);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_2, sizeof(Fsm_state_t));
    dest = fsm_execute_event(handle, 3);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_2, sizeof(Fsm_state_t));
    dest = fsm_execute_event(handle, 3);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_2, sizeof(Fsm_state_t));

    TEST_ASSERT_EQUAL(1, state_1_calls_counter);
    TEST_ASSERT_EQUAL(6, state_2_calls_counter);

    dest = fsm_execute_event(handle, 4);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_3, sizeof(Fsm_state_t));

    dest = fsm_execute_event(handle, 5);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_1, sizeof(Fsm_state_t));

    TEST_ASSERT_EQUAL(0, fsm_delete(handle));
}

void test_fsm_get_state(void)
{
    int handle = fsm_create((State_transition *)fsm_states, (Fsm_state_t *)&state_3,
                            sizeof(fsm_states) / sizeof(State_transition));
    TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, handle);

    fsm_execute_event(handle, 4);
    Fsm_state_t *dest = fsm_get_current_state(handle);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_3, sizeof(Fsm_state_t));

    fsm_execute_event(handle, 5);
    dest = fsm_get_current_state(handle);
    TEST_ASSERT_EQUAL_MEMORY(dest, &state_1, sizeof(Fsm_state_t));

    TEST_ASSERT_EQUAL(0, fsm_delete(handle));
}

void test_fsm_get_state_2(void)
{
    int handle = fsm_create((State_transition *)fsm_states, (Fsm_state_t *)&state_3,
                            sizeof(fsm_states) / sizeof(State_transition));
    TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, handle);

    fsm_execute_event(handle, 4);
    Fsm_state_t *dest = fsm_get_current_state(FSM_INVALID_HANDLE);
    TEST_ASSERT_EQUAL(NULL, dest);

    TEST_ASSERT_EQUAL(0, fsm_delete(handle));
}

void test_fsm_set_state(void)
{
    int handle = fsm_create((State_transition *)fsm_states, (Fsm_state_t *)&state_3,
                            sizeof(fsm_states) / sizeof(State_transition));
    TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, handle);

    Fsm_state_t *state = fsm_get_current_state(handle);
    TEST_ASSERT_EQUAL_MEMORY(state, &state_3, sizeof(Fsm_state_t));

    bool is_ok = fsm_set_current_state(handle, (Fsm_state_t *)&state_2);
    TEST_ASSERT_EQUAL(true, is_ok);

    state = fsm_get_current_state(handle);
    TEST_ASSERT_EQUAL_MEMORY(state, &state_2, sizeof(Fsm_state_t));

    TEST_ASSERT_EQUAL(0, fsm_delete(handle));
}

void test_fsm_set_state_2(void)
{
    int handle = fsm_create((State_transition *)fsm_states, (Fsm_state_t *)&state_3,
                            sizeof(fsm_states) / sizeof(State_transition));
    TEST_ASSERT_NOT_EQUAL(FSM_INVALID_HANDLE, handle);

    bool is_ok = fsm_set_current_state(FSM_INVALID_HANDLE, (Fsm_state_t *)&state_2);
    TEST_ASSERT_EQUAL(false, is_ok);

    TEST_ASSERT_EQUAL(0, fsm_delete(handle));
}

#endif    // TEST
