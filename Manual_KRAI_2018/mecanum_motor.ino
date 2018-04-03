/*-------------Mobilisasi--------------*/
#define display_speed true
#define CCW 1
#define CW 0
#define motor_cap 255

int CS1[]  = { -1, 30, 32, 34, 38}; // CS1[0] is not used
int CS2[]  = { -1, 31, 33, 35, 39};
int pwm[]  = { -1,  5,  6, 7,  9};

//range : 0 ~ 1
float error1 = 0.89277;//0.938440822 //0.91875
float error2 = 1;//0.906521183 //0.7
float error3 = 0.935319;//0.922782865 //0.8596491228070175
float error4 = 0.960664;//1           //1
int PS2_x = 0, PS2_y = 0;
float max_speed = 0, speed_x = 0, speed_y = 0, omega = 0;
float  rotation_const = 0, L1 = 0, L2 = 0; //L1 = Y-axis distance from each wheel to COG ; L2 = X-axis distance from each wheel to COG
float v[] = {0, 0, 0, 0, 0};

void motor_mecanum_init() {
  for (int i = 1; i <= 4; i++) {
    pinMode(CS1[i], OUTPUT);
    pinMode(CS2[i], OUTPUT);
    pinMode(pwm[i], OUTPUT);
  }
}

void start_mobilitation(float max_speed, float rotation_const, bool speed_type) {
  L1 = 1; //L1 = Y-axis distance from each wheel to COG
  L2 = 1; //L2 = X-axis distance from each wheel to COG

  PS2_x = ps2_LY;
  PS2_y = ps2_LX;

  if (ps2_L2()) omega = 1;
  else if (ps2_R2()) omega = -1;
  else omega = 0;

  /********************************************************Normal Steering***************************************************/
  if (ps2_L1() == 0 && ps2_R1() == 0 && ps2_LY == 128 && ps2_LX == 128) { //Normal D-Pad

    error1 = 1; error2 = 1; error3 = 1; error4 = 1;
    speed_x = 0;
    speed_y = 0;

    v[1] = 0;
    v[2] = 0;
    v[3] = 0;
    v[4] = 0;

    if (ps2_up()) {
      v[1] += 120;
      v[2] += 120;
      v[3] += 120;
      v[4] += 120;
      omega += 0.185;
    } 
    if (ps2_down()) {
      v[1] -= 120;
      v[2] -= 120;
      v[3] -= 120;
      v[4] -= 120;
      omega += 0.185;
    } 
    if (ps2_left()) {
      v[1] -= (60);
      v[2] += (80);
      v[3] += (80);
      v[4] -= (80);
      //omega += 0.512;
      omega += 0.185;
    } 
    if (ps2_right()) {
      v[1] += (75);
      v[2] -= (65);
      v[3] -= (75);
      v[4] += (75);
      //omega += -0.548;
      omega += 0.185;
    }
    //v[1] += - (L1 + L2) * omega * rotation_const;
    //v[2] += (L1 + L2) * omega * rotation_const;
    //v[3] +=  - (L1 + L2) * omega * rotation_const;
    //v[4] += (L1 + L2) * omega * rotation_const;
  }
  if (ps2_L1() == 0 && ps2_R1() == 0 && (ps2_LY != 128 || ps2_LX != 128)) { //Normal Analog

    error1 = 1; error2 = 1; error3 = 1; error4 = 1;
    speed_x = - (128 - (float)PS2_x) / 128 * max_speed;
    speed_y = (128 - (float)PS2_y) / 128 * max_speed;
    //       ^ xs
    //       |
    // ys    |
    // <-----|
    v[1] = (speed_x - speed_y - (L1 + L2) * omega * rotation_const) * error1;
    v[2] = (speed_x + speed_y + (L1 + L2) * omega * rotation_const) * error2;
    v[3] = (speed_x + speed_y - (L1 + L2) * omega * rotation_const) * error3;
    v[4] = (speed_x - speed_y + (L1 + L2) * omega * rotation_const) * error4;
  }
  
  /********************************************************Backward Steering*********************************************************************/
  if (ps2_L1() && ps2_R1() && ps2_LY == 128 && ps2_LX == 128) { //Backward D-Pad

    error1 = 1; error2 = 1; error3 = 1; error4 = 1;
    speed_x = 0;
    speed_y = 0;

    v[1] = 0;
    v[2] = 0;
    v[3] = 0;
    v[4] = 0;

    if (ps2_down()) {
      v[1] += 120;
      v[2] += 120;
      v[3] += 120;
      v[4] += 120;
      omega += 0.185;
    } 
    if (ps2_up()) {
      v[1] -= 120;
      v[2] -= 120;
      v[3] -= 120;
      v[4] -= 120;
      omega += 0.15;
    } 
   if (ps2_right()) {
      v[1] -= (60);
      v[2] += (60);
      v[3] += (60);
      v[4] -= (60);
      omega += 0.512;
    }
    if (ps2_left()) {
      v[1] += (60);
      v[2] -= (60);
      v[3] -= (60);
      v[4] += (60);
      omega += -0.548;
    }
    v[1] += - (L1 + L2) * omega * rotation_const;
    v[2] += (L1 + L2) * omega * rotation_const;
    v[3] +=  - (L1 + L2) * omega * rotation_const;
    v[4] += (L1 + L2) * omega * rotation_const;
  }
  //---------------------
  if (ps2_L1() && ps2_R1() && (ps2_LY != 128 || ps2_LX != 128)) { //Backward Analog 
    
    error1 = 1; error2 = 1; error3 = 1; error4 = 1;
    speed_x = (128 - (float)PS2_x) / 128 * max_speed;
    speed_y = - (128 - (float)PS2_y) / 128 * max_speed;
    //       ^ xs
    //       |
    // ys    |
    // <-----|
    v[1] = (speed_x - speed_y - (L1 + L2) * omega * rotation_const) * error1;
    v[2] = (speed_x + speed_y + (L1 + L2) * omega * rotation_const) * error2;
    v[3] = (speed_x + speed_y - (L1 + L2) * omega * rotation_const) * error3;
    v[4] = (speed_x - speed_y + (L1 + L2) * omega * rotation_const) * error4;
  }
  
  /****************************************************Mobilitation Equation*************************************************/
  //       ^ xs
  //       |
  // ys    |
  // <-----|
  //  v[1] = (speed_x - speed_y - (L1 + L2) * omega * rotation_const) * error1;
  //  v[2] = (speed_x + speed_y + (L1 + L2) * omega * rotation_const) * error2;
  //  v[3] = (speed_x + speed_y - (L1 + L2) * omega * rotation_const) * error3;
  //  v[4] = (speed_x - speed_y + (L1 + L2) * omega * rotation_const) * error4;
  motor_drive_all(v[1], v[2], v[3], v[4]);
  

  if (display_speed) {
    Serial.print("vx:");
    Serial.print(speed_x);
    Serial.print(" vy:");
    Serial.print(speed_y);
    Serial.print(" v1:");
    Serial.print(v[1]);
    Serial.print(" v2:");
    Serial.print(v[2]);
    Serial.print(" v3:");
    Serial.print(v[3]);
    Serial.print(" v4:");
    Serial.print(v[4]);
    Serial.print(" Omega:");
    Serial.print(omega);
    Serial.println();
  }
}// void start mobilitation

void motor_drive(int v, int num) {
  if (v > motor_cap) v = motor_cap;
  if (v < -motor_cap) v = -motor_cap;
  if (v > 0) {
    digitalWrite(CS1[num], CW);
    digitalWrite(CS2[num], CCW);
    analogWrite(pwm[num], v);
  } else if (v < 0) {
    digitalWrite(CS1[num], CCW);
    digitalWrite(CS2[num], CW);
    analogWrite(pwm[num], -v);
//  } else if (v == 0 || MEN1[num] == LOW || MEN2[num] == LOW) {
  } else if (v == 0 ) {
    digitalWrite(CS1[num], HIGH);
    digitalWrite(CS2[num], HIGH);
    analogWrite(pwm[num], 0);
  }
}

void motor_drive_all(int v1, int v2, int v3, int v4) {
  motor_drive(v1, 1);
  motor_drive(v2, 2);
  motor_drive(v3, 3);
  motor_drive(v4, 4);
}

float map_to_float(float x, float a, float b, float c, float d) {
  float f = x / (b - a) * (d - c) + c;
  return f;
}


