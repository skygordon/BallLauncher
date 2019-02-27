
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h> //Used in support of TFT Display
#include <string.h>  //used for some string handling and processing.
#include <mpu9255_esp32.h>
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
 
#define BACKGROUND TFT_GREEN
#define BALL_COLOR TFT_BLUE
 
const int DT = 40; //milliseconds
const int EXCITEMENT = 1000; //how much force to apply to ball
const uint8_t BUTTON_PIN = 16; //CHANGE YOUR WIRING TO PIN 16!!! (FROM 19)
uint8_t state = 0;  //system state for step counting
uint8_t ball0 = 0;
uint8_t ball1 = 0;
uint8_t ball2 = 0;
uint8_t ball3 = 0;
uint32_t primary_timer; //main loop timer
 
// state variables:
float x_pos = 64; // x position for first ball 
float y_pos = 80; // y position for first ball 
float x_vel = 0; // x velocity for first ball 
float y_vel = 0; // y velocity for first ball 
float x_accel = 0; // x acceleration for first ball 
float y_accel = 0; // y acceleration for first ball 

float x_pos1 = 64; // x position for second ball 
float y_pos1 = 80; // y position for second ball 
float x_vel1 = 0; // x velocity for second ball 
float y_vel1 = 0; // y velocity for second ball 
float x_accel1 = 0; // x acceleration for second ball 
float y_accel1 = 0; // y acceleration for second ball 

float x_pos2 = 64; // x position for third ball 
float y_pos2 = 80; // y position for third ball 
float x_vel2 = 0; // x velocity for third ball 
float y_vel2 = 0; // y velocity for third ball 
float x_accel2 = 0; // x acceleration for third ball 
float y_accel2 = 0; // y acceleration for third ball 

float x_pos3 = 64; // x position for fourth ball 
float y_pos3 = 80; // y position for fourth ball
float x_vel3 = 0; // x velocity for fourth ball
float y_vel3 = 0; // y velocity for fourth ball
float x_accel3 = 0; // x acceleration for fourth ball
float y_accel3 = 0; // y acceleration for fourth ball
 
//physics constants:
const float MASS = 1; //for starters
const int RADIUS = 5; //radius of ball
const float K_FRICTION = 0.15;  //friction coefficient
const float K_SPRING = 0.9;  //spring coefficient
 
//boundary constants:
const int LEFT_LIMIT = RADIUS; //left side of screen limit
const int RIGHT_LIMIT = 127-RADIUS; //right side of screen limit
const int TOP_LIMIT = RADIUS; //top of screen limit
const int BOTTOM_LIMIT = 159-RADIUS; //bottom of screen limit
 
bool pushed_last_time; //for finding change of button (using bool type...same as uint8_t)

MPU9255 imu; //imu object called, appropriately, imu
//will eventually replace with your improved step function!
void step(float x_force, float y_force ){
  // updates acceleration (using f = m*a)
  x_accel = (x_force-K_FRICTION*x_vel)/MASS;
  y_accel = (y_force-K_FRICTION*y_vel)/MASS;
  //integrate to get velocity from current acceleration
  x_vel = x_vel + 0.001*DT*x_accel; //integrate, 0.001 is conversion from milliseconds to seconds
  y_vel = y_vel + 0.001*DT*y_accel; //integrate!!
  moveBall();
}

void step1(float x_force, float y_force ){
  // updates acceleration (using f = m*a)
  x_accel1 = (x_force-K_FRICTION*x_vel1)/MASS;
  y_accel1 = (y_force-K_FRICTION*y_vel1)/MASS;
  //integrate to get velocity from current acceleration
  x_vel1 = x_vel1 + 0.001*DT*x_accel1; //integrate, 0.001 is conversion from milliseconds to seconds
  y_vel1 = y_vel1 + 0.001*DT*y_accel1; //integrate!!
  moveBall1();
}

void step2(float x_force, float y_force ){
  // updates acceleration (using f = m*a)
  x_accel2 = (x_force-K_FRICTION*x_vel2)/MASS;
  y_accel2 = (y_force-K_FRICTION*y_vel2)/MASS;
  //integrate to get velocity from current acceleration
  x_vel2 = x_vel2 + 0.001*DT*x_accel2; //integrate, 0.001 is conversion from milliseconds to seconds
  y_vel2 = y_vel2 + 0.001*DT*y_accel2; //integrate!!
  moveBall2();
}

void step3(float x_force, float y_force ){
  // updates acceleration (using f = m*a)
  x_accel3 = (x_force-K_FRICTION*x_vel3)/MASS;
  y_accel3 = (y_force-K_FRICTION*y_vel3)/MASS;
  //integrate to get velocity from current acceleration
  x_vel3 = x_vel3 + 0.001*DT*x_accel3; //integrate, 0.001 is conversion from milliseconds to seconds
  y_vel3 = y_vel3 + 0.001*DT*y_accel3; //integrate!!
  moveBall3();
}

void moveBall(){
    float old_x_vel = x_vel;
    float old_y_vel = y_vel;
    
    float old_x_pos = x_pos;
    float old_y_pos = y_pos;
    
    float temp_pos_x = old_x_pos+x_vel*.001*DT;
    float temp_pos_y = old_y_pos+y_vel*.001*DT;
    
    if(temp_pos_x>RIGHT_LIMIT){
      x_pos = RIGHT_LIMIT- K_SPRING*(temp_pos_x-RIGHT_LIMIT);
      x_vel = -K_SPRING*old_x_vel;
      
    }else if(temp_pos_x<LEFT_LIMIT){
      x_pos = LEFT_LIMIT-K_SPRING*(temp_pos_x-LEFT_LIMIT);
      x_vel = -K_SPRING*old_x_vel;
      
    }else{
      x_pos = temp_pos_x;
    }
    
    if(temp_pos_y<TOP_LIMIT){
      y_pos = TOP_LIMIT-K_SPRING*(temp_pos_y-TOP_LIMIT);
      y_vel = -K_SPRING*old_y_vel;
      
    }else if(temp_pos_y>BOTTOM_LIMIT){
      y_pos = BOTTOM_LIMIT-K_SPRING*(temp_pos_y-BOTTOM_LIMIT);
      y_vel = -K_SPRING*old_y_vel;
      
    }else{
      y_pos = temp_pos_y;
    }
}
void moveBall1(){
    float old_x_vel1 = x_vel1;
    float old_y_vel1 = y_vel1;
    
    float old_x_pos1 = x_pos1;
    float old_y_pos1 = y_pos1;
    
    float temp_pos_x1 = old_x_pos1+x_vel1*.001*DT;
    float temp_pos_y1 = old_y_pos1+y_vel1*.001*DT;
    
    if(temp_pos_x1>RIGHT_LIMIT){
      x_pos1 = RIGHT_LIMIT- K_SPRING*(temp_pos_x1-RIGHT_LIMIT);
      x_vel1 = -K_SPRING*old_x_vel1;
      
    }else if(temp_pos_x1<LEFT_LIMIT){
      x_pos1 = LEFT_LIMIT-K_SPRING*(temp_pos_x1-LEFT_LIMIT);
      x_vel1 = -K_SPRING*old_x_vel1;
      
    }else{
      x_pos1 = temp_pos_x1;
    }
    
    if(temp_pos_y1<TOP_LIMIT){
      y_pos1 = TOP_LIMIT-K_SPRING*(temp_pos_y1-TOP_LIMIT);
      y_vel1 = -K_SPRING*old_y_vel1;
      
    }else if(temp_pos_y1>BOTTOM_LIMIT){
      y_pos1 = BOTTOM_LIMIT-K_SPRING*(temp_pos_y1-BOTTOM_LIMIT);
      y_vel1 = -K_SPRING*old_y_vel1;
      
    }else{
      y_pos1 = temp_pos_y1;
    }
    
}
void moveBall2(){
    float old_x_vel2 = x_vel2;
    float old_y_vel2 = y_vel2;
    
    float old_x_pos2 = x_pos2;
    float old_y_pos2 = y_pos2;
    
    float temp_pos_x2 = old_x_pos2+x_vel2*.001*DT;
    float temp_pos_y2 = old_y_pos2+y_vel2*.001*DT;
    
    if(temp_pos_x2>RIGHT_LIMIT){
      x_pos2 = RIGHT_LIMIT- K_SPRING*(temp_pos_x2-RIGHT_LIMIT);
      x_vel2 = -K_SPRING*old_x_vel2;
      
    }else if(temp_pos_x2<LEFT_LIMIT){
      x_pos2 = LEFT_LIMIT-K_SPRING*(temp_pos_x2-LEFT_LIMIT);
      x_vel2 = -K_SPRING*old_x_vel2;
      
    }else{
      x_pos2 = temp_pos_x2;
    }
    
    if(temp_pos_y2<TOP_LIMIT){
      y_pos2 = TOP_LIMIT-K_SPRING*(temp_pos_y2-TOP_LIMIT);
      y_vel2 = -K_SPRING*old_y_vel2;
      
    }else if(temp_pos_y2>BOTTOM_LIMIT){
      y_pos2 = BOTTOM_LIMIT-K_SPRING*(temp_pos_y2-BOTTOM_LIMIT);
      y_vel2 = -K_SPRING*old_y_vel2;
      
    }else{
      y_pos2 = temp_pos_y2;
    }
    
}
void moveBall3(){
    float old_x_vel3 = x_vel3;
    float old_y_vel3 = y_vel3;
    
    float old_x_pos3 = x_pos3;
    float old_y_pos3 = y_pos3;
    
    float temp_pos_x3 = old_x_pos3+x_vel3*.001*DT;
    float temp_pos_y3 = old_y_pos3+y_vel3*.001*DT;
    
    if(temp_pos_x3>RIGHT_LIMIT){
      x_pos3 = RIGHT_LIMIT- K_SPRING*(temp_pos_x3-RIGHT_LIMIT);
      x_vel3 = -K_SPRING*old_x_vel3;
      
    }else if(temp_pos_x3<LEFT_LIMIT){
      x_pos3 = LEFT_LIMIT-K_SPRING*(temp_pos_x3-LEFT_LIMIT);
      x_vel3 = -K_SPRING*old_x_vel3;
      
    }else{
      x_pos3 = temp_pos_x3;
    }
    
    if(temp_pos_y3<TOP_LIMIT){
      y_pos3 = TOP_LIMIT-K_SPRING*(temp_pos_y3-TOP_LIMIT);
      y_vel3 = -K_SPRING*old_y_vel3;
      
    }else if(temp_pos_y3>BOTTOM_LIMIT){
      y_pos3 = BOTTOM_LIMIT-K_SPRING*(temp_pos_y3-BOTTOM_LIMIT);
      y_vel3 = -K_SPRING*old_y_vel3;
      
    }else{
      y_pos3 = temp_pos_y3;
    }
    
}

void ballstatemachine(uint8_t button1,float x,float y){
  switch(state){
    case 0:
      //awaiting button 1 down
      //no ball 
      if(button1==0){
        state = 1;
        Serial.println("button pushed");
    }
      break;
    case 1:
      ball0 = 1;
      step(x,y);
      step1(0,0);
      step2(0,0);
      step3(0,0);
      if(button1==1){
        state = 2;
        Serial.println("button released");
    }
    break;
    case 2:
      step(0,0);
      step1(0,0);
      step2(0,0);
      step3(0,0);
      if(button1==0){
        state = 3;
        Serial.println("button pushed");
    }
      break;
    case 3:
      ball1 = 1;
      step(0,0);
      step1(x,y);
      step2(0,0);
      step3(0,0);
      if(button1==1){
        state = 4;
        Serial.println("button released");
    }
      break;
    case 4:
      step(0,0);
      step1(0,0);
      step2(0,0);
      step3(0,0);
      
      if(button1==0){
        state = 5;
      }
      break;
    case 5:
      ball2 = 1;
      step(0,0);
      step1(0,0);
      step2(x,y);
      step3(0,0);
      if(button1==1){
        state=6;
      }
      break;
    case 6:  
      step(0,0);
      step1(0,0);
      step2(0,0);
      step3(0,0);
      if(button1==0){
       state = 7;  
    }
      break;
    case 7:
      ball3 = 1;
      step(0,0);
      step1(0,0);
      step2(0,0);
      step3(x,y);
      if(button1==1){
        state = 8;
    }
      break;
    case 8:
      step(0,0);
      step1(0,0);
      step2(0,0);
      step3(0,0);
      if(button1==0){
        state = 9;
      }
      break;
}}
 
void setup() {
  Serial.begin(115200); //for debugging if needed.
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  tft.init();
  tft.setRotation(2);
  tft.setTextSize(1);
  tft.fillScreen(BACKGROUND);
  if (imu.setupIMU(1)){
    Serial.println("IMU Connected!");
  }else{
    Serial.println("IMU Not Connected :/");
    Serial.println("Restarting");
    ESP.restart(); // restart the ESP (proper way)
  }
  primary_timer = millis();
}


void loop() {
  //Serial.println(ball0);
  //Serial.println(ball1);
  //draw circle in previous location of ball in color background (redraws minimal num of pixels, therefore is quick!)
  if(ball0==1){
    tft.fillCircle(x_pos,y_pos,RADIUS,BACKGROUND);
  }
  if(ball1==1){
    tft.fillCircle(x_pos1,y_pos1,RADIUS,BACKGROUND);
  }
  if(ball2==1){
    tft.fillCircle(x_pos2,y_pos2,RADIUS,BACKGROUND);
  }
  if(ball3==1){
    tft.fillCircle(x_pos3,y_pos3,RADIUS,BACKGROUND);
  }
  imu.readAccelData(imu.accelCount);//read imu
  float y = 5000*imu.accelCount[0]*imu.aRes;
  float x = 5000*imu.accelCount[1]*imu.aRes;
  uint8_t button1 = digitalRead(BUTTON_PIN);
  ballstatemachine(button1,x,y);
  if(ball0==1){
    tft.fillCircle(x_pos,y_pos,RADIUS,BALL_COLOR); //draw new ball location
  }
  if(ball1==1){
    tft.fillCircle(x_pos1,y_pos1,RADIUS,BALL_COLOR); //draw new ball location
  }
  if(ball2==1){
    tft.fillCircle(x_pos2,y_pos2,RADIUS,BALL_COLOR); //draw new ball location
  }
  if(ball3==1){
    tft.fillCircle(x_pos3,y_pos3,RADIUS,BALL_COLOR); //draw new ball location
  }
 
  while (millis()-primary_timer < DT);
  primary_timer = millis();
  //wait for primary timer to increment
}
