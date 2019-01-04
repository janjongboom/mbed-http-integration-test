#include <algorithm>
#include <string>
#include "mbed.h"
#include "http_request.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

using namespace utest::v1;

// send a message to the host, and wait until the host responds with the same key and value '1'
static void send_confirmed_message(const char *key, const char *value) {
    char _key[20] = { 0 };
    char _value[128] = { 0 };

    // send to the host
    greentea_send_kv(key, value);

    // wait until we get a message back
    // if this takes too long the timeout will trigger, so no need to handle this here
    while (1) {
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

        // the key should be the same as 'key', with value 0 (failed) or 1 (success)
        if (strcmp(_key, key) == 0) {
            TEST_ASSERT_EQUAL(1, atoi(_value));
            break;
        }
    }
}

static control_t dump_message(const size_t call_count) {
    // setup a network connection
    NetworkInterface* network = NetworkInterface::get_default_instance();
    TEST_ASSERT(network);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, network->connect());

    // use MAC address to create a new 'toilet', to dump things (at ptsv2)
    std::string mac(network->get_mac_address());
    std::replace(mac.begin(), mac.end(), ':', '_');

    // send the mac address to the host
    send_confirmed_message("create_toilet", mac.c_str());

    // read random noise from the analog pin A0, this is what we'll send to the cloud
    static AnalogIn a0(A0);
    char body[10];
    int length = snprintf(body, 100, "%.03f", a0.read());

    // now we can post a new message
    std::string first("http://ptsv2.com/t/jj");
    std::string last("/post");
    std::string url = first + mac + last;

    HttpRequest req(network, HTTP_POST, url.c_str());
    req.set_header("Content-Type", "text/plain");

    // send and verify that the message was successfully processed
    HttpResponse* res = req.send(body, length);
    TEST_ASSERT(res);
    TEST_ASSERT_EQUAL(200, res->get_status_code());

    // send the value back to the host, to check if it was actually persisted in the cloud
    send_confirmed_message("dumped_message", body);

    return CaseNext;
}

utest::v1::status_t greentea_setup(const size_t number_of_cases) {
    // here we specify the timeout (60s) and the host runner (the name of our Python file)
    GREENTEA_SETUP(1*60, "http_host_tests");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("dump message", dump_message)
};

Specification specification(greentea_setup, cases);

int main() {
    return !Harness::run(specification);
}
