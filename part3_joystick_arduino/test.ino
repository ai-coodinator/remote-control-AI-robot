/* 
 * This example shows how to control MDDS30 in Serial Simplified mode with Arduino.
 * Set MDDS30 input mode to 0b11001100
 * Open Serial Monitor, set baudrate to 9600bps and "No line ending".
 * Send 0: Left motor stops
 *      1: Left motor rotates CW with half speed
 *      2: Left motor rotates CW with full speed
 *      3: Left motor rotates CCW with half speed
 *      4: Left motor rotates CCW with full speed
 *      5: Right motor stops
 *      6: Right motor rotates CW with half speed
 *      7: Right motor rotates CW with full speed
 *      8: Right motor rotates CCW with half speed
 *      9: Right motor rotates CCW with full speed
 * 
 * Note: This example also compatible with MDDS10 and MDDS60
 *
 * Hardware Connection:
 *   Arduino Uno    MDDS30
 *   GND ---------- GND
 *   4 ------------ IN1
 *
 * Related Products:
 * - SmartDriveDuo-30: http://www.cytron.com.my/P-MDDS60
 * - CT UNO: http://www.cytron.com.my/p-ct-uno
 * - DC Brush Motors: http://www.cytron.com.my/c-84-dc-motor
 * - LiPo Battery: http://www.cytron.com.my/c-87-power/c-97-lipo-rechargeable-battery-and-charger
 * - Male to Male Jumper: https://www.cytron.com.my/p-wr-jw-mm65
 *
 * URL: http://www.cytron.com.my
 */

#include <SoftwareSerial.h>
#include <Cytron_SmartDriveDuo.h>
#define IN1 4 // Arduino pin 4 is connected to MDDS60 pin IN1.
#define BAUDRATE  9600
Cytron_SmartDriveDuo smartDriveDuo30(SERIAL_SIMPLFIED, IN1, BAUDRATE);

float motor_L;
float motor_R;
signed int speedLeft, speedRight;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available()) {

    String motor = Serial.readStringUntil('z');
    //Serial.println(motor);

    int kg = motor.indexOf(",");
    //Serial.println(kg);
    int length = motor.length();
    //Serial.println(length);

    float motor_updown = motor.substring(0, kg).toFloat();
    float motor_LR     = motor.substring(kg + 1 , length).toFloat();

//    Serial.println(motor_updown);
//    Serial.println(motor_LR);

    if (motor_updown == 0.0 && motor_LR > 0.0) {
      motor_L = motor_LR;
      motor_R = -motor_LR;
      
    } else if (motor_updown == 0.0 && motor_LR < 0.0) {
      motor_L = motor_LR;
      motor_R = -motor_LR;

    } else if (motor_updown > 0.0 && motor_LR > 0.0) {
      motor_L = motor_updown;
      motor_R = 0;

    } else if (motor_updown < 0.0 && motor_LR > 0.0) {
      motor_L = 0;
      motor_R = motor_updown;

    } else if (motor_updown > 0.0 && motor_LR < 0.0) {
      motor_L = 0;
      motor_R = motor_updown;

    } else if (motor_updown < 0.0 && motor_LR < 0.0) {
      motor_L = motor_updown;
      motor_R = 0;
      
    } else {
       motor_L = motor_updown;
       motor_R = motor_updown;
    }
  
    speedLeft  = motor_L * 100;
    speedRight = motor_R * 100;
    
    smartDriveDuo30.control(speedLeft, speedRight);

    Serial.flush();
  }
}
