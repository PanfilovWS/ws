#include <Servo.h>
#include <ros.h>
#include "std_msgs/Int16.h"
#include <std_msgs/Byte.h>
#include <FastLED.h>/
#include "GyverButton.h"/

#define END_SWITCH A12 
#define TOP_BUTTON A15  
#define CAP_SERVO 46  
#define NUM_LEDS 24
#define DATA_PIN 30

CRGB leds[NUM_LEDS];

int cap_servo_open = 0;
int cap_servo_close = 85;

//arduino to ROS connect init
class NewHardware : public ArduinoHardware
{
  public:
  NewHardware():ArduinoHardware(&Serial1, 115200){};
};
 
//ROS node init
ros::NodeHandle_<NewHardware>  nh;
 
//Servos and pins init
Servo cap_servo;

std_msgs::Int16 top_button;
ros::Publisher button_pub("top_button", &top_button);

std_msgs::Int16 end_switch;
ros::Publisher end_switch_pub("end_switch", &end_switch);


void CbCapServo( const std_msgs::Int16& msg){
    if (msg.data == 1){
        cap_servo.write(cap_servo_open);
    } 
    if (msg.data == 0){
      cap_servo.write(cap_servo_close);
    }
}

ros::Subscriber<std_msgs::Int16> subCapServo("top_cap", &CbCapServo );

void LedRope(const std_msgs::Byte& std_msg){                       //в зависимости от значения включается определенный цвет
  for(int i = 0; i < NUM_LEDS; i++){
    if (std_msg.data == 0) leds[i] = CHSV(0, 0, 0);
    if (std_msg.data == 1) leds[i] = CHSV(0, 255, 255);
    if (std_msg.data == 2) leds[i] = CHSV(60, 255, 255);
    if (std_msg.data == 3) leds[i] = CHSV(100, 255, 255);
    FastLED.show();
    }
}

std_msgs::Byte led_control;
ros::Subscriber<std_msgs::Byte> led_control_sub("led_control", &LedRope);

void setup() {

  Serial.begin(115200);

  cap_servo.attach(CAP_SERVO);
  cap_servo.write(cap_servo_open);  

  pinMode(END_SWITCH, INPUT);
  pinMode(TOP_BUTTON, INPUT_PULLUP);
  //Led_Pin
  LEDS.addLeds<WS2812, DATA_PIN,RGB>(leds,NUM_LEDS); //объявляем ленту
  LEDS.setBrightness(60);  
  //ROS nodes init
  nh.initNode();

  nh.subscribe(subCapServo);

  nh.subscribe(led_control_sub);

  nh.advertise(button_pub);
  nh.advertise(end_switch_pub);

}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();

  top_button.data = !digitalRead(TOP_BUTTON);
  button_pub.publish( &top_button );

  end_switch.data = digitalRead(END_SWITCH);
  end_switch_pub.publish( &end_switch );
  delay(50);

}
