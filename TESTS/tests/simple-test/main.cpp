#include "mbed.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

using namespace utest::v1;

static control_t simple_test(const size_t call_count) {
    /* test content here */
    TEST_ASSERT_EQUAL(4, 2 * 2);

    return CaseNext;
}

utest::v1::status_t greentea_setup(const size_t number_of_cases) {
    // here we specify the timeout (60s) and the host runner (the name of our Python file)
    GREENTEA_SETUP(1*60, "default_auto");

    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("simple test", simple_test)
};

Specification specification(greentea_setup, cases);

int main() {
    return !Harness::run(specification);
}
