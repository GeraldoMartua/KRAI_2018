
#define pneumatic_Left 11
#define pneumatic_Middle 10
#define pneumatic_Right 41
#define pneumatic_No_Used 23

int i = 1;

//void pneumatik_launch_Mid(bool stateLaunch);

//void pneumatik_launch_Right(bool stateLaunch);

//bool Left_Value = 0;
//bool Middle_Value = 0;
//bool Right_Value = 0;

bool pneumatic_LeftState = LOW;         // the current state of the output pin
bool button_LeftState = LOW;             // the current reading from the input pin
bool lastButton_LeftState = HIGH;   // the previous reading from the input pin

long lastDebounceTime_Left = 0;    // the last time the output pin was toggled
long debounceDelay_Left = 50;    // the debounce time; increase if the output flickers


bool pneumatic_MiddleState = LOW;         // the current state of the output pin
bool button_MiddleState = LOW;             // the current reading from the input pin
bool lastButton_MiddleState = HIGH;   // the previous reading from the input pin

long lastDebounceTime_Middle = 0;    // the last time the output pin was toggled
long debounceDelay_Middle = 50;    // the debounce time; increase if the output flickers


bool pneumatic_RightState = LOW;         // the current state of the output pin
bool button_RightState = LOW;             // the current reading from the input pin
bool lastButton_RightState = HIGH;   // the previous reading from the input pin

long lastDebounceTime_Right = 0;    // the last time the output pin was toggled
long debounceDelay_Right = 50;    // the debounce time; increase if the output flickers


void pneumatic_init(){
pinMode(pneumatic_Left,OUTPUT);
pinMode(pneumatic_Middle,OUTPUT);
pinMode(pneumatic_Right,OUTPUT);
pinMode(pneumatic_No_Used,OUTPUT);
digitalWrite(pneumatic_No_Used,LOW);
}

void pneumatic_running(){


   if(ps2_square() || ps2_triangle() ) pneumatik_launch_Left(1);
   else pneumatik_launch_Left(0);

   if(ps2_cross() || ps2_triangle() ) pneumatik_launch_Mid(1);
   else pneumatik_launch_Mid(0);

   if(ps2_circle() || ps2_triangle() ) pneumatik_launch_Right(1);
   else pneumatik_launch_Right(0);
  
  
}

void pneumatik_launch_Left(bool stateLaunch){
  /*if(stateLaunch){
    digitalWrite(pneumatic_Left, HIGH);  
  } else{
    digitalWrite(pneumatic_Left, LOW);
  }*/

  if(stateLaunch != lastButton_LeftState){
    lastDebounceTime_Left = millis();
    lastButton_LeftState = stateLaunch;
    }

  if((millis() - lastDebounceTime_Left) > debounceDelay_Left){
    if(button_LeftState != lastButton_LeftState){
      button_LeftState = lastButton_LeftState;
      if(button_LeftState == 1){
        pneumatic_LeftState = !pneumatic_LeftState;
        digitalWrite(pneumatic_Left, pneumatic_LeftState);
        }
      }
    }
}


void pneumatik_launch_Mid(bool stateLaunch){
  /*if(stateLaunch){
    digitalWrite(pneumatic_Middle, HIGH);  
  } else{
    digitalWrite(pneumatic_Middle, LOW);
  }*/
  if(stateLaunch != lastButton_MiddleState){
    lastDebounceTime_Middle = millis();
    lastButton_MiddleState = stateLaunch;
    }

  if((millis() - lastDebounceTime_Middle) > debounceDelay_Middle){
    if(button_MiddleState != lastButton_MiddleState){
      button_MiddleState = lastButton_MiddleState;
      if(button_MiddleState == 1){
        pneumatic_MiddleState = !pneumatic_MiddleState;
        digitalWrite(pneumatic_Middle, pneumatic_MiddleState);
        }
      }
    }
}

void pneumatik_launch_Right(bool stateLaunch){
  /*if(stateLaunch){
    digitalWrite(pneumatic_Right, HIGH);  
  } else{
    digitalWrite(pneumatic_Right, LOW);
  }*/
  if(stateLaunch != lastButton_RightState){
    lastDebounceTime_Right = millis();
    lastButton_RightState = stateLaunch;
    }

  if((millis() - lastDebounceTime_Right) > debounceDelay_Right){
    if(button_RightState != lastButton_RightState){
      button_RightState = lastButton_RightState;
      if(button_RightState == 1){
        pneumatic_RightState = !pneumatic_RightState;
        digitalWrite(pneumatic_Right, pneumatic_RightState);
        }
      }
    }
} 




  
