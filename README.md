# DFRobot_HumanDetection

* [中文版本](./README_CN.md)
  
  This is a driver library for human millimeter-wave detection, capable of detecting respiratory sleep and fall detection functions.
  The respiratory sleep function is based on millimeter-wave radar technology to perceive human biological presence and movement, continuously recording human presence. It real-time judges the sleep status and respiratory heart rate of the target based on changes in body movement amplitude and respiratory rate during sleep. After a sleep period ends, it outputs a sleep score and integrates with health applications based on relevant sleep parameters.
  Fall detection function uses algorithmic judgment based on personnel posture parameters to detect the fall status of the target through changes in speed, distance, posture, and other movements. It continuously monitors whether the target is at risk of falling and responds to prolonged abnormal stationary states with static residence alarms.

![Product Image](./resources/images/SEN0623.png)

## Product Link (https://www.dfrobot.com/product-2861.html)

    SKU: SEN0623

## Table of Contents
  - [Overview](#overview)
  - [Library Installation](#library-installation)
  - [Methods](#methods)
  - [Compatibility](#compatibility)
  - [Version](#version)
  - [Authors](#authors)

## Overview
    This is a driver library for human millimeter-wave detection, capable of detecting respiratory sleep and fall detection functions.

## Library Installation

Before using this library, download the library files and paste them into the \Arduino\libraries directory. Then open the examples folder and run the demos in that folder.
## Methods

```C++
    /**
     * @fn begin
     * @brief Initialize the sensor
     * @return Initialization status
     * @retval 0 Initialization successful
     * @retval 1 Initialization failed
    */
    uint8_t begin(void);

    /** 
     * @fn configWorkMode
     * @brief Initialize mode
     * @param mode Mode selection
     * @return Initialization status
     * @retval 0 Mode configuration successful
     * @retval 1 Mode configuration failed
    */
    uint8_t configWorkMode(eWorkMode mode);

    /**
     * @fn getWorkMode
     * @brief Get working mode
     * @return Working mode
    */
    uint8_t getWorkMode(void);

    /**
     * @fn configLEDLight
     * @brief Configure LED light
     * @param led LED selection
     * @param sta 0: On, 1: Off
     * @return Control status
     * @retval 0 Configuration successful
     * @retval 1 Configuration failed
    */
    uint8_t configLEDLight(eLed led, uint8_t sta);

    /**
     * @fn getLEDLightStatic
     * @brief Get LED light status
     * @param led LED selection
     * @return Light status
     * @retval 0 Light off
     * @retval 1 Light on
    */
    uint8_t getLEDLightState(eLed led);

    /**
     * @fn sensorRet
     * @brief Reset sensor
     * @return Reset status
     * @retval 0 Reset successful
     * @retval 1 Reset failed
    */
    uint8_t sensorRet(void);

    /**
     * @fn smHumanData
     * @brief Query human-related data in sleep mode
     * @param hm Data selection
    */
    uint16_t smHumanData(esmHuman hm);

    /**
     * @fn getHeartRate
     * @brief Get heart rate
     * @return Heart rate
    */
    uint8_t getHeartRate(void);

    /**
     * @fn getBreatheState
     * @brief Get breath detection information
     * @return Breath information
     * @retval 1 Normal
     * @retval 2 Too fast
     * @retval 3 Too slow
     * @retval 4 None
    */
    uint8_t getBreatheState(void);

    /**
     * @fn getBreatheValue
     * @brief Get breathing value
     * @return Breathing value
    */
    uint8_t getBreatheValue(void);

    /**
     * @fn getSleepSwitch
     * @brief Get sleep-related data
     * @param sl Data to retrieve
     * @return Retrieved data
    */
    uint16_t smSleepData(eSmSleep sl);

    /**
     * @fn getSleepComposite
     * @brief Query sleep composite status
     * @return Composite query data
    */
    sSleepComposite getSleepComposite(void);

    /**
     * @fn getSleepStatistics
     * @brief Query sleep statistics status
     * @return Sleep statistics data
    */
    sSleepStatistics getSleepStatistics(void);

    /**
     * @fn configSleep
     * @brief Configure sleep mode functionality
     * @param sl Function selection
     * @param data Configuration data
     * @return Configuration status
    */
    uint8_t configSleep(eSmSleepConfig sl,uint8_t data);

    /**
     * @fn installAngle
     * @brief Radar angle installation setting in fall mode
     * @param x x angle
     * @param y y angle
     * @param z z angle
    */
    void dmInstallAngle(int16_t x, int16_t y, int16_t z);

    /**
     * @fn dmGetInstallAngle
     * @brief Get radar installation angle
     * @param x x angle
     * @param y y angle
     * @param z z angle
    */
    void dmGetInstallAngle(int16_t *x, int16_t *y, int16_t *z);

    /**
     * @fn dmInstallHeight
     * @brief Set radar installation height
     * @param he Installation height
    */
    void dmInstallHeight(uint16_t he);

    /**
     * @fn dmGetInstallHeight
     * @brief Get installation height
     * @return Retrieved installation height
    */
    uint16_t dmGetInstallHeight(void);

    /**
     * @fn autoMeasureHeight
     * @brief Get automatic height measurement data
     * @return Automatic height measurement data
    */
    uint16_t dmAutoMeasureHeight(void);

    /**
     * @fn dmHumanData
     * @brief Get human-related data in fall mode
    */
    uint16_t dmHumanData(eDmHuman dh);
    
    /**
     * @fn track
     * @brief Track point query
     * @param x x coordinate
     * @param y y coordinate
    */
    void track(uint16_t *x, uint16_t *y);

    /**
     * @fn trackFrequency
     * @brief Get track point reporting frequency
     * @return Track point reporting frequency
    */
    uint32_t trackFrequency(void);

    /**
     * @fn unmannedTime
     * @brief Unmanned time query
    */
    uint32_t unmannedTime(void);


    /**
     * @fn getFallData
     * @brief Get fall detection function data
     * @param dm Data selection
     * @return Retrieved data
    */
    uint16_t getFallData(eDmFall dm);

    /**
     * @fn getFallTime
     * @brief Get fall duration
     * @return Fall duration
    */
    uint32_t getFallTime(void);

    /**
     * @fn staticResidencyTime
     * @brief Static residency time query
     * @return Residency time
    */
    uint32_t staticResidencyTime(void);

    
    /**
     * @fn accumulatedHeightDuration
     * @brief Accumulated height duration query
     * @return Accumulated height duration data
    */
    uint32_t accumulatedHeightDuration(void);

    /**
     * @fn dmHumanConfig
     * @brief Configure human settings in fall mode
     * @param con Configuration selection
     * @param da Configuration data
     * @return Configuration status
     * @retval 1 Configuration failed
     * @retval 0 Configuration successful
    */
    uint8_t dmHumanConfig(eDmHumanConfig con,uint16_t data);

    /**
     * @fn unattendedTimeConfig
     * @brief Set unattended time query in fall mode
     * @brief time Time to set
     * @return Setting status
     * @retval 0 Success
     * @retval 1 Failed
    */
    uint8_t unattendedTimeConfig(uint32_t time);
    /**
     * @fn dmFallConfig
     * @brief Configure fall mode
     * @param con Configuration option
     * @param data Configuration data
     * @return Configuration status
     * @retval 0 Success
     * @retval 1 Failed
     * 
    */
    uint8_t dmFallConfig(eDmFallConfig con, uint32_t data);

```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Mega2560           |      √       |              |             | 
Leonardo           |      √       |              |             | 
ESP32              |      √       |              |             | 

## Version

- 2024/6/3 - Version V1.0.0

## Creator

Written by TangJie (jie.tang@dfrobot.com), 2019. (Welcome to our [website](https://www.dfrobot.com/))





