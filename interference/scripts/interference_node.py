#!/usr/bin/env python

import rospy
from std_msgs.msg import Float64


class Interferencer:

    value_a = None
    publisher = None

    def __init__(self):
        rospy.init_node('interference_node', anonymous=True)

        rospy.Subscriber("wave_a", Float64, callback=self.callback, callback_args="a", queue_size=1)
        rospy.Subscriber("wave_b", Float64, callback=self.callback, callback_args="b", queue_size=1)

        self.publisher = rospy.Publisher("interference", Float64, queue_size=1)

    def callback(self, msg, name):
        if name is "a":
            self.value_a = msg.data
        elif self.value_a:
            self.publisher.publish(msg.data + self.value_a)


if __name__ == '__main__':
    Interferencer()
    rospy.spin()
