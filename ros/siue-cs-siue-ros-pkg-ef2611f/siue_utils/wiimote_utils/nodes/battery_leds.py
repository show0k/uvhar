#!/usr/bin/env python

PKG = 'wiimote_utils'
import roslib; roslib.load_manifest(PKG)
import rospy
from sys import argv,exit
from wiimote.msg import State
from wiimote.msg import TimedSwitch
from wiimote.msg import LEDControl

class BatteryMonitor(object):
    def __init__(self):

        rospy.init_node('batt_leds')
        
        self.led_pub = rospy.Publisher('wiimote/leds', LEDControl)
        self.batt_sub = rospy.Subscriber('wiimote/state', State, self.stateCallback);
        self.led_on_msg = TimedSwitch(switch_mode=TimedSwitch.ON)
        self.led_off_msg = TimedSwitch(switch_mode=TimedSwitch.OFF)
        self.led_control = LEDControl()
        
        
    def stateCallback(self, data):
        batt_level = data.percent_battery   
        
        if batt_level > 90:
            #all LED
            self.led_control.timed_switch_array = [self.led_on_msg, self.led_on_msg, self.led_on_msg, self.led_on_msg]
        elif batt_level > 50:
            #3 LED
            self.led_control.timed_switch_array = [self.led_on_msg, self.led_on_msg, self.led_on_msg, self.led_off_msg]
        elif batt_level > 25:
            #2 LED
            self.led_control.timed_switch_array = [self.led_on_msg, self.led_on_msg, self.led_off_msg, self.led_off_msg]
        elif batt_level > 10:
            #1 LED
            self.led_control.timed_switch_array = [self.led_on_msg, self.led_off_msg, self.led_off_msg, self.led_off_msg]
        else:
            #flash LED 0
            flash_msg = TimedSwitch(switch_mode=TimedSwitch.REPEAT, pulse_pattern=[1,1], num_cycles=TimedSwitch.FOREVER)
            self.led_control.timed_switch_array = [flash_msg, self.led_off_msg, self.led_off_msg, self.led_off_msg]
            
            
        #publish result
        self.led_pub.publish(self.led_control)
        
    def disableAllLED(self):
        self.led_control.timed_switch_array = [self.led_off_msg, self.led_off_msg, self.led_off_msg, self.led_off_msg]     
        self.led_pub.publish(self.led_control)
        
        

if __name__ == '__main__':
    rospy.loginfo('Displaying battery state on wiimote LEDs...')
    batt = BatteryMonitor()
    
    rospy.spin()
        
    batt.disableAllLED()
