#ifndef PS5DualSense_H
#define PS5DualSense_H

// Anything in this file will be made available to any ino or source (cpp) files which import it
#include <usbhub.h>
#include <PS5BT.h>

// //LED colors natively supported by the entrollerEnums.h header file in the USB 2.0 library
#define BT_CONTROLLER_LED_GREEN             Green
#define BT_CONTROLLER_LED_RED               Red
#define BT_CONTROLLER_LED_BLUE              Blue
#define BT_CONTROLLER_LED_YELLOW            Yellow
#define BT_CONTROLLER_LED_PURPLE            Purple
#define BT_CONTROLLER_LED_LB                Lightblue
#define BT_CONTROLLER_LED_OFF               Off

extern ColorsEnum btLEDColor;
extern ColorsEnum btLEDDrive;
extern ColorsEnum btLEDMaint;
extern ColorsEnum btLEDMode1;
extern ColorsEnum btLEDMode2;
extern ColorsEnum btLEDMode3;
extern ColorsEnum btLEDMode4;
extern ColorsEnum btLEDOff;

extern USB Usb;
extern BTD Btd;
typedef PS5BT BTCNTL;
extern BTCNTL BTNav;

extern bool ledSupport;

// PS5 controller functions
String mapBTButtons();
void powerOff();

void setMicLED(bool &muteSounds);
void setLED(ColorsEnum &btLEDColor);

#endif // PS5DualSense_H