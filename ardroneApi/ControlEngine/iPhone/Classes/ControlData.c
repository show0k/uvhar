/*
 *  ControlData.m
 *  ARDroneEngine
 *
 *  Created by Frederic D'HAEYER on 14/01/10.
 *  Copyright 2010 Parrot SA. All rights reserved.
 *
 */
#include "ConstantsAndMacros.h"
#include "ControlData.h"

//#define DEBUG_CONTROL

ControlData ctrldata = { 0 };
instance_navdata_t ctrlnavdata;
extern instance_navdata_t inst_nav; 
extern char iphone_mac_address[];

void initControlData(void)
{
	ctrldata.framecounter = 0;
	ctrldata.needSetFrequency = TRUE;
	setNavdataDemo(TRUE);
	ctrldata.needGetConfiguration = TRUE;
	strcpy(ctrldata.pairing_mac_address, NULL_MAC);
	ctrldata.needSetPairing = FALSE;
	ctrldata.needSetEmergency = FALSE;
	ctrldata.needSetTakeOff = FALSE;
	ctrldata.isInEmergency = FALSE;
	ctrldata.is_client = FALSE;

	ctrldata.needAnimation = -1;
	ctrldata.needVideoSwitch = -1;
	ctrldata.needChangeCameraDetection = -1;
	ctrldata.needChangeEnemyColor = -1;
	ctrldata.needChangeEnemyOutdoorShell = -1;
	
	ctrldata.needLedAnimation = -1;
	ctrldata.ledAnimFreq = 0.0f;
	ctrldata.ledAnimDuration = 0;
	
	ctrldata.wifiReachabled = FALSE;
	
	strcpy(ctrldata.error_msg, "");
	strcpy(ctrldata.takeoff_msg, "take_off");
	strcpy(ctrldata.emergency_msg, "emergency");
	
	initNavdataControlData();
	resetControlData();
	ardrone_tool_start_reset();
}

void initNavdataControlData(void)
{
	//drone data
	ardrone_navdata_reset_data(&ctrlnavdata);
}

void resetControlData(void)
{
	//printf("reset control data\n");
	ctrldata.accelero_active = FALSE;
	inputPitch(0.0);
	inputRoll(0.0);
	inputYaw(0.0);
	inputGaz(0.0);
	ctrldata.bootstrap = TRUE;
	initNavdataControlData();
}

void setNavdataDemo(bool_t navdata_demo)
{
	ctrldata.navdata_demo = navdata_demo;
	ctrldata.needSetNavdataDemo = TRUE;
}

void switchTakeOff(void)
{
#ifdef DEBUG_CONTROL
	PRINT("%s\n", __FUNCTION__);
#endif		
	ctrldata.needSetTakeOff = TRUE;
}

void emergency(void)
{
#ifdef DEBUG_CONTROL
	PRINT("%s\n", __FUNCTION__);
#endif
	ctrldata.needSetEmergency = TRUE;
}

void switchVideoChannel(int videoChannel)
{
#ifdef DEBUG_CONTROL
	PRINT("%s\n", __FUNCTION__);
#endif
	ctrldata.needVideoSwitch = videoChannel;
}

void changeCameraDetection(int camera_type)
{
#ifdef DEBUG_CONTROL
	PRINT("%s\n", __FUNCTION__);
#endif
	ctrldata.needChangeCameraDetection = camera_type;
}

void changeEnemyColor(int enemy_color)
{
#ifdef DEBUG_CONTROL
	PRINT("%s\n", __FUNCTION__);
#endif
	ctrldata.needChangeEnemyColor = enemy_color;
}

void changeEnemyOutdoorShell(int enemy_outdoor)
{
#ifdef DEBUG_CONTROL
	PRINT("%s\n", __FUNCTION__);
#endif
	ctrldata.needChangeEnemyOutdoorShell = enemy_outdoor;
}

void changeLedAnimation(int led_anim, float freq, unsigned int duration)
{
	ctrldata.needLedAnimation = led_anim;
	ctrldata.ledAnimFreq = freq;
	ctrldata.ledAnimDuration = duration;
}

void inputYaw(float percent)
{
#ifdef DEBUG_CONTROL
	PRINT("%s : %f\n", __FUNCTION__, percent);
#endif
	if(-1.0 <= percent && percent <= 1.0)
		ctrldata.yaw = percent;
	else if(-1.0 < percent)
		ctrldata.yaw = -1.0;
	else
		ctrldata.yaw = 1.0;
}

void inputGaz(float percent)
{
#ifdef DEBUG_CONTROL
	PRINT("%s : %f\n", __FUNCTION__, percent);
#endif
	if(-1.0 <= percent && percent <= 1.0)
		ctrldata.gaz = percent;
	else if(-1.0 < percent)
		ctrldata.gaz = -1.0;
	else
		ctrldata.gaz = 1.0;
}

void inputPitch(float percent)
{
#ifdef DEBUG_CONTROL
	PRINT("%s : %f, accelero_enable : %d\n", __FUNCTION__, percent, ctrldata.accelero_active);
#endif
	if(-1.0 <= percent && percent <= 1.0)
		ctrldata.accelero_theta = -percent;
	else if(-1.0 < percent)
		ctrldata.accelero_theta = 1.0;
	else
		ctrldata.accelero_theta = -1.0;
}

void inputRoll(float percent)
{
#ifdef DEBUG_CONTROL
	PRINT("%s : %f, accelero_enable : %d\n", __FUNCTION__, percent, ctrldata.accelero_active);
#endif
	if(-1.0 <= percent && percent <= 1.0)
		ctrldata.accelero_phi = percent;
	else if(-1.0 < percent)
		ctrldata.accelero_phi = -1.0;
	else
		ctrldata.accelero_phi = 1.0;
}

void sendControls(void)
{
	ardrone_at_set_progress_cmd(ctrldata.accelero_active, ctrldata.accelero_phi, ctrldata.accelero_theta, ctrldata.gaz, ctrldata.yaw);
}

void checkErrors(void)
{
	input_state_t* input_state = ardrone_tool_get_input_state();
	
	strcpy(ctrldata.error_msg, "");
	
	if(!ctrldata.wifiReachabled)
	{
		strcpy(ctrldata.error_msg, "WIFI NOT REACHABLE");
		resetControlData();
	}
	else
	{
		if(ctrldata.needAnimation >= 0)
		{
			ardrone_at_set_anim(ctrldata.needAnimation, MAYDAY_TIMEOUT[ctrldata.needAnimation]);
			ctrldata.needAnimation = -1;
		}
		
		if(ctrldata.needVideoSwitch >= 0)
		{
			ardrone_at_zap(ctrldata.needVideoSwitch);
			ctrldata.needVideoSwitch = -1;
		}
		
		if(ctrldata.needChangeEnemyColor >= 0)
		{
			ARDRONE_CONFIGURATION_SET(enemy_colors, ctrldata.needChangeEnemyColor);
			ctrldata.needChangeEnemyColor = -1;
		}	
		
		if(ctrldata.needChangeEnemyOutdoorShell >= 0)
		{
			ARDRONE_CONFIGURATION_SET(enemy_without_shell, ctrldata.needChangeEnemyOutdoorShell);
			ctrldata.needChangeEnemyOutdoorShell = -1;
		}
		
		if(ctrldata.needLedAnimation >= 0)
		{
			ardrone_at_set_led_animation(ctrldata.needLedAnimation, ctrldata.ledAnimFreq, ctrldata.ledAnimDuration);
			ctrldata.needLedAnimation = -1;
		}	
		
		if(ctrldata.needSetTakeOff)
		{
			if(ctrlnavdata.emergencyLanding)
			{
				ctrldata.needSetEmergency = TRUE;
			}
			else
			{
				printf("Take off ...\n");
				if(!ctrlnavdata.startPressed)
					ardrone_tool_set_ui_pad_start(1);
				else
					ardrone_tool_set_ui_pad_start(0);
				ctrldata.needSetTakeOff = FALSE;
			}
		}
		
		if(ctrldata.needSetEmergency)
		{
			ctrldata.isInEmergency = ctrlnavdata.emergencyLanding;
			ardrone_tool_set_ui_pad_select(1);
			ctrldata.needSetEmergency = FALSE;
		}
		
		if( ctrldata.needSetNavdataDemo)
		{
			if( control_ack_configure_navdata_demo(ctrldata.navdata_demo))
				ctrldata.needSetNavdataDemo = FALSE;
		}
		else if( ctrldata.needSetPairing )
		{
			if(control_ack_configure_mac_address(ctrldata.pairing_mac_address))
				ctrldata.needSetPairing = FALSE;
		}
		else if(ctrldata.needSetFrequency)
		{
			if(control_ack_configure_ultrasound_frequency(!ctrldata.is_client))
				ctrldata.needSetFrequency = FALSE;
		}
		else if(ctrldata.needChangeCameraDetection >= 0)
		{
			if(control_ack_configure_detect_type((CAD_TYPE)ctrldata.needChangeCameraDetection))
				ctrldata.needChangeCameraDetection = -1;
		}	
		else if(ctrldata.needGetConfiguration)
		{
			//PRINT("Request configuration file\n");
			if(control_get_configuration())
				ctrldata.needGetConfiguration = FALSE;
		}
		
		if((ctrldata.framecounter % (kFPS / 2)) == 0)
		{
			if(ctrlnavdata.bootstrap)
			{
				setNavdataDemo(TRUE);
				ctrldata.needSetFrequency = TRUE;
				ctrldata.needGetConfiguration = TRUE;
			}
		}
		
		if(ardrone_navdata_client_get_num_retries() >= NAVDATA_MAX_RETRIES)
		{
			strcpy(ctrldata.error_msg, "CONTROL LINK NOT AVAILABLE");
			resetControlData();
		}
		else if(ctrlnavdata.emergencyLanding)
		{
			if(!ctrldata.isInEmergency && input_state->select)
				ardrone_tool_set_ui_pad_select(0);
			
			if(ctrlnavdata.cutoutProblem)
			{
				strcpy(ctrldata.error_msg, "CUT OUT EMERGENCY");
			}
			else if(ctrlnavdata.motorsProblem)
			{
				strcpy(ctrldata.error_msg, "MOTORS EMERGENCY");
			}
			else if(ctrlnavdata.cameraProblem)
			{
				strcpy(ctrldata.error_msg, "CAMERA EMERGENCY");
			}
			else if(ctrlnavdata.adcProblem)
			{
				strcpy(ctrldata.error_msg, "PIC WATCHDOG EMERGENCY");
			}
			else if(ctrlnavdata.adcVersionProblem)
			{
				strcpy(ctrldata.error_msg, "PIC VERSION EMERGENCY");
			}
			else if(ctrlnavdata.anglesProblem)
			{
				strcpy(ctrldata.error_msg, "TOO MUCH ANGLE EMERGENCY");
			}
			else if(ctrlnavdata.vbatLowProblem)
			{
				strcpy(ctrldata.error_msg, "BATTERY LOW EMERGENCY");
			}
			else if(ctrlnavdata.userEmergency)
			{
				strcpy(ctrldata.error_msg, "USER EMERGENCY");
			}
			else if(ctrlnavdata.ultrasoundProblem)
			{
				strcpy(ctrldata.error_msg, "ULTRASOUND EMERGENCY");
			}
			else
			{
				strcpy(ctrldata.error_msg, "UNKNOWN EMERGENCY");
			}
						
			strcpy(ctrldata.emergency_msg, "");
			strcpy(ctrldata.takeoff_msg, "take_off");
			
			resetControlData();
			ardrone_tool_start_reset();
		}
		else if(!ctrlnavdata.emergencyLanding)
		{	
			if(ctrldata.isInEmergency && input_state->select)
			{
				ardrone_tool_set_ui_pad_select(0);
			}
			
			if(video_stage_get_num_retries() > VIDEO_MAX_RETRIES)
			{
				strcpy(ctrldata.error_msg, "VIDEO CONNECTION ALERT");
			}
			else if(ctrlnavdata.vbatLowProblem)
			{
				strcpy(ctrldata.error_msg, "BATTERY LOW ALERT");
			}
			else if(ctrlnavdata.ultrasoundProblem)
			{
				strcpy(ctrldata.error_msg, "ULTRASOUND ALERT");
			}
			else if(ctrlnavdata.visionProblem && ctrlnavdata.flyingState)
			{
				strcpy(ctrldata.error_msg, "VISION ALERT");
			}

			if(!ctrlnavdata.timerElapsed)
				strcpy(ctrldata.emergency_msg, "emergency");
			
			if(input_state->start)
			{
				if(ctrlnavdata.startPressed)
				{
					strcpy(ctrldata.takeoff_msg, "take_land");
				}
				else
				{	
					strcpy(ctrldata.takeoff_msg, "take_off");
					strcpy(ctrldata.error_msg, "START NOT RECEIVED");
					
				}
			}
			else
			{
				strcpy(ctrldata.takeoff_msg, "take_off");
			}			
		}
	}
}

