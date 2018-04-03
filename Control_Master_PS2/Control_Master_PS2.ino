#include <PS2X_lib.h>
#include <LiquidCrystal.h>

PS2X ps2x;
LiquidCrystal lcd(8, 9, 10, 11, 2, 3);

#define status1 12
#define status2 13

#define PS2_CLK 4//47
#define PS2_CMD 5//49
#define PS2_ATT 6//51
#define PS2_DAT 7//53
#define pressures false
#define rumble false
int error;
int menu;
unsigned long Millis = 0;   

char flagerror = '?';
char flagawal = '<';
int LX = 128; /*Left analog X-axis*/
int LY = 128; /*Left analog Y-axis*/
int RX = 128; /*Right analog X-axis*/
int RY = 128; /*Right analog Y-axis*/
/*Button Set 1: dpad_left, dpad_down, dpad_right, dpad_up, squareps, cross, circle, triangle*/
int buttonset1 = 0;
/*Button Set 2: select, start, L3, R3, L2, L1, R2, R1*/
int buttonset2 = 0;
/*Button Set 1 variables*/
int triangle = 0;
int circle = 0;
int cross = 0;
int squareps = 0;
int dpad_up = 0;
int dpad_right = 0;
int dpad_down = 0;
int dpad_left = 0;
/*Button Set 2 variables*/
int R1 = 0;
int R2 = 0;
int L1 = 0;
int L2 = 0;
int R3 = 0;
int L3 = 0;
int start = 0;
int select = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(status1, INPUT);
  pinMode(status2, INPUT);
  lcd_init();
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATT, PS2_DAT, pressures, rumble);
}

void loop() {
  /*Status Indikator*/
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATT, PS2_DAT, pressures, rumble);
  if (millis() - Millis >= 180) {
    lcd.clear();  
    if (digitalRead(status2)) menu--;
    if (digitalRead(status1)) menu++;
    if (menu > 2) menu = 0;
    if (menu < 0) menu = 2;
    switch (menu) {
      case 0:  
        lcd.setCursor(0, 0);
        lcd.print("<CONTROL MASTER>");
        lcd.setCursor(0, 1);
        lcd.print("Stick:");
        if(error == 0) lcd.print("Connected");
        else if(error == 1) lcd.print("Disconnected");
        else if(error == 2) lcd.print("CMD Error");
        else if(error == 3) lcd.print("Refusing");   
        break;
      case 1:
        lcd.setCursor(0, 0);
        lcd.print("LX:");
        lcd.print(LX);
        lcd.setCursor(6, 0);
        lcd.print("  RX:");
        lcd.print(RX);
        lcd.setCursor(0, 1);
        lcd.print("LY:");
        lcd.print(LY);
        lcd.setCursor(6, 1);
        lcd.print("  RY:");
        lcd.print(RY);
        break;
      case 2:
        lcd.setCursor(0, 0);
        lcd.print("BTNSet1:");      
        if (buttonset1 < 128) lcd.print("0");
        if (buttonset1 < 64) lcd.print("0");
        if (buttonset1 < 32) lcd.print("0");
        if (buttonset1 < 16) lcd.print("0");
        if (buttonset1 < 8) lcd.print("0");
        if (buttonset1 < 4) lcd.print("0");
        if (buttonset1 < 2) lcd.print("0");
        lcd.print(buttonset1,BIN);
        lcd.setCursor(0, 1);
        lcd.print("BTNSet2:");
        if (buttonset2 < 128) lcd.print("0");
        if (buttonset2 < 64) lcd.print("0");
        if (buttonset2 < 32) lcd.print("0");
        if (buttonset2 < 16) lcd.print("0");
        if (buttonset2 < 8) lcd.print("0");
        if (buttonset2 < 4) lcd.print("0");
        if (buttonset2 < 2) lcd.print("0");
        lcd.print(buttonset2,BIN);
        break;
      default:
        lcd.print("Menu Error");
        break;
    }
  }  
  
  /*Read data from ps2 controller*/
  LX = ps2x.Analog(PSS_LX);
  LY = ps2x.Analog(PSS_LY);
  RX = ps2x.Analog(PSS_RX);
  RY = ps2x.Analog(PSS_RY);
  buttonset1 = buttonset1GetValue(); /*Button Set 1: dpad_left, dpad_down, dpad_right, dpad_up, squareps, cross, circle, triangle*/
  buttonset2 = buttonset2GetValue(); /*Button Set 2: select, start, L3, R3, L2, L1, R2, R1*/

  /*Send data to Serial*/
  if(error == 0){
    Serial.write(flagawal);
    Serial.write(LX);
    Serial.write(LY);
    Serial.write(RX);
    Serial.write(RY);
    Serial.write(buttonset1);
    Serial.write(buttonset2);    
  }else{
    Serial.write(flagerror);
    Serial.write(128);
    Serial.write(128);
    Serial.write(128);
    Serial.write(128);
    Serial.write(0);
    Serial.write(0);    
  }
}

void lcd_init() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("<CONTROL MASTER>");
}

int buttonset1GetValue() {
  if (ps2x.Button(PSB_GREEN)) triangle = 1;
  else triangle = 0;
  if (ps2x.Button(PSB_RED)) circle = 2;
  else circle = 0;
  if (ps2x.Button(PSB_BLUE)) cross = 4;
  else cross = 0;
  if (ps2x.Button(PSB_PINK)) squareps = 8;
  else squareps = 0;
  if (ps2x.Button(PSB_PAD_UP)) dpad_up = 16;
  else dpad_up = 0;
  if (ps2x.Button(PSB_PAD_RIGHT)) dpad_right = 32;
  else dpad_right = 0;
  if (ps2x.Button(PSB_PAD_DOWN)) dpad_down = 64;
  else dpad_down = 0;
  if (ps2x.Button(PSB_PAD_LEFT)) dpad_left = 128;
  else dpad_left = 0;
  return (triangle + circle + cross + squareps + dpad_up + dpad_right + dpad_down + dpad_left);
}

int buttonset2GetValue() {
  if (ps2x.Button(PSB_R1)) R1 = 1;
  else R1 = 0;
  if (ps2x.Button(PSB_R2)) R2 = 2;
  else R2 = 0;
  if (ps2x.Button(PSB_L1)) L1 = 4;
  else L1 = 0;
  if (ps2x.Button(PSB_L2)) L2 = 8;
  else L2 = 0;
  if (ps2x.Button(PSB_R3)) R3 = 16;
  else R3 = 0;
  if (ps2x.Button(PSB_L3)) L3 = 32;
  else L3 = 0;
  if (ps2x.Button(PSB_START)) start = 64;
  else start = 0;
  if (ps2x.Button(PSB_SELECT)) select = 128;
  else select = 0;
  return (R1 + R2 + L1 + L2 + R3 + L3 + start + select);
}
