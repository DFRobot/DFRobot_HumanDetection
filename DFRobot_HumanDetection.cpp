/*!
 * @file DFRobot_HumanDetection.cpp
 * @brief 这是人体毫米波驱动库的实现部分
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @License     The MIT License (MIT)
 * @author [tangjie](jie.tang@dfrobot.com)
 * @version  V1.0
 * @date  2024-06-03
 * @url https://github.com/DFRobot/DFRobot_HumanDetection
 */

#include "DFRobot_HumanDetection.h"
#include "stdio.h"

DFRobot_HumanDetection::DFRobot_HumanDetection(Stream *s) : _s(s)
{
}

uint8_t DFRobot_HumanDetection::begin(void)
{
    delay(10000); // Startup initialization wait time
    uint8_t data = 0x0f;
    uint8_t buf[10];
    if (getData(0x01, 0x83, 1, &data, buf) == 0)
    {
        // delay(100);
        return 0;
    }
    return 1;
}

uint8_t DFRobot_HumanDetection::configWorkMode(eWorkMode mode)
{
    uint8_t data = 0x0f;
    uint8_t buf[10];
    uint8_t cmdBuf[10] = {0x53, 0x59, 0x02, 0x08, 0x00, 0x01, 0x00, 0x00, 0x54, 0x43};

    if (getData(0x02, 0xA8, 1, &data, buf) == 0) // Query the current working mode
    {
        if (buf[6] == mode) // Determine whether mode switching is necessary
        {
            return 0;
        }
        else // Mode switch required
        {
            cmdBuf[6] = mode;
            cmdBuf[7] = sumData(7, cmdBuf);
            _s->write(cmdBuf, 10);

            delay(10000); // Waiting for mode switch to start

            if (getData(0x02, 0xA8, 1, &data, buf) == 0) // Query current working mode
            {
                if (buf[6] == mode) // Verify if the mode switch was successful
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            return 1;
        }
    }
    return 1;
}

uint8_t DFRobot_HumanDetection::getWorkMode(void)
{
    uint8_t buf[10];
    uint8_t data = 0x0f, ret = 2;
    if (getData(0x02, 0xA8, 1, &data, buf) == 0)
    {
        ret = buf[6];
    }
    return ret;
}

uint8_t DFRobot_HumanDetection::configLEDLight(eLed led, uint8_t sta)
{
    uint8_t data = sta;
    uint8_t buf[10];
    if (led == eFALLLed)
    {
        if (getData(0x01, 0x04, 1, &data, buf) == 0)
        {
            return 0;
        }
    }
    else
    {
        if (getData(0x01, 0x03, 1, &data, buf) == 0)
        {
            return 0;
        }
    }
    return 1;
}

uint8_t DFRobot_HumanDetection::getLEDLightState(eLed led)
{
    uint8_t data = 0x0f;
    uint8_t buf[10];
    uint8_t ret = 2;
    if (led == eFALLLed)
    {
        if (getData(0x01, 0x84, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
    }
    else
    {
        if (getData(0x01, 0x83, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
    }
    return ret;
}

uint8_t DFRobot_HumanDetection::sensorRet(void)
{
    uint8_t data = 0x0f;
    uint8_t buf[10];
    if (getData(0x01, 0x02, 1, &data, buf) == 0)
    {
        delay(10000);
        return 0;
    }
    return 1;
}

uint16_t DFRobot_HumanDetection::smHumanData(esmHuman hm)
{
    uint8_t data = 0x0f;
    uint8_t buf[15];
    uint16_t ret = 0;
    switch (hm)
    {
    case eHumanPresence:
        if (getData(0x80, 0x81, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;
    case eHumanMovement:
        if (getData(0x80, 0x82, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;
    case eHumanMovingRange:
        if (getData(0x80, 0x83, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;
    case eHumanDistance:
        if (getData(0x80, 0x84, 1, &data, buf) == 0)
        {
            ret = buf[6] << 8 | buf[7];
        }
        break;
    default:
        break;
    }
    return ret;
}

uint8_t DFRobot_HumanDetection::getHeartRate(void)
{
    uint8_t data = 0x0f, ret = 0xff;
    uint8_t buf[15];
    if (getData(0x85, 0x82, 1, &data, buf) == 0)
    {
        ret = buf[6];
    }
    return ret;
}

uint8_t DFRobot_HumanDetection::getBreatheState(void)
{
    uint8_t data = 0x0f, ret = 0xff;
    uint8_t buf[15];
    if (getData(0x81, 0x81, 1, &data, buf) == 0)
    {
        ret = buf[6];
    }
    return ret;
}

uint8_t DFRobot_HumanDetection::getBreatheValue(void)
{
    uint8_t data = 0x0f, ret = 0xff;
    uint8_t buf[15];
    if (getData(0x81, 0x82, 1, &data, buf) == 0)
    {
        ret = buf[6];
    }
    return ret;
}

uint16_t DFRobot_HumanDetection::smSleepData(eSmSleep sl)
{
    uint16_t ret = 0xff;
    uint8_t data = 0x0f;
    uint8_t buf[15];

    switch (sl)
    {
    case eReportingmode:
        if (getData(0x84, 0x8C, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;
    case eInOrNotInBed:
        if (getData(0x84, 0x81, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;
    case eSleepState:
        if (getData(0x84, 0x82, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;
    case eWakeDuration:
        if (getData(0x84, 0x83, 1, &data, buf) == 0)
        {
            ret = buf[6] << 8 | buf[7];
        }
        break;
    case eLightsleep:
        if (getData(0x84, 0x84, 1, &data, buf) == 0)
        {
            ret = buf[6] << 8 | buf[7];
        }
        break;
    case eDeepSleepDuration:
        if (getData(0x84, 0x85, 1, &data, buf) == 0)
        {
            ret = buf[6] << 8 | buf[7];
        }
        break;
    case eSleepQuality:
        if (getData(0x84, 0x86, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;
    case eSleepDisturbances:
        if (getData(0x84, 0x8E, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;
    case eSleepQualityRating:
        if (getData(0x84, 0x90, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;
    case eAbnormalStruggle:
        if (getData(0x84, 0x91, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;
    case eUnattendedState:
        if (getData(0x84, 0x92, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;
    case eAbnormalStruggleSwitch:
        if (getData(0x84, 0x93, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;
    case eUnattendedSwitch:
        if (getData(0x84, 0x94, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;
    case eUnattendedTime:
        if (getData(0x84, 0x95, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;
    case esleepDeadline:
        if (getData(0x84, 0x96, 1, &data, buf) == 0)
        {
            ret = buf[6];
        }
        break;

    default:
        break;
    }
    return ret;
}

sSleepComposite DFRobot_HumanDetection::getSleepComposite(void)
{
    uint8_t data = 0x0f;
    uint8_t buf[20];
    sSleepComposite sleepData = {
        .presence = 0,
        .sleepState = 0,
        .averageRespiration = 0,
        .averageHeartbeat = 0,
        .turnoverNumber = 0,
        .largeBodyMove = 0,
        .minorBodyMove = 0,
        .apneaEvents = 0,
    };
    if (getData(0x84, 0x8D, 1, &data, buf) == 0)
    {
        memcpy(&sleepData, &buf[6], sizeof(sSleepComposite));
    }
    return sleepData;
}

sSleepStatistics DFRobot_HumanDetection::getSleepStatistics(void)
{
    uint8_t data = 0x0f;
    uint8_t buf[22];
    sSleepStatistics sleepSta = {
        .sleepQualityScore = 0,
        .sleepTime = 0,
        .wakeDuration = 0,
        .shallowSleepPercentage = 0,
        .deepSleepPercentage = 0,
        .timeOutOfBed = 0,
        .exitCount = 0,
        .turnOverCount = 0,
        .averageRespiration = 0,
        .averageHeartbeat = 0,
        .apneaEvents = 0,
    };

    if (getData(0x84, 0x8F, 1, &data, buf) == 0)
    {
        memcpy(&sleepSta, &buf[6], sizeof(sSleepStatistics));
    }
    return sleepSta;
}

uint8_t DFRobot_HumanDetection::configSleep(eSmSleepConfig sl, uint8_t data)
{
    // uint8_t _data = data;
    uint8_t buf[15];
    switch (sl)
    {
    case eReportingmodeC:
        if (getData(0x84, 0x0F, 1, &data, buf) == 0)
        {
            return 0;
        }
        break;
    case eAbnormalStruggleC:
        if (getData(0x84, 0x13, 1, &data, buf) == 0)
        {
            return 0;
        }
        break;
    case eUnattendedStateC:
        if (getData(0x84, 0x14, 1, &data, buf) == 0)
        {
            return 0;
        }
        break;
    case eUnattendedTimeC:
        if (getData(0x84, 0x15, 1, &data, buf) == 0)
        {
            return 0;
        }
        break;
    case esleepDeadlineC:
        if (getData(0x84, 0x16, 1, &data, buf) == 0)
        {
            return 0;
        }
        break;
    default:
        break;
    }
    return 1;
}

void DFRobot_HumanDetection::dmInstallAngle(int16_t x, int16_t y, int16_t z)
{
    uint8_t sendBuf[6];
    uint8_t readBuf[15];
    sendBuf[0] = x >> 8 & 0xff;
    sendBuf[1] = x & 0xff;
    sendBuf[2] = y >> 8 & 0xff;
    sendBuf[3] = y & 0xff;
    sendBuf[4] = z >> 8 & 0xff;
    sendBuf[5] = y & 0xff;
    if (getData(0x06, 0x01, 6, sendBuf, readBuf) == 0)
    {
    }
}

void DFRobot_HumanDetection::dmGetInstallAngle(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t readBuf[15];
    uint8_t data = 0x0f;
    if (getData(0x06, 0x81, 1, &data, readBuf) == 0)
    {
        *x = readBuf[6] << 8 | readBuf[7];
        *y = readBuf[8] << 8 | readBuf[9];
        *z = readBuf[10] << 8 | readBuf[11];
    }
    else
    {
        *x = 0;
        *y = 0;
        *z = 0;
    }
}

void DFRobot_HumanDetection::dmInstallHeight(uint16_t he)
{
    uint8_t buf[2];
    uint8_t readBuf[15];
    buf[0] = he >> 8 & 0xff;
    buf[1] = he & 0xff;
    getData(0x06, 0x02, 2, buf, readBuf);
}

uint16_t DFRobot_HumanDetection::dmGetInstallHeight(void)
{
    uint8_t readBuf[15];
    uint8_t data = 0x0f;
    uint16_t ret = 0;
    if (getData(0x06, 0x82, 1, &data, readBuf) == 0)
    {
        ret = readBuf[6] << 8 | readBuf[7];
    }
    return ret;
}

uint16_t DFRobot_HumanDetection::dmAutoMeasureHeight(void)
{
    uint8_t readBuf[15];
    uint8_t data = 0x0f;
    uint16_t ret = 0;
    if (getData(0x83, 0x90, 1, &data, readBuf) == 0)
    {
        ret = readBuf[6] << 8 | readBuf[7];
    }
    return ret;
}

uint16_t DFRobot_HumanDetection::dmHumanData(eDmHuman dh)
{
    uint8_t readBuf[15];
    uint8_t data = 0x0f;
    uint16_t ret = 0;
    switch (dh)
    {
    case eExistence:
        if (getData(0x80, 0x81, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6];
        }
        break;
    case eMotion:
        if (getData(0x80, 0x82, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6];
        }
        break;
    case eBodyMove:
        if (getData(0x80, 0x83, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6];
        }
        break;
    case eTrajectorySwitch:
        if (getData(0x83, 0x94, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6];
        }
        break;
    case eSeatedHorizontalDistance:
        if (getData(0x80, 0x8D, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6] << 8 | readBuf[7];
        }
        break;
    case eMotionHorizontalDistance:
        if (getData(0x80, 0x8e, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6] << 8 | readBuf[7];
        }
        break;
    default:
        break;
    }
    return ret;
}

void DFRobot_HumanDetection::track(uint16_t *x, uint16_t *y)
{
    uint8_t readBuf[15];
    uint8_t data = 0x0f;
    if (getData(0x83, 0x8e, 1, &data, readBuf) == 0)
    {
        *x = readBuf[6] << 8 | readBuf[7];
        *y = readBuf[8] << 8 | readBuf[9];
    }
    else
    {
        *x = 0;
        *y = 0;
    }
}

uint32_t DFRobot_HumanDetection::trackFrequency(void)
{
    uint8_t readBuf[15];
    uint8_t data = 0x0f;
    uint32_t ret = 0;
    if (getData(0x83, 0x93, 1, &data, readBuf) == 0)
    {
        ret = readBuf[6] << 24 | readBuf[7] << 16 | readBuf[8] << 8 | readBuf[9];
    }
    return ret;
}

uint32_t DFRobot_HumanDetection::getUnmannedTime(void)
{
    uint8_t readBuf[15];
    uint8_t data = 0x0f;
    uint32_t ret = 0;
    if (getData(0x80, 0x92, 1, &data, readBuf) == 0)
    {
        ret = readBuf[6] << 24 | readBuf[7] << 16 | readBuf[8] << 8 | readBuf[9];
    }
    return ret;
}

void DFRobot_HumanDetection::dmUnmannedTime(uint32_t Time)
{
    uint8_t buf[4];
    uint8_t readBuf[15];
    buf[0] = Time >> 24 & 0xff;
    buf[1] = Time >> 16 & 0xff;
    buf[2] = Time >> 8 & 0xff;
    buf[3] = Time & 0xff;
    getData(0x80, 0x12, 4, buf, readBuf);
}

uint16_t DFRobot_HumanDetection::getFallData(eDmFall dm)
{
    uint8_t readBuf[15];
    uint8_t data = 0x0f;
    uint16_t ret = 0;
    switch (dm)
    {
    case eFallState:
        if (getData(0x83, 0x81, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6];
        }
        break;
    case eFallBreakHeight:
        if (getData(0x83, 0x91, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6] << 8 | readBuf[7];
        }
        break;
    case eHeightRatioSwitch:
        if (getData(0x83, 0x95, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6];
        }
        break;
    case estaticResidencyState:
        if (getData(0x83, 0x85, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6];
        }
        break;
    case estaticResidencySwitch:
        if (getData(0x83, 0x8B, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6];
        }
        break;
    case eFallSensitivity:
        if (getData(0x83, 0x8D, 1, &data, readBuf) == 0)
        {
            ret = readBuf[6];
        }
        break;
    default:
        break;
    }
    return ret;
}

uint32_t DFRobot_HumanDetection::getFallTime(void)
{
    uint8_t readBuf[15];
    uint8_t data = 0x0f;
    uint32_t ret = 0;
    if (getData(0x83, 0x8c, 1, &data, readBuf) == 0)
    {
        ret = readBuf[6] << 24 | readBuf[7] << 16 | readBuf[8] << 8 | readBuf[9];
    }
    return ret;
}

void DFRobot_HumanDetection::dmFallTime(uint32_t Time)
{
    uint8_t buf[4];
    uint8_t readBuf[15];
    buf[0] = Time >> 24 & 0xff;
    buf[1] = Time >> 16 & 0xff;
    buf[2] = Time >> 8 & 0xff;
    buf[3] = Time & 0xff;
    getData(0x83, 0x0C, 4, buf, readBuf);
}

uint32_t DFRobot_HumanDetection::getStaticResidencyTime(void)
{
    uint8_t readBuf[15];
    uint8_t data = 0x0f;
    uint32_t ret = 0;
    if (getData(0x83, 0x8a, 1, &data, readBuf) == 0)
    {
        ret = readBuf[6] << 24 | readBuf[7] << 16 | readBuf[8] << 8 | readBuf[9];
    }
    return ret;
}

uint32_t DFRobot_HumanDetection::accumulatedHeightDuration(void)
{
    uint8_t readBuf[15];
    uint8_t data = 0x0f;
    uint32_t ret = 0;
    if (getData(0x83, 0x8F, 1, &data, readBuf) == 0)
    {
        ret = readBuf[6] << 24 | readBuf[7] << 16 | readBuf[8] << 8 | readBuf[9];
    }

    return ret;
}

uint8_t DFRobot_HumanDetection::dmHumanConfig(eDmHumanConfig con, uint16_t data)
{
    uint8_t readBuf[15];
    uint8_t sendBuf[2];
    sendBuf[0] = data >> 8 & 0xff;
    sendBuf[1] = data & 0xff;
    uint8_t ret = 1;
    switch (con)
    {
    case eSeatedHorizontalDistanceC:
        if (getData(0x80, 0x0D, 2, sendBuf, readBuf) == 0)
        {
            ret = 0;
        }
        break;
    case eMotionHorizontalDistanceC:
        if (getData(0x80, 0x0E, 2, sendBuf, readBuf) == 0)
        {
            ret = 0;
        }
        break;
    default:
        break;
    }
    return ret;
}

uint8_t DFRobot_HumanDetection::unattendedTimeConfig(uint32_t time)
{
    uint8_t readBuf[15];
    uint8_t sendBuf[4];
    sendBuf[0] = time >> 24 & 0xff;
    sendBuf[1] = time >> 16 & 0xff;
    sendBuf[2] = time >> 8 & 0xff;
    sendBuf[3] = time & 0xff;
    uint8_t ret = 1;
    if (getData(0x80, 0x12, 4, sendBuf, readBuf) == 0)
    {
        ret = 0;
    }
    return ret;
}

uint8_t DFRobot_HumanDetection::dmFallConfig(eDmFallConfig con, uint32_t data)
{
    uint8_t readBuf[15];
    uint8_t sendBuf[4];
    uint8_t ret = 1;
    switch (con)
    {
    case eFallBreakHeightC:
        sendBuf[0] = (data & 0xffff) >> 8;
        sendBuf[1] = data & 0xff;
        if (getData(0x83, 0x11, 2, sendBuf, readBuf) == 0)
        {
            ret = 0;
        }
        break;
    case eHeightRatioSwitchC:
        sendBuf[0] = data & 0xff;
        if (getData(0x83, 0x15, 1, sendBuf, readBuf) == 0)
        {
            ret = 0;
        }
        break;
    case eReportFreqC:
        sendBuf[0] = data >> 24 & 0xff;
        sendBuf[1] = data >> 16 & 0xff;
        sendBuf[2] = data >> 8 & 0xff;
        sendBuf[3] = data & 0xff;
        if (getData(0x83, 0x13, 4, sendBuf, readBuf) == 0)
        {
            ret = 0;
        }
        break;
    case eReportSwitchC:
        sendBuf[0] = data & 0xff;
        if (getData(0x83, 0x14, 1, sendBuf, readBuf) == 0)
        {
            ret = 0;
        }
        break;
    case eAltTimeC:
        sendBuf[0] = data >> 24 & 0xff;
        sendBuf[1] = data >> 16 & 0xff;
        sendBuf[2] = data >> 8 & 0xff;
        sendBuf[3] = data & 0xff;
        if (getData(0x83, 0x0f, 4, sendBuf, readBuf) == 0)
        {
            ret = 0;
        }
        break;
    case eFallSensitivityC:
        sendBuf[0] = data & 0xff;
        if (getData(0x83, 0x0D, 1, sendBuf, readBuf) == 0)
        {
            ret = 0;
        }
        break;
    case eResidenceSwitchC:
        sendBuf[0] = data & 0xff;
        if (getData(0x83, 0x0b, 1, sendBuf, readBuf) == 0)
        {
            ret = 0;
        }
    case eResidenceTime:
        sendBuf[0] = data >> 24 & 0xff;
        sendBuf[1] = data >> 16 & 0xff;
        sendBuf[2] = data >> 8 & 0xff;
        sendBuf[3] = data & 0xff;
        if (getData(0x83, 0x0A, 4, sendBuf, readBuf) == 0)
        {
            ret = 0;
        }
    default:
        break;
    }
    return ret;
}

uint8_t DFRobot_HumanDetection::getData(uint8_t con, uint8_t cmd, uint16_t len, uint8_t *senData, uint8_t *retData)
{
    uint32_t timeStart = millis();
    uint32_t timeStart1 = 0;
    uint8_t data = 0, state = CMD_WHITE;
    uint16_t _len = 0;
    uint8_t count = 0;

    uint8_t cmdBuf[20];
    cmdBuf[0] = 0x53;
    cmdBuf[1] = 0x59;
    cmdBuf[2] = con;
    cmdBuf[3] = cmd;
    cmdBuf[4] = (len >> 8) & 0xff;
    cmdBuf[5] = len & 0xff;
    memcpy(&cmdBuf[6], senData, len);
    cmdBuf[6 + len] = sumData(6 + len, cmdBuf);
    cmdBuf[7 + len] = 0x54;
    cmdBuf[8 + len] = 0x43;

    while (true)
    {
        if ((millis() - timeStart1) > 1000)
        {
            while (_s->available() > 0)
            {
                _s->read();
            }
            _s->write(cmdBuf, 9 + len);
            timeStart1 = millis();
            count = 0;
        }

        if (_s->available() > 0)
        {
            data = _s->read();
            // DBG(data);
            // timeStart1 = millis();
        }

        // Update timeout check
        if ((millis() - timeStart) > TIME_OUT)
        {
            DBG("Time out");
            return 2;
        }

        switch (state)
        {
        case CMD_WHITE:
            if (data == 0x53)
            {
                retData[0] = data;
                state = CMD_HEAD;
                count = 0; // Reset count
            }
            break;
        case CMD_HEAD:
            if (data == 0x59)
            {
                state = CMD_CONFIG;
                retData[1] = data;
            }
            else
            {
                state = CMD_WHITE;
            }
            break;
        case CMD_CONFIG:
            if (data == con)
            {
                state = CMD_CMD;
                retData[2] = data;
            }
            else
            {
                state = CMD_WHITE;
            }
            break;
        case CMD_CMD:
            if (data == cmd)
            {
                state = CMD_LEN_H;
                retData[3] = data;
            }
            else
            {
                state = CMD_WHITE;
            }
            break;
        case CMD_LEN_H:
            retData[4] = data;
            _len = data << 8;
            state = CMD_LEN_L;
            break;
        case CMD_LEN_L:
            retData[5] = data;
            _len |= data;
            state = CMD_DATA;
            // DBG(_len);
            break;
        case CMD_DATA:
            if (count < _len)
            {
                retData[6 + count] = data;
                count++;
            }
            else
            {
                if (data == sumData(6 + count, retData))
                {
                    retData[6 + _len] = data;
                    state = CMD_END_H;
                }
                else
                {
                    state = CMD_WHITE;
                }
            }
            break;
        case CMD_END_H:
            retData[7 + _len] = data;
            state = CMD_END_L;
            break;
        case CMD_END_L:
            retData[8 + _len] = data;
            delay(50);
            return 0;
        default:
            break;
        }
        delay(50);
    }
    delay(50);
    return 0;
}

uint8_t DFRobot_HumanDetection::sumData(uint8_t len, uint8_t *buf)
{
    uint16_t data = 0;
    uint8_t *_buf = buf;
    for (uint8_t i = 0; i < len; i++)
    {
        data += _buf[i];
    }
    return data & 0xff;
}