//Universal-Solder PLC MEGA328 with DS1307 RTC
//
// this tests all the digital and analog in/out available on the board

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h> //include for RTC DS1307

//define all the pins
int digitalIn1 = 7;
int digitalIn2 = 8;
int digitalIn3 = 12;
int digitalIn4 = 13;

int relay1 = 2;
int relay2 = 3;
int relay3 = 4;
int relay4 = 5;
int relay5 = A2;
int relay6 = A3;

int analogIn1 = A0;
int analogIn2 = A1;
int analogIn3 = A6;
int analogIn4 = A7;

int analogOut1 = 6;
int analogOut2 = 9;
int analogOut3 = 10;
int analogOut4 = 11;

//for the blink without delay thing
//ripped from the "blink without delay" example. With slight modifications

unsigned long previousMillis = 0;
const long interval = 1000;

unsigned long previousMillis1 = 0;
const long interval1 = 1500;

int relay5state = LOW;
int relay6state = LOW;

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

tmElements_t tm;

void setup() {
  // put your setup code here, to run once:

  pinMode(digitalIn1, INPUT);
  pinMode(digitalIn2, INPUT);
  pinMode(digitalIn3, INPUT);
  pinMode(digitalIn4, INPUT);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);

  pinMode(analogIn1, INPUT);
  pinMode(analogIn2, INPUT);
  pinMode(analogIn3, INPUT);
  pinMode(analogIn4, INPUT);

  pinMode(analogOut1, OUTPUT);
  pinMode(analogOut2, OUTPUT);
  pinMode(analogOut3, OUTPUT);
  pinMode(analogOut4, OUTPUT);

  bool parse=false;
  bool config=false;

  // get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      config = true;
    }
  }

  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor
  delay(200);
  
  if (parse && config) {
    Serial.print("DS1307 configured Time=");
    Serial.print(__TIME__);
    Serial.print(", Date=");
    Serial.println(__DATE__);
  } else if (parse) {
    Serial.println("DS1307 Communication Error :-{");
    Serial.println("Please check your circuitry");
  } else {
    Serial.print("Could not parse info from the compiler, Time=\"");
    Serial.print(__TIME__);
    Serial.print("\", Date=\"");
    Serial.print(__DATE__);
    Serial.println("\"");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

// read the analog inputs, re-scale and push to the analog outputs

  analogWrite (analogOut1, map(analogRead(analogIn1) , 0 , 1023 , 0 , 255));
  analogWrite (analogOut2, map(analogRead(analogIn2) , 0 , 1023 , 0 , 255));
  analogWrite (analogOut3, map(analogRead(analogIn3) , 0 , 1023 , 0 , 255));
  analogWrite (analogOut4, map(analogRead(analogIn4) , 0 , 1023 , 0 , 255));

// read the digital inputs, invert the logic and push to the first 4 relay outputs

  digitalWrite (relay1 , !digitalRead(digitalIn1));
  digitalWrite (relay2 , !digitalRead(digitalIn2));
  digitalWrite (relay3 , !digitalRead(digitalIn3));
  digitalWrite (relay4 , !digitalRead(digitalIn4));


 // this bit is ripped from the "blink without delay" example. With slight modifications
 // it toggles relay 5 and 6 at differnt speeds.

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you toggled the relay
    previousMillis = currentMillis;

    // if the relay is off turn it on and vice-versa:
    if (relay5state == LOW) {
      relay5state = HIGH;
    } else {
      relay5state = LOW;
    }

    digitalWrite(relay5 , relay5state);

    unsigned long currentMillis1 = millis();

    if (currentMillis1 - previousMillis1 >= interval1) {
      // save the last time you toggled the relay
      previousMillis1 = currentMillis1;

      // if the relay is off turn it on and vice-versa:
      if (relay6state == LOW) {
        relay6state = HIGH;
      } else {
        relay6state = LOW;
      }

      digitalWrite(relay6 , relay6state);

    }
  }
  
  tmElements_t tm;
  if (RTC.read(tm)) {
    Serial.print("Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);
}

bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}
void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
