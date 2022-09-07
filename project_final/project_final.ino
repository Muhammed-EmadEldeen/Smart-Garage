#define alarm_leds 13 

#define servo1 8

#define ir_sensor_out 5
#define ir_sensor_in 4  
#define temp_sensor A5
#define ldr_sensor A0

#define a 26 
#define b 27
#define c 28
#define d 29
#define e 30
#define f 31
#define g 32

#define alarm_temperature 50.0

#include <Servo.h>

bool ir_sensor_out_old=true;
bool ir_sensor_in_old=true;

bool car_entering=false;
bool car_leaving=false;

int temperature_reading;

int gate=0;
int light = 0;
int empty_places=5;


unsigned long interval =5000;
//
unsigned long current_time = 0;
//
unsigned long new_time=0;



Servo myservo;




void setup() {
  
  pinMode(buzzer,OUTPUT);
  pinMode(smart_leds,OUTPUT);
  pinMode(alarm_leds,OUTPUT);

  pinMode(a,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(c,OUTPUT);
  pinMode(d,OUTPUT);
  pinMode(e,OUTPUT);
  pinMode(f,OUTPUT);
  pinMode(g,OUTPUT);

  myservo.attach(servo1);

  pinMode(ir_sensor_out,INPUT);
  pinMode(ir_sensor_in,INPUT);
  pinMode(temp_sensor,INPUT);
  pinMode(ldr_sensor,INPUT);
  
  Serial.begin(9600);
  display_empty_places();
}




void loop() {
  power_light();
  if(check_alarm_case()){
    alarm_case();
  }
  
  if (ir_out_detect()){
    if(car_leaving){
      car_leaving = false;
      empty_places++;
      display_empty_places();
      close_gate();
    }
    else {
      if(empty_places){
        current_time=millis();
        car_entering=true;
        open_gate();
        }
      else{
        no_available_places();
      }
    }
  }
  if (ir_in_detect()){
    if(car_entering){
      car_entering= false;
      empty_places--;
      display_empty_places();
      close_gate();
       
    }
    else{
      if(empty_places<5){
        current_time=millis();
        open_gate();
        car_leaving=true;
      }
    }
  }
  if(car_entering || car_leaving){
    new_time=millis();
    if(new_time-current_time>interval){
      alarm_car();
    }
  }

  
 if(!car_entering && !car_leaving && !check_alarm_case()){
   close_gate();
 }
}




bool ir_out_detect(){
  bool ir_sensor_out_new=digitalRead(ir_sensor_out);
  if(ir_sensor_out_old && !ir_sensor_out_new){
    ir_sensor_out_old =ir_sensor_out_new;
    return true;
  }
  else{
    ir_sensor_out_old =ir_sensor_out_new;
    return false;
  }
}


bool ir_in_detect(){
  bool ir_sensor_in_new=digitalRead(ir_sensor_in);
  if(ir_sensor_in_old && !ir_sensor_in_new){
    ir_sensor_in_old =ir_sensor_in_new;
    return true;
  }
  else{
    ir_sensor_in_old =ir_sensor_in_new;
    return false;
  }
}



void power_light(){
  if (analogRead(ldr_sensor)>700){
    while(light<256){
      analogWrite(smart_leds,light);
      light++;
      delay(5);
      
    }


  }
  else {
    while(light>=0){
      analogWrite(smart_leds,light);
      light--;
      delay(5);
    }
    
  }
}



float get_temp_value(){
  Serial.println(((5./1023.)*analogRead(temp_sensor))/0.01);
  return ((5./1023.)*analogRead(temp_sensor))/0.01;
}




void alarm_case(){

  open_gate();
  digitalWrite(alarm_leds,1);
  digitalWrite(buzzer,1);
  delay(100);
  digitalWrite(alarm_leds,0);
  digitalWrite(buzzer,0);
  delay(100);
}



void blinking_led(){
  digitalWrite(alarm_leds,1);
  delay(100);
  digitalWrite(alarm_leds,0);
  delay(100);
  
}


void buzzer_sound(){
  for(int i=0; i<150;i+=7){
      analogWrite(buzzer,i);
      delay(10);
  }
  for(int i=150; i>=0;i-=7){
    analogWrite(buzzer,i);
    delay(10);
  }
}


void close_gate(){
  while(gate>=0){
    myservo.write(gate);
    gate--;
    delay(10);
  }
}

void open_gate(){
  while(gate<80){
    myservo.write(gate);
    gate++;
    delay(20);
  }
}



void alarm_car(){
  open_gate();
  digitalWrite(alarm_leds,1);
  digitalWrite(buzzer,1);
  delay(300);
  digitalWrite(alarm_leds,0);
  digitalWrite(buzzer,0);
  delay(300);
  
}


bool check_alarm_case(){
  return get_temp_value()>alarm_temperature;
}  


void no_available_places(){
  for(int i=0;i<2;i++){
    clear_display();
    delay(100);
    display_empty_places();
    digitalWrite(buzzer,1);
    delay(100); 
    digitalWrite(buzzer,0);
    
  } 

}
 

// 7 segment optimizations

void display1() 
{ 
  digitalWrite(a,0);
  digitalWrite(b,1);
  digitalWrite(c,1);
  digitalWrite(d,0);
  digitalWrite(e,0);
  digitalWrite(f,0);
  digitalWrite(g,0);
} 

void display2() 
{
  digitalWrite(a,1);
  digitalWrite(b,1);
  digitalWrite(c,0);
  digitalWrite(d,1);
  digitalWrite(e,1);
  digitalWrite(f,0);
  digitalWrite(g,1);
} 

void display3() 
{ 
  digitalWrite(a,1);
  digitalWrite(b,1);
  digitalWrite(c,1);
  digitalWrite(d,1);
  digitalWrite(e,0);
  digitalWrite(f,0);
  digitalWrite(g,1);
} 

void display4() 
{ 
  digitalWrite(a,0);
  digitalWrite(b,1);
  digitalWrite(c,1);
  digitalWrite(d,0);
  digitalWrite(e,0);
  digitalWrite(f,1);
  digitalWrite(g,1);
  
  
} 

void display5() 
{ 
  digitalWrite(a,1);
  digitalWrite(b,0);
  digitalWrite(c,1);
  digitalWrite(d,1);
  digitalWrite(e,0);
  digitalWrite(f,1);
  digitalWrite(g,1);
  
  
} 

void display0() 
{ 
  digitalWrite(a,1);
  digitalWrite(b,1);
  digitalWrite(c,1);
  digitalWrite(d,1);  
  digitalWrite(e,1);  
  digitalWrite(f,1); 
  digitalWrite(g,0); 
} 

void clear_display() 
{ 
  digitalWrite(a,0);
  digitalWrite(b,0);
  digitalWrite(g,0);
  digitalWrite(c,0);
  digitalWrite(d,0);  
  digitalWrite(e,0);  
  digitalWrite(f,0);  
} 

void display_empty_places(){
  if (empty_places==5){
    display5();
  }
  else if(empty_places==4){
    display4();
  }
  else if(empty_places==3){
    display3();
  }
  else if(empty_places==2){
    display2();
  }
  else if(empty_places==1){
    display1();
  }
  else if(empty_places==0){
    display0();
  }
  else{
    clear_display();
  }
}
