/*
 *  ControlData.h
 *  ARDroneEngine
 *
 *  Created by Frederic D'HAEYER on 14/01/10.
 *  Copyright 2010 Parrot SA. All rights reserved.
 *
 */
#ifndef _CONTROLDATA_H_
#define _CONTROLDATA_H_
#include "ConstantsAndMacros.h"
#include "wifi.h"

typedef enum _EMERGENCY_STATE_
{
	EMERGENCY_STATE_EMERGENCY,
	EMERGENCY_STATE_RESET
} EMERGENCY_STATE;

typedef struct 
{
	/**
	 * Current bootstrap
	 */
	bool_t bootstrap;
	
	/**
	 * Progressive commands
	 * And accelerometers values transmitted to drone, FALSE otherwise
	 */
	float yaw, gaz, accelero_phi, accelero_theta;
	bool_t accelero_active;
	
	/**
	 * variable to know if setting is needed
	 */
	bool_t needSetFrequency;
	EMERGENCY_STATE	isInEmergency;
	
	bool_t is_client;
	
	bool_t needSetNavdataDemo;
	bool_t needGetConfiguration;
	bool_t needSetPairing;
	char pairing_mac_address[MAXADDRS];
	
	bool_t wifiReachabled;
	
	int framecounter;
	bool_t navdata_demo;
	bool_t needSetEmergency;
	bool_t needSetTakeOff;
	
	int needAnimation;
	int needVideoSwitch;
	int needChangeCameraDetection;
	int needChangeEnemyColor;
	int needChangeEnemyOutdoorShell;
	int needLedAnimation;
	float ledAnimFreq;
	unsigned int ledAnimDuration;
	
	/**
	 * Strings to display in interface
	 */	
	char error_msg[64];
	char takeoff_msg[16];
	char emergency_msg[16];
} ControlData;

void initControlData(void);
void resetControlData(void);
void initNavdataControlData(void);
void checkErrors(void);
void controlfps(void);
void sendControls(void);
void switchTakeOff(void);
void emergency(void);
void switchVideoChannel(int video_channel);
void changeCameraDetection(int camera_type);
void changeEnemyColor(int enemy_color);
void changeEnemyOutdoorShell(int enemy_outdoor);
void changeLedAnimation(int led_anim, float freq, unsigned int duration);
void handleAccelerometers(void);
void disableAccelerometers(void);
void inputYaw(float percent);
void inputGaz(float percent);
void inputPitch(float percent);
void inputRoll(float percent);
void signal_input(int new_input);
void send_inputs(void);

void setNavdataDemo(bool_t navdata_demo);
#endif // _CONTROLDATA_H_
