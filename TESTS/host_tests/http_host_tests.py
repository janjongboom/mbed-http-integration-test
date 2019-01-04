from mbed_host_tests import BaseHostTest
from mbed_host_tests.host_tests_logger import HtrunLogger
import os, urllib2, json

class HTTPHostTests(BaseHostTest):
    mac = ""

    # called when the test starts, value is the MAC address of the device
    def _callback_create_toilet(self, key, value, timestamp):
        self.logger.prn_inf('Received \'create_toilet\' mac=%s' % value)

        self.mac = value

        # by doing a GET request to the URL it will automatically create the toilet
        try:
            urllib2.urlopen("http://ptsv2.com/t/jj" + value).read()
            self.send_kv('create_toilet', 1)
        except Exception as e:
            self.logger.prn_info('Create toilet (%s) failed %s' % (value, str(e)))
            self.send_kv('create_toilet', 0)

    # called after a new sensor value was sent to the cloud, verify if the values match
    def _callback_dumped_message(self, key, value, timestamp):
        self.logger.prn_inf('Received \'dump_message\' value=%s' % value)

        # get the last message sent to the cloud, and compare with the value we received from the device
        raw = urllib2.urlopen("http://ptsv2.com/t/jj" + self.mac + "/d/latest/json").read()
        cloudValue = json.loads(raw)['Body']

        self.logger.prn_inf("Value in the cloud is %s" % cloudValue)

        if (value == cloudValue):
            self.send_kv('dumped_message', 1)
        else:
            self.send_kv('dumped_message', 0)

    def setup(self):
        self.register_callback('create_toilet', self._callback_create_toilet)
        self.register_callback('dumped_message', self._callback_dumped_message)

    def result(self):
        pass

    def teardown(self):
        pass

    def __init__(self):
        super(HTTPHostTests, self).__init__()

        self.logger = HtrunLogger('TEST')
