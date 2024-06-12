#include <Wire.h>// master and slave communication library file   
#include <util/atomic.h> // For the ATOMIC_BLOCK macro which stores the position value when interrupt is working
#define SLAVE_ADDRESS 0x01
#include <AccelStepper.h>
// direction Digital 52 (CW), pulses Digital 53 (CLK)
AccelStepper stepper(1, 53, 52);
char c;
//Pin definitions
//Encoder pins for motor 1 & 2
//#define ENCA1 2 // Motor 1 - Brown 
//#define ENCB1 3 // Motor 1 - Black 
//#define ENCA2 18 // Motor 2 - Brown 
//#define ENCB2 19 // Motor 2 - Black 
//(Cytron)PWM pins & DIR pins for 1,2,3&4 motor
#define PWM1 4
#define DIR1 5
#define PWM2 6
#define DIR2 7
#define PWM3 8
#define DIR3 9
#define PWM4 10
#define DIR4 11

#define PWM5 12 // FLIP PGDC
#define DIR5 13

#define PWM6 2   // HIT PGDC
#define DIR6 3

#define PWM_1 14    // Linear ACtuator
#define PWM_2 15 

#define rel1 16     // RELAY 1
#define rel2 17     // RELAY 2

#define rel 1     // RELAY for ELMAG

void setup() {
  Wire.begin(SLAVE_ADDRESS); 
  Wire.onReceive(receiveData);
  pinMode(rel1,OUTPUT);  //Relay 1
  pinMode(rel2,OUTPUT);  //Relay 2
  digitalWrite(rel1,LOW);  
  digitalWrite(rel2,LOW);

  pinMode(rel,OUTPUT);  //Relay FOR ELMAG
  digitalWrite(rel,HIGH); 

 // where the LA is connected to
  pinMode(PWM_1, OUTPUT);
  pinMode(PWM_2, OUTPUT);
  digitalWrite(PWM_1,LOW); 
  digitalWrite(PWM_2,LOW);
// Set up serial monitor
  Serial.begin(57600);

//PWM and DIR enable declarations
  pinMode(PWM1,OUTPUT);
  pinMode(DIR1,OUTPUT);
  pinMode(PWM2,OUTPUT);
  pinMode(DIR2,OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(DIR3, OUTPUT);
  pinMode(PWM4, OUTPUT);
  pinMode(DIR4, OUTPUT);

  pinMode(PWM5, OUTPUT);
  pinMode(DIR5, OUTPUT);
  pinMode(PWM6, OUTPUT);
  pinMode(DIR6, OUTPUT);

  analogWrite(PWM5,0);    // FLIP PGDC
  digitalWrite(DIR5,LOW);
  analogWrite(PWM6,0);    // HIT PGDC
  digitalWrite(DIR6,LOW);

  stepper.setMaxSpeed(4000);//1100
  stepper.setAcceleration(30000);
 // stepper.moveTo(0);
}
//Function to receive data from master which get data from PS2 ctrller
void receiveData(char byteCount) {
  while (Wire.available()) {
    c = Wire.read();
    Serial.println(c);
    if (c=='W')  //Forward + R1
    {
       digitalWrite(PWM_1, HIGH);
       digitalWrite(PWM_2, LOW);
    }
    
    else if(c=='R') //Backward + R1
    {        
      digitalWrite(PWM_1, LOW);
      digitalWrite(PWM_2, HIGH);
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
    
    else if(c =='M')
    {
       CWrotate();
    }
    
    else if(c =='N')
    {
       CCWrotate();
    }
    
//    else if(c == 'I')
//    {
//         pos = 80; 
//         myservo.attach(12);
//         myservo.write(pos);
//         myservo.detach();
//    }
//    else if(c == 'J')
//    {
//         pos = 0; 
//         myservo.attach(12);
//         myservo.write(pos);
//         myservo.detach();
//    }
    else
    {
      digitalWrite(PWM_1, LOW); 
      digitalWrite(PWM_2, LOW);
      Stop();
    }
  }
}

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
  digitalWrite(DIR1,LOW); //(inwards) clockwise
  analogWrite(PWM2,pwm);
  digitalWrite(DIR2,HIGH);//(inwards) counter-clockwise
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,HIGH);//(inwards) counter-clockwise
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,LOW); //(inwards) clockwise
  //Serial.println("Move forward");
}

void backward() {
 // motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,HIGH);//(inwards) counter-clockwise
  analogWrite(PWM2,pwm);
  digitalWrite(DIR2,LOW);
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,LOW);
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,HIGH);//(inwards) counter-clockwise
  //Serial.println("Move backward");
  
}
void left() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,HIGH);//(inwards) counter-clockwise
  analogWrite(PWM2,pwm);
  digitalWrite(DIR2,HIGH);//(inwards) counter-clockwise
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,LOW);
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,LOW);
  //Serial.println("Move left");
}

void right() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,LOW);// direction to the motor
  analogWrite(PWM2,pwm);
  digitalWrite(DIR2,LOW);
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,HIGH);//(inwards) counter-clockwise
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,HIGH);//(inwards) counter-clockwise
  //Serial.println("Move right");
}

void leftcrossfd() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,LOW);//pwm value to the motor 1
  digitalWrite(DIR1,LOW);// direction to the motor 1
  analogWrite(PWM2,pwm);//pwm value to the motor 2
  digitalWrite(DIR2,HIGH);//(inwards) counter-clockwise
  analogWrite(PWM3,LOW);
  digitalWrite(DIR3,LOW);
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,LOW);
  //Serial.println("Move leftcrossfd");
}

void rightcrossbd() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,LOW);//pwm value to the motor 1
  digitalWrite(DIR1,LOW);// direction to the motor
  analogWrite(PWM2,pwm);
  digitalWrite(DIR2,LOW);
  analogWrite(PWM3,LOW);
  digitalWrite(DIR3,LOW);
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,HIGH);//(inwards) counter-clockwise
  //Serial.println("Move righttcoessbd");
}

void leftcrossbd() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,HIGH);//(inwards) counter-clockwise
  analogWrite(PWM2,LOW);
  digitalWrite(DIR2,LOW);
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,LOW);
  analogWrite(PWM4,LOW);
  digitalWrite(DIR4,LOW);
  //Serial.println("Move leftcrossdd");
}

void rightcrossfd() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,LOW);// direction to the motor
  analogWrite(PWM2,LOW);
  digitalWrite(DIR2,LOW);
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,HIGH);//(inwards) counter-clockwise
  analogWrite(PWM4,LOW);
  digitalWrite(DIR4,LOW);
  //Serial.println("Move rightcrossfd");
}

void CWrotate() {
//motor power
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,LOW);//(inwards) clockwise
  analogWrite(PWM2,pwm);
  digitalWrite(DIR2,LOW);
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,LOW);
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,LOW);
  //Serial.println("Move CWrotate");
}

void CCWrotate() {
  float pwm = 255;
 //Motor directions & pwm ctrl
  analogWrite(PWM1,pwm);//pwm value to the motor 1
  digitalWrite(DIR1,HIGH);//(inwards) counter-clockwise
  analogWrite(PWM2,pwm);
  digitalWrite(DIR2,HIGH);
  analogWrite(PWM3,pwm);
  digitalWrite(DIR3,HIGH);
  analogWrite(PWM4,pwm);
  digitalWrite(DIR4,HIGH);
  //Serial.println("Move CWrotate");
}

void loop() {
      if(c=='P')  //X Pressed
      {
      digitalWrite(rel1,LOW);
      digitalWrite(rel2,HIGH);
      delay(400);
      digitalWrite(rel1,HIGH);
      digitalWrite(rel2,LOW); 
      delay(400);
      }

      else if ( c == 'O')
      {
        //hit pgdc
          analogWrite(PWM6, 150);
          digitalWrite(DIR6,LOW);
          delay(35);
          analogWrite(PWM6, 0);
          digitalWrite(DIR6,LOW);
//          delay(1000);
      }

      else if (c == 'I')
      {
        //electromag off + pgdc forward
//          digitalWrite(rel,LOW); 
          analogWrite(PWM5, 200);
          digitalWrite(DIR5,HIGH);
          delay(35);
          analogWrite(PWM5, 0);
          digitalWrite(DIR5,LOW);
      }
      else if (c == 'J')
      { 
        //pgdc backward + ELMAG ON
          analogWrite(PWM5, 200);
          digitalWrite(DIR5,LOW);
          delay(35);
          analogWrite(PWM5, 0);
          digitalWrite(DIR5,LOW);
//          digitalWrite(rel,HIGH); 
      }      
      
      else
      {
      digitalWrite(rel1,LOW);
      digitalWrite(rel2,LOW);
      }

      if (c == 'Q')
      {
        //stepper up
        stepper.runToNewPosition(500);
      }
}