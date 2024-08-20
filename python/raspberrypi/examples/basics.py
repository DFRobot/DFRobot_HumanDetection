# -*- coding:utf-8 -*-
'''!
 @file basics.py
 @brief This is a basic usage example for a human millimeter-wave radar.
 @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 @license     The MIT License (MIT)
 @author [tangjie](jie.tang@dfrobot.com)
 @version  V1.0
 @date  2024-06-03
 @url https://github.com/DFRobot/DFRobot_HumanDetection
'''
from __future__ import print_function
import sys
import os
sys.path.append("../")
import time

from DFRobot_HumanDetection import *

hu = DFRobot_HumanDetection()

def setup():
    while hu.begin() != 0:
        print("init error!!!\n")
        time.sleep(1)
    print("init success\n")
    while (hu.config_work_mode(SleepMode) != 0):
       print("init error\n")
       time.sleep(1)
    print("init success")
    work_mode = hu.get_workmode()
    if work_mode == 1:
       print("Fall detection mode\n")
    elif work_mode == 2:
       print("Sleep mode\n")
    else:
       print("read error\n")
    hu.config_LED_light(HPLed, 1)
    led_mode = hu.get_LED_light_state(HPLed)
    if led_mode == 0:
       print("off\n")
    elif led_mode == 1:
       print("on\n")
    else:
       print("read error\n")



  
def loop():
    print("Presence information:")
    hu_data = hu.sm_human_data(eHumanPresence)
    if hu_data == 0:
       print("No presence detected\n")
    elif hu_data == 1:
       print("Presence detected\n")
    else:
       print("read error\n")
    print("Motion information:")
    hu_data = hu.sm_human_data(eHumanMovement)
    if hu_data == 0:
       print("null\n")
    elif hu_data == 1:
       print("Stationary\n")
    elif hu_data == 2:
       print("Active\n")
    else:
       print("read error\n")
     

if __name__ == "__main__":
  setup()
  while True:
    loop()