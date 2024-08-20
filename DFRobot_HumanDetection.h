/*!
 * @file DFRobot_HumanDetection.h
 * @brief This is the declaration part of the human millimeter-wave driver library
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @License     The MIT License (MIT)
 * @author [tangjie](jie.tang@dfrobot.com)
 * @version  V1.0
 * @date  2024-06-03
 * @url https://github.com/DFRobot/DFRobot_HumanDetection
 */

#ifndef _DFROBOT_HUMAN_DETECTION_
#define _DFROBOT_HUMAN_DETECTION_
#include "Arduino.h"

#if (defined ARDUINO_AVR_UNO) && (defined ESP8266)
#include "SoftwareSerial.h"
#else
#include "HardwareSerial.h"
#endif

//#define ENABLE_DBG ///< Uncomment this macro to see detailed runtime process of the program
#ifdef ENABLE_DBG
#define DBG(...)                     \
    {                                \
        Serial.print("[");           \
        Serial.print(__FUNCTION__);  \
        Serial.print("(): ");        \
        Serial.print(__LINE__);      \
        Serial.print(" ] ");         \
        Serial.println(__VA_ARGS__); \
    }
#else
#define DBG(...)
#endif

#define TIME_OUT 5 * 1000

#define CMD_HEAD 0
#define CMD_CONFIG 1
#define CMD_CMD 2
#define CMD_LEN_H 3
#define CMD_LEN_L 4
#define CMD_END_H 5
#define CMD_END_L 6
#define CMD_WHITE 7
#define CMD_DATA 8

/**
 * @brief Sleep composite state data
 */
typedef struct
{
    uint8_t presence;           ///< Presence state
    uint8_t sleepState;         ///< Sleep state
    uint8_t averageRespiration; ///< Average respiration
    uint8_t averageHeartbeat;   ///< Average heartbeat
    uint8_t turnoverNumber;     /// Turnover number
    uint8_t largeBodyMove;      ///< Large body movement percentage
    uint8_t minorBodyMove;      ///< Minor body movement percentage
    uint8_t apneaEvents;        ///< Apnea events

} sSleepComposite;

/**
 * @brief Sleep statistics query
 */
typedef struct
{
    uint8_t sleepQualityScore;      ///< Sleep quality score
    uint16_t sleepTime;             ///< Sleep duration in minutes
    uint8_t wakeDuration;           ///< Wake duration
    uint8_t shallowSleepPercentage; ///< Shallow sleep duration percentage
    uint8_t deepSleepPercentage;    ///< Deep sleep duration percentage
    uint8_t timeOutOfBed;           ///< Time out of bed
    uint8_t exitCount;              ///< Exit count
    uint8_t turnOverCount;          ///< Turnover count
    uint8_t averageRespiration;     ///< Average respiration
    uint8_t averageHeartbeat;       /// Average heartbeat
    uint8_t apneaEvents;            ///< Apnea events

} sSleepStatistics;

class DFRobot_HumanDetection
{
public:
    /**
     * @brief Working mode configuration structure
     */
    typedef enum
    {
        eSleepMode = 0x02,
        eFallingMode = 0x01,
    } eWorkMode;

    /**
     * @brief LED light selection
     */
    typedef enum
    {
        eFALLLed = 0x03,
        eHPLed = 0x04,
    } eLed;

    /**
     * @brief Human-related data in sleep mode
     */
    typedef enum
    {
        eHumanPresence,    ///< Human presence query
        eHumanMovement,    ///< Movement information query
        eHumanMovingRange, ///< Movement distance, range 0~100
        eHumanDistance,

    } esmHuman;

    /**
     * @brief Sleep data in sleep mode
     */
    typedef enum
    {
        // eSleepSwitch,///<Get sleep switch query
        eInOrNotInBed,           ///< Get in or out of bed status
        eSleepState,             ///< Get sleep state
        eWakeDuration,           ///< Get wake duration
        eLightsleep,             ///< Shallow sleep
        eDeepSleepDuration,      ///< Get deep sleep duration
        eSleepQuality,           ///< Get sleep quality
        eSleepDisturbances,      ///< Sleep abnormality query
        eSleepQualityRating,     ///< Sleep quality rating
        eAbnormalStruggle,       ///< Abnormal struggle
        eUnattendedState,        ///< No one timing query
        eAbnormalStruggleSwitch, ///< Abnormal struggle switch query
        eUnattendedSwitch,       ///< No timing switch query
        eUnattendedTime,         ///< No timing time query
        esleepDeadline,          ///< Sleep deadline
        eReportingmode,          // Reporting mode
    } eSmSleep;

    /**
     * @brief Sleep function configuration in sleep mode
     */
    typedef enum
    {
        eReportingmodeC,    ///< Reporting mode
        eAbnormalStruggleC, ///< Abnormal struggle
        eUnattendedStateC,  ///< No timing
        eUnattendedTimeC,   ///< No timing time
        esleepDeadlineC,    ///< Sleep deadline

    } eSmSleepConfig;

    /**
     * @brief Human data in falling mode
     */
    typedef enum
    {
        // eHumanSwitch,///<Get fall mode human presence switch query
        eExistence,                ///< Human presence query
        eMotion,                   ///< Motion query
        eBodyMove,                 ///< Body movement query
        eTrajectorySwitch,         ///< Trajectory point switch query
        eSeatedHorizontalDistance, ///< Seated horizontal distance
        eMotionHorizontalDistance, /// Motion horizontal distance

    } eDmHuman;

    /**
     * @brief Get fall data
     */
    typedef enum
    {
        // eFallSwitch,///<Fall mode switch query
        eFallState,             ///< Fall state
        eFallBreakHeight,       ///< Fall break height
        eHeightRatioSwitch,     ///< Height ratio switch query
        estaticResidencyState,  ///< Static residency state
        estaticResidencySwitch, ///< Static residency switch
        eFallSensitivity,       ///< Fall sensitivity
    } eDmFall;

    /**
     * @brief Human configuration in falling mode
     */
    typedef enum
    {
        eSeatedHorizontalDistanceC, ///< Seated horizontal distance
        eMotionHorizontalDistanceC, /// Motion horizontal distance
    } eDmHumanConfig;

    /**
     * @brief Get fall data
     */
    typedef enum
    {
        eFallBreakHeightC,   ///< Fall break height
        eHeightRatioSwitchC, ///< Height ratio switch
        eReportFreqC,        ///< Trajectory point information reporting frequency
        eReportSwitchC,      ///< Trajectory point reporting switch
        eAltTimeC,           ///< Height cumulative time
        eFallSensitivityC,   ///< Fall sensitivity setting
        eResidenceSwitchC,   ///< Residency switch
        eResidenceTime,      ///< Residency time

    } eDmFallConfig;

    /**
     * @fn DFRobot_HumanDetection
     * @brief Constructor of the millimeter-wave human detection sensor
     * @param s Serial reception object
     */
    DFRobot_HumanDetection(Stream *s);
    ~DFRobot_HumanDetection() {};

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
     * @param led LED selection to turn on
     * @param sta 0: On, 1: Off
     * @return Control status
     * @retval 0 Configuration successful
     * @retval 1 Configuration failed
     */
    uint8_t configLEDLight(eLed led, uint8_t sta);

    /**
     * @fn getLEDLightState
     * @brief Get LED light status
     * @param led LED selection
     * @return Light status
     * @retval 0 Light off
     * @retval 1 Light on
     */
    uint8_t getLEDLightState(eLed led);

    /**
     * @fn sensorRet
     * @brief Reset the sensor
     * @return Reset status
     * @retval 0 Reset successful
     * @retval 1 Reset failed
     */
    uint8_t sensorRet(void);

    /**
     * @fn smHumanData
     * @brief Query human-related content in sleep mode
     * @param hm Data content selection
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
     * @brief Get respiration detection information
     * @return Respiration information
     * @retval 1 Normal
     * @retval 2 Too fast
     * @retval 3 Too slow
     * @retval 4 None
     */
    uint8_t getBreatheState(void);

    /**
     * @fn getBreatheValue
     * @brief Get respiration value
     * @return Respiration value
     */
    uint8_t getBreatheValue(void);

    /**
     * @fn smSleepData
     * @brief Get sleep-related data
     * @param sl Data to retrieve
     * @return Retrieved data
     */
    uint16_t smSleepData(eSmSleep sl);

    /**
     * @fn getSleepComposite
     * @brief Query sleep composite state
     * @return Queryed composite data
     */
    sSleepComposite getSleepComposite(void);

    /**
     * @fn getSleepStatistics
     * @brief Query sleep statistics status
     * @return Queryed sleep statistics data
     */
    sSleepStatistics getSleepStatistics(void);

    /**
     * @fn configSleep
     * @brief Configure sleep mode function
     * @param sl Function selection
     * @param data Configuration data
     * @return Setting status
     */
    uint8_t configSleep(eSmSleepConfig sl, uint8_t data);

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
     * @return Retrieved automatic height measurement data
     */
    uint16_t dmAutoMeasureHeight(void);

    /**
     * @fn dmHumanData
     * @brief Get human-related data in fall mode
     * @return Retrieved data
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
     * @return Retrieved track point reporting frequency
     */
    uint32_t trackFrequency(void);

    /**
     * @fn getUnmannedTime
     * @brief Unmanned time query
     */
    uint32_t getUnmannedTime(void);

    /**
     * @fn UnmannedTime
     * @brief Set unmanned time
     * @param Time Unmanned time
     */
    void dmUnmannedTime(uint32_t Time);

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
     * @return Retrieved fall duration
     */
    uint32_t getFallTime(void);

    /**
     * @fn dmFallTime
     * @brief Set fall duration
     * @param Time Fall duration
     */
    void dmFallTime(uint32_t Time);

    /**
     * @fn getStaticResidencyTime
     * @brief Residency time query
     * @return Retrieved residency time
     */
    uint32_t getStaticResidencyTime(void);

    /**
     * @fn accumulatedHeightDuration
     * @brief Height cumulative time query
     * @return Retrieved height cumulative time data
     */
    uint32_t accumulatedHeightDuration(void);

    /**
     * @fn dmHumanConfig
     * @brief Configure human configuration in fall mode
     * @param con Configuration selection
     * @param da Configuration data
     * @return Configuration status
     */
    uint8_t dmHumanConfig(eDmHumanConfig con, uint16_t data);

    /**
     * @fn unattendedTimeConfig
     * @brief Set unmanned time query in fall mode
     * @brief time Set time
     * @return Setting status
     */
    uint8_t unattendedTimeConfig(uint32_t time);
    /**
     * @fn dmFallConfig
     * @brief Fall mode configuration
     * @param con Configuration option
     * @param data Configuration data
     * @return Configuration status
     *
     */
    uint8_t dmFallConfig(eDmFallConfig con, uint32_t data);

private:
    /**
     * @fn getData
     * @brief Send command and get data
     * @param con Control word
     * @param cmd Command word
     * @param len Data length
     * @param senData Sent data
     * @param retData Returned data
     * @return Communication status
     */
    uint8_t getData(uint8_t con, uint8_t cmd, uint16_t len, uint8_t *senData, uint8_t *retData);
    uint8_t sumData(uint8_t len, uint8_t *buf);
    Stream *_s = NULL;
};

#endif

