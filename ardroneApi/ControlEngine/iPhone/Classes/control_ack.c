//
//  control_ack.m
//  iPhoneDemo
//
//  Created by Mykonos on 20/10/08.
//  Copyright 2008 Parrot SA. All rights reserved.
//
#include "control_ack.h"
#include <config.h>

#undef ARDRONE_CONFIG_KEY_IMM
#undef ARDRONE_CONFIG_KEY_REF
#undef ARDRONE_CONFIG_KEY_STR
#define ARDRONE_CONFIG_KEY_IMM(KEY, NAME, INI_TYPE, C_TYPE, C_TYPE_PTR, RW, DEFAULT, CALLBACK)
#define ARDRONE_CONFIG_KEY_REF(KEY, NAME, INI_TYPE, C_TYPE, C_TYPE_PTR, RW, DEFAULT, CALLBACK)
#define ARDRONE_CONFIG_KEY_STR(KEY, NAME, INI_TYPE, C_TYPE, C_TYPE_PTR, RW, DEFAULT, CALLBACK)
#include <config_keys.h> // must be included before to have types

static ardrone_control_ack_event_t control_ack_event = { 0 };
static bool_t control_ack_is_init = FALSE;

void control_ack_init(void)
{
	if(!control_ack_is_init)
	{
		control_ack_event.status = ARDRONE_CONTROL_EVENT_IDLE;
		control_ack_is_init = TRUE;
	}
}

static void control_ack_event_configure_start(struct _ardrone_control_event_t* event)
{
	//printf("%s\n", __FUNCTION__);
}

static void control_ack_event_configure_end(struct _ardrone_control_event_t* event)
{
	switch(event->status)
	{
		case ARDRONE_CONTROL_EVENT_FINISH_SUCCESS:
			
			break;
			
		case ARDRONE_CONTROL_EVENT_FINISH_FAILURE:
			break;
			
		default:
			// Nothing to do
			break;
	}
}

static void control_ack_event_configure(void)
{
	control_ack_event.event			= ACK_CONTROL_MODE;
	control_ack_event.num_retries	= 5;
	control_ack_event.status		= ARDRONE_CONTROL_EVENT_WAITING;

	control_ack_event.ardrone_control_event_start = control_ack_event_configure_start;
	control_ack_event.ardrone_control_event_end	  = control_ack_event_configure_end;
	
	control_ack_event.ack_state = ACK_COMMAND_MASK_TRUE;
	
	ardrone_control_send_event( (ardrone_control_event_t*)&control_ack_event );
}

bool_t control_ack_configure_navdata_demo(bool_t enable)
{
	bool_t b = FALSE;
	if(control_ack_is_init)
	{
		switch(control_ack_event.status)
		{
			case ARDRONE_CONTROL_EVENT_IDLE:
				printf("control_ack_event navdata demo configure \n");
				control_ack_event_configure();
				ARDRONE_CONFIGURATION_SET(navdata_demo, enable);
				break;

			case ARDRONE_CONTROL_EVENT_FINISH_FAILURE:
				control_ack_event.status = ARDRONE_CONTROL_EVENT_IDLE;
				break;
				
			case ARDRONE_CONTROL_EVENT_FINISH_SUCCESS:
				if(enable)
				{
					ardrone_navdata_write_to_file(FALSE);
					printf("[NavdataDemo] enabled\n");
				}
				else
				{
					ardrone_navdata_write_to_file(TRUE);
					printf("[NavdataDemo] disabled\n");
				}
				control_ack_event.status = ARDRONE_CONTROL_EVENT_IDLE;
				b = TRUE;
				break;
	
			default:
				// Nothing to do
				break;
		}
	}
	
	return b;
}

bool_t control_ack_configure_mac_address(const char *mac_address)
{
	bool_t b = FALSE;
	if(control_ack_is_init)
	{
		switch(control_ack_event.status)
		{
			case ARDRONE_CONTROL_EVENT_IDLE:
				printf("control_ack_event mac address configure \n");
				control_ack_event_configure();
				ARDRONE_CONFIGURATION_SET(owner_mac, (char*)mac_address);
				break;
				
			case ARDRONE_CONTROL_EVENT_FINISH_FAILURE:
				control_ack_event.status = ARDRONE_CONTROL_EVENT_IDLE;
				break;
				
			case ARDRONE_CONTROL_EVENT_FINISH_SUCCESS:
				printf("[Pairing] done\n");
				control_ack_event.status = ARDRONE_CONTROL_EVENT_IDLE;
				b = TRUE;
				break;
				
			default:
				// Nothing to do
				break;
		}
	} 

	return b;
}

bool_t control_ack_configure_detect_type(CAD_TYPE type)
{
	bool_t b = FALSE;
	if(control_ack_is_init)
	{
		switch(control_ack_event.status)
		{
			case ARDRONE_CONTROL_EVENT_IDLE:
				printf("control_ack_event detect type configure \n");
				control_ack_event_configure();
				ARDRONE_CONFIGURATION_SET(detect_type, type);
				break;
				
			case ARDRONE_CONTROL_EVENT_FINISH_FAILURE:
				control_ack_event.status = ARDRONE_CONTROL_EVENT_IDLE;
				break;
				
			case ARDRONE_CONTROL_EVENT_FINISH_SUCCESS:
				control_ack_event.status = ARDRONE_CONTROL_EVENT_IDLE;
				printf("[detect type] done");
				b = TRUE;
				break;
				
			default:
				// Nothing to do
				break;
		}
	}
	
	return b;
}

bool_t control_ack_configure_ultrasound_frequency(bool_t isServer)
{
	bool_t b = FALSE;
	if(control_ack_is_init)
	{
		switch(control_ack_event.status)
		{
			case ARDRONE_CONTROL_EVENT_IDLE:
				printf("control_ack_event us frequency configure \n");
				control_ack_event_configure();
				ARDRONE_CONFIGURATION_SET(ultrasound_freq, (isServer ? ADC_CMD_SELECT_ULTRASOUND_25Hz : ADC_CMD_SELECT_ULTRASOUND_22Hz) );			
				break;
				
			case ARDRONE_CONTROL_EVENT_FINISH_FAILURE:
				control_ack_event.status = ARDRONE_CONTROL_EVENT_IDLE;
				break;
				
			case ARDRONE_CONTROL_EVENT_FINISH_SUCCESS:
				control_ack_event.status = ARDRONE_CONTROL_EVENT_IDLE;
				printf("[US] done\n");
				b = TRUE;
				break;
				
			default:
				// Nothing to do
				break;
		}
	}
	
	return b;
}
