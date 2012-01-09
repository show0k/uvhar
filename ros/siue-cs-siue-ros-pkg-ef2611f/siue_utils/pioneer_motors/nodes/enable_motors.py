#!/usr/bin/env python

PKG = 'pioneer_motors'
import roslib; roslib.load_manifest(PKG)
import rospy
from sys import argv,exit
from p2os_driver.msg import MotorState

class MotorsEnable(object):
		def __init__(self):

				rospy.init_node('enable_motors')
				self.motor_pub = rospy.Publisher('/cmd_motor_state', MotorState)
				self.motor_pub.publish(MotorState(1))
				

if __name__ == '__main__':
		rospy.loginfo('Enabling motors...')
		enable = MotorsEnable()
