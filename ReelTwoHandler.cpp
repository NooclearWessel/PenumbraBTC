
// // #include "ReelTwoHandler.h"


// // bool handleMarcduinoAction(const char* action)
// // {
// //     String LD_text = "";
// //     bool panelTypeSelected = false;
// //     char buffer[1024];
// //     snprintf(buffer, sizeof(buffer), "%s", action);
// //     char* cmd = buffer;
// //     // if (*cmd == '#')
// //     // {
// //     //     // Std Marcduino Function Call Configured
// //     //     uint32_t seq = strtolu(cmd+1, &cmd);
// //     //     if (*cmd == '\0')
// //     //     {
// //     //         if (seq >= 1 && seq <= SizeOfArray(DEFAULT_MARCDUINO_COMMANDS))
// //     //         {
// //     //             // If the commands starts with "BM" we direct it to the body marc controller
// //     //             const char* marcCommand = DEFAULT_MARCDUINO_COMMANDS[seq-1];
// //     //             if (marcCommand[0] == 'B' && marcCommand[1] == 'M')
// //     //             {
// //     //                 sendBodyMarcCommand(&marcCommand[2]);
// //     //             }
// //     //             else
// //     //             {
// //     //                 // Otherwise we send it to the dome Marcduino
// //     //                 sendMarcCommand(marcCommand);
// //     //             }
// //     //             return true;
// //     //         }
// //     //         else
// //     //         {
// //     //             SHADOW_DEBUG("Marcduino sequence range is 1-%d in action command \"%s\"\n", SizeOfArray(DEFAULT_MARCDUINO_COMMANDS), action)
// //     //             return false;
// //     //         }
// //     //     }
// //     //     SHADOW_DEBUG("Excepting number after # in action command \"%s\"\n", action)
// //     //     return false;
// //     // }
// //     for (;;)
// //     {
// //         char buf[100];
// //         if (*cmd == '"')
// //         {
// //             // Skip the quote
// //             cmd++;
// //             char* marcCommand = cmd;
// //             char* nextCmd = strchr(cmd, ',');
// //             if (nextCmd != nullptr)
// //             {
// //                 size_t len = nextCmd - marcCommand;
// //                 strncpy(buf, marcCommand, len);
// //                 buf[len] = '\0';
// //                 cmd = nextCmd;
// //                 marcCommand = buf;
// //             }
// //             else
// //             {
// //                 cmd += strlen(marcCommand);
// //             }
// //             // If the commands starts with "BM" we direct it to the body marc controller
// //             if (marcCommand[0] == 'B' && marcCommand[1] == 'M')
// //             {
// //                 sendBodyMarcCommand(&marcCommand[2]);
// //             }
// //             else
// //             {
// //                 sendMarcCommand(marcCommand);
// //             }
// //         }
// //         else if (*cmd == '$')
// //         {
// //             char* mp3Cmd = cmd;
// //             char* nextCmd = strchr(cmd, ',');
// //             if (nextCmd != nullptr)
// //             {
// //                 size_t len = nextCmd - mp3Cmd;
// //                 strncpy(buf, mp3Cmd, len);
// //                 buf[len] = '\0';
// //                 cmd = nextCmd;
// //                 mp3Cmd = buf;
// //             }
// //             else
// //             {
// //                 cmd += strlen(mp3Cmd);
// //             }
// //             sendMarcCommand(mp3Cmd);
// //         }
// //         else if (startswith(cmd, "MP3="))
// //         {
// //             char* mp3Cmd = cmd;
// //             char* nextCmd = strchr(cmd, ',');
// //             if (nextCmd != nullptr)
// //             {
// //                 size_t len = nextCmd - mp3Cmd;
// //                 buf[0] = '$';
// //                 strncpy(&buf[1], mp3Cmd, len);
// //                 buf[len+1] = '\0';
// //                 cmd = nextCmd;
// //                 mp3Cmd = buf;
// //             }
// //             else
// //             {
// //                 cmd += strlen(mp3Cmd);
// //             }
// //             sendMarcCommand(mp3Cmd);
// //         }
// //         else if (startswith(cmd, "Panel=M"))
// //         {
// //             static const char* sCommands[] = {
// //                 ":CL00",
// //                 ":SE51",
// //                 ":SE52",
// //                 ":SE53",
// //                 ":SE54",
// //                 ":SE55",
// //                 ":SE56",
// //                 ":SE57"
// //             };
// //             uint32_t num = strtolu(cmd, &cmd);
// //             if (num >= 1 && num <= SizeOfArray(sCommands))
// //             {
// //                 if (num > 1)
// //                 {
// //                     sendMarcCommand(":CL00");  // close all the panels prior to next custom routine
// //                     delay(50); // give panel close command time to process before starting next panel command 
// //                 }
// //                 sendMarcCommand(sCommands[num-1]);
// //                 panelTypeSelected = true;
// //             }
// //             else
// //             {
// //                 SHADOW_DEBUG("Marc Panel range is 1 - %d in action command \"%s\"\n", SizeOfArray(sCommands), action)
// //                 return false;
// //             }
// //         }
// //         else if (startswith(cmd, "Panel"))
// //         {
// //             uint32_t num = strtolu(cmd, &cmd);
// //             if (num >= 1 && num <= SizeOfArray(sPanelStatus))
// //             {
// //                 PanelStatus &panel = sPanelStatus[num-1];
// //                 panel.fStatus = 1;
// //                 if (*cmd == '[')
// //                 {
// //                     cmd++;
// //                     do
// //                     {
// //                         if (startswith(cmd, "delay="))
// //                         {
// //                             uint32_t delayNum = strtolu(cmd, &cmd);
// //                             if (delayNum < 31)
// //                             {
// //                                 panel.fStartDelay = delayNum;
// //                             }
// //                             else
// //                             {
// //                                 panel.fStatus = 0;
// //                             }
// //                         }
// //                         else if (startswith(cmd, "dur="))
// //                         {
// //                             uint32_t duration = strtolu(cmd, &cmd);
// //                             if (duration < 31)
// //                             {
// //                                 panel.fDuration = duration;
// //                             }
// //                             else
// //                             {
// //                                 panel.fStatus = 0;
// //                             }
// //                         }
// //                         else if (*cmd == ',')
// //                         {
// //                             cmd++;
// //                         }
// //                     }
// //                     while (*cmd != '\0' && *cmd != ']');
// //                     if (*cmd == ']')
// //                         cmd++;
// //                 }
// //                 if (panel.fStatus)
// //                 {
// //                     panelTypeSelected = true;
// //                     panel.fStartTime = millis();
// //                 }
// //             }
// //             else
// //             {
// //                 SHADOW_DEBUG("Panel range is 1 - %d in action command \"%s\"\n", SizeOfArray(sPanelStatus), action)
// //                 return false;
// //             }
// //         }
// //         else if (startswith(cmd, "LDText=\""))
// //         {
// //             char* startString = ++cmd;
// //             while (*cmd != '\0' && *cmd != '"')
// //                 cmd++;
// //             if (*cmd == '"')
// //                 *cmd = '\0';
// //             LD_text = startString;
// //         }
// //         else if (startswith(cmd, "LD="))
// //         {
// //             uint32_t num = strtolu(cmd, &cmd);
// //             if (num >= 1 && num < 8)
// //             {
// //                 // If a custom panel movement was selected - need to briefly pause before changing light sequence to avoid conflict)
// //                 if (panelTypeSelected)
// //                 {
// //                     delay(30);
// //                 }
// //                 switch (num)
// //                 {
// //                     case 1:
// //                         sendMarcCommand("@0T1");
// //                         break;              
// //                     case 2:
// //                         sendMarcCommand("@0T4");
// //                         break;              
// //                     case 3:
// //                         sendMarcCommand("@0T5");
// //                         break;
// //                     case 4:
// //                         sendMarcCommand("@0T6");
// //                         break;
// //                     case 5:
// //                         sendMarcCommand("@0T10");
// //                         break;
// //                     case 6:
// //                         sendMarcCommand("@0T11");
// //                         break;
// //                     case 7:
// //                         sendMarcCommand("@0T92");
// //                         break;
// //                     case 8:
// //                         sendMarcCommand("@0T100");
// //                         delay(50);
// //                         String custString = "@0M";
// //                         custString += LD_text;
// //                         sendMarcCommand(custString.c_str());
// //                         break;
// //                 }
// //             }
// //             else
// //             {
// //                 SHADOW_DEBUG("LD range is 1 - 8 in action command \"%s\"\n", action)
// //                 return false;
// //             }
// //         }
// //         if (*cmd != ',')
// //             break;
// //         cmd++;
// //     }
// //     if (*cmd != '\0')
// //     {
// //         SHADOW_DEBUG("Ignoring unknown trailing \"%s\" in action \"%s\"\n", cmd, action);
// //     }
// //     if (panelTypeSelected)
// //     {
// //         printf("panelTypeSelected\n");
// //         sRunningCustRoutine = true;
// //     }
// //     return true;
// // }


// void sendMarcCommand(const char* cmd)
// {
//     // SHADOW_VERBOSE("Sending MARC: \"%s\"\n", cmd)
//     MD_SERIAL.print(cmd); MD_SERIAL.print("\r");
// #if defined(MARC_SOUND_PLAYER)
//     sMarcSound.handleCommand(cmd);
// #endif
// }

// void sendBodyMarcCommand(const char* cmd)
// {
//     // SHADOW_VERBOSE("Sending BODYMARC: \"%s\"\n", cmd)
//     BODY_MD_SERIAL.print(cmd); BODY_MD_SERIAL.print("\r");
// }

