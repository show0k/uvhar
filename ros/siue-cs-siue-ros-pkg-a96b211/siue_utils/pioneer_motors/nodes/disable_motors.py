#!/usr/bin/env python

PKG = 'p2os_motors'
import roslib; roslib.load_manifest(PKG)
import rospy
from sys import argv,exit
from p2os_driver.msg import MotorState

class MotorsEnable(object):
		def __init__(self):

				rospy.init_node('disable_motors')
				self.motor_pub = rospy.Publisher('/cmd_motor_state', MotorState)
				self.motor_pub.publish(MotorState(0))
				

if __name__ == '__main__':
		rospy.loginfo('Disabling motors...')
		enable = MotorsEnable()

