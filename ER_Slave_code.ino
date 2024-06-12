#include <Wire.h>// master and slave communication library file   
//#include <util/atomic.h> For the ATOMIC_BLOCK macro which stores the position value when interrupt is working
#include <AccelStepper.h>
#define SLAVE_ADDRESS 0x01
char c;
unsigned int flag;
unsigned int f,h,e,g;

//cytron - locomotion pins
#define PWM1 4
#define DIR1 5
#define PWM2 6
#define DIR2 7
#define PWM3 8
#define DIR3 9
#define PWM4 10
#define DIR4 11

#define relay1 22   // PNEUMATIC RELAY 1
#define relay2 23   // PNEUMATIC RELAY 2

#define rly_em 50   // ELECTROMAGNET RELAY

#define DIR_L 48    // For linear actuator
#define PWM_L 49   // For Linear actuator

#define DIR_C 12   //CIM MOTOR
#define PWM_C 13  //CIM MOTOR

#define DIR_LD 2   //FLIPPING MOTOR
#define PWM_LD 3  //FLIPPING MOTOR
//slingshot limiting motor movement
#define TRIGGER_PIN 42
#define ECHO_PIN 43
#define BUZZER_PIN 44
// Define a stepper motor 1 for arduino 
// direction Digital A1 (CW), pulses Digital A0 (CLK)
AccelStepper stepper(1, A0, 34); // STEP,DIRECTION PINS define stepper motor parameters
float speeed = 140;

void receiveData(int byteCount) {
  while (Wire.available()) {
    c = Wire.read();
    Serial.println(c);
    if (c == 'W')
    {
        digitalWrite(DIR_L, HIGH);  //For linear actuator
        digitalWrite(PWM_L, 255);    
    }
    
    else if(c == 'R')
    {        
      digitalWrite(DIR_L,LOW);   //For linear actuator
      digitalWrite(PWM_L, 255);
    }
    
    else if(c == 'N')
    {
      analogWrite(PWM_C,speeed); //SIM MOTOR
      digitalWrite(DIR_C,HIGH); // SIM MOTOR 
    }
    
    else if(c == 'M')
    {
      analogWrite(PWM_C,speeed); //SIM MOTOR  
      digitalWrite(DIR_C,LOW);     
    }
    
    else if(c == 'P')
    {
      flag = 1;// electromagnet
    }
     
    else if(c == 'A')
    {
      forward();
    }
    
    else if(c =='B')
    {
      right();
    }

    else if(c =='C')
    {
      left();
    }
    
    else if(c =='D')
    {
      backward();
    }
    
    else if(c =='E')
    {
       leftcrossfd();
    }
    
    else if(c =='F')
    {
        rightcrossfd();
    }

    else if(c =='G')
    {
       leftcrossbd();
    }
    
    else if(c =='H')
    {
       rightcrossbd();
    }
    
    else if(c =='U')   // Previous value 'N'
    {
       CWrotate();
    }
    
    else if(c =='Y')  // Previous value 'M'
    {
       CCWrotate();
    }
    else if(c == 'I')  // L2 PRESSED
    {
       up_move();//AFTER down_move MOVE UP TO TOP & FLIPtilt();
       f = 1;
    }
    
    else if(c == 'O') //Circle pressed
    {
      claw_actuate();
     h = 1;
    }
    
    else if(c == 'Q') //square released
    {
       picking();
       g = 1;
       //pg_bc(); 
       
    }
    
    else if(c == 'X') // Triangle pressed
    {
       
    }
    else if(c == 'J')
    {
      down_move();//FLIP & DOWN TO PICK
      e = 1;
    }
    else
    {        
      analogWrite(DIR_L, LOW); //LINEAR ACTUATOR
      digitalWrite(PWM_L, 0);
      analogWrite(PWM_C,LOW);//SIM MOTOR
      digitalWrite(DIR_C,0);
      
      Stop();
    }
  }
}

void setup() {
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);

  pinMode(rly_em, OUTPUT);
  pinMode(DIR_C,OUTPUT);  // CYTRON
  pinMode(PWM_C,OUTPUT);
  analogWrite(PWM_C,LOW);
  digitalWrite(DIR_C,LOW);
 
  pinMode(DIR_L, OUTPUT);  // LINEAR ACTUATOR CYTRON
  pinMode(PWM_L, OUTPUT);
  digitalWrite(DIR_L,LOW);  // LINEAR ACTUATOR
  analogWrite(PWM_L,LOW);

  pinMode(PWM_LD, OUTPUT);
  pinMode(DIR_LD, OUTPUT);
  analogWrite(PWM_LD,0); // up_move-FLIP MOTOR
  digitalWrite(DIR_LD,LOW); // up_move-FLIP MOTOR
  
  digitalWrite(rly_em,LOW); // ON Electromagnet
  Serial.begin(57600);
  pinMode(PWM1,OUTPUT);
  pinMode(DIR1,OUTPUT);
  pinMode(PWM2,OUTPUT);
  pinMode(DIR2,OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(DIR3, OUTPUT);
  pinMode(PWM4, OUTPUT);
  pinMode(DIR4, OUTPUT);

  stepper.setMaxSpeed(7000); // Set maximum speed value for the stepper
  stepper.setSpeed(2000);
  stepper.setAcceleration(2000); // Set acceleration value for the stepper
  stepper.setCurrentPosition(0); // Set the current position to 0 steps

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}
//Function to receive data from master which get data from PS2 ctrller


void Stop(){
  analogWrite(PWM1,LOW);//pwm value to the motor 1
  digitalWrite(DIR1,LOW);// direction to the motor
  analogWrite(PWM2,LOW);
  digitalWrite(DIR2,LOW);
  analogWrite(PWM3,LOW);
  digitalWrite(DIR3,LOW);
  analogWrite(PWM4,LOW);
  digitalWrite(DIR4,LOW);
  //Serial.println("stop");
}

void forward() {

 float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,HIGH); //(inwards) clockwise
  analogWrite(PWM2,pwm);
  digitalWrite(DIR2,LOW);//(inwards) counter-clockwise
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,LOW);//(inwards) counter-clockwise
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,HIGH); //(inwards) clockwise
  //Serial.println("Move forward");
}

void backward() {
 // motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,LOW);//(inwards) counter-clockwise
  analogWrite(PWM2,pwm);
  digitalWrite(DIR2,HIGH);
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,HIGH);
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,LOW);//(inwards) counter-clockwise
  //Serial.println("Move backward");
  
}
void left() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,LOW);//(inwards) counter-clockwise
  analogWrite(PWM2,pwm);
  digitalWrite(DIR2,LOW);//(inwards) counter-clockwise
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,HIGH);
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,HIGH);
  //Serial.println("Move left");
}

void right() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,HIGH);// direction to the motor
  analogWrite(PWM2,pwm);
  digitalWrite(DIR2,HIGH);
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,LOW);//(inwards) counter-clockwise
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,LOW);//(inwards) counter-clockwise
  //Serial.println("Move right");
}

void leftcrossfd() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,LOW);//pwm value to the motor 1
  digitalWrite(DIR1,LOW);// direction to the motor 1
  analogWrite(PWM2,pwm);//pwm value to the motor 2
  digitalWrite(DIR2,LOW);//(inwards) counter-clockwise
  analogWrite(PWM3,LOW);
  digitalWrite(DIR3,LOW);
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,HIGH);
  //Serial.println("Move leftcrossfd");
}

void rightcrossbd() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,LOW);//pwm value to the motor 1
  digitalWrite(DIR1,LOW);// direction to the motor
  analogWrite(PWM2,pwm);
  digitalWrite(DIR2,HIGH);
  analogWrite(PWM3,LOW);
  digitalWrite(DIR3,LOW);
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,LOW);//(inwards) counter-clockwise
  //Serial.println("Move righttcoessbd");
}

void leftcrossbd() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,LOW);//(inwards) counter-clockwise
  analogWrite(PWM2,LOW);
  digitalWrite(DIR2,LOW);
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,HIGH);
  analogWrite(PWM4,LOW);
  digitalWrite(DIR4,LOW);
  //Serial.println("Move leftcrossdd");
}

void rightcrossfd() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,HIGH);// direction to the motor
  analogWrite(PWM2,LOW);
  digitalWrite(DIR2,LOW);
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,LOW);//(inwards) counter-clockwise
  analogWrite(PWM4,LOW);
  digitalWrite(DIR4,LOW);
  //Serial.println("Move rightcrossfd");
}

void CWrotate() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,HIGH);//(inwards) clockwise
  analogWrite(PWM2,pwm);
  digitalWrite(DIR2,HIGH);
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,HIGH);
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,HIGH);
  //Serial.println("Move CWrotate");
}

void CCWrotate() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,LOW);//(inwards) counter-clockwise
  analogWrite(PWM2,pwm);
  digitalWrite(DIR2,LOW);
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,LOW);
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,LOW);
  //Serial.println("Move CWrotate");
}

void down_move(){
  if(e == 1){
  // pgdc motor code & after stepper will work
      analogWrite(PWM_LD,255); // up_move MOTOR
      digitalWrite(DIR_LD,HIGH); // up_move MOTOR
      delay(700);
      analogWrite(PWM_LD,0); // up_move MOTOR
      digitalWrite(DIR_LD,LOW); // up_move MOTOR
  stepper.moveTo(-33000); // Set desired move: 800 steps (in quater-step resolution that's one rotation)
  stepper.runToPosition(); // Moves the motor to target position w/ acceleration/ deceleration and it blocks until is in position
  e = 0;
}
}

void up_move(){
  if(f == 1){
      stepper.moveTo(33000);
      //while (stepper1.currentPosition() != 0) {
      stepper.runToPosition();
      analogWrite(PWM_C,255); // flip MOTOR
      digitalWrite(DIR_C,LOW); // flip MOTOR
      delay(700);
     analogWrite(PWM_C,0); // flip MOTOR
      digitalWrite(DIR_C,LOW); // flip MOTOR
     f = 0;
}
}

void picking(){
  if(g == 1){
       digitalWrite(relay2, HIGH);//actuate pneumatics--3rd claw
       digitalWrite(relay1, HIGH);//actuate pneumatics--1&2nd claw
    g = 0;
}
}
    
void claw_actuate(){
  if(h == 1){
  digitalWrite(relay2,LOW);//release bottom ring  
  delay(300);
  digitalWrite(relay2,HIGH);//closed
  digitalWrite(relay1,LOW);//release remaining rings
  delay(150);
  digitalWrite(relay1,HIGH);//lock after place a ring in bottom claw
  h = 0;
}
}
void loop() {  
    if (flag == 1)
    {
       digitalWrite(rly_em, HIGH); 
       delay(1000); 
       digitalWrite(rly_em, LOW);
       flag = 0;
    }
    long duration, distance;
// Clear the trigger pin
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
// Set the trigger pin high for 10 microseconds
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
// Read the echo pin, calculate the distance in centimeters
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;
  if (distance < 3) {
// If the distance is less than 3 cm, turn on the buzzer
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    // Otherwise, turn off the buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
 delay(50); // Delay for 1 second before the next measurement
    
}
