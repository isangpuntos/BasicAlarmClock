#include <LiquidCrystal.h>
#include <EEPROM.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int seconds = 55;
int minutes = 30;
int hours = 2;
int alarm_seconds = 55;
int alarm_minutes = 30;
int alarm_hours = 2;
int adjustAlarmMode = 0;
unsigned long previousMs = 0;
char meridiem[3] = "AM";
char ss[3] = "55";
char mm[3] = "50";
char hh[3] = "02";
char alarm_ss[3] = "55";
char alarm_mm[3] = "50";
char alarm_hh[3] = "02";
char alarm_meridiem[3] = "AM";

void setup() 
{
  lcd.begin(16, 2);
  Serial.begin(9600);
  //EEPROM.write(0,3);
  //EEPROM.write(1,0);
  //EEPROM.write(2,5);
  //EEPROM.write(3,5);
  //EEPROM.write(4,43);
  //EEPROM.write(5,35);
  hours = EEPROM.read(0);
  minutes = EEPROM.read(1);
  seconds = EEPROM.read(2);
  sprintf(ss,"%02d",seconds);
  sprintf(mm,"%02d",minutes);
  sprintf(hh,"%02d",hours);
  
  alarm_hours = EEPROM.read(3);
  alarm_minutes = EEPROM.read(4);
  alarm_seconds = EEPROM.read(5);
  sprintf(alarm_ss,"%02d",alarm_seconds);
  sprintf(alarm_mm,"%02d",alarm_minutes);
  sprintf(alarm_hh,"%02d",alarm_hours);
  
  pinMode(6,INPUT);
  
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
}

void loop() 
{ 
  adjustAlarmMode = digitalRead(A4);
  runClock(); 
  if(adjustAlarmMode)
  {
    adjustAlarm();
    lcd.setCursor(0, 0);
    lcd.print("SET ALARM:      ");
    lcd.setCursor(0, 1);
    lcd.print(alarm_hh); lcd.print(":"); lcd.print(alarm_mm);lcd.print(":"); lcd.print(alarm_ss); lcd.print(" "); lcd.print(alarm_meridiem);   
  }
  else
  {
   lcd.setCursor(0, 0);
   lcd.print(hh); lcd.print(":"); lcd.print(mm);lcd.print(":"); lcd.print(ss); lcd.print(" "); lcd.print(meridiem);
   lcd.setCursor(0, 1);
   lcd.print("                ");
  }

  if(alarm_hours == hours && alarm_minutes == minutes && alarm_seconds == seconds && (strcmp(alarm_meridiem,meridiem)==0))
  {
    digitalWrite(7,HIGH);
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
  }
  if(digitalRead(6))
  {
    digitalWrite(7,LOW);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
  }
}

void runClock()
{
  unsigned long currentMs = millis();
  int adjustSec = digitalRead(A0);
  int adjustMin = digitalRead(A1);
  int adjustHrs = digitalRead(A2);
  int adjustMer = digitalRead(A3);

  if(currentMs - previousMs > 1000 || (adjustSec && !adjustAlarmMode))
  {
    if(adjustSec)
      delay(250);
    seconds++;
    previousMs = currentMs;
    sprintf(ss,"%02d",seconds);
    EEPROM.update(2, seconds);
  }
  if(seconds >= 60 || (adjustMin && !adjustAlarmMode))
  {
    if(adjustMin)
      delay(250);
    seconds %= 60;
    sprintf(ss,"%02d",seconds);
    minutes++;
    sprintf(mm,"%02d",minutes);
    EEPROM.update(1, minutes);
  }
  if(minutes >= 60 || (adjustHrs && !adjustAlarmMode))
  {
    if(adjustHrs)
      delay(250);
    minutes %= 60;
    sprintf(mm,"%02d",minutes);
    hours++;
    hours = hours%13;
    hours += (hours == 0? 1 : 0);
    sprintf(hh,"%02d",hours);
    if(hours == 12) 
    {
      meridiem[0] = meridiem[0] == 'A'? 'P' : 'A';
    }
    EEPROM.update(0, hours);
  }
  if(adjustMer)
  {
      delay(150);
      meridiem[0] = meridiem[0] == 'A'? 'P' : 'A';
  }
}


void adjustAlarm()
{
  unsigned long currentMs = millis();
  int adjustSec = digitalRead(A0);
  int adjustMin = digitalRead(A1);
  int adjustHrs = digitalRead(A2);
  int adjustMer = digitalRead(A3);
  if(adjustSec)
  {
    delay(150);
    alarm_seconds++;
    alarm_seconds %= 60;
    sprintf(alarm_ss,"%02d",alarm_seconds);
    EEPROM.update(5, alarm_seconds);
  }
  if(adjustMin)
  {
    delay(150);
    alarm_minutes++;
    sprintf(alarm_mm,"%02d",alarm_minutes);
    EEPROM.update(4, alarm_minutes);
  }
  if(adjustHrs)
  {
    delay(150);
    alarm_hours++;
    alarm_hours = alarm_hours%13;
    alarm_hours += (alarm_hours == 0? 1 : 0);
    sprintf(alarm_hh,"%02d",alarm_hours);
    EEPROM.update(3, alarm_hours);
  }
  if(adjustMer)
  {
      delay(150);
      alarm_meridiem[0] = alarm_meridiem[0] == 'A'? 'P' : 'A';
  }
}
