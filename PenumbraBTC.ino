// =======================================================================================
//                                       Penumbra BTC
// =======================================================================================
//    This sketch is a modified implementation of rimim's PenumbraShadowMD sketch found
//    in the ReelTwo repository. 
//        https://github.com/reeltwo/PenumbraShadowMD
//
//    It retains many of the original (with some refactorization) physics-based 
//    calculations and Marcduino functions, while implementing them through newer
//    Bluetooth-based controller functions with configurations for PS5 DualSense/Edge, 
//    and eventually XBox One/Nintendo Switch Pro.
//
// =======================================================================================
//                          Last Revised Date: 04/05/2024
//                             Revised By: NooclearWessel
//             Inspired by the PADAWAN / KnightShade SHADOW / Penumbra efforts
// =======================================================================================
//
//         This program is free software: you can redistribute it and/or modify it for
//         your personal use and the personal use of other astromech club members.  
//
//         This program is distributed in the hope that it will be useful 
//         as a courtesy to fellow astromech club members wanting to develop
//         their own droid control system.
//
//         IT IS OFFERED WITHOUT ANY WARRANTY; without even the implied warranty of
//         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//         You are using this software at your own risk and, as a fellow club member, it is
//         expected you will have the proper experience / background to handle and manage that 
//         risk appropriately.  It is completely up to you to insure the safe operation of
//         your droid and to validate and test all aspects of your droid control system.
//
// =======================================================================================
//   Note: You will need an ESP32 with a USB Host MAX3421 chip.
//
//   Required Libraries:
///
//   https://github.com/reeltwo/Reeltwo
//   https://github.com/rimim/espsoftwareserial
//   https://github.com/felis/USB_Host_Shield_2.0
//
// =======================================================================================
//
//   Sabertooth (Foot Drive):
//         Set Sabertooth 2x32 or 2x25 Dip Switches: 1 and 2 Down, All Others Up
//
//   SyRen 10 Dome Drive:
//         For SyRen packetized Serial Set Switches: 1, 2 and 4 Down, All Others Up
//
// =======================================================================================
//
//   Cytron SmartDriveDuo MDDS30 (Foot Drive):
//         Set Dip Switches 1,2,3 Up. 4,5,6 Down.
//
//   Cytron SmartDriveDuo MDDS10 (Dome Drive):
//         Set Dip Switches 1,2,3,6 Up. 4,5 Down.
//
//   Set Dip Switches 7 and 8 for your battery type. 0 Down 1 Up.
//      7:0 8:0 - LiPo
//      7:0 8:1 - NiMH
//      7:1 8:0 - SLA (Lead Acid)
//      7:1 8:1 - No battery monitoring
//
// =======================================================================================
//
// ---------------------------------------------------------------------------------------
//                        General User Settings Macros
// ---------------------------------------------------------------------------------------

#define PANEL_COUNT 10                // Number of panels
#define USE_DEBUG                     // Define to enable debug diagnostic
#define USE_PREFERENCES
#define USE_SABERTOOTH_PACKET_SERIAL
//#define USE_CYTRON_PACKET_SERIAL

//For Speed Setting (Normal): set this to whatever speeds works for you. 0-stop, 127-full speed.
#define DEFAULT_DRIVE_SPEED_NORMAL          70
//For Speed Setting (Over Throttle): set this for when needing extra power. 0-stop, 127-full speed.
#define DEFAULT_DRIVE_SPEED_OVER_THROTTLE   100

// the higher this number the faster it will spin in place, lower - the easier to control.
// Recommend beginner: 40 to 50, experienced: 50+, I like 75
#define DEFAULT_TURN_SPEED                  50
                         
// If using a speed controller for the dome, sets the top speed. Use a number up to 127
#define DEFAULT_DOME_SPEED                  100
                         
// Ramping- the lower this number the longer R2 will take to speedup or slow down,
// change this by increments of 1
#define DEFAULT_RAMPING                     1
                 
// For controllers that centering problems, use the lowest number with no drift
#define DEFAULT_JOYSTICK_FOOT_DEADBAND      20

// For controllers that centering problems, use the lowest number with no drift
#define DEFAULT_JOYSTICK_DOME_DEADBAND      20

// Used to set the Sabertooth DeadZone for foot motors
#define DEFAULT_DRIVE_DEADBAND              20

//This may need to be set to true for some configurations
#define DEFAULT_INVERT_TURN_DIRECTION       false

// Speed used when dome automation is active - Valid Values: 50 - 100
#define DEFAULT_AUTO_DOME_SPEED             70

// milliseconds for dome to complete 360 turn at domeAutoSpeed - Valid Values: 2000 - 8000 (2000 = 2 seconds)
#define DEFAULT_AUTO_DOME_TURN_TIME         2500

// Motor serial communication baud rate. Default 9600
#define DEFAULT_MOTOR_BAUD                  9600

// Marcduino serial communication baud rate. Default 9600 for AstroPixels with AstroPixelsPlus sketch
#define DEFAULT_MARCDUINO_BAUD              9600

//Set this to your BT controller MAC address if you want to hardcode it instead of using pairing mode
#define BT_CONTROLLER_MAC       "XX:XX:XX:XX:XX:XX"

// Also used for PS5 DualSense Edge with the back-paddles
#define USE_PS5_DS_CONTROLLER
//#define USE_XBOXONE_CONTROLLER
//#define USE_SWITCHPRO_CONTROLLER

//Starting controller configuration
#define DEFAULT_CONTROL_MODE                 "drive"

// ---------------------------------------------------------------------------------------
//                        General User Settings Variables
// ---------------------------------------------------------------------------------------

String BTControllerMac = BT_CONTROLLER_MAC;
String BTControllerBackupMac = "XX:XX:XX:XX:XX:XX";  //Set to the MAC Address of your BACKUP controller (Optional)

byte drivespeed1 = DEFAULT_DRIVE_SPEED_NORMAL;
byte drivespeed2 = DEFAULT_DRIVE_SPEED_OVER_THROTTLE;
byte turnspeed = DEFAULT_TURN_SPEED;
byte domespeed = DEFAULT_DOME_SPEED;
byte ramping = DEFAULT_RAMPING;

byte joystickFootDeadZoneRange = DEFAULT_JOYSTICK_FOOT_DEADBAND;
byte joystickDomeDeadZoneRange = DEFAULT_JOYSTICK_DOME_DEADBAND;

byte driveDeadBandRange = DEFAULT_DRIVE_DEADBAND;

bool invertTurnDirection = DEFAULT_INVERT_TURN_DIRECTION;

byte domeAutoSpeed = DEFAULT_AUTO_DOME_SPEED;
int time360DomeTurn = DEFAULT_AUTO_DOME_TURN_TIME;

#define SHADOW_DEBUG(...)       //uncomment this for console DEBUG output
// #define SHADOW_VERBOSE(...)   //uncomment this for console VERBOSE output

#define SHADOW_DEBUG(...) printf(__VA_ARGS__);
// #define SHADOW_VERBOSE(...) printf(__VA_ARGS__);

#ifdef USE_PREFERENCES
#include <Preferences.h>
#define PREFERENCE_CONTROLLER_MAC           "btmac"
#define PREFERENCE_SPEED_NORMAL             "smspeednorm"
#define PREFERENCE_SPEED_OVER_THROTTLE      "smspeedmax"
#define PREFERENCE_TURN_SPEED               "smspeedturn"
#define PREFERENCE_DOME_SPEED               "smspeeddome"
#define PREFERENCE_RAMPING                  "smramping"
#define PREFERENCE_FOOTSTICK_DEADBAND       "smfootdband"
#define PREFERENCE_DOMESTICK_DEADBAND       "smdomedband"
#define PREFERENCE_DRIVE_DEADBAND           "smdrivedband"
#define PREFERENCE_INVERT_TURN_DIRECTION    "sminvertturn"
#define PREFERENCE_DOME_AUTO_SPEED          "smdomeautospeed"
#define PREFERENCE_DOME_DOME_TURN_TIME      "smdometurntime"
#define PREFERENCE_MOTOR_BAUD               "smmotorbaud"
#define PREFERENCE_MARCDUINO_BAUD           "smmarcbaud"
Preferences preferences;
#endif

#include "ReelTwoHandler.h"
// Marcduino Action Syntax:
// #<1-76> Standard Marcduino Functions
// // MP3=<182->,LD=<1-8>,LDText="Hello World",Panel=M<1-8>,Panel<1-10>[delay=1,open=5]
bool handleMarcduinoAction(const char* action);
void sendMarcCommand(const char* cmd);
void sendBodyMarcCommand(const char* cmd);

class MarcduinoButtonAction
{
public:
    MarcduinoButtonAction(const char* name, const char* default_action) :
        fNext(NULL),
        fName(name),
        fDefaultAction(default_action)
    {
        if (*head() == NULL)
            *head() = this;
        if (*tail() != NULL)
            (*tail())->fNext = this;
        *tail() = this;
    }

    static MarcduinoButtonAction* findAction(String name)
    {
        for (MarcduinoButtonAction* btn = *head(); btn != NULL; btn = btn->fNext)
        {
            if (name.equalsIgnoreCase(btn->name()))
                return btn;
        }
        return nullptr;
    }

    static void listActions()
    {
        for (MarcduinoButtonAction* btn = *head(); btn != NULL; btn = btn->fNext)
        {
            printf("%s: %s\n", btn->name().c_str(), btn->action().c_str());
        }
    }

    void reset()
    {
        preferences.remove(fName);
    }

    void setAction(String newAction)
    {
        if (strlen(fName) > 15)
        {
            String key = fName;
            key = key.substring(0, 15);
            preferences.putString(key.c_str(), newAction);
        }
        else
        {
            preferences.putString(fName, newAction);
        }
    }

    void trigger()
    {
        //("TRIGGER: %s\n", fName);
        handleMarcduinoAction(action().c_str());
    }

    String name()
    {
        return fName;
    }

    String action()
    {
        if (strlen(fName) > 15)
        {
            String key = fName;
            key = key.substring(0, 15);
            return preferences.getString(key.c_str(), fDefaultAction);
        }
        return preferences.getString(fName, fDefaultAction);
    }

private:
    MarcduinoButtonAction* fNext;
    const char* fName;
    const char* fDefaultAction;

    static MarcduinoButtonAction** head()
    {
        static MarcduinoButtonAction* sHead;
        return &sHead;
    }

    static MarcduinoButtonAction** tail()
    {
        static MarcduinoButtonAction* sTail;
        return &sTail;
    }
};

#define MARCDUINO_ACTION(var,act) \
MarcduinoButtonAction var(#var,act);

// Maps button clicks to MarcduinoSequences or commands in AstroPixels Plus
#include "MarcduinoCommandMap.h"

// ---------------------------------------------------------------------------------------
//               SYSTEM VARIABLES - USER CONFIG SECTION COMPLETED
// ---------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------
//                          Drive Controller Settings
// ---------------------------------------------------------------------------------------

int motorControllerBaudRate = DEFAULT_MOTOR_BAUD;
int marcDuinoBaudRate = DEFAULT_MARCDUINO_BAUD;

#define FOOT_MOTOR_ADDR      128      // Serial Address for Foot Motor
#define DOME_MOTOR_ADDR      129      // Serial Address for Dome Motor

#define ENABLE_UHS_DEBUGGING 1

// ---------------------------------------------------------------------------------------
//                          Libraries
// ---------------------------------------------------------------------------------------
#include <ReelTwo.h>
#include <core/SetupEvent.h>
#include <core/StringUtils.h>

// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#ifdef USE_SABERTOOTH_PACKET_SERIAL
#include <motor/SabertoothDriver.h>
#endif

#ifdef USE_CYTRON_PACKET_SERIAL
#include <motor/CytronSmartDriveDuoDriver.h>
#endif

#include "pin-map.h"

#define CONSOLE_BUFFER_SIZE     300
static unsigned sPos;
static char sBuffer[CONSOLE_BUFFER_SIZE];

// ---------------------------------------------------------------------------------------
//                    Panel Management Variables
// ---------------------------------------------------------------------------------------
static bool sRunningCustRoutine = false;

struct PanelStatus
{
    uint8_t fStatus = 0;
    uint32_t fStartTime = 0;
    uint8_t fStartDelay = 1;
    uint8_t fDuration = 5;
};

PanelStatus sPanelStatus[PANEL_COUNT];

// ---------------------------------------------------------------------------------------
//                          Variables
// ---------------------------------------------------------------------------------------

long previousDomeMillis = millis();
long previousFootMillis = millis();
long previousMarcDuinoMillis = millis();
long previousDomeToggleMillis = millis();
long previousSpeedToggleMillis = millis();
long currentMillis = millis();

int serialLatency = 25;   //This is a delay factor in ms to prevent queueing of the Serial data.
                          //25ms seems approprate for HardwareSerial, values of 50ms or larger are needed for Softare Emulation
                          
int marcDuinoButtonCounter = 0;
int speedToggleButtonCounter = 0;
int domeToggleButtonCounter = 0;

#ifdef USE_SABERTOOTH_PACKET_SERIAL
SabertoothDriver FootMotorImpl(FOOT_MOTOR_ADDR, MOTOR_SERIAL);
SabertoothDriver DomeMotorImpl(DOME_MOTOR_ADDR, MOTOR_SERIAL);

SabertoothDriver* FootMotor=&FootMotorImpl;
SabertoothDriver* DomeMotor=&DomeMotorImpl;
#endif

#ifdef USE_CYTRON_PACKET_SERIAL
CytronSmartDriveDuoMDDS30Driver FootMotorImpl(FOOT_MOTOR_ADDR, MOTOR_SERIAL);
CytronSmartDriveDuoMDDS10Driver DomeMotorImpl(DOME_MOTOR_ADDR, MOTOR_SERIAL);

CytronSmartDriveDuoDriver* FootMotor=&FootMotorImpl;
CytronSmartDriveDuoDriver* DomeMotor=&DomeMotorImpl;
#endif

///////Setup for USB and Bluetooth Devices////////////////////////////

// ----------------------------------------------------------------------------------
//                              PS5 DualSense (and Edge)
// ----------------------------------------------------------------------------------
#ifdef USE_PS5_DS_CONTROLLER
#include "PS5DualSense.h"
#endif

// ----------------------------------------------------------------------------------
//                                      XBox One - Not ready
// ----------------------------------------------------------------------------------
#ifdef USE_XBOXONE_CONTROLLER
// #include <XBOXONESBT.h>
// #include "XBoxOne.h"
#endif

// ----------------------------------------------------------------------------------
//                                     Switch Pro - Not ready
// ----------------------------------------------------------------------------------

#ifdef USE_SWITCHPRO_CONTROLLER
// #include <SwitchProBT.h>
// #include "SwitchPro.h"
#endif

// ----------------------------------------------------------------------------------
//                                  Controller Setup
// ----------------------------------------------------------------------------------

//BT Controller Setup
String actionMode = DEFAULT_CONTROL_MODE;
String previousMode = "";
String navCommand = "";
String actionCommand = "";
uint8_t player_led_mask = 0;  //not used
bool muteSounds = false;
uint32_t power_timer;

//Used for BT Controller Fault Detection
uint32_t msgLagTime = 0;
uint32_t lastMsgTime = 0;
uint32_t currentTime = 0;
uint32_t lastLoopTime = 0;
uint16_t lastMessageCounter = -1;  //Not yet implemented

int badBTData = 0;
int badBTDataDome = 0;

bool isFootMotorStopped = true;
bool isDomeMotorStopped = true;

bool overSpeedSelected = false;

bool isBTNavigatonInitialized = false;
bool isSecondaryBTNavigatonInitialized = false;

bool isStickEnabled = true;

bool WaitingforReconnect = false;
bool controllerConnected = false;

// Dome Automation Variables
bool domeAutomation = false;
int domeTurnDirection = 1;  // 1 = positive turn, -1 negative turn
float domeTargetPosition = 0; // (0 - 359) - degrees in a circle, 0 = home
unsigned long domeStopTurnTime = 0;    // millis() when next turn should stop
unsigned long domeStartTurnTime = 0;  // millis() when next turn should start
int domeStatus = 0;  // 0 = stopped, 1 = prepare to turn, 2 = turning

byte action = 0;
unsigned long DriveMillis = 0;

int footDriveSpeed = 0;

// =======================================================================================

// static const char* DEFAULT_MARCDUINO_COMMANDS[] = {
// #include "MarcduinoCommands.h"
// };

bool handleMarcduinoAction(const char* action)
{
    String LD_text = "";
    bool panelTypeSelected = false;
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s", action);
    char* cmd = buffer;
    // if (*cmd == '#')
    // {
    //     // Std Marcduino Function Call Configured
    //     uint32_t seq = strtolu(cmd+1, &cmd);
    //     if (*cmd == '\0')
    //     {
    //         if (seq >= 1 && seq <= SizeOfArray(DEFAULT_MARCDUINO_COMMANDS))
    //         {
    //             // If the commands starts with "BM" we direct it to the body marc controller
    //             const char* marcCommand = DEFAULT_MARCDUINO_COMMANDS[seq-1];
    //             if (marcCommand[0] == 'B' && marcCommand[1] == 'M')
    //             {
    //                 sendBodyMarcCommand(&marcCommand[2]);
    //             }
    //             else
    //             {
    //                 // Otherwise we send it to the dome Marcduino
    //                 sendMarcCommand(marcCommand);
    //             }
    //             return true;
    //         }
    //         else
    //         {
    //             SHADOW_DEBUG("Marcduino sequence range is 1-%d in action command \"%s\"\n", SizeOfArray(DEFAULT_MARCDUINO_COMMANDS), action)
    //             return false;
    //         }
    //     }
    //     SHADOW_DEBUG("Excepting number after # in action command \"%s\"\n", action)
    //     return false;
    // }
    for (;;) {
        char buf[100];
        if (*cmd == '"') {
            // Skip the quote
            cmd++;
            char* marcCommand = cmd;
            char* nextCmd = strchr(cmd, ',');
            if (nextCmd != nullptr) {
                size_t len = nextCmd - marcCommand;
                strncpy(buf, marcCommand, len);
                buf[len] = '\0';
                cmd = nextCmd;
                marcCommand = buf;
            } else {
                cmd += strlen(marcCommand);
            }
            // If the commands starts with "BM" we direct it to the body marc controller
            if (marcCommand[0] == 'B' && marcCommand[1] == 'M') {
                sendBodyMarcCommand(&marcCommand[2]);
            } else {
                sendMarcCommand(marcCommand);
            }
        } else if (*cmd == '$') {
            char* mp3Cmd = cmd;
            char* nextCmd = strchr(cmd, ',');
            if (nextCmd != nullptr) {
                size_t len = nextCmd - mp3Cmd;
                strncpy(buf, mp3Cmd, len);
                buf[len] = '\0';
                cmd = nextCmd;
                mp3Cmd = buf;
            } else {
                cmd += strlen(mp3Cmd);
            }
            sendMarcCommand(mp3Cmd);
        } else if (startswith(cmd, "MP3=")) {
            char* mp3Cmd = cmd;
            char* nextCmd = strchr(cmd, ',');
            if (nextCmd != nullptr) {
                size_t len = nextCmd - mp3Cmd;
                buf[0] = '$';
                strncpy(&buf[1], mp3Cmd, len);
                buf[len+1] = '\0';
                cmd = nextCmd;
                mp3Cmd = buf;
            } else {
                cmd += strlen(mp3Cmd);
            }
            sendMarcCommand(mp3Cmd);
        } else if (startswith(cmd, "Panel=M")) {
            static const char* sCommands[] = {
                ":CL00",
                ":SE51",
                ":SE52",
                ":SE53",
                ":SE54",
                ":SE55",
                ":SE56",
                ":SE57"
            };
            uint32_t num = strtolu(cmd, &cmd);
            if (num >= 1 && num <= SizeOfArray(sCommands)) {
                if (num > 1) {
                    sendMarcCommand(":CL00");  // close all the panels prior to next custom routine
                    delay(50); // give panel close command time to process before starting next panel command 
                }
                sendMarcCommand(sCommands[num-1]);
                panelTypeSelected = true;
            } else {
                SHADOW_DEBUG("Marc Panel range is 1 - %d in action command \"%s\"\n", SizeOfArray(sCommands), action)
                return false;
            }
        } else if (startswith(cmd, "Panel")) {
            uint32_t num = strtolu(cmd, &cmd);
            if (num >= 1 && num <= SizeOfArray(sPanelStatus)) {
                PanelStatus &panel = sPanelStatus[num-1];
                panel.fStatus = 1;
                if (*cmd == '[') {
                    cmd++;
                    do {
                        if (startswith(cmd, "delay=")) {
                            uint32_t delayNum = strtolu(cmd, &cmd);
                            if (delayNum < 31) {
                                panel.fStartDelay = delayNum;
                            } else {
                                panel.fStatus = 0;
                            }
                        } else if (startswith(cmd, "dur=")) {
                            uint32_t duration = strtolu(cmd, &cmd);
                            if (duration < 31) {
                                panel.fDuration = duration;
                            } else {
                                panel.fStatus = 0;
                            }
                        } else if (*cmd == ',') {
                            cmd++;
                        }
                    }  while (*cmd != '\0' && *cmd != ']');
                    if (*cmd == ']')
                        cmd++;
                }

                if (panel.fStatus) {
                    panelTypeSelected = true;
                    panel.fStartTime = millis();
                }
            } else {
                SHADOW_DEBUG("Panel range is 1 - %d in action command \"%s\"\n", SizeOfArray(sPanelStatus), action)
                return false;
            }
        } else if (startswith(cmd, "LDText=\"")) {
            char* startString = ++cmd;
            while (*cmd != '\0' && *cmd != '"')
                cmd++;
            if (*cmd == '"')
                *cmd = '\0';
            LD_text = startString;
        } else if (startswith(cmd, "LD=")) {
            uint32_t num = strtolu(cmd, &cmd);
            if (num >= 1 && num < 8) {
                // If a custom panel movement was selected - need to briefly pause before changing light sequence to avoid conflict)
                if (panelTypeSelected) {
                    delay(30);
                }

                switch (num) {
                    case 1:
                        sendMarcCommand("@0T1");
                        break;              
                    case 2:
                        sendMarcCommand("@0T4");
                        break;              
                    case 3:
                        sendMarcCommand("@0T5");
                        break;
                    case 4:
                        sendMarcCommand("@0T6");
                        break;
                    case 5:
                        sendMarcCommand("@0T10");
                        break;
                    case 6:
                        sendMarcCommand("@0T11");
                        break;
                    case 7:
                        sendMarcCommand("@0T92");
                        break;
                    case 8:
                        sendMarcCommand("@0T100");
                        delay(50);
                        String custString = "@0M";
                        custString += LD_text;
                        sendMarcCommand(custString.c_str());
                        break;
                }
            } else {
                SHADOW_DEBUG("LD range is 1 - 8 in action command \"%s\"\n", action)
                return false;
            }
        }
        if (*cmd != ',')
            break;
        cmd++;
    }

    if (*cmd != '\0') {
        SHADOW_DEBUG("Ignoring unknown trailing \"%s\" in action \"%s\"\n", cmd, action);
    }

    if (panelTypeSelected) {
        printf("panelTypeSelected\n");
        sRunningCustRoutine = true;
    }
    return true;
} //handleMarcduinoAction


// =======================================================================================
//                          Main Program
// =======================================================================================

// #if defined(USE_HCR_VOCALIZER) || defined(USE_MP3_TRIGGER) || defined(USE_DFMINI_PLAYER)
// #define SOUND_DEBUG(...) printf(__VA_ARGS__);
// #define MARC_SOUND_VOLUME               500     // 0 - 1000
// #define MARC_SOUND_RANDOM               true    // Play random sounds
// #define MARC_SOUND_RANDOM_MIN           1000    // Min wait until random sound
// #define MARC_SOUND_RANDOM_MAX           10000   // Max wait until random sound
// #define MARC_SOUND_STARTUP              255     // Startup sound
// #define MARC_SOUND_PLAYER               MarcSound::kHCR
// #include "MarcduinoSound.h"
// #define MARC_SOUND
// #endif

// =======================================================================================
//                          Initialize - Setup Function
// =======================================================================================
void setup() {
    REELTWO_READY();

#ifdef USE_PREFERENCES
    if (!preferences.begin("penumbrashadow", false)) {
        DEBUG_PRINTLN("Failed to init prefs");
    } else {
        // Serial.print(F("\r\n INIT PREFS begin"));

        BTControllerMac = preferences.getString(PREFERENCE_CONTROLLER_MAC, BT_CONTROLLER_MAC);
        drivespeed1 = preferences.getInt(PREFERENCE_SPEED_NORMAL, DEFAULT_DRIVE_SPEED_NORMAL);
        drivespeed2 = preferences.getInt(PREFERENCE_SPEED_OVER_THROTTLE, DEFAULT_DRIVE_SPEED_OVER_THROTTLE);
        turnspeed = preferences.getInt(PREFERENCE_TURN_SPEED, DEFAULT_TURN_SPEED);
        domespeed = preferences.getInt(PREFERENCE_DOME_SPEED, DEFAULT_DOME_SPEED);
        ramping = preferences.getInt(PREFERENCE_RAMPING, DEFAULT_RAMPING);
        joystickFootDeadZoneRange = preferences.getInt(PREFERENCE_FOOTSTICK_DEADBAND, DEFAULT_JOYSTICK_FOOT_DEADBAND);
        joystickDomeDeadZoneRange = preferences.getInt(PREFERENCE_DOMESTICK_DEADBAND, DEFAULT_JOYSTICK_DOME_DEADBAND);
        driveDeadBandRange = preferences.getInt(PREFERENCE_DRIVE_DEADBAND, DEFAULT_DRIVE_DEADBAND);
        invertTurnDirection = preferences.getBool(PREFERENCE_INVERT_TURN_DIRECTION, DEFAULT_INVERT_TURN_DIRECTION);
        domeAutoSpeed = preferences.getInt(PREFERENCE_DOME_AUTO_SPEED, DEFAULT_AUTO_DOME_SPEED);
        time360DomeTurn = preferences.getInt(PREFERENCE_DOME_DOME_TURN_TIME, DEFAULT_AUTO_DOME_TURN_TIME);
        motorControllerBaudRate = preferences.getInt(PREFERENCE_MOTOR_BAUD, DEFAULT_MOTOR_BAUD);
        marcDuinoBaudRate = preferences.getInt(PREFERENCE_MARCDUINO_BAUD, DEFAULT_MARCDUINO_BAUD);
    }
#endif
    PrintReelTwoInfo(Serial, "Penumbra Shadow MD");

    //Setup for BT Controller
    BTNav.attachOnInit(onInitBTNav); // onInitBTNav is called upon a new connection
    controllerConnected = true;
    
    //Setup for SABERTOOTH_SERIAL Motor Controllers - Sabertooth (Feet & Dome)
    MOTOR_SERIAL_INIT(motorControllerBaudRate);
    // Don't use autobaud(). It is flaky and causes delays. Default baud rate is 9600
    // If your syren is set to something else call setBaudRate(9600) below or change it
    // using Describe.
    // FootMotor->setBaudRate(9600);   // Send the autobaud command to the Sabertooth controller(s).
    FootMotor->setTimeout(10);      //DMB:  How low can we go for safety reasons?  multiples of 100ms
    FootMotor->setDeadband(driveDeadBandRange);
    FootMotor->stop();
    DomeMotor->setTimeout(20);      //DMB:  How low can we go for safety reasons?  multiples of 100ms
    // DomeMotor->stop();

    // //Setup for MD_SERIAL MarcDuino Dome Controller Board - Tested using AstroPixels with the APP Sketch
    MD_SERIAL_INIT(marcDuinoBaudRate);

    //Setup for BODY_MD_SERIAL Optional MarcDuino Control Board for Body Panels
    BODY_MD_SERIAL_INIT(marcDuinoBaudRate);

    // randomSeed(analogRead(0));  // random number seed for dome automation   

    SetupEvent::ready();
} //setup

void sendMarcCommand(const char* cmd)
{
    // SHADOW_VERBOSE("Sending MARC: \"%s\"\n", cmd)
    MD_SERIAL.print(cmd); MD_SERIAL.print("\r");
#if defined(MARC_SOUND_PLAYER)
    sMarcSound.handleCommand(cmd);
#endif
}

void sendBodyMarcCommand(const char* cmd)
{
    // SHADOW_VERBOSE("Sending BODYMARC: \"%s\"\n", cmd)
    BODY_MD_SERIAL.print(cmd); BODY_MD_SERIAL.print("\r");
}

////////////////////////////////
// This function is called when settings have been changed and needs a reboot
void reboot() {
    DEBUG_PRINTLN("Restarting...");
    preferences.end();
    delay(1000);
    ESP.restart();
} //reboot

// =======================================================================================
//           Main Program Loop - This is the recurring check loop for entire sketch
// =======================================================================================

void loop() {
    //LOOP through functions from highest to lowest priority.
    if (!readUSB())
        return;
    
// Shut down the controller if the PS button is clicked for 400 milliseconds
    // if (BTNav.getButtonPress(PS)) {
    //     if (millis() - power_timer > 1000) {
    //         if (!isFootMotorStopped) {
    //             FootMotor->stop();
    //             isFootMotorStopped = true;
    //             footDriveSpeed = 0;
    //         }
    //         BTNav.disconnect();
    //     }
    // } else
    //   power_timer = millis();
    footMotorDrive();
    domeDrive();
    modeControl();
    toggleSettings();
    //custMarcDuinoPanel();

    // If dome automation is enabled - Call function
    // if (domeAutomation && time360DomeTurn > 1999 && time360DomeTurn < 8001 && domeAutoSpeed > 49 && domeAutoSpeed < 101)  
    // {
    //    autoDome(); 
    // }

    if (Serial.available()) {
        int ch = Serial.read();
        MD_SERIAL.print((char)ch);


        if (ch == 0x0A || ch == 0x0D) {
            char* cmd = sBuffer;
            if (startswith(cmd, "#SMZERO")) {
                preferences.clear();
                DEBUG_PRINT("Clearing preferences. ");
                reboot();
            } else if (startswith(cmd, "#SMRESTART")) {
                reboot();
            } else if (startswith(cmd, "#SMLIST")) {
                printf("Button Actions\n");
                printf("-----------------------------------\n");
                MarcduinoButtonAction::listActions();
            } else if (startswith(cmd, "#SMDEL")) {
                String key(cmd);
                key.trim();
                MarcduinoButtonAction* btn = MarcduinoButtonAction::findAction(key);
                if (btn != nullptr) {
                    btn->reset();
                    printf("Trigger: %s reset to default %s\n", btn->name().c_str(), btn->action().c_str());
                } else {
                    printf("Trigger Not Found: %s\n", key.c_str());
                }
            } else if (startswith(cmd, "#SMCONFIG")) {
                printf("Drive Speed Normal:  %3d (#SMNORMALSPEED) [0..127]\n", drivespeed1);
                printf("Drive Speed Normal:  %3d (#SMNORMALSPEED) [0..127]\n", drivespeed1);
                printf("Drive Speed Max:     %3d (#SMMAXSPEED)    [0..127]\n", drivespeed2);
                printf("Turn Speed:          %3d (#SMTURNSPEED)   [0..127]\n", turnspeed);
                printf("Dome Speed:          %3d (#SMDOMESPEED)   [0..127]\n", domespeed);
                printf("Ramping:             %3d (#SMRAMPING)     [0..10]\n", ramping);
                printf("Foot Stick Deadband: %3d (#SMFOOTDB)      [0..127]\n", joystickFootDeadZoneRange);
                printf("Dome Stick Deadband: %3d (#SMDOMEDB)      [0..127]\n", joystickDomeDeadZoneRange);
                printf("Drive Deadband:      %3d (#SMDRIVEDB)     [0..127]\n", driveDeadBandRange);
                printf("Invert Turn:         %3d (#SMINVERT)      [0..1]\n", invertTurnDirection);
                printf("Dome Auto Speed:     %3d (#SMAUTOSPEED)   [50..100]\n", domeAutoSpeed);
                printf("Dome Auto Time:     %4d (#SMAUTOTIME)    [2000..8000]\n", time360DomeTurn);
                printf("Marcduino Baud:   %6d (#SMMARCBAUD)\n", marcDuinoBaudRate);
                printf("Motor Baud:       %6d (#SMMOTORBAUD)\n", motorControllerBaudRate);
            } else if (startswith(cmd, "#SMNORMALSPEED")) {
                uint32_t val = strtolu(cmd, &cmd);
                if (val == drivespeed1) {
                    printf("Unchanged.\n");
                } else if (val <= 127) {
                    drivespeed1 = val;
                    preferences.putInt(PREFERENCE_SPEED_NORMAL, drivespeed1);
                    printf("Normal Speed Changed.\n");
                } else {
                    printf("Must be in range 0-127\n");
                }
            } else if (startswith(cmd, "#SMMAXSPEED")) {
                uint32_t val = strtolu(cmd, &cmd);
                if (val == drivespeed2) {
                    printf("Unchanged.\n");
                } else if (val <= 127) {
                    drivespeed2 = val;
                    preferences.putInt(PREFERENCE_SPEED_OVER_THROTTLE, drivespeed2);
                    printf("Max Speed Changed.\n");
                } else {
                    printf("Must be in range 0-127\n");
                }
            } else if (startswith(cmd, "#SMTURNSPEED")) {
                uint32_t val = strtolu(cmd, &cmd);
                if (val == turnspeed) {
                    printf("Unchanged.\n");
                } else if (val <= 127) {
                    turnspeed = val;
                    preferences.putInt(PREFERENCE_TURN_SPEED, turnspeed);
                    printf("Turn Speed Changed.\n");
                } else {
                    printf("Must be in range 0-127\n");
                }
            } else if (startswith(cmd, "#SMDOMESPEED")) {
                uint32_t val = strtolu(cmd, &cmd);
                if (val == domespeed) {
                    printf("Unchanged.\n");
                } else if (val <= 127) {
                    domespeed = val;
                    preferences.putInt(PREFERENCE_DOME_SPEED, val);
                    printf("Dome Speed Changed.\n");
                } else {
                    printf("Must be in range 0-127\n");
                }
            } else if (startswith(cmd, "#SMRAMPING")) {
                uint32_t val = strtolu(cmd, &cmd);
                if (val == ramping) {
                    printf("Unchanged.\n");
                } else if (val <= 10) {
                    ramping = val;
                    preferences.putInt(PREFERENCE_RAMPING, ramping);
                    printf("Ramping Changed.\n");
                } else {
                    printf("Must be in range 0-10\n");
                }
            } else if (startswith(cmd, "#SMFOOTDB")) {
                uint32_t val = strtolu(cmd, &cmd);
                if (val == joystickFootDeadZoneRange) {
                    printf("Unchanged.\n");
                } else if (val <= 127) {
                    joystickFootDeadZoneRange = val;
                    preferences.putInt(PREFERENCE_FOOTSTICK_DEADBAND, joystickFootDeadZoneRange);
                    printf("Foot Joystick Deadband Changed.\n");
                } else {
                    printf("Must be in range 0-127\n");
                }
            } else if (startswith(cmd, "#SMDOMEDB")) {
                uint32_t val = strtolu(cmd, &cmd);
                if (val == joystickDomeDeadZoneRange) {
                    printf("Unchanged.\n");
                } else if (val <= 127) {
                    joystickDomeDeadZoneRange = val;
                    preferences.putInt(PREFERENCE_DOMESTICK_DEADBAND, joystickDomeDeadZoneRange);
                    printf("Dome Joystick Deadband Changed.\n");
                } else {
                    printf("Must be in range 0-127\n");
                }
            } else if (startswith(cmd, "#SMDRIVEDB")) {
                uint32_t val = strtolu(cmd, &cmd);
                if (val == driveDeadBandRange) {
                    printf("Unchanged.\n");
                } else if (val <= 127) {
                    driveDeadBandRange = val;
                    preferences.putInt(PREFERENCE_DRIVE_DEADBAND, driveDeadBandRange);
                    printf("Drive Controller Deadband Changed.\n");
                } else {
                    printf("Must be in range 0-127\n");
                }
            } else if (startswith(cmd, "#SMINVERT")) {
                bool invert = (strtolu(cmd, &cmd) == 1);
                if (invert == invertTurnDirection) {
                    printf("Unchanged.\n");
                } else {
                    invertTurnDirection = invert;
                    preferences.putInt(PREFERENCE_INVERT_TURN_DIRECTION, invertTurnDirection);
                    if (invert) {
                        printf("Invert Turn Direction Enabled.\n");
                    } else {
                        printf("Invert Turn Direction Disabled.\n");
                    }
                }
            } else if (startswith(cmd, "#SMAUTOSPEED")) {
                uint32_t val = strtolu(cmd, &cmd);
                if (val == domeAutoSpeed) {
                    printf("Unchanged.\n");
                } else if (val <= 100) {
                    domeAutoSpeed = val;
                    preferences.putInt(PREFERENCE_DOME_AUTO_SPEED, domeAutoSpeed);
                    printf("Auto Dome Speed Changed.\n");
                } else {
                    printf("Must be in range 50-100\n");
                }
            } else if (startswith(cmd, "#SMAUTOTIME")) {
                uint32_t val = strtolu(cmd, &cmd);
                if (val == time360DomeTurn) {
                    printf("Unchanged.\n");
                } else if (val <= 8000) {
                    time360DomeTurn = val;
                    preferences.putInt(PREFERENCE_DOME_DOME_TURN_TIME, time360DomeTurn);
                    printf("Auto Dome Turn Time Changed.\n");
                } else {
                    printf("Must be in range 0-127\n");
                }
            } else if (startswith(cmd, "#SMMOTORBAUD")) {
                uint32_t val = strtolu(cmd, &cmd);
                if (val == motorControllerBaudRate) {
                    printf("Unchanged.\n");
                } else  {
                    motorControllerBaudRate = val;
                    preferences.putInt(PREFERENCE_MOTOR_BAUD, motorControllerBaudRate);
                    printf("Motor Controller Serial Baud Rate Changed. Needs Reboot.\n");
                }
            } else if (startswith(cmd, "#SMMARCBAUD")) {
                uint32_t val = strtolu(cmd, &cmd);
                if (val == marcDuinoBaudRate) {
                    printf("Unchanged.\n");
                } else {
                    marcDuinoBaudRate = val;
                    preferences.putInt(PREFERENCE_MARCDUINO_BAUD, marcDuinoBaudRate);
                    printf("Marcduino Serial Baud Rate Changed. Needs Reboot.\n");
                }
            // }
            // else if (startswith(cmd, "#SMPLAY"))
            // {
            //     String key(cmd);
            //     key.trim();
            //     MarcduinoButtonAction* btn = MarcduinoButtonAction::findAction(key);
            //     if (btn != nullptr)
            //     {
            //         btn->trigger();
            //     }
            //     else
            //     {
            //         printf("Trigger Not Found: %s\n", key.c_str());
            //     }
            // } else if (startswith(cmd, "#SMSET")) {
            //     // Skip whitespace
            //     while (*cmd == ' ')
            //         cmd++;
            //     char* keyp = cmd;
            //     char* valp = strchr(cmd, ' ');
            //     if (valp != nullptr)
            //     {
            //         *valp++ = '\0';
            //         String key(keyp);
            //         key.trim();
            //         MarcduinoButtonAction* btn = MarcduinoButtonAction::findAction(key);
            //         if (btn != nullptr)
            //         {
            //             String action(valp);
            //             action.trim();
            //             btn->setAction(action);
            //             printf("Trigger: %s set to %s\n", key.c_str(), action.c_str());
            //         }
            //         else
            //         {
            //             printf("Trigger Not Found: %s\n", key.c_str());
            //         }
            //     }
            } else {
                printf("Unknown: %s\n", sBuffer);
            }
            sPos = 0;
        } else if (sPos < SizeOfArray(sBuffer)-1) {
            sBuffer[sPos++] = ch;
            sBuffer[sPos] = '\0';
        }
    }

    // Clear inbound buffer of any data sent from the MarcDuino board
    if (MD_SERIAL.available()) {
        int ch = MD_SERIAL.read();
        Serial.print((char)ch);
    }
    if (BODY_MD_SERIAL.available()) {
        int ch = BODY_MD_SERIAL.read();
        Serial.print((char)ch);
    }
} //loop


void driveControl() {

    if (actionCommand == navCommand) {
        cancelActions();
        return;
    } else if (navCommand == "lt1") {
        Serial.print("sending drive lt1");
        drive_lt1_MD.trigger();
        return;
    } else if (navCommand == "rt1") {
        Serial.print("sending drive rt1");
        drive_rt1_MD.trigger();
        return;
    } else if (navCommand == "action1") {
        drive_action1_MD.trigger();
        return;
    } else if (navCommand == "action2") {
        drive_action2_MD.trigger();
        return;
    } else if (navCommand == "action3") {
        drive_action3_MD.trigger();
        return;
    } else if (navCommand == "action4") {
        drive_action4_MD.trigger();
        return;
    // } else if (navCommand == "lt3") {
    //     // TODO: Some MD command
    //     return;
    // } else if (navCommand == "rt3") {
    //     // TODO: Some MD command
    //     return;
    } else {
      // Something snuck past the goalie
      // Provide feedback...rumble for 2 seconds
      return;
    } // if-elseif-else

} //driveControl


void cancelActions() {
    stop_sequence.trigger();
    stop_sounds.trigger();
} //cancelActions

void maintControl() {
// TODO - Move these from BTCToggleSettings
//   1. Enable Disable Dome Automation
//   2. Enable and Disable Overspeed
//   3. enable / disable drive stick

    if (navCommand == "lt1") {
        volume_down.trigger();
        return;
    } else if (navCommand == "rt1") {
        volume_up.trigger();
        return;
    } else if (actionCommand == navCommand) {
        cancelActions();
        return;
    } else  if (navCommand == "action1") {
        maint_action1_MD.trigger();
        return;
    } else if (navCommand == "action2") {
        maint_action2_MD.trigger();
        return;
    } else if (navCommand == "action3") {
        maint_action3_MD.trigger();
        return;
    } else if (navCommand == "action4") {
        maint_action4_MD.trigger();
        return;
    // } else if (navCommand == "lt3") {
    //     // TODO: Some MD command
    //     return;
    // } else if (navCommand == "rt3") {
    //     // TODO: Some MD command
    //     return;
    } else {
      // Something snuck past the goalie
      // Provide feedback...rumble for 2 seconds
      return;
    } // if-elseif-else

} //maintControl


void mode1Control() {

    if (actionCommand == navCommand) {
        cancelActions();
        return;
    } else if (navCommand == "lt1") {
        mode1_lt1_MD.trigger();
        return;
    } else if (navCommand == "rt1") {
        mode1_rt1_MD.trigger();
        return;
    } else  if (navCommand == "action1") {
        mode1_action1_MD.trigger();
        return;
    } else if (navCommand == "action2") {
        mode1_action2_MD.trigger();
        return;
    } else if (navCommand == "action3") {
        mode1_action3_MD.trigger();
        return;
    } else if (navCommand == "action4") {
        mode1_action4_MD.trigger();
        return;
    // } else if (navCommand == "lt3") {
    //     // TODO: Some MD command
    //     return;
    // } else if (navCommand == "rt3") {
    //     // TODO: Some MD command
    //     return;
    } else {
      // Something snuck past the goalie
      // Provide feedback...rumble for 2 seconds
      return;
    } // if-elseif-else

} //mode1Control

void mode2Control() {

    if (actionCommand == navCommand) {
        cancelActions();
        return;
    } else if (navCommand == "lt1") {
        mode2_lt1_MD.trigger();
        return;
    } else if (navCommand == "rt1") {
        mode2_rt1_MD.trigger();
        return;
    } else  if (navCommand == "action1") {
        mode2_action1_MD.trigger();
        return;
    } else if (navCommand == "action2") {
        mode2_action2_MD.trigger();
        return;
    } else if (navCommand == "action3") {
        mode2_action3_MD.trigger();
        return;
    } else if (navCommand == "action4") {
        mode2_action4_MD.trigger();
        return;
    // } else if (navCommand == "lt3") {
    //     // TODO: Some MD command
    //     return;
    // } else if (navCommand == "rt3") {
    //     // TODO: Some MD command
    //     return;
    } else {
      // Something snuck past the goalie
      // Provide feedback...rumble for 2 seconds
      return;
    } // if-elseif-else

} //mode2Control


void mode3Control() {

    if (actionCommand == navCommand) {
        cancelActions();
        return;
    } else if (navCommand == "lt1") {
        mode3_lt1_MD.trigger();
        return;
    } else if (navCommand == "rt1") {
        mode3_rt1_MD.trigger();
        return;
    } else  if (navCommand == "action1") {
        mode3_action1_MD.trigger();
        return;
    } else if (navCommand == "action2") {
        mode3_action2_MD.trigger();
        return;
    } else if (navCommand == "action3") {
        mode3_action3_MD.trigger();
        return;
    } else if (navCommand == "action4") {
        mode3_action4_MD.trigger();
        return;
    // } else if (navCommand == "lt3") {
    //     // TODO: Some MD command
    //     return;
    // } else if (navCommand == "rt3") {
    //     // TODO: Some MD command
    //     return;
    } else {
      // Something snuck past the goalie
      // Provide feedback...rumble for 2 seconds
      return;
    } // if-elseif-else

} //mode3Control

// Might move drive mode to another button and use this for the arrow keys to allow for more command variety
void mode4Control() {

    // if (actionCommand == navCommand) {
    //    cancelActions();
    //    return;
    // } else if (navCommand == "lt1") {
    //     mode4_lt1_MD.trigger();
    //     return;
    // } else if (navCommand == "r1") {
    //     mode4_rt1_MD.trigger();
    //     return;
    // } else  if (navCommand == "action1") {
    //     mode4_action1_MD.trigger();
    //     return;
    // } else if (navCommand == "action2") {
    //     mode4_action2_MD.trigger();
    //     return;
    // } else if (navCommand == "action3") {
    //     mode4_action3_MD.trigger();
    //     return;
    // } else if (navCommand == "action4") {
    //     mode4_action4_MD.trigger();
    //     return;
    // // } else if (navCommand == "lt3") {
    // //     // TODO: Some MD command
    // //     return;
    // // } else if (navCommand == "rt3") {
    // //     // TODO: Some MD command
    // //     return;
    // } else {
    //   // Something snuck past the goalie
    //   // Provide feedback...rumble for 2 seconds
    //   return;
    // } // if-elseif-else
} //mode4Control

void modeControl() {
// TODO
//    1. Fix tracking of last command from prior mode. If still running, cancel it (maybe cancel all)
//    2. Create PS5 button shutdown routine that uses a long-press
//    3. 

    if (BTNav.connected()) {
        // Serial.print("\r\n Connected");
//         if ((millis() - previousMarcDuinoMillis) > 1000)
//         {
//             marcDuinoButtonCounter = 0;
//             previousMarcDuinoMillis = millis();
//         }
//         marcDuinoButtonCounter += 1;

        navCommand = "";
        navCommand = mapBTButtons();

        if (navCommand != "" ) {

            // Shut down the controller if the PS button is clicked
            if (navCommand == "power") {
                Serial.print("\r\ Shutting Down Controller");
                if (millis() - power_timer > 1000) {
                    if (!isFootMotorStopped) {
                        FootMotor->stop();
                        isFootMotorStopped = true;
                        footDriveSpeed = 0;
                    }
                    // TODO: MD command to stop all functions
                    powerOff();
                }
            } else
                power_timer = millis();

            // Mute all sounds
            if (navCommand == "mute") {
                setMicLED(muteSounds);
                if (muteSounds) {
                    mute_sounds.trigger();
                } else{
                    volume_mid.trigger();
                }
                return;
            } //mute

            // Cancel all actions
            if (navCommand == "cancel") {
                cancelActions();
                return;
            } //mute

            // Change action modes
            if (navCommand == "drive" || navCommand == "maint" || navCommand == "mode1" || navCommand == "mode2" || navCommand == "mode3" || navCommand == "mode4") {
                actionMode = navCommand;

                if (actionMode != previousMode) {
                    Serial.print(F("\r\nPrevious Mode: "));
                    Serial.print(previousMode);

                    Serial.print(F("\r\nNew Mode: "));
                    Serial.print(actionMode);

                    // Not all supported controllers have LEDs
                    if (ledSupport == true) {
                        if (actionMode == "drive") {
                            btLEDColor = btLEDDrive;
                            setLED(btLEDColor);
                        } else if (actionMode == "maint") {
                            btLEDColor = btLEDMaint;
                            setLED(btLEDColor);
                        } else if (actionMode == "mode1") {
                            btLEDColor = btLEDMode1;
                            setLED(btLEDColor);
                        } else if (actionMode == "mode2") {
                            btLEDColor = btLEDMode2;
                            setLED(btLEDColor);
                        } else if (actionMode == "mode3") {
                            btLEDColor = btLEDMode3;
                            setLED(btLEDColor);
                        } else {
                            btLEDColor = btLEDMode4;
                            setLED(btLEDColor);
                        } //if-elseif-else
                    } //end ledSupport check
                    previousMode = actionMode;
                    return;
                } //Mode change
            } else {
                // navCommand must be an action request
                if (actionMode == "drive") {
                    driveControl();
                    return;
                } //drive commands

                if (actionMode == "maint") {
                    maintControl();
                    return;
                } //maint commands

                if (actionMode == "mode1") {
                    mode1Control();
                    return;
                } //mode1 commands

                if (actionMode == "mode2") {
                    mode2Control();
                    return;
                } //mode2 commands

                if (actionMode == "mode3") {
                    mode3Control();
                    return;
                } //mode3 commands

                if (actionMode == "mode4") {
                    mode4Control();
                    return;
                } //mode4 commands

                actionCommand = navCommand;

            } // Mode/Actions check
        } //navCommand

    } else {
        cancelActions();
        return;    
    } //if-else

} //modeControl

// =======================================================================================
//           footDrive Motor Control Section
// =======================================================================================
// TODO
//   1. Considering a mapDriveButtons() function in each controller's header. Worried
//         about added overhead. Supported controllers should have the same buttons, so
//         these actions may remain in main sketch. TBD
//   2. 


bool BTFootMotorDrive(BTCNTL myBTNav = BTNav) {
    int stickSpeed = 0;
    int turnnum = 0;
  
    if (isBTNavigatonInitialized) {    
         // Additional fault control.  Do NOT send additional commands to Sabertooth if no controllers have initialized.
        if (!isStickEnabled) {
        #ifdef SHADOW_VERBOSE
            if (abs(myBTNav.getAnalogHat(LeftHatY)-128) > joystickFootDeadZoneRange) {
                // SHADOW_VERBOSE("Drive Stick is disabled\n")
            }
        #endif

            if (!isFootMotorStopped) {
                FootMotor->stop();
                isFootMotorStopped = true;
                footDriveSpeed = 0;
              
             //   SHADOW_VERBOSE("\n***Foot Motor STOPPED***\n")
            }
            return false;
        } else if (!myBTNav.connected()) {
            if (!isFootMotorStopped) {
                FootMotor->stop();
                isFootMotorStopped = true;
                footDriveSpeed = 0;
             //   SHADOW_VERBOSE("\n***Foot Motor STOPPED***\n")
            }
          
            return false;
        } else if (myBTNav.getAnalogButton(R2)) {
            //Analog R2 - Emergency Break overrides drive
            if (!isFootMotorStopped) {
                FootMotor->stop();
                isFootMotorStopped = true;
                footDriveSpeed = 0;

             //   SHADOW_VERBOSE("\n***Foot Motor STOPPED***\n")              
            }
            return false;
        } else {
            //Unlock motors with analog L2
            if (myBTNav.getAnalogButton(L2)) {
                int joystickPosition = myBTNav.getAnalogHat(LeftHatY);
              
                if (overSpeedSelected) { //Over throttle is selected
                    stickSpeed = (map(joystickPosition, 0, 255, -drivespeed2, drivespeed2));   
                } else {
                    stickSpeed = (map(joystickPosition, 0, 255, -drivespeed1, drivespeed1));
                }

                if ( abs(joystickPosition-128) < joystickFootDeadZoneRange) {
                    // This is RAMP DOWN code when stick is now at ZERO but prior FootSpeed > 20
                    if (abs(footDriveSpeed) > 50) {   
                        if (footDriveSpeed > 0) {
                            footDriveSpeed -= 3;
                        } else {
                            footDriveSpeed += 3;
                        }
                        //SHADOW_VERBOSE("ZERO FAST RAMP: footSpeed: %d\nStick Speed: %d\n", footDriveSpeed, stickSpeed)
                    } else if (abs(footDriveSpeed) > 20) {   
                        if (footDriveSpeed > 0) {
                            footDriveSpeed -= 2;
                        } else {
                            footDriveSpeed += 2;
                        }  
                      //  SHADOW_VERBOSE("ZERO MID RAMP: footSpeed: %d\nStick Speed: %d\n", footDriveSpeed, stickSpeed)
                    } else {        
                        footDriveSpeed = 0;
                    }
                } else {
                    isFootMotorStopped = false;
                    if (footDriveSpeed < stickSpeed) {
                        if ((stickSpeed-footDriveSpeed)>(ramping+1)) {
                            footDriveSpeed+=ramping;
                          // SHADOW_VERBOSE("RAMPING UP: footSpeed: %d\nStick Speed: %d\n", footDriveSpeed, stickSpeed)
                        } else {
                            footDriveSpeed = stickSpeed;
                        }
                    } else if (footDriveSpeed > stickSpeed) {
                        if ((footDriveSpeed-stickSpeed)>(ramping+1)) {
                            footDriveSpeed-=ramping;
                            //SHADOW_VERBOSE("RAMPING DOWN: footSpeed: %d\nStick Speed: %d\n", footDriveSpeed, stickSpeed)
                        } else {
                            footDriveSpeed = stickSpeed;  
                        }
                    } else {
                        footDriveSpeed = stickSpeed;  
                    }
                } //if-else
                turnnum = (myBTNav.getAnalogHat(LeftHatX));

                //TODO:  Is there a better algorithm here?  
                if ( abs(footDriveSpeed) > 50)
                    turnnum = (map(myBTNav.getAnalogHat(LeftHatX), 54, 200, -(turnspeed/4), (turnspeed/4)));
                else if (turnnum <= 200 && turnnum >= 54)
                    turnnum = (map(myBTNav.getAnalogHat(LeftHatX), 54, 200, -(turnspeed/3), (turnspeed/3)));
                else if (turnnum > 200)
                    turnnum = (map(myBTNav.getAnalogHat(LeftHatX), 201, 255, turnspeed/3, turnspeed));
                else if (turnnum < 54)
                    turnnum = (map(myBTNav.getAnalogHat(LeftHatX), 0, 53, -turnspeed, -(turnspeed/3)));
                  
                if (abs(turnnum) > 5) {
                    isFootMotorStopped = false;   
                }

                currentMillis = millis();
              
                if ((currentMillis - previousFootMillis) > serialLatency) {
                    if (footDriveSpeed != 0 || abs(turnnum) > 5) {
                        // SHADOW_VERBOSE("Motor: FootSpeed: %d\nTurnnum: %d\nTime of command: %lu\n", footDriveSpeed, turnnum, millis())              
                        FootMotor->turn(turnnum * (invertTurnDirection ? 1 : -1));
                        FootMotor->drive(footDriveSpeed);
                    } else {    
                        if (!isFootMotorStopped) {
                            FootMotor->stop();
                            isFootMotorStopped = true;
                            footDriveSpeed = 0;
                          
                        //   SHADOW_VERBOSE("\n***Foot Motor STOPPED***\n")
                        }
                    }
                  
                    // The Sabertooth won't act on mixed mode packet serial commands until
                    // it has received power levels for BOTH throttle and turning, since it
                    // mixes the two together to get diff-drive power levels for both motors.
                  
                    previousFootMillis = currentMillis;
                    return true; //we sent a foot command   
                }
            } //if getAnalogButton(L2)
        }
    }
    return false;
} // BTFootMotorDrive

void footMotorDrive() {
    //Flood control prevention
    if ((millis() - previousFootMillis) < serialLatency)
        return;
  
    if (BTNav.connected())
        BTFootMotorDrive(BTNav);
} //footMotorDrive  


// =======================================================================================
//           domeDrive Motor Control Section
// =======================================================================================

int BTDomeDrive(BTCNTL myBTNav = BTNav) {
    int domeRotationSpeed = 0;
    int joystickPosition = myBTNav.getAnalogHat(RightHatX);
        
    domeRotationSpeed = (map(joystickPosition, 0, 255, -domespeed, domespeed));
    if ( abs(joystickPosition-128) < joystickDomeDeadZoneRange ) 
       domeRotationSpeed = 0;
          
    if (domeRotationSpeed != 0 && domeAutomation == true)  // Turn off dome automation if manually moved
    {   
        domeAutomation = false; 
        domeStatus = 0;
        domeTargetPosition = 0; 
        
       // SHADOW_VERBOSE("Dome Automation OFF\n")
    }    
    return domeRotationSpeed;
} //BTDomeDrive

void rotateDome(int domeRotationSpeed, String mesg) {
    //Constantly sending commands to the SyRen (Dome) is causing foot motor delay.
    //Lets reduce that chatter by trying 3 things:
    // 1.) Eliminate a constant stream of "don't spin" messages (isDomeMotorStopped flag)
    // 2.) Add a delay between commands sent to the SyRen (previousDomeMillis timer)
    // 3.) Switch to real UART on the MEGA (Likely the *CORE* issue and solution)
    // 4.) Reduce the timout of the SyRen - just better for safety!
    
    currentMillis = millis();
    if ((!isDomeMotorStopped || domeRotationSpeed != 0) &&
        ((currentMillis - previousDomeMillis) > (2*serialLatency)))
    {
        if (domeRotationSpeed != 0)
        {
            isDomeMotorStopped = false;
            // SHADOW_VERBOSE("Dome rotation speed: %d\n", domeRotationSpeed)        
            DomeMotor->motor(domeRotationSpeed);
        }
        else
        {
            isDomeMotorStopped = true; 
            // SHADOW_VERBOSE("\n***Dome motor is STOPPED***\n")
            DomeMotor->stop();
        }
        previousDomeMillis = currentMillis;      
    }
} //rotateDome

void domeDrive() {
    //Flood control prevention
    //This is intentionally set to double the rate of the Dome Motor Latency
    if ((millis() - previousDomeMillis) < (2*serialLatency))
        return;
  
    int domeRotationSpeed = 0;
    int BTNavControlSpeed = 0;
    if (BTNav.connected()) {
        BTNavControlSpeed = BTDomeDrive(BTNav);
        domeRotationSpeed = BTNavControlSpeed;
        rotateDome(domeRotationSpeed,"Controller Move");
    } else if (!isDomeMotorStopped) {
        DomeMotor->stop();
        isDomeMotorStopped = true;
    }  
} //domeDrive

// =======================================================================================
//                               Toggle Control Section
// =======================================================================================
// TODO - Move to "maint" mode

void BTToggleSettings(BTCNTL myBTNav = BTNav) {

//    // enable / disable drive stick
//     if (myBTNav.getButtonClick(PS) && myBTNav.getButtonClick(CROSS)) {
//         //SHADOW_DEBUG("Disabling the DriveStick\n")
//         //SHADOW_DEBUG("Stopping Motors\n")
//         FootMotor->stop();
//         isFootMotorStopped = true;
//         isStickEnabled = false;
//         footDriveSpeed = 0;
//     }

//     if (myBTNav.getButtonClick(PS) && myBTNav.getButtonClick(CIRCLE)) {
//         //SHADOW_DEBUG("Enabling the DriveStick\n");
//         isStickEnabled = true;
//     }

//     // Enable and Disable Overspeed
//     if (myBTNav.getButtonClick(L3) && myBTNav.getButtonClick(L1) && isStickEnabled) {
//         if ((millis() - previousSpeedToggleMillis) > 1000) {
//             speedToggleButtonCounter = 0;
//             previousSpeedToggleMillis = millis();
//         } 

//         speedToggleButtonCounter += 1;       
//         if (speedToggleButtonCounter == 1) {
//             if (!overSpeedSelected) {
//                 overSpeedSelected = true;
//                 //SHADOW_VERBOSE("Over Speed is now: ON\n");
//             } else {      
//                 overSpeedSelected = false;
//                 //SHADOW_VERBOSE("Over Speed is now: OFF\n")
//             }
//         }
//     }

// // Enable Disable Dome Automation
//     if(myBTNav.getButtonClick(L2) && myBTNav.getButtonClick(CROSS)) {
//         domeAutomation = false;
//         domeStatus = 0;
//         domeTargetPosition = 0;
//         DomeMotor->stop();
//         isDomeMotorStopped = true;
//         //SHADOW_DEBUG("Dome Automation OFF\n")
//     } 

//     if(myBTNav.getButtonClick(L2) && myBTNav.getButtonClick(CIRCLE)) {
//         domeAutomation = true;
//         //SHADOW_DEBUG("Dome Automation On\n")
//     } 
} //BTToggleSettings

void toggleSettings() {
    if (BTNav.connected())
        // BTToggleSettings(BTNav);
        BTToggleSettings();
}  //toggleSettings

// =======================================================================================
// This function handles the processing of custom MarcDuino panel routines
// =======================================================================================
// TODO
//    1. Move to MDHandler.h

// void custMarcDuinoPanel()
// {
//     if (!sRunningCustRoutine)
//         return;
//     sRunningCustRoutine = false;
//     for (int i = 0; i < SizeOfArray(sPanelStatus); i++)
//     {
//         PanelStatus &panel = sPanelStatus[i];
//         if (panel.fStatus == 1)
//         {
//             if (panel.fStartTime + panel.fStartDelay * 1000 < millis())
//             {
//                 char cmd[10];
//                 snprintf(cmd, sizeof(cmd), ":OP%02d", i+1);
//                 sendMarcCommand(cmd);
//                 panel.fStatus = 2;
//             }
//         }
//         else if (panel.fStatus == 2)
//         {
//             if (panel.fStartTime + (panel.fStartDelay + panel.fDuration) * 1000 < millis())
//             {
//                 char cmd[10];
//                 snprintf(cmd, sizeof(cmd), ":CL%02d", i+1);
//                 sendMarcCommand(cmd);
//                 panel.fStatus = 0;
//             }
//         }
//         if (panel.fStatus != 0)
//             sRunningCustRoutine = true;
//     }
// }

// =======================================================================================
//                             Dome Automation Function
//
//    Features toggles 'on' via L2 + CIRCLE.  'off' via L2 + CROSS.  Default is 'off'.
//
//    This routines randomly turns the dome motor in both directions.  It assumes the 
//    dome is in the 'home' position when the auto dome feature is toggled on.  From
//    there it turns the dome in a random direction.  Stops for a random length of 
//    of time.  Then returns the dome to the home position.  This randomly repeats.
//
//    It is driven off the user variable - time360DomeTurn.  This records how long
//    it takes the dome to do a 360 degree turn at the given auto dome speed.  Tweaking
//    this parameter to be close provides the best results.
//
//    Activating the dome controller manually immediately cancels the auto dome feature
//    or you can toggle the feature off by pressing L2 + CROSS.
// =======================================================================================
void autoDome() {
    long rndNum;
    int domeSpeed;
    
    if (domeStatus == 0)  // Dome is currently stopped - prepare for a future turn
    { 
        if (domeTargetPosition == 0)  // Dome is currently in the home position - prepare to turn away
        {
            domeStartTurnTime = millis() + (random(3, 10) * 1000);

            rndNum = random(5,354);

            domeTargetPosition = rndNum;  // set the target position to a random degree of a 360 circle - shaving off the first and last 5 degrees

            if (domeTargetPosition < 180)  // Turn the dome in the positive direction
            {
                domeTurnDirection = 1;
                domeStopTurnTime = domeStartTurnTime + ((domeTargetPosition / 360) * time360DomeTurn);              
            }
            else  // Turn the dome in the negative direction
            {
                domeTurnDirection = -1;
                domeStopTurnTime = domeStartTurnTime + (((360 - domeTargetPosition) / 360) * time360DomeTurn);              
            }
        }
        else  // Dome is not in the home position - send it back to home
        {
            domeStartTurnTime = millis() + (random(3, 10) * 1000);   
            if (domeTargetPosition < 180)
            {
                domeTurnDirection = -1;
                domeStopTurnTime = domeStartTurnTime + ((domeTargetPosition / 360) * time360DomeTurn);
            }
            else
            {
                domeTurnDirection = 1;
                domeStopTurnTime = domeStartTurnTime + (((360 - domeTargetPosition) / 360) * time360DomeTurn);
            }
            domeTargetPosition = 0;
        }
        domeStatus = 1;  // Set dome status to preparing for a future turn

        SHADOW_DEBUG("Dome Automation: Initial Turn Set\nCurrent Time: %lu\nNext Start Time: %lu\nNext Stop Time: %lu\nDome Target Position: %f\n",
            millis(), domeStartTurnTime, domeStopTurnTime, domeTargetPosition);
    }    

    if (domeStatus == 1)  // Dome is prepared for a future move - start the turn when ready
    {
        if (domeStartTurnTime < millis())
        {
            domeStatus = 2; 
            SHADOW_DEBUG("Dome Automation: Ready To Start Turn\n")
        }
    }
    
    if (domeStatus == 2) // Dome is now actively turning until it reaches its stop time
    {      
        if (domeStopTurnTime > millis())
        {
            domeSpeed = domeAutoSpeed * domeTurnDirection;
            DomeMotor->motor(domeSpeed);

            SHADOW_DEBUG("Turning Now!!\n")
        }
        else  // turn completed - stop the motor
        {
            domeStatus = 0;
            DomeMotor->stop();

            SHADOW_DEBUG("STOP TURN!!\n")
        }      
    }
}

// =======================================================================================
//           BT Controller Device Mgt Functions
// =======================================================================================

void onInitBTNav() {

    String btAddress = getLastConnectedBtMAC();
    isBTNavigatonInitialized = true;
    badBTData = 0;

    //SHADOW_DEBUG("\nBT Address of Last connected Device when BT Connected: %s\n", btAddress.c_str());
    
    if (btAddress == BTControllerMac || btAddress == BTControllerBackupMac) {
    //     //SHADOW_DEBUG("\nWe have our controller connected.\n")
          
        controllerConnected = true;
        WaitingforReconnect = true;
        if (ledSupport == true) {
            setLED(btLEDColor);
        }
    }
#ifdef USE_PREFERENCES
    else if (BTControllerMac[0] == 'X') {
        SHADOW_DEBUG("\nAssigning %s as controller.\n", btAddress.c_str());
        preferences.putString(PREFERENCE_CONTROLLER_MAC, btAddress);
        BTControllerMac = btAddress;
        controllerConnected = true;
        WaitingforReconnect = true;
        if (ledSupport == true) {
            setLED(btLEDColor);
        }
    }
#endif
    else {
        // Prevent connection from anything but the MAIN controllers          
        SHADOW_DEBUG("\nWe have an invalid controller trying to connect as tha FOOT controller, it will be dropped.\n")
        FootMotor->stop();
        DomeMotor->stop();
        isFootMotorStopped = true;
        footDriveSpeed = 0;
        BTNav.disconnect();    
        isBTNavigatonInitialized = false;
        controllerConnected = false;        
    } 
} //onInitBTNav

String getLastConnectedBtMAC() {
    char buffer[20];
    uint8_t* addr = Btd.disc_bdaddr;
    snprintf(buffer, sizeof(buffer), "%02X:%02X:%02X:%02X:%02X:%02X",
        addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
    return buffer;
}

bool criticalFaultDetect() {
// TODO
//    1. Begin working on this. Especially the commented code.

    if (BTNav.connected()) {
        currentTime = millis();
        lastMsgTime = BTNav.getLastMessageTime();
        msgLagTime = currentTime - lastMsgTime;            
        
        if (WaitingforReconnect) {
            if (msgLagTime < 200) { 
                WaitingforReconnect = false; 
            }
            lastMsgTime = currentTime;            
        } 

        if (currentTime >= lastMsgTime) {
            msgLagTime = currentTime - lastMsgTime;
        } else {
            msgLagTime = 0;
        }
        
        if (msgLagTime > 300 && !isFootMotorStopped) {
            SHADOW_DEBUG("It has been 300ms since we heard from the BT Foot Controller\n")
            SHADOW_DEBUG("Shutting down motors, and watching for a new BT Foot message\n")
            FootMotor->stop();
            isFootMotorStopped = true;
            footDriveSpeed = 0;
        }
        
        if ( msgLagTime > 10000 ) {
            SHADOW_DEBUG("It has been 10s since we heard from the BT Foot Controller\nmsgLagTime:%u  lastMsgTime:%u  millis: %lu\n",
                          msgLagTime, lastMsgTime, millis())
            SHADOW_DEBUG("Disconnecting the Foot controller\n")
            FootMotor->stop();
            isFootMotorStopped = true;
            footDriveSpeed = 0;
            BTNav.disconnect();
            WaitingforReconnect = true;
            return true;
        }

// TODO - Review for BT updates
        // if (BTNav.connected() && lastMessageCounter != BTNav.getMessageCounter()) {
        //     lastMessageCounter = BTNav.getMessageCounter();

        //Check BT Controller Signal Data
        // if (!BTNav.getStatus(Plugged) && !BTNav.getStatus(Unplugged)) {
        //     //We don't have good data from the controller.
        //     //Wait 15ms if no second controller - 100ms if some controller connected, Update USB, and try again
        //     if (BTNav.connected())
        //     {
        //           delay(100);     
        //     } else
        //     {
        //           delay(15);
        //     }
            
        //     // Usb.Task();   
        //     lastMsgTime = BTNav.getLastMessageTime();
            
        //     if(!BTNav.getStatus(Plugged) && !BTNav.getStatus(Unplugged))
        //     {
        //         badBTData++;
        //         SHADOW_DEBUG("\n**Invalid data from PS3 FOOT Controller. - Resetting Data**\n")
        //         return true;
        //     }
        // }
        // else if (badBTData > 0)
        // {

        //     badBTData = 0;
        // }
        
        if ( badBTData > 10 ) {
            SHADOW_DEBUG("Too much bad data coming from the BT Controller\n")
            SHADOW_DEBUG("Disconnecting the controller and stop motors.\n")

            FootMotor->stop();
            isFootMotorStopped = true;
            footDriveSpeed = 0;
            BTNav.disconnect();
            WaitingforReconnect = true;
            return true;
        }
    } else if (!isFootMotorStopped) {
        SHADOW_DEBUG("No foot controller was found\n")
        SHADOW_DEBUG("Shuting down motors and watching for a new PS3 foot message\n")

        FootMotor->stop();
        isFootMotorStopped = true;
        footDriveSpeed = 0;
        WaitingforReconnect = true;
        return true;
    }
    
    return false;
}

// =======================================================================================
//           USB Read Function - Supports Main Program Loop
// =======================================================================================
bool readUSB() {     
    Usb.Task();

    //The more devices we have connected to the USB or BlueTooth, the more often Usb.Task need to be called to eliminate latency.
    if (BTNav.connected()) {
        if (criticalFaultDetect()) {
            //We have a fault condition that we want to ensure that we do NOT process any controller data!!!
            return false;
        }   
    } else if (!isFootMotorStopped) {
        SHADOW_DEBUG("No foot controller was found\n")
        SHADOW_DEBUG("Shuting down motors, and watching for a new PS3 foot message\n")

        FootMotor->stop();
        isFootMotorStopped = true;
        footDriveSpeed = 0;
        WaitingforReconnect = true;
    }

    return true;
}
