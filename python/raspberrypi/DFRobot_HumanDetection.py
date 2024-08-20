# -*- coding:utf-8 -*-
'''!
    @file DFRobot_HumanDetection.py
    @brief This is the declaration part of the human millimeter-wave driver library
    @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
    @License     The MIT License (MIT)
    @author [tangjie](jie.tang@dfrobot.com)
    @version  V1.0
    @date  2024-06-03
    @url https://github.com/DFRobot/DFRobot_HumanDetection
'''

import serial
import time


TIME_OUT = 5 # Timeout: 5 seconds

CMD_HEAD   = 0
CMD_CONFIG = 1
CMD_CMD    = 2
CMD_LEN_H  = 3
CMD_LEN_L  = 4
CMD_END_H  = 5
CMD_END_L  = 6
CMD_WHITE  = 7
CMD_DATA =8

SleepMode = 2 #sleep mode
FallingMode = 1# Fall mode
'''!
@brief LED light selection
'''
FALLLed  = 1 
HPLed    = 2

'''!
@brief Data about the human body in sleep mode
'''
eHumanPresence = 1
eHumanMovement = 2
eHumanMovingRange = 3
eHumanDistance =4

'''!
@brief Sleep data in sleep mode
'''

eInOrNotInBed = 1
eSleepState = 2
eWakeDuration = 3
eLightsleep = 4
eDeepSleepDuration = 5
eSleepQuality = 6
eSleepDisturbances = 7
eSleepQualityRating = 8
eAbnormalStruggle = 9
eUnattendedState = 10
eAbnormalStruggleSwitch = 11
eUnattendedSwitch = 12
eUnattendedTime = 13
esleepDeadline = 14
eReportingmode =15

'''!
@brief Sleep mode configuration for sleep functionality
'''
eReportingmodeC = 1
eAbnormalStruggleC = 2
eUnattendedStateC = 3
eUnattendedTimeC = 4
esleepDeadlineC = 5

'''!
@brief Human body data in fall mode
'''
eExistence = 1
eMotion = 2
eBodyMove = 3
eTrajectorySwitch = 4
eSeatedHorizontalDistance = 5
eMotionHorizontalDistance = 6

'''!
@brief Get fall data
'''
eFallState = 1
eFallBreakHeight = 2
eHeightRatioSwitch = 3
estaticResidencyState = 4
estaticResidencySwitch = 5
eFallSensitivity = 6

'''!
@brief Human configuration in fall mode
'''
eSeatedHorizontalDistanceC = 1
eMotionHorizontalDistanceC = 2

'''!
@brief Get fall data
'''
eFallBreakHeightC = 1
eHeightRatioSwitchC = 2
eReportFreqC = 3
eReportSwitchC = 4
eAltTimeC = 5
eFallSensitivityC = 6
eResidenceSwitchC = 7
eResidenceTime = 8


class DFRobot_HumanDetection:

    def __init__(self):
        self.ser = serial.Serial('/dev/ttyAMA0', 115200)


    def begin(self):
        '''!
            @fn begin
            @brief Initialize sensor
            @return Initialization status
            @retval 0 Initialization successful
            @retval 1 Initialization failed
        '''
        time.sleep(6)
        senData = bytearray([0x0f])
        cmdBuf = self._getData(0x01,0x83,len(senData),senData)
        if (cmdBuf[0]  != 0xf5):
            time.sleep(0.1)
            return 0
        return 1
    
    def config_work_mode(self, mode):
        '''!
            @fn config_work_mode
            @brief Configure working mode
            @param mode Mode selection
            @return Configuration status
            @retval 0 Mode configuration successful
            @retval 1 Mode configuration failed
        '''
        cmd=[0x53, 0x59, 0x02, 0x08, 0x00, 0x01, 0x00, 0x00, 0x54, 0x43]
        senData = bytearray(1)
        senData[0] = 0x0f
        cmdBuf = self._getData(0x02,0xA8,len(senData),senData)
        if(cmdBuf[0] != 0xf5):
            if(cmdBuf[6] == mode):
                return 0
            else:
                cmd[6] = mode
                cmd[7] = self._sumData(7, cmd)
                self.ser.reset_input_buffer()
                self.ser.write(cmd[:10])
                time.sleep(10)
                self._getData(0x02,0xA8,len(senData),senData)
                if(cmdBuf[6] == mode):
                    return 0
                else:
                    return 1
        else:
            return 1
    
    def get_workmode(self):
        '''!
            @fn get_workmode
            @brief Get working mode
            @return Working mode
        '''
        senData = bytearray([0x0f])
        cmdBuf = self._getData(0x02,0xA8,len(senData),senData)
        if(cmdBuf[0] == 0xf5):
            return 2
        return cmdBuf[6]
    
    def config_LED_light(self,led,sta):
        '''!
            @fn config_LED_light
            @brief Configure LED light
            @param led LED selection
            @param sta 0: on, 1: off
            @return Control status
            @retval 0 Configuration successful
            @retval 1 Configuration failed
        '''
        senData = bytearray(1)
        senData[0] = sta
        if (led == FALLLed):
            cmdBuf = self._getData(0x01,0x04,len(senData),senData)
            if(cmdBuf[0] == 0xf5):
                return 1
        else:
            cmdBuf = self._getData(0x01,0x03,len(senData),senData)
            if(cmdBuf[0] == 0xf5):
                return 1
        return 0
    
    def get_LED_light_state(self,led):
        '''!
            @brief Get LED light state
            @param led LED selection
            @return Light status
            @retval 0 Light off
            @retval 1 Light on
        '''
        senData = bytearray([0x0f])
        if (led == FALLLed):
            cmdBuf = self._getData(0x01,0x84,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                return cmdBuf[6]
        else:
            cmdBuf = self._getData(0x01,0x83,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                return cmdBuf[6]
        return 2
    
    def sensor_ret(self):
        '''!
            @fn sensor_ret
            @brief Reset sensor
            @return Reset status
            @retval 0 Reset successful
            @retval 1 Reset failed
        '''
        senData = bytearray([0x0f])
        cmdBuf = self._getData(0x01,0x02,len(senData),senData)
        if(cmdBuf[0] != 0xf5 ):
            return 0
        return 1

    def sm_human_data(self, hm):
        '''!
            @fn sm_human_data
            @brief Query human-related content in sleep mode
            @param hm Data content selection
        '''
        ret = 0
        senData = bytearray([0x0f])
        if hm == eHumanPresence:
            cmdBuf = self._getData(0x80,0x81,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6] 
        elif hm == eHumanMovement:
            cmdBuf = self._getData(0x80,0x82,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif hm == eHumanMovingRange:
            cmdBuf = self._getData(0x80,0x83,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif hm == eHumanDistance:
            cmdBuf = self._getData(0x80,0x84,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        return ret

    def get_heart_rate(self):
        '''!
            @fn get_heart_rate
            @brief Get heart rate
            @return Heart rate
        '''
        senData = bytearray([0x0f])
        ret = 0xff
        cmdBuf = self._getData(0x85,0x82,len(senData),senData)
        if(cmdBuf[0] != 0xf5 ):
            ret = cmdBuf[6]
        return ret

    def get_breathe_state(self):
        '''!
            @fn getBreatheState
            @brief Get respiratory detection information
            @return Respiratory information
            @retval 1 Normal
            @retval 2 Too fast
            @retval 3 Too slow
            @retval 4 None
        '''
        senData = bytearray([0x0f])
        ret = 0xff
        cmdBuf = self._getData(0x81,0x81,len(senData),senData)
        if(cmdBuf[0] != 0xf5 ):
            ret = cmdBuf[6]
        return ret

    def get_breathe_value(self):
        '''!
            @fn getBreatheValue
            @brief Obtain respiratory value
            @return Respiratory value
        '''
        senData = bytearray([0x0f])
        ret = 0xff
        cmdBuf = self._getData(0x81,0x82,len(senData),senData)
        if(cmdBuf[0] != 0xf5 ):
            ret = cmdBuf[6]
        return ret

    def sm_sleep_data(self, sl):
        '''!
            @fn sm_sleep_data
            @brief Obtain sleep-related data
            @param sl Data to be obtained
            @return Obtained data
        '''
        senData = bytearray([0x0f])
        ret = 0xff
        if sl == eReportingmode:
            cmdBuf = self._getData(0x84,0x8C,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif sl == eInOrNotInBed:
            cmdBuf = self._getData(0x84,0x81,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif sl == eSleepState:
            cmdBuf = self._getData(0x84,0x82,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif sl == eWakeDuration:
            cmdBuf = self._getData(0x84,0x83,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6] | cmdBuf[7]
        elif sl == eLightsleep:
            cmdBuf = self._getData(0x84,0x84,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6] | cmdBuf[7]
        elif sl == eDeepSleepDuration:
            cmdBuf = self._getData(0x84,0x85,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6] | cmdBuf[7]
        elif sl == eSleepQuality:
            cmdBuf = self._getData(0x84,0x86,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif sl == eSleepDisturbances:
            cmdBuf = self._getData(0x84,0x8E,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif sl == eSleepQualityRating:
            cmdBuf = self._getData(0x84,0x90,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif sl == eAbnormalStruggle:
            cmdBuf = self._getData(0x84,0x91,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif sl == eUnattendedState:
            cmdBuf = self._getData(0x84,0x92,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif sl == eAbnormalStruggleSwitch:
            cmdBuf = self._getData(0x84,0x93,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif sl == eUnattendedSwitch:
            cmdBuf = self._getData(0x84,0x94,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif sl == eUnattendedTime:
            cmdBuf = self._getData(0x84,0x95,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif sl == esleepDeadline:
            cmdBuf = self._getData(0x84,0x96,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        return ret

    def config_sleep(self,sl,data):
        '''!
            @fn config_sleep
            @brief Configure sleep mode functionality
            @param sl Function selection
            @param data Configuration data
            @return Configuration status
        '''
        senData = bytearray(1)
        senData[0] = data
        ret = 1
        if sl == eReportingmodeC:
            cmdBuf = self._getData(0x84,0x84,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret =0
        elif sl == eAbnormalStruggleC:
            cmdBuf = self._getData(0x84,0x13,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret =0
        elif sl == eUnattendedStateC:
            cmdBuf = self._getData(0x84,0x14,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret =0
        elif sl == eUnattendedTimeC:
            cmdBuf = self._getData(0x84,0x15,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret =0
        elif sl == esleepDeadlineC:
            cmdBuf = self._getData(0x84,0x16,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret =0
        return ret
    
    def dm_install_angle(self,x,y,z):
        '''!
            @fn dm_install_angle
            @brief Used for radar angle installation settings in fall mode
            @param x x angle
            @param y y angle
            @param z z angle
        '''
        senData = bytearray(6)
        senData[0] = x >> 8 & 0xff
        senData[1] = x & 0xff
        senData[2] = y >> 8 & 0xff
        senData[3] = y & 0xff
        senData[4] = z >> 8 & 0xff
        senData[5] = y & 0xff
        self._getData(0x06,0x01,len(senData),senData)
    
    def dm_get_install_angle(self):
        '''!
            @fn dm_get_install_angle
            @brief Obtain radar installation angle
            @param x x angle
            @param y y angle
            @param z z angle
        '''
        senData = bytearray([0x0f])
        cmdBuf = self._getData(0x06,0x81,len(senData),senData)
        if(cmdBuf[0] != 0xf5 ):
            x = cmdBuf[6]<< 8 | cmdBuf[7]
            y = cmdBuf[8] << 8 | cmdBuf[9]
            z = cmdBuf[10] << 8 | cmdBuf[11]
        else:
            x = 0
            y = 0
            z = 0
        return x,y,z
    
    def dm_install_height(self,he):
        '''!
            @fn dm_install_height
            @brief Set radar installation height
            @param he Installation height
        '''
        senData = bytearray(2)
        senData[0] = he >> 8 & 0xff
        senData[1] = he & 0xff
        self._getData(0x06,0x02,len(senData),senData)

    def dm_getInstall_height(self):
        '''!
            @fn dm_getInstall_height
            @brief Get installation height
            @return Installation height
        '''
        senData = bytearray([0x0f])
        ret = 0
        cmdBuf = self._getData(0x06,0x82,len(senData),senData)
        if(cmdBuf[0] != 0xf5 ):
            ret = cmdBuf[6]<< 8 | cmdBuf[7]
        return ret 
    
    def dm_auto_measure_height(self):
        '''!
            @fn autoMeasureHeight
            @brief Get automatic height measurement data
            @return Automatic height measurement data
        '''
        senData = bytearray([0x0f])
        ret = 0
        cmdBuf = self._getData(0x83,0x90,len(senData),senData)
        if(cmdBuf[0] != 0xf5 ):
            ret = cmdBuf[6]<< 8 | cmdBuf[7]
        return ret 
    
    def dm_humanData(self, dh):
        '''!
            @fn dmHumanData
            @brief Get human-related data in fall detection mode
            @return Retrieved data
        '''
        senData = bytearray([0x0f])
        ret = 0
        if dh == eExistence:
            cmdBuf = self._getData(0x80,0x81,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif dh == eMotion:
            cmdBuf = self._getData(0x80,0x82,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif dh == eBodyMove:
            cmdBuf = self._getData(0x80,0x83,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif dh == eTrajectorySwitch:
            cmdBuf = self._getData(0x80,0x94,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif dh == eSeatedHorizontalDistance:
            cmdBuf = self._getData(0x80,0x8D,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif dh == eMotionHorizontalDistance:
            cmdBuf = self._getData(0x80,0x8e,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        return ret
    
    def track(self):
        '''!
            @fn track
            @param x x coordinate
            @param y y coordinate
        '''
        senData = bytearray([0x0f])
        cmdBuf = self._getData(0x83,0x8e,len(senData),senData)
        if(cmdBuf[0] != 0xf5 ):
            x = cmdBuf[6]<< 8 | cmdBuf[7]
            y = cmdBuf[8] << 8 | cmdBuf[9]
        else:
            x = 0
            y = 0
        return x,y
    
    def track_frequency(self):
        '''!
            @fn trackFrequency
            @brief Get the frequency of track point reporting.
            @return Frequency of track point reporting.
        '''
        senData = bytearray([0x0f])
        cmdBuf = self._getData(0x83,0x93,len(senData),senData)
        ret = 0
        if(cmdBuf[0] != 0xf5 ):
            ret = cmdBuf[6]<<24 | cmdBuf[7]<<16 | cmdBuf[8]<<8 | cmdBuf[9]
        return ret

    def unmannedTime(self):
        '''!
            @fn unmannedTime
            @brief Query unmanned time.
        '''
        senData = bytearray([0x0f])
        cmdBuf = self._getData(0x80,0x92,len(senData),senData)
        ret = 0
        if(cmdBuf[0] != 0xf5 ):
            ret = cmdBuf[6]<<24 | cmdBuf[7]<<16 | cmdBuf[8]<<8 | cmdBuf[9]
        return ret

    def getFall_data(self,dm):
        '''!
            @fn getFallData
            @brief Get fall detection data.
            @param dm Data selection.
            @return Retrieved data.
        '''
        senData = bytearray([0x0f])
        ret = 0
        if dm == eFallState:
            cmdBuf = self._getData(0x83,0x81,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif dm == eFallBreakHeight:
            cmdBuf = self._getData(0x83,0x91,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6] << 8 | cmdBuf[7]
        elif dm == eHeightRatioSwitch:
            cmdBuf = self._getData(0x83,0x95,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif dm == estaticResidencyState:
            cmdBuf = self._getData(0x83,0x85,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif dm == estaticResidencySwitch:
            cmdBuf = self._getData(0x83,0x8B,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        elif dm == eFallSensitivity:
            cmdBuf = self._getData(0x83,0x8D,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = cmdBuf[6]
        return ret 
    
    def get_fall_time(self):
        '''!
            @fn getFallTime
            @brief Get fall duration.
            @return Retrieved fall duration.
        '''
        senData = bytearray([0x0f])
        cmdBuf = self._getData(0x83,0x8c,len(senData),senData)
        ret = 0
        if(cmdBuf[0] != 0xf5 ):
            ret = cmdBuf[6]<<24 | cmdBuf[7]<<16 | cmdBuf[8]<<8 | cmdBuf[9]
        return ret
    
    def static_residency_time(self):
        '''!
            @fn staticResidencyTime
            @brief Query residency duration.
            @return Retrieved residency duration.
        '''
        senData = bytearray([0x0f])
        cmdBuf = self._getData(0x83,0x8a,len(senData),senData)
        ret = 0
        if(cmdBuf[0] != 0xf5 ):
            ret = cmdBuf[6]<<24 | cmdBuf[7]<<16 | cmdBuf[8]<<8 | cmdBuf[9]
        return ret
        
    def accumulated_height_duration(self):
        '''!
            @fn accumulatedHeightDuration
            @brief Query accumulated height duration.
            @return Retrieved accumulated height duration data.
        '''
        senData = bytearray([0x0f])
        cmdBuf = self._getData(0x83,0x8F,len(senData),senData)
        ret = 0
        if(cmdBuf[0] != 0xf5 ):
            ret = cmdBuf[6]<<24 | cmdBuf[7]<<16 | cmdBuf[8]<<8 | cmdBuf[9]
        return ret
    
    def dm_human_config(self,con,data):
        '''!
            @fn dmHumanConfig
            @brief Configure human settings in fall mode.
            @param con Configuration selection.
            @param da Configuration data.
            @return Configuration status.
            @retval 1 Configuration failed.
            @retval 0 Configuration successful.
        '''
        senData = bytearray(2)
        senData[0] = data >> 8 & 0xff
        senData[1] = data & 0xff
        ret = 1
        if con == eSeatedHorizontalDistanceC:
            cmdBuf = self._getData(0x80,0x0D,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = 0
        elif con == eMotionHorizontalDistanceC:
            cmdBuf = self._getData(0x80,0x0E,len(senData),senData)
            if(cmdBuf[0] != 0xf5 ):
                ret = 0
        return ret

    def unattended_time_config(self,time):
        '''!
            @fn unattendedTimeConfig
            @brief Set unattended time query in fall mode.
            @brief time Set time.
            @return Setting status.
            @retval 0 Success.
            @retval 1 Failure.
        '''
        senData = bytearray(2)
        senData[0] = time >> 24 & 0xff
        senData[1] = time >> 16 & 0xff
        senData[2] = time >> 8 & 0xff
        senData[3] = time & 0xff
        ret = 1
        cmdBuf = self._getData(0x80,0x12,len(senData),senData)
        if(cmdBuf[0] != 0xf5 ):
                ret = 0
        return ret 
    
    def dm_fall_config(self,con,data):
        '''!
            @fn dmFallConfig
            @brief Configuration of fall mode.
            @param con Configuration option.
            @param data Configuration data.
            @return Configuration status.
            @retval 0 Success.
            @retval 1 Failure.
        '''
        senData = bytearray(4)
        ret = 1
        if con == eFallBreakHeightC:
            senData[0]= data >>8 & 0xff
            senData[1] = data &0xff
            cmdBuf = self._getData(0x83,0x11,2,senData)
            if cmdBuf[0] != 0xf5:
                ret = 0
        elif con == eHeightRatioSwitchC:
            senData[0]= data & 0xff
            cmdBuf = self._getData(0x83,0x15,1,senData)
            if cmdBuf[0] != 0xf5:
                ret = 0
        elif con == eReportFreqC:
            senData[0] = data >> 24 & 0xff
            senData[1] = data >> 16 & 0xff
            senData[2] = data >> 8 & 0xff
            senData[3] = data & 0xff
            cmdBuf = self._getData(0x83,0x13,4,senData)
            if cmdBuf[0] != 0xf5:
                ret = 0
        elif con == eReportSwitchC:
            senData[0]= data & 0xff
            cmdBuf = self._getData(0x83,0x14,1,senData)
            if cmdBuf[0] != 0xf5:
                ret = 0
        elif con == eAltTimeC:
            senData[0] = data >> 24 & 0xff
            senData[1] = data >> 16 & 0xff
            senData[2] = data >> 8 & 0xff
            senData[3] = data & 0xff
            cmdBuf = self._getData(0x83,0x0f,4,senData)
            if cmdBuf[0] != 0xf5:
                ret = 0
        elif con == eFallSensitivityC:
            senData[0]= data & 0xff
            cmdBuf = self._getData(0x83,0x0D,1,senData)
            if cmdBuf[0] != 0xf5:
                ret = 0
        elif con == eResidenceSwitchC:
            senData[0]= data & 0xff
            cmdBuf = self._getData(0x83,0x0b,1,senData)
            if cmdBuf[0] != 0xf5:
                ret = 0
        elif con == eResidenceTime:
            senData[0] = data >> 24 & 0xff
            senData[1] = data >> 16 & 0xff
            senData[2] = data >> 8 & 0xff
            senData[3] = data & 0xff
            cmdBuf = self._getData(0x83,0x0A,4,senData)
            if cmdBuf[0] != 0xf5:
                ret = 0
        return ret

    def _getData(self, con, cmd, length, senData):
        timeStart = time.time()
        state = CMD_WHITE
        data = 0
        _len = 0
        count = 0
        retData = bytearray(22 + length)
        
        cmdBuf = bytearray(20)
        cmdBuf[0] = 0x53
        cmdBuf[1] = 0x59
        cmdBuf[2] = con
        cmdBuf[3] = cmd
        cmdBuf[4] = (length >> 8) & 0xff
        cmdBuf[5] = length & 0xff
        cmdBuf[6:6+length] = senData
        cmdBuf[6 + length] = self._sumData(6 + length, cmdBuf)
        cmdBuf[7 + length] = 0x54
        cmdBuf[8 + length] = 0x43

        self.ser.reset_input_buffer()
        self.ser.write(cmdBuf[:9 + length])

        while True:
            if(time.time() - timeStart) > 1:
                self.ser.reset_output_buffer()
                self.ser.reset_input_buffer()
                self.ser.write(cmdBuf[:9 + length])
                #timeStart = time.time()

            if self.ser.in_waiting > 0:
                data = self.ser.read(1)[0]
                data = ord(data)
                #print(data)
                #timeStart = time.time()

            if (time.time() - timeStart) > TIME_OUT:
                time.sleep(0.05)
                retData[0] = 0xf5
                return retData

            if state == CMD_WHITE:
                if data == 0x53:
                    retData[0] = data
                    state = CMD_HEAD
                    count = 0
            elif state ==CMD_HEAD:
                if data == 0x59:
                    state = CMD_CONFIG
                    retData[1] = data
                else:
                    state = CMD_WHITE
            elif state == CMD_CONFIG:
                if data == con:
                    state = CMD_CMD
                    retData[2] = data
                else:
                    state = CMD_WHITE
            elif state == CMD_CMD:
                if data == cmd:
                    state = CMD_LEN_H
                    retData[3] = data
                else:
                    state = CMD_WHITE
            elif state == CMD_LEN_H:
                retData[4] = data
                _len = data << 8
                state = CMD_LEN_L
            elif state == CMD_LEN_L:
                retData[5] = data
                _len |= data
                state = CMD_DATA
            elif state == CMD_DATA:
                if count < _len:
                    retData[6 + count] = data
                    count += 1
                else:
                    retData[6 + _len] = data
                    state = CMD_END_H
            elif state == CMD_END_H:
                retData[7 + _len] = data
                state = CMD_END_L
            elif state == CMD_END_L:
                retData[8 + _len] = data
                time.sleep(0.05)
                return retData

    def _sumData(self, length, buf):
        data = 0
        for i in range(length):
            data += buf[i]
        return data & 0xff
    