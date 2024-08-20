/**！
 * @file basics.ino
 * @brief This is an example of the C1001 mmWave Human Detection Sensor detecting the presence of people and their respiration and heart rates.
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

  hu.configLEDLight(hu.eHPLed, 1);  // Set HP LED switch, it will not light up even if the sensor detects a person when set to 0.
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
  Serial.print("Existing information:");
  switch (hu.smHumanData(hu.eHumanPresence)) {
    case 0:
      Serial.println("No one is present");
      break;
    case 1:
      Serial.println("Someone is present");
      break;
    default:
      Serial.println("Read error");
  }

  Serial.print("Motion information:");
  switch (hu.smHumanData(hu.eHumanMovement)) {
    case 0:
      Serial.println("None");
      break;
    case 1:
      Serial.println("Still");
      break;
    case 2:
      Serial.println("Active");
      break;
    default:
      Serial.println("Read error");
  }

  Serial.print("Body movement parameters: ");
  Serial.println(hu.smHumanData(hu.eHumanMovingRange));
  Serial.print("Respiration rate: ");
  Serial.println(hu.getBreatheValue());
  Serial.print("Heart rate: ");
  Serial.println(hu.getHeartRate());
  Serial.println("-----------------------");
  delay(1000);
}