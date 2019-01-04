# Mbed OS Integration Test example

This is an example application that contains simple integration tests that can be ran with Greentea. It also contains a host runner example that runs on the computer, and can be used to communicate back and forth from the device. More information can be found in the [Testing tools](https://os.mbed.com/docs/latest/tools/testing.html) section in the Mbed OS documentation.

[Here's a video](https://www.youtube.com/watch?v=SGHJiI7BUYM).

## Which tests

There are two tests included:

* [simple-test](TESTS/tests/simple-test) - bare minimum Greentea test.
* [toilet-test](TESTS/tests/toilet-test) - test for the Mbed HTTP library that sends data to the cloud. A [host test](TESTS/host_tests/http_host_tests.py) is included to verify that the right data was sent.

## How to run

1. Clone this repository via Mbed CLI:

    ```
    $ mbed import https://github.com/janjongboom/mbed-http-integration-test
    ```

1. Set your target and compiler:

    ```
    $ mbed target K64F
    $ mbed toolchain GCC_ARM
    ```

1. If you're using WiFi, set your credentials in `mbed_app.json`.
1. If you're not using the built-in network adapter, add the driver, and then select the driver in [TESTS/tests/toilet-test/main.cpp](TESTS/tests/toilet-test/main.cpp). For more information see [IP Networking](https://os.mbed.com/docs/latest/reference/ip-networking.html) in the Mbed OS documentation.
1. Run the tests:

    ```
    $ mbed test -v -n tests-tests-*
    ```

1. Example output:

    ```
    mbedgt: test case report:
    +--------------+---------------+-------------------------+--------------+--------+--------+--------+--------------------+
    | target       | platform_name | test suite              | test case    | passed | failed | result | elapsed_time (sec) |
    +--------------+---------------+-------------------------+--------------+--------+--------+--------+--------------------+
    | K64F-GCC_ARM | K64F          | tests-tests-simple-test | simple test  | 1      | 0      | OK     | 0.0                |
    | K64F-GCC_ARM | K64F          | tests-tests-toilet-test | dump message | 1      | 0      | OK     | 10.77              |
    +--------------+---------------+-------------------------+--------------+--------+--------+--------+--------------------+
    mbedgt: test case results: 2 OK
    mbedgt: completed in 52.50 sec
    ```
