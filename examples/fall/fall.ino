/**！
 * @file fall.ino
 * @brief This is the fall detection usage routine of the C1001 mmWave Human Detection Sensor.
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
  while (hu.configWorkMode(hu.eFallingMode) != 0) {
    Serial.println("error!!!");
    delay(1000);
  }
  Serial.println("Work mode switch successful");

  hu.configLEDLight(hu.eFALLLed, 1);         // Set HP LED switch, it will not light up even if the sensor detects a person present when set to 0.
  hu.configLEDLight(hu.eHPLed, 1);           // Set FALL LED switch, it will not light up even if the sensor detects a person falling when set to 0.
  hu.dmInstallHeight(270);                   // Set installation height, it needs to be set according to the actual height of the surface from the sensor, unit: CM.
  hu.dmFallTime(5);                          // Set fall time, the sensor needs to delay the current set time after detecting a person falling before outputting the detected fall, this can avoid false triggering, unit: seconds.
  hu.dmUnmannedTime(1);                      // Set unattended time, when a person leaves the sensor detection range, the sensor delays a period of time before outputting a no person status, unit: seconds.
  hu.dmFallConfig(hu.eResidenceTime, 200);   // Set dwell time, when a person remains still within the sensor detection range for more than the set time, the sensor outputs a stationary dwell status. Unit: seconds.
  hu.dmFallConfig(hu.eFallSensitivityC, 3);  // Set fall sensitivity, range 0~3, the larger the value, the more sensitive.
  hu.sensorRet();                            // Module reset, must perform sensorRet after setting data, otherwise the sensor may not be usable.

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
  Serial.print("FALL status:");
  switch (hu.getLEDLightState(hu.eFALLLed)) {
    case 0:
      Serial.println("Off");
      break;
    case 1:
      Serial.println("On");
      break;
    default:
      Serial.println("Read error");
  }

  Serial.print("Radar installation height: ");
  Serial.print(hu.dmGetInstallHeight());
  Serial.println(" cm");
  Serial.print("Fall duration: ");
  Serial.print(hu.getFallTime());
  Serial.println(" seconds");
  Serial.print("Unattended duration: ");
  Serial.print(hu.getUnmannedTime());
  Serial.println(" seconds");
  Serial.print("Dwell duration: ");
  Serial.print(hu.getStaticResidencyTime());
  Serial.println(" seconds");
  Serial.print("Fall sensitivity: ");
  Serial.print(hu.getFallData(hu.eFallSensitivity));
  Serial.println(" seconds");
  Serial.println("===============================");
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
  Serial.print( hu.smHumanData(hu.eHumanMovingRange));

  Serial.print("Fall status:");
  switch (hu.getFallData(hu.eFallState)) {
    case 0:
      Serial.println("Not fallen");
      break;
    case 1:
      Serial.println("Fallen");
      break;
    default:
      Serial.println("Read error");
  }

  Serial.print("Stationary dwell status: ");
  switch (hu.getFallData(hu.estaticResidencyState)) {
    case 0:
      Serial.println("No stationary dwell");
      break;
    case 1:
      Serial.println("Stationary dwell present");
      break;
    default:
      Serial.println("Read error");
  }
  Serial.println();
  delay(1000);
}
