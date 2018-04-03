
#define pneumatic_Left 22
#define pneumatic_Middle 28
#define pneumatic_Right 29
#define pneumatic_No_Used 23

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

void pneumatic_init(){
pinMode(pneumatic_Left,OUTPUT);
pinMode(pneumatic_Middle,OUTPUT);
pinMode(pneumatic_Right,OUTPUT);
pinMode(pneumatic_No_Used,OUTPUT);
digitalWrite(pneumatic_No_Used,LOW);
}

void pneumatic_running(){

 if(ps2_square()) pneumatik_launch_Left(1);
  else pneumatik_launch_Left(0);

   if(ps2_cross()) pneumatik_launch_Mid(1);
   else pneumatik_launch_Mid(0);

   if(ps2_circle()) pneumatik_launch_Right(1);
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
  if(stateLaunch){
    digitalWrite(pneumatic_Middle, HIGH);  
  } else{
    digitalWrite(pneumatic_Middle, LOW);
  }
}

void pneumatik_launch_Right(bool stateLaunch){
  if(stateLaunch){
    digitalWrite(pneumatic_Right, HIGH);  
  } else{
    digitalWrite(pneumatic_Right, LOW);
  }
} 




  
