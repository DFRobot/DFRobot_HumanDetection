/**！
 * @file sleep.ino
 * @brief This is an example of sleep detection using human millimeter wave radar.
 * 
 * ---------------------------------------------------------------------------------------------------
 *    board   |             MCU                | Leonardo/Mega2560/M0 | ESP32 |
 *     VCC    |            3.3V/5V             |        VCC           |  VCC  |
 *     GND    |              GND               |        GND           |  GND  |
 *     RX     |              TX                |     Serial1 TX1      |  D2   |
 *     TX     |              RX                |     Serial1 RX1      |  D3   |
 * ---------------------------------------------------------------------------------------------------
 * 
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [tangjie](jie.tang@dfrobot.com)
 * @version  V1.0
 * @date  2024-06-03
 * @url https://github.com/DFRobot/DFRobot_HumanDetection
 */

#include "DFRobot_HumanDetection.h"



DFRobot_HumanDetection hu(&Serial1);

void setup() {
  Serial.begin(115200);

  
  #if defined(ESP32)
  Serial1.begin(115200, SERIAL_8N1, /*rx =*/D3, /*tx =*/D2);
  #else
  Serial1.begin(115200);
  #endif

  Serial.println("Start initialization");
  while (hu.begin() != 0) {
    Serial.println("init error!!!");
    delay(1000);
  }
  Serial.println("Initialization successful");

  Serial.println("Start switching work mode");
  while (hu.configWorkMode(hu.eSleepMode) != 0) {
    Serial.println("error!!!");
    delay(1000);
  }
  Serial.println("Work mode switch successful");

  Serial.print("Current work mode:");
  switch (hu.getWorkMode()) {
    case 1:
      Serial.println("Fall detection mode");
      break;
    case 2:
      Serial.println("Sleep detection mode");
      break;
    default:
      Serial.println("Read error");
  }

  hu.configLEDLight(hu.eHPLed, 1);  // Set HP LED switch
  hu.sensorRet();                   // Module reset, must perform sensorRet after setting data, otherwise the sensor may not be usable

  Serial.print("HP LED status:");
  switch (hu.getLEDLightState(hu.eHPLed)) {
    case 0:
      Serial.println("Off");
      break;
    case 1:
      Serial.println("On");
      break;
    default:
      Serial.println("Read error");
  }

  Serial.println();
  Serial.println();
}

void loop() {
  Serial.print("Bed entry status:");
  switch (hu.smSleepData(hu.eInOrNotInBed)) {
    case 0:
      Serial.println("Out of bed");
      break;
    case 1:
      Serial.println("In bed");
      break;
    default:
      Serial.println("Read error");
  }

  Serial.print("Sleep status:");
  switch (hu.smSleepData(hu.eSleepState)) {
    case 0:
      Serial.println("Deep sleep");
      break;
    case 1:
      Serial.println("Light sleep");
      break;
    case 2:
      Serial.println("Awake");
      break;
    case 3:
      Serial.println("None");
      break;
    default:
      Serial.println("Read error");
  }
  Serial.print("Awake duration: ");
  Serial.println(hu.smSleepData(hu.eWakeDuration));
  Serial.print("Deep sleep duration: ");
  Serial.println(hu.smSleepData(hu.eDeepSleepDuration));
  Serial.print("Sleep quality score: ");
  Serial.println( hu.smSleepData(hu.eSleepQuality));

  sSleepComposite comprehensiveState = hu.getSleepComposite();
  Serial.println("Comprehensive sleep status:{");

  Serial.print("\tExistence status: ");
  switch (comprehensiveState.presence) {
    case 0:
      Serial.println("No one");
      break;
    case 1:
      Serial.println("Someone is present");
      break;
    default:
      Serial.println("Read error");
  }

  Serial.print("\tSleep status:");
  switch (comprehensiveState.sleepState) {
    case 0:
      Serial.println("Deep sleep");
      break;
    case 1:
      Serial.println("Light sleep");
      break;
    case 2:
      Serial.println("Awake");
      break;
    case 3:
      Serial.println("None");
      break;
    default:
      Serial.println("Read error");
  }

  Serial.print("\tAverage respiration rate: ");
  Serial.println(comprehensiveState.averageRespiration);
  Serial.print("\tAverage heart rate: ");
  Serial.println(comprehensiveState.averageHeartbeat);
  Serial.print("\tNumber of turns: ");
  Serial.println(comprehensiveState.turnoverNumber);
  Serial.print("\tProportion of significant body movement: ");
  Serial.println(comprehensiveState.largeBodyMove);
  Serial.print("\tProportion of minor body movement: ");
  Serial.println(comprehensiveState.minorBodyMove);
  Serial.print("\tNumber of apneas: ");
  Serial.println(comprehensiveState.apneaEvents);
  Serial.println("}");

  Serial.print("Sleep abnormalities:");
  switch (hu.smSleepData(hu.eSleepDisturbances)) {
    case 0:
      Serial.println("Sleep duration less than 4 hours");
      break;
    case 1:
      Serial.println("Sleep duration more than 12 hours");
      break;
    case 2:
      Serial.println("Long time abnormal absence of person");
      break;
    case 3:
      Serial.println("None");
      break;
    default:
      Serial.println("Read error");
  }

  sSleepStatistics statistics = hu.getSleepStatistics();  // Get sleep statistics, the sensor reports the whole night's sleep statistics data when it judges the sleep process to be over.
  Serial.println("Sleep statistics:{");
  Serial.print("\tSleep quality score: ");
  Serial.println(statistics.sleepQualityScore);
  Serial.print("\tProportion of awake time: ");
  Serial.println(statistics.sleepTime);
  Serial.print("\tProportion of light sleep time: ");
  Serial.println(statistics.wakeDuration);
  Serial.print("\tProportion of light sleep time: ");
  Serial.println(statistics.shallowSleepPercentage);
  Serial.print("\tProportion of deep sleep time: ");
  Serial.println(statistics.deepSleepPercentage);
  Serial.print("\tOut of bed duration: ");
  Serial.println(statistics.timeOutOfBed);
  Serial.print("\tNumber of times out of bed: ");
  Serial.println(statistics.exitCount);
  Serial.print("\tNumber of turns: ");
  Serial.println(statistics.turnOverCount);
  Serial.print("\tAverage respiration: ");
  Serial.println(statistics.averageRespiration);
  Serial.print("\tAverage heartbeat: ");
  Serial.println(statistics.averageHeartbeat);
  Serial.println("}");

  Serial.print("Sleep quality rating: ");
  switch (hu.smSleepData(hu.eSleepQualityRating)) {
    case 0:
      Serial.println("None");
      break;
    case 1:
      Serial.println("Good sleep quality");
      break;
    case 2:
      Serial.println("Average sleep quality");
      break;
    case 3:
      Serial.println("Poor sleep quality");
      break;
    default:
      Serial.println("Read error");
  }

  Serial.print("Abnormal struggle status: ");
  switch (hu.smSleepData(hu.eAbnormalStruggle)) {
    case 0:
      Serial.println("None");
      break;
    case 1:
      Serial.println("Normal status");
      break;
    case 2:
      Serial.println("Abnormal struggle status");
      break;
    default:
      Serial.println("Read error");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}