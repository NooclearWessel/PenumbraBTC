// reserved actions - May be used in place of mode controls
MARCDUINO_ACTION(stop_sequence, ":SE00")      // Stopsequence
MARCDUINO_ACTION(stop_sounds, "$s")           // Stop Sounds
MARCDUINO_ACTION(mute_sounds, "$O")           // All sounds and volume off
MARCDUINO_ACTION(volume_down, "$-")           // AstroPixels Volume Down
MARCDUINO_ACTION(volume_up, "$+")             // AstroPixels Volume Up
MARCDUINO_ACTION(volume_mid, "$m")            // APP Volume Mid
MARCDUINO_ACTION(volume_max, "$f")            // APP Volume Max

// drive mode
MARCDUINO_ACTION(drive_lt1_MD, ":SE03")       // SmirkWaveSequence
MARCDUINO_ACTION(drive_rt1_MD, ":SE04")       // OpenCloseWaveSequence
MARCDUINO_ACTION(drive_action1_MD, ":SE08")   // AstroPixels LeiaMessage
MARCDUINO_ACTION(drive_action2_MD, ":SE07")   // AstroPixels CantinaSequence
MARCDUINO_ACTION(drive_action3_MD, ":SE02")   // WaveSequence
MARCDUINO_ACTION(drive_action4_MD, ":SE01")   // ScreamSequence

// maint mode
MARCDUINO_ACTION(maint_lt1_MD, "")            // Placeholder
MARCDUINO_ACTION(maint_rt1_MD, "")            // Placeholder
MARCDUINO_ACTION(maint_action1_MD, "")        // Placeholder
MARCDUINO_ACTION(maint_action2_MD, "")        // Placeholder
MARCDUINO_ACTION(maint_action3_MD, "")        // Placeholder
MARCDUINO_ACTION(maint_action4_MD, "")        // Placeholder

// mode1
MARCDUINO_ACTION(mode1_lt1_MD, "$813")         // AstroPixels HarlemShakeSequence
MARCDUINO_ACTION(mode1_rt1_MD, ":SE09")        // AstroPixels DiscoSequence
MARCDUINO_ACTION(mode1_action1_MD, ":SE05")    // BeepCantinaSequence
MARCDUINO_ACTION(mode1_action2_MD, ":SE06")    // ShortSequence
MARCDUINO_ACTION(mode1_action3_MD, ":SE55")    // MarchingAntsPanelSequence
MARCDUINO_ACTION(mode1_action4_MD, ":SE56")    // FaintPanelSequence

// mode2
MARCDUINO_ACTION(mode2_lt1_MD, ":SE57")        // RythmicPanelSequence
MARCDUINO_ACTION(mode2_rt1_MD, "$821")         // GirlOnFireSequence
MARCDUINO_ACTION(mode2_action1_MD, "$720")     // YodaClearMind
MARCDUINO_ACTION(mode2_action2_MD, ":SE10")
MARCDUINO_ACTION(mode2_action3_MD, ":SE11")
MARCDUINO_ACTION(mode2_action4_MD, ":SE13")

// mode3 - Body Servos
MARCDUINO_ACTION(mode3_lt1_MD, "BM:OP00")      // Open
// MARCDUINO_ACTION(mode3_lt1_MD_CL, "BM:CL00")      // Close
MARCDUINO_ACTION(mode3_rt1_MD, "BM:OP01")
// MARCDUINO_ACTION(mode3_rt1_MD_CL, "BM:CL01")
MARCDUINO_ACTION(mode3_action1_MD, "BM:OP02")
// MARCDUINO_ACTION(mode3_action1_MD_CL, "BM:CL02")
MARCDUINO_ACTION(mode3_action2_MD, "BM:OP03")
// MARCDUINO_ACTION(mode3_action2_MD_CL, "BM:CL03")
MARCDUINO_ACTION(mode3_action3_MD, "BM:OP04")
// MARCDUINO_ACTION(mode3_action3_MD_CL, "BM:CL04")
MARCDUINO_ACTION(mode3_action4_MD, "BM:OP05")
// MARCDUINO_ACTION(mode3_action4_MD_CL, "BM:Cl05")


// mode4
// MARCDUINO_ACTION(mode4_lt1_MD, "#12")
// MARCDUINO_ACTION(mode4_rt1_MD, "#13")
// MARCDUINO_ACTION(mode4_action1_MD, "#14")
// MARCDUINO_ACTION(mode4_action2_MD, "#11")
// MARCDUINO_ACTION(mode4_action3_MD, "#26")
// MARCDUINO_ACTION(mode4_action4_MD, "#23")

// MARCDUINO_ACTION(btnUP_PS_MD, "$71,LD=5")
// MARCDUINO_ACTION(btnLeft_PS_MD, "$81,LD=1")
// MARCDUINO_ACTION(btnRight_PS_MD, "$83,LD=1")
// MARCDUINO_ACTION(btnDown_PS_MD, "$82,LD=1")
// MARCDUINO_ACTION(btnUP_L1_MD, "#8")
// MARCDUINO_ACTION(btnLeft_L1_MD, "#3")
// MARCDUINO_ACTION(btnRight_L1_MD, "#5")
// MARCDUINO_ACTION(btnDown_L1_MD, "#9")
// MARCDUINO_ACTION(FTbtnUP_PS_MD, "#38")          // Arrow UP + PS
// MARCDUINO_ACTION(FTbtnLeft_PS_MD, "#40")        // Arrow Left + PS
// MARCDUINO_ACTION(FTbtnRight_PS_MD, "#41")       // Arrow Right + PS
// MARCDUINO_ACTION(FTbtnDown_PS_MD, "#39")        // Arrow Down + PS
// MARCDUINO_ACTION(FTbtnUP_L1_MD, "#34")          // Arrow UP + L1
// MARCDUINO_ACTION(FTbtnLeft_L1_MD, "#36")        // Arrow Left + L1
// MARCDUINO_ACTION(FTbtnRight_L1_MD, "#37")       // Arrow Right + L1
// MARCDUINO_ACTION(FTbtnDown_L1_MD, "#35")        // Arrow Down + L1






// ---------------------------------------------------------------------------------------
//                          MarcDuino Button Settings
// ---------------------------------------------------------------------------------------
// Std MarcDuino Function Codes:
//     1 = Close All Panels
//     2 = Scream - all panels open
//     3 = Wave, One Panel at a time
//     4 = Fast (smirk) back and forth wave
//     5 = Wave 2, Open progressively all panels, then close one by one
//     6 = Beep cantina - w/ marching ants panel action
//     7 = Faint / Short Circuit
//     8 = Cantina Dance - orchestral, rhythmic panel dance
//     9 = Leia message
//    10 = Disco
//    11 = Quite mode reset (panel close, stop holos, stop sounds)
//    12 = Full Awake mode reset (panel close, rnd sound, holo move,holo lights off)
//    13 = Mid Awake mode reset (panel close, rnd sound, stop holos)
//    14 = Full Awake+ reset (panel close, rnd sound, holo move, holo lights on)
//    15 = Scream, with all panels open (NO SOUND)
//    16 = Wave, one panel at a time (NO SOUND)
//    17 = Fast (smirk) back and forth (NO SOUND)
//    18 = Wave 2 (Open progressively, then close one by one) (NO SOUND)
//    19 = Marching Ants (NO SOUND)
//    20 = Faint/Short Circuit (NO SOUND)
//    21 = Rhythmic cantina dance (NO SOUND)
//    22 = Random Holo Movement On (All) - No other actions
//    23 = Holo Lights On (All)
//    24 = Holo Lights Off (All)
//    25 = Holo reset (motion off, lights off)
//    26 = Volume Up
//    27 = Volume Down
//    28 = Volume Max
//    29 = Volume Mid
//    30 = Open All Dome Panels
//    31 = Open Top Dome Panels
//    32 = Open Bottom Dome Panels
//    33 = Close All Dome Panels
//    34 = Open Dome Panel #1
//    35 = Close Dome Panel #1
//    36 = Open Dome Panel #2
//    37 = Close Dome Panel #2
//    38 = Open Dome Panel #3
//    39 = Close Dome Panel #3
//    40 = Open Dome Panel #4
//    41 = Close Dome Panel #4
//    42 = Open Dome Panel #5
//    43 = Close Dome Panel #5
//    44 = Open Dome Panel #6
//    45 = Close Dome Panel #6
//    46 = Open Dome Panel #7
//    47 = Close Dome Panel #7
//    48 = Open Dome Panel #8
//    49 = Close Dome Panel #8
//    50 = Open Dome Panel #9
//    51 = Close Dome Panel #9
//    52 = Open Dome Panel #10
//    53 = Close Dome Panel #10
//   *** BODY PANEL OPTIONS ASSUME SECOND MARCDUINO MASTER BOARD ON MEGA ADK SERIAL #3 ***
//    54 = Open All Body Panels
//    55 = Close All Body Panels
//    56 = Open Body Panel #1
//    57 = Close Body Panel #1
//    58 = Open Body Panel #2
//    59 = Close Body Panel #2
//    60 = Open Body Panel #3
//    61 = Close Body Panel #3
//    62 = Open Body Panel #4
//    63 = Close Body Panel #4
//    64 = Open Body Panel #5
//    65 = Close Body Panel #5
//    66 = Open Body Panel #6
//    67 = Close Body Panel #6
//    68 = Open Body Panel #7
//    69 = Close Body Panel #7
//    70 = Open Body Panel #8
//    71 = Close Body Panel #8
//    72 = Open Body Panel #9
//    73 = Close Body Panel #9
//    74 = Open Body Panel #10
//    75 = Close Body Panel #10
//   *** MAGIC PANEL LIGHTING COMMANDS
//    76 = Magic Panel ON
//    77 = Magic Panel OFF
//    78 = Magic Panel Flicker (10 seconds) 
//
// Std MarcDuino Logic Display Functions (For custom functions)
//     1 = Display normal random sequence
//     2 = Short circuit (10 second display sequence)
//     3 = Scream (flashing light display sequence)
//     4 = Leia (34 second light sequence)
//     5 = Display “Star Wars”
//     6 = March light sequence
//     7 = Spectrum, bar graph display sequence
//     8 = Display custom text
//
// Std MarcDuino Panel Functions (For custom functions)
//     1 = Panels stay closed (normal position)
//     2 = Scream sequence, all panels open
//     3 = Wave panel sequence
//     4 = Fast (smirk) back and forth panel sequence
//     5 = Wave 2 panel sequence, open progressively all panels, then close one by one)
//     6 = Marching ants panel sequence
//     7 = Faint / short circuit panel sequence
//     8 = Rhythmic cantina panel sequence
//     9 = Custom Panel Sequence