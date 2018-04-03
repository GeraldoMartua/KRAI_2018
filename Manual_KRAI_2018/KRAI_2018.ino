#include <PS2X_lib.h>
#include <Wire.h>
#include <LiquidCrystal.h>
//#include <LiquidCrystal_I2C.h>
/*----------gripper mode---------*/
bool gripperMode = 0;
/*-----------Servo--------------*/
#define degreeLeft0 160
#define degreeRight0 150
int degreeLeft_last = -1;
int degreeRight_last = -1;
int mode = 0;
int mode_last = -1;
int mode_counter = 0;
bool river = 0;
/*---------Limit Switch --------*/
#define limitswitch1 33
#define limitswitch2 37

bool ready_on = 0;
bool grip_on = 0;
bool retry_on = 0;
bool climb_on = 0;
int climb_counter = 0;
int climb_speed = 0;
/*---------LCD w i2c-----------*/
LiquidCrystal lcd(48, 49, 50, 51, 52, 53);
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
int menu = 0;
/*------------PS2 Controller----------*/
#define display_PS2 true
#define PS2_CLK 47
#define PS2_CMD 49
#define PS2_ATT 51                        
#define PS2_DAT 53
#define PS2_pressures false
#define PS2_rumble false

char flagerror = '?';
char flagawal = '<';
int ps2_LX = 128; //Left analog X-axis*/
int ps2_LY = 128; //Left analog Y-axis*/
int ps2_RX = 128; //Right analog X-axis*/
int ps2_RY = 128; //Right analog Y-axis*/
int buttonset1 = 0; //Button Set 1: dpad_left, dpad_down, dpad_right, dpad_up, squareps, cross, circle, triangle
int buttonset2 = 0; //Button Set 2: select, start, L3, R3, L2, L1, R2, R1
bool errorGet = 0;
//void motor_mecanum_init();
//void start_mobilitation(float max_speed, float rotation_const, bool speed_type); 
//void motor_drive(int v, int num);
//void motor_drive_all(int v1, int v2, int v3, int v4);
#define BT_STATE 47
#define LED 13
 

int degreeLeft, degreeRight;
void setup() {
  Serial.begin(9600);
//  Serial2.begin(9600);
  Serial3.begin(115200);
  lcd_init();
  motor_mecanum_init();
  pneumatic_init();  //update 19th March 2018
}

void loop() {
  serial_controller();
  if(errorGet != 0 || digitalRead(BT_STATE) == 0) {
    ps2_safety();
  } else {
    lcd.clear();
    lcd.print("Choose Menu");
    lcd.setCursor(0, 1);
    switch (menu) {
      case 0:
        lcd.print("1. Manual Slow Speed");
        break;
      case 1:
        lcd.print("2. Manual Full Speed");
        break;
      case 2:
        lcd.print("3. Motor Debug");
        break;
      default:
        lcd.print("Menu Error");
        break;
    }

    if (ps2_left()) menu--;
    if (ps2_right()) menu++;
    if (menu > 2) menu = 0;
    if (menu < 0) menu = 2;

    if (ps2_select()) {
      switch (menu) {
        case 0:
          lcd.clear();
          lcd.print("Manual Slow Speed");
          degreeLeft = degreeLeft0 + 90;
          degreeRight = degreeRight0 - 90;
          while (ps2_start() == 0) {
            serial_controller();
            start_mobilitation(30, 40, 0); 
            pneumatic_running();//update 19th March 2018
          };
          break;
        case 1:
          lcd.clear();
          lcd.print("Manual Full Speed");
          while (ps2_start() == 0) {
            serial_controller();
            start_mobilitation(100, 50, 1) ;
            pneumatic_running();//update 19th March 2018
          };
          break;
        case 2:
          lcd.clear();
          lcd.print("Motor Debug");
          while (ps2_start() == 0) {
            serial_controller();
             motor_drive_all(40, 40, 40, 40);
             pneumatic_running();//update 19th March 2018
          }
          break;
        }
      delay(180);
    }
  }
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
/*Serial Call Function*/
void serial_controller() {
 while (Serial3.available() >= 7) {
    if (Serial3.read() == flagawal) { // Checks whether data is comming from the serial port
      ps2_LX = Serial3.read();
      ps2_LY = Serial3.read();
      ps2_RX = Serial3.read();
      ps2_RY = Serial3.read();
      buttonset1 = Serial3.read(); /*Button Set 1: dpad_left, dpad_down, dpad_right, dpad_up, squareps, cross, circle, triangle*/
      buttonset2 = Serial3.read(); /*Button Set 2: select, start, L3, R3, L2, L1, R2, R1*/
      errorGet = 0;
    } else if (Serial3.read() == flagerror){ // Checks whether data is comming from the serial port
      ps2_LX = Serial3.read();
      ps2_LY = Serial3.read();
      ps2_RX = Serial3.read();
      ps2_RY = Serial3.read();
      buttonset1 = Serial3.read(); /*Button Set 1: dpad_left, dpad_down, dpad_right, dpad_up, squareps, cross, circle, triangle*/
      buttonset2 = Serial3.read(); /*Button Set 2: select, start, L3, R3, L2, L1, R2, R1*/
      errorGet = 1;       
    }
  }  
}

void ps2_safety(){
  stop_all();
  if (digitalRead(BT_STATE) == 0) {
    lcd.print("--------------------");
    lcd.setCursor(0,1);
    lcd.print("--Error: Bluetooth--");
    lcd.setCursor(0,2);
    lcd.print("Wait for Connection!");
    lcd.setCursor(0,3);
    lcd.print("--------------------");
  } else if(errorGet != 0){
    lcd.print("--------------------");
    lcd.setCursor(0,1);
    lcd.print("----Error: Stick----");
    lcd.setCursor(0,2);
    lcd.print("--Check Connection--");
    lcd.setCursor(0,3);
    lcd.print("--------------------");
  } else if(digitalRead(BT_STATE) == 0 && errorGet != 0){
    lcd.print("----Error: Stick----");
    lcd.setCursor(0,1);
    lcd.print("--Check Connection--");
    lcd.setCursor(0,2);
    lcd.print("--Error: Bluetooth--");
    lcd.setCursor(0,3);
    lcd.print("Wait for Connection!");
  } else {
    lcd.print("--------------------");
    lcd.setCursor(0,1);
    lcd.print("---Error: Unknown---");
    lcd.setCursor(0,2);
    lcd.print("---Turn OFF ROBOT---");
    lcd.setCursor(0,3);
    lcd.print("--------------------");  
  }
}


/* LCD Initialize*/
void lcd_init() {
  lcd.begin(20, 4);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("LCD Ready");
  delay(1000);
  lcd.clear();
}

void stop_all() {
  //motor_drive_all(0, 0, 0, 0);
  digitalWrite(23, LOW);
  digitalWrite(41, LOW);
  digitalWrite(43, LOW);
  digitalWrite(45, LOW);
}

/*Button Set 1: dpad_left, dpad_down, dpad_right, dpad_up, squareps, cross, circle, triangle*/
bool ps2_triangle() {
  return (bool)((buttonset1 / 1) & 0x1);
}

bool ps2_circle() {
  return (bool)((buttonset1 / 2) & 0x1);
}

bool ps2_cross() {
  return (bool)((buttonset1 / 4) & 0x1);
}

bool ps2_square() {
  return (bool)((buttonset1 / 8) & 0x1);
}

bool ps2_up() {
  return (bool)((buttonset1 / 16) & 0x1);
}

bool ps2_right() {
  return (bool)((buttonset1 / 32) & 0x1);
}

bool ps2_down() {
  return (bool)((buttonset1 / 64) & 0x1);
}

bool ps2_left() {
  return (bool)((buttonset1 / 128) & 0x1);
}

/*Button Set 2: select, start, L3, R3, L2, L1, R2, R1*/
bool ps2_R1() {
  return (bool)((buttonset2 / 1) & 0x1);
}

bool ps2_R2() {
  return (bool)((buttonset2 / 2) & 0x1);
}

bool ps2_L1() {
  return (bool)((buttonset2 / 4) & 0x1);
}

bool ps2_L2() {
  return (bool)((buttonset2 / 8) & 0x1);
}

bool ps2_R3() {
  return (bool)((buttonset2 / 16) & 0x1);
}

bool ps2_L3() {
  return (bool)((buttonset2 / 32) & 0x1);
}

bool ps2_start() {
  return (bool)((buttonset2 / 64) & 0x1);
}

bool ps2_select() {
  return (bool)((buttonset2 / 128) & 0x1);
}
