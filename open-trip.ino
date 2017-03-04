// Base includes
#include <EEPROM.h>
#include "EEPROMAnything.h"
#include "ArraySize.h"

// Buttons includes
#include <Button.h>
#include <ButtonEventCallback.h>
#include <PushButton.h>
#include <Bounce2.h>

// Compass includes
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

const int pin_sensor = 2;
const int pin_display_a_cs = 3;
const int pin_display_a_wr = 4;
const int pin_display_a_data = 5;
const int pin_display_b_cs = 6;
const int pin_display_b_wr = 7;
const int pin_display_b_data = 8;
const int pin_joystick_up = 9;
const int pin_joystick_down = 10;
const int pin_motor_output_a = 11;
const int pin_motor_output_b = 12;
const int pin_button_up = 14;
const int pin_button_center = 15;
const int pin_button_down = 16;
const int pin_light = 17;

int selectedMenuOption = 0;
int selectedSubMenuOption = 0;
int btnHoldDelay = 1000;
int btnHoldRepeat = 30;
// int showInDisplay1 = 0;
// int showInDisplay2 = 2;

// IMPORTANT: these are the indexes for menu[] array.
// If new options are added, update these values.
int maxMenuIndex = 5;
int minSubmenuIndex = 6; // Partial
int maxSubmenuIndex = 9; // Speed

long int displayValues[] = {
    0, // Partial
    0, // Total
    0, // Heading
    0  // Speed
};

struct Configuration {
  int version;
  int showInDisplay1;
  int showInDisplay2;
  int lightsOn;
  int autoCalibrate;
  float trip_partial;
  float trip_total;
  int circumference;
  float declinationAngle;
} config;

bool inMenu = false;
bool inSubMenu = false;
bool editMode = false;
bool lightsOn = false;
bool autoCalibrate = false;

unsigned long previousMillis = 0;
const long displayRefreshInterval = 500;

PushButton button_up = PushButton(pin_button_up);
PushButton button_center = PushButton(pin_button_center);
PushButton button_down = PushButton(pin_button_down);
// PushButton sensor = PushButton(pin_sensor);

// Assign a unique ID to this sensor at the same time
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

// Interruption variables for wheel sensor
const byte interruptPin = pin_sensor;
volatile byte revs = 0;

void setup() {
  Serial.begin(9600);
  
  loadConfig();

  pinMode(pin_display_a_cs, OUTPUT);
  pinMode(pin_display_a_wr, OUTPUT);
  pinMode(pin_display_a_data, OUTPUT);
  pinMode(pin_display_b_cs, OUTPUT);
  pinMode(pin_display_b_wr, OUTPUT);
  pinMode(pin_display_b_data, OUTPUT);

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), revsCount, FALLING);

  // Init displays
  Init_1621(0);
  Init_1621(1);

  button_up.onPress(onButtonUpPressed);
  button_up.onHoldRepeat(btnHoldDelay, btnHoldRepeat, onButtonUpPressed);

  button_center.onRelease(onButtonCenterReleased);
  button_center.onHold(btnHoldDelay, onButtonCenterHold);

  button_down.onPress(onButtonDownPressed);
  button_down.onHoldRepeat(btnHoldDelay, btnHoldRepeat, onButtonDownPressed);

  // sensor.onPress(onSensorDetect);

  // Initialise the compass
  if(!mag.begin()) {
    Serial.println(F("Ooops, no HMC5883 detected... Check your wiring!"));
  }
}

void loop() {
  button_up.update();
  button_center.update();
  button_down.update();
  //sensor.update();
  sensorUpdate();

  calculateHeading();
}

// Interrupt Service Routine (ISR)
void revsCount() {
  revs++;
}

void sensorUpdate() {
  // This will count as many revolutions as added by 
  // the interruptions and reset them for the next loop
  config.trip_partial += (config.circumference / 100000.0) * revs; // Count 1 every 100 meters
  
  if(revs > 0){
    revs = 0;
    
    Serial.println(F("SENSOR detected"));
    displayValues[0] = config.trip_partial;
    Serial.print("trip_partial: ");
    Serial.println(config.trip_partial, 6);
    
    updateScreens();
  }
}

void loadConfig() {
  Serial.println(F("---> Loading saved data..."));

  EEPROM_read(0, config);

  Serial.println(config.version);
  Serial.println(config.showInDisplay1);
  Serial.println(config.showInDisplay2);
  Serial.println(config.lightsOn);
  Serial.println(config.autoCalibrate);
  Serial.println(config.trip_partial);
  Serial.println(config.trip_total);
  Serial.println(config.circumference);
  Serial.println(config.declinationAngle, 6);

  if(config.version != 123){
    Serial.println(F("---> No configuration data found. Setting up default values..."));
    
    config.showInDisplay1 = 0;
    config.showInDisplay2 = 2;
    config.lightsOn = 0;
    config.autoCalibrate = 0;
    config.trip_partial = 0;
    config.trip_total = 0;
    config.circumference = 2007;
    config.declinationAngle = 0.01425352f; // Madrid
    config.version = 123;

    saveConfig();
  }

  Serial.println(F("---> DONE"));
}

void saveConfig(){
  EEPROM_write(0, config);
  Serial.println(F("---> CONFIG SAVED!"));
}

void onButtonUpPressed(Button& btn){
  Serial.println(F("button UP pressed"));

  if(inMenu){
    HT1621_all_off(16, 0);
    HT1621_all_off(16, 1);

    if(selectedMenuOption > 0)
      selectedMenuOption--;
  } else if(inSubMenu){
  } else if(editMode){
  } else {
    config.trip_partial += 1;
    displayValues[0] = config.trip_partial;
  }

  updateScreens();
}

void onButtonCenterReleased(Button& btn, int duration){
  // This avoids calling this callback when releasing
  // the center button after a Hold event
  if(duration >= btnHoldDelay)
    return;

  Serial.println(F("button CENTER pressed"));
  if(inMenu){
    inMenu = false;
    inSubMenu = true;
    Serial.println(F("State changed: inSubMenu"));
  } else if(inSubMenu){
    inSubMenu = false;
    editMode = true;
    Serial.println(F("State changed: editMode"));
  } else if(editMode){
    // TO-DO: Do something in edit mode
  } else {
    config.trip_partial = 0;
    displayValues[0] = config.trip_partial;
  }
  updateScreens();
}

void onButtonCenterHold(Button& btn){
  Serial.println(F("button CENTER hold"));

  if(inMenu){
    inMenu = false;
    inSubMenu = false;
    Serial.println(F("State changed: none"));
  } else if(inSubMenu){
    inSubMenu = false;
    inMenu = true;
    Serial.println(F("State changed: inMenu"));
  } else if(editMode){
    editMode = false;
    inSubMenu = true;
    Serial.println(F("State changed: inSubMenu"));
  } else {
    inMenu = true;
    Serial.println(F("State changed: inMenu"));
  }

  updateScreens();
}

void onButtonDownPressed(Button& btn){
  Serial.println(F("button DOWN pressed"));
  if(inMenu){
    HT1621_all_off(16, 0);
    HT1621_all_off(16, 1);
    
    if(selectedMenuOption < maxMenuIndex)
      selectedMenuOption++;
  } else if(inSubMenu){
  } else if(editMode){
  } else {
    config.trip_partial -= 1;

    if(config.trip_partial < 0){
      config.trip_partial = 0;
    }

    displayValues[0] = config.trip_partial;
  }
  updateScreens();
}

// void onSensorDetect(Button& btn){
//   Serial.println(F("SENSOR detected"));
//   //config.trip_partial += config.circumference / 100000.0; // Count 1 every 100 meters
//   config.trip_partial += 1;
//   displayValues[0] = config.trip_partial;

//   Serial.print("trip_partial: ");
//   Serial.println(config.trip_partial, 6);

//   updateScreens();
// }

void calculateHeading() {
  unsigned long currentMillis = millis();

  // Get a new compass event
  sensors_event_t event; 
  mag.getEvent(&event);

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(event.magnetic.y, event.magnetic.x);

  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  //float declinationAngle = 0.22;
  heading += config.declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2 * PI;

  // Check for wrap due to addition of declination.
  if(heading > 2 * PI)
    heading -= 2 * PI;

  // Update data and display
  if (currentMillis - previousMillis >= displayRefreshInterval) {
    previousMillis = currentMillis;

    // Convert radians to degrees for readability.
    float headingDegrees = heading * 180 / M_PI;
    displayValues[2] = headingDegrees;
 
    updateScreens();
  }
}

void updateScreens() {
  if(inMenu){
    config.version = 000;
    saveConfig();
    Serial.println(F("--> CONFIG CLEARED"));
    inMenu = false;

  //   display_word(selectedMenuOption, 0);
  //   display_word(minSubmenuIndex, 1);
  // } else if(inSubMenu){
  //   display_word(selectedSubMenuOption, 0);
  //   display_data(12345, 1, 1, 0, 1);
  // } else if(editMode){
  //   display_data(88888, 1, 1, 0, 0);
  //   // display_data(88888, 1, 1, 0, 1);
  //   HT1621_all_off(6, 1);
  //   display_mark(1, 1);
  //   delay(500);
  //   clear_mark(1, 1);
  //   delay(500);
  } else {
    display_data(displayValues[0], 1, 1, 0, 0);
    display_degrees(displayValues[2], 0, 0, 1, 1);
  }
}

