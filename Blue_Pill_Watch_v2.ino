#include "Adafruit_GFX.h"
#include "SPI.h"
#include <Adafruit_SharpMem_SPI.h>
#include <RTClock.h>
#include <CmdMessenger.h>
#include "sprites.h"

//DEFINE PINS
#define PIN_SS PA4 //slave select on the LCD
#define PIN_BAT PA1 //battery read analog pin
#define PIN_BTN1 PB8
#define PIN_BTN2 PB9
#define PIN_BTN3 PB10

//GLOBAL VARS ------------------------------------------------------

//general state machine
byte machineState = 0;
const byte STATE_MAIN = 0;
const byte STATE_MENU_SELECT = 1;
const byte STATE_MENU_DISPLAY = 2;
const byte STATE_MENU_EDIT = 3;


//MENU SYSTEM VARS -------------------------------------------------
// -- menu state machine
byte menuState = 0; //current selected menu, regardless if only showing icon+text or the actual settings within
byte settingsState = 0; //current highlighted setting inside a menu
byte selectedSetting = 255; //the setting currently being edited, only valid for settings of type 'number' or 'select', where you need to change the value with left/right. 255 = nothing is selected
byte totalMenuItems = 4;
byte totalSettingsItems = 7;
// -- Struct declaration
struct setting {
  byte type; // 0 = empty, 1 = number, 2 = checkbox, 3 = select, 4 = exit
  String title;
  int value;
  byte options[3];
  //number field - no. digits, min, max

};
typedef struct setting Setting;
struct menuitem {
  String title;
  Setting settings[7];
};

typedef struct menuitem Menuitem;

// -- Build the menu structure
const Menuitem menuClock = { 
  "Clock", 
  {
    { 1,"Hour",11,{2,0,23} },
    { 1,"Minute",0,{2,0,59} },
    { 1,"Day",1,{2,1,31} },
    { 1,"Month",1,{2,1,12} },
    { 1,"Year",2016,{4,2016,0} },    
    { 2,"Auto-Update",1 },
    { 4,"Back" }
  }
};
const Menuitem menuBluetooth = {
  "Bluetooth",
  {
    { 2,"Enable"},
    { 0,"" },
    { 0,"" },
    { 0,"" },
    { 0,"" },
    { 0,"" },
    { 4,"Back" }
  }
};
const Menuitem menuGpio = {
  "GPIO",
  {
    { 2,"Pin 1" },
    { 2,"Pin 2" },
    { 2,"Pin 3" },
    { 0,"" },
    { 0,"" },
    { 0,"" },
    { 4,"Back" }
  }
};
const Menuitem menuLed = {
  "LED",
  {
    { 2,"Enabled" },
    { 0,"" },
    { 0,"" },
    { 0,"" },
    { 0,"" },
    { 0,"" },
    { 4,"Back" }
  }
};

//GLOBAL SETTINGS ---------------------------------------------
bool setTimeAuto = true;

//time
#define TZ_OFFSET 3
time_t tt = 0;
byte currentHours = 0;
byte currentMinutes = 0;
byte currentSeconds = 0;
bool colon = 0;
unsigned long previousMillis = 0;
unsigned long currentMilis = 0;

//battery level in percent 0-100
byte battery = 0;

//buttons
unsigned long btnMenuTimer = 0;
unsigned long btnLeftTimer = 0;
unsigned long btnRightTimer = 0;
boolean btnMenuDown = false;
boolean btnLeftDown = false;
boolean btnRightDown = false;

//display
#define BLACK 0
#define WHITE 1

//INIT CLASSES
// init the display class
Adafruit_SharpMem display(PIN_SS);

//init the rtc class
RTClock rtc (RTCSEL_LSE);

//serial commands
CmdMessenger cmdMessenger = CmdMessenger(Serial1);
enum {
  kAcknowledge         , // Command to acknowledge that cmd was received
  kError               , // Command to report errors
  iBat, // batery percentage status
  iTime, // current time
};


void setup(void) {
    //SETUP PINS
    pinMode(PIN_BTN1,INPUT_PULLDOWN);
    pinMode(PIN_BTN2,INPUT_PULLDOWN);
    pinMode(PIN_BTN3,INPUT_PULLDOWN);
    //START SERIAL
    Serial.begin(9600);
    Serial.println("Serial connected!");
    Serial1.begin(9600);
    //ATTACH COMMANDS TO SERIAL INPUT
    cmdMessenger.printLfCr();
    attachSerialCallbacks();
    //START DISPLAY
    display.begin();
    display.clearDisplay();
    display.setTextColor(BLACK,WHITE);
    display.refresh();
    //REFRESH DATA
    getBattery();
    //START SECONDS TIMER INTERRUPT
    //rtc.attachSecondsInterrupt(everySecond);
    cmdMessenger.sendCmd(kAcknowledge,"ON");
}

void loop(void) {
  handleButtons();
  cmdMessenger.feedinSerialData();
  //do this once a second
  unsigned long currentMillis = millis();
  if( currentMillis - previousMillis > 1000) {
      previousMillis = currentMillis;
      everySecond();
  }
  
}

void everySecond() {
   getRTC();
   if(currentSeconds == 30 ) { //once a minute
      getBattery();
   }
   serialDebug();
   switch(machineState) {
    case STATE_MAIN:
      //clock screen
      display.clearDisplay();
      drawClock();
    break;
    case STATE_MENU_SELECT:
      //display the menu

    break;
    case STATE_MENU_DISPLAY:
      //display settings within a menu

    break;
    case STATE_MENU_EDIT:
      //editing a setting within a menu

    default:
      break;
  }
  
  drawTopbar();
  display.refresh();
}

//link Serial (the main serial) and Serial1 (the bluetooth serial) together
void forwardSerial(){
  if (Serial1.available()) {
    int inByte = Serial1.read();
    Serial.write(inByte);
  }
}

void serialDebug(){
  Serial.print(machineState);
  Serial.print("/menuSelect:");
  Serial.print(menuState);
}


void getRTC() {
    tt = rtc.getTime();
    breakTime(tt);
}

void breakTime(time_t timeInput){
  uint8_t year;
  uint8_t month, monthLength;
  uint32_t time;
  unsigned long days;

  time = (uint32_t)timeInput;
  currentSeconds = time % 60;
  time /= 60; // now it is minutes
  currentMinutes = time % 60;
  time /= 60; // now it is hours
  currentHours = time % 24;
  /*
  time /= 24; // now it is days
  tm.Wday = ((time + 4) % 7) + 1;  // Sunday is day 1 
  
  year = 0;  
  days = 0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  tm.Year = year; // year is offset from 1970 
  
  days -= LEAP_YEAR(year) ? 366 : 365;
  time  -= days; // now it is days in this year, starting at 0
  
  days=0;
  month=0;
  monthLength=0;
  for (month=0; month<12; month++) {
    if (month==1) { // february
      if (LEAP_YEAR(year)) {
        monthLength=29;
      } else {
        monthLength=28;
      }
    } else {
      monthLength = monthDays[month];
    }
    
    if (time >= monthLength) {
      time -= monthLength;
    } else {
        break;
    }
  }
  tm.Month = month + 1;  // jan is month 1  
  tm.Day = time + 1;     // day of month
  */
}

