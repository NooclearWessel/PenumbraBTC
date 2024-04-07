#include "PS5DualSense.h"
//#include <usbhub.h>
//#include <PS5BT.h>
bool ledSupport = true;

//LED colors natively supported by the entrollerEnums.h header file in the USB 2.0 library
ColorsEnum btLEDColor = BT_CONTROLLER_LED_GREEN;
ColorsEnum btLEDDrive = BT_CONTROLLER_LED_GREEN;
ColorsEnum btLEDMaint = BT_CONTROLLER_LED_RED;
ColorsEnum btLEDMode1 = BT_CONTROLLER_LED_BLUE;
ColorsEnum btLEDMode2 = BT_CONTROLLER_LED_YELLOW;
ColorsEnum btLEDMode3 = BT_CONTROLLER_LED_PURPLE;
ColorsEnum btLEDMode4 = BT_CONTROLLER_LED_LB;
ColorsEnum btLEDOff   = BT_CONTROLLER_LED_OFF;

USB Usb;
BTD Btd(&Usb);
// typedef PS5BT BTCNTL;
// BTCNTL BTNav(&Btd, PAIR);
BTCNTL BTNav(&Btd);

void powerOff() {
  BTNav.disconnect();
} //powerOff

void setMicLED(bool &muteSounds) {
    muteSounds = !muteSounds;
    BTNav.setMicLed(muteSounds);
} //setMicLED

void setLED(ColorsEnum &btLEDColor) {
    BTNav.setLed(btLEDColor);
} //setLED

// Consider moving this to an overall controller header file. Most of this is applicable to the other controllers.
String mapBTButtons() {
    // Reserved for gas/brakes
    // } else if (BTNav.getButtonClick(L2)) {
    //     return "lt2";
    // } else if (BTNav.getButtonClick(R2)) {
    //     return "rt2";

    if (BTNav.getButtonClick(PS)) {
        return "power";
    } else if (BTNav.getButtonClick(MICROPHONE)) {
        return "mute";
    //Might change cancel to PS but with a shorter button press, then change UP to mode1 and CREATE to drive.
    } else if (BTNav.getButtonClick(CREATE)) { 
        return "cancel";
    } else if (BTNav.getButtonClick(OPTIONS)) {
        return "maint";
    } else if (BTNav.getButtonClick(UP)) {
        return "drive";
    } else if (BTNav.getButtonClick(DOWN)) {
        return "mode1";
    } else if (BTNav.getButtonClick(LEFT)) {
        return "mode2";
    } else if (BTNav.getButtonClick(RIGHT)) {
        return "mode3";
    } else if (BTNav.getButtonClick(L1)) {
        return "lt1";
    } else if (BTNav.getButtonClick(R1)) {
        return "rt1";
    } else if (BTNav.getButtonClick(TRIANGLE)) {
        return "action1";
    } else if (BTNav.getButtonClick(CIRCLE)) {
        return "action2";
    } else if (BTNav.getButtonClick(CROSS)) {
        return "action3";
    } else if (BTNav.getButtonClick(SQUARE)) {
        return "action4";
    } else if (BTNav.getButtonClick(L3)) {
        return "lt3";
    } else if (BTNav.getButtonClick(R3)) {
        return "rt3";
    } else {
        return "";
    } //if-else if-else

} //mapBTButtons



// Options (3 lines) (Maintenance Mode - Once to enter. Once to end.)
//  - Disable motors
//  Controls the following:
//    1. Volume
//    2. Lights
//    3. Resets
//    4. Servo tests
//    5. Light tests
//    6. Sound tests
//  Button Mapping:
//    LH Up    : ?  = Lights ?
//    LH Down  : ?  = Lights ?
//    LH Left: : 55 = Close All Body Panels
//    LH Right : 54 = Open All Body Panels
//    RH Up    : 31 = Open Top Dome Panels
//    RH Down  : 32 = Open Bottom Dome Panels
//    RH Left  : 33 = Close All Dome Panels
//    RH Right : 30 = Open All Dome Panels
//    Triangle = 11 = Quite mode reset (panel close, stop holos, stop sounds)
//    Circle   = 12 = Full Awake mode reset (panel close, rnd sound, holo move, holo lights off)
//    Cross    = 13 = Mid Awake mode reset (panel close, rnd sound, stop holos)
//    Square   = 14 = Full Awake+ reset (panel close, rnd sound, holo move, holo lights on)
//    L1       = 29 = Volume Mid
//    L2       = 27 = Volume Down
//    R1       = 28 = Volume Max
//    R2       = 26 = Volume Up
//    Create   = ?  = Open charge panel
//    Touchpad = 1 = Close All Panels

// Reserved Buttons
//    PS         = Power On/Off
//    OPTIONS    = Enter maintenance mode
//    CREATE     = Cancel current operation (reset)
//    MICROPHONE = Mute sounds
//    UP         = MD Red
//    RIGHT      = MD Green
//    DOWN       = MD Yellow
//    LEFT       = MD Purple
//    L2         = Drive
//    R2         = Brake

// Back paddles not used yet
// L3
// R3
// TOUCHPAD

//PS5Parser.h describes these

//disconnect()
//connected()
//getBatteryLevel() 

//setPlayerLed(player_led_mask)
//setRumbleOn(RumbleHigh)
//setRumbleOn(RumbleLow)
//setRumbleOff()
//setLed(Green)
//setMicLed(muteSounds);
//leftTrigger.setTriggerForce(oldR2Value, 255)

//getMessageCounter()
//getAngle(Pitch)
//getAngle(Roll)

//getButtonPress(PS)
//getButtonClick(CREATE)
//getButtonClick(OPTIONS)
//getButtonClick(TOUCHPAD)
//getButtonClick(MICROPHONE)

//getAnalogHat(LeftHatX))
//getAnalogHat(LeftHatY))
//getAnalogHat(RightHatX)
//getAnalogHat(RightHatY)
//getAnalogButton(L2)
//getAnalogButton(R2)

//getButtonClick(TRIANGLE)
//getButtonClick(CIRCLE)
//getButtonClick(CROSS)
//getButtonClick(SQUARE)

//getButtonClick(UP)
//getButtonClick(RIGHT)
//getButtonClick(DOWN
//getButtonClick(LEFT)

//getButtonClick(L1)
//getButtonClick(R1)

//getButtonClick(TOUCHPAD)

    // if (printTouch) { // Print the x, y coordinates of the touchpad
    //   if (PS5.isTouching(0) || PS5.isTouching(1)) // Print newline and carriage return if any of the fingers are touching the touchpad
    //     Serial.print(F("\r\n"));
    //   for (uint8_t i = 0; i < 2; i++) { // The touchpad track two fingers
    //     if (PS5.isTouching(i)) { // Print the position of the finger if it is touching the touchpad
    //       Serial.print(F("X")); Serial.print(i + 1); Serial.print(F(": "));
    //       Serial.print(PS5.getX(i));
    //       Serial.print(F("\tY")); Serial.print(i + 1); Serial.print(F(": "));
    //       Serial.print(PS5.getY(i));
    //       Serial.print(F("\t"));
    //     }
    //   }
    // }

