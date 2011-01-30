//
//  MainViewController.m
//  ARDroneEngine
//
//  Created by Mykonos on 17/12/09.
//  Copyright 2009 Parrot SA. All rights reserved.
//
#import "MainViewController.h"
#import "OpenGLVideo.h"

extern ControlData ctrldata;
extern instance_navdata_t ctrlnavdata;

@interface MainViewController ()
ARDroneNavigationData navigationData;
ARDroneEnemiesData humanEnemiesData;	
ARDroneDetectionCamera detectionCamera;
ARDroneCamera droneCamera;
float originDroneCameraTrans[3];

id<NavdataProtocol>navdata_delegate;
CGRect screenFrame;
BOOL screenOrientationRight;
ARDroneHUDConfiguration hudconfig; 

-(void)TimerHandler;
-(void)update;
@end

@implementation MainViewController
- (id) initWithFrame:(CGRect)frame withState:(BOOL)inGame withDelegate:(id<ARDroneProtocolOut>)delegate withNavdataDelegate:(id<NavdataProtocol>)_navdata_delegate withHUDConfiguration:(ARDroneHUDConfiguration*)hudconfiguration
{
	NSLog(@"Main View Controller Frame : %@", NSStringFromCGRect(frame));
	if(self = [super init])
	{
		bye = FALSE;
		
		screenOrientationRight = YES;
		screenFrame = frame;
		gameEngine = delegate;
		navdata_delegate = _navdata_delegate;
		
		initControlData();
 		
		vp_os_memset(&navigationData, 0x0, sizeof(ARDroneNavigationData)); 
		vp_os_memset(&detectionCamera, 0x0, sizeof(ARDroneCamera));
		vp_os_memset(&droneCamera, 0x0, sizeof(ARDroneCamera)); 
		vp_os_memset(&originDroneCameraTrans, 0x0, sizeof(float) * 3); 
		humanEnemiesData.count = 0;
		
		for(int i = 0 ; i < ARDRONE_MAX_ENEMIES ; i++)
		{
			vp_os_memset(&humanEnemiesData.data[i], 0x0, sizeof(ARDroneEnemyData));
			humanEnemiesData.data[i].width = 1.0;
			humanEnemiesData.data[i].height = 1.0;		
		}

		if(hudconfiguration == nil)
		{
			hudconfig.enableBackToMainMenu = NO;
			hudconfig.enableSwitchScreen = YES;
			hudconfig.enableBatteryPercentage = YES;
		}
		else
		{
			vp_os_memcpy(&hudconfig, hudconfiguration, sizeof(ARDroneHUDConfiguration));
		}
		
		hud = [[HUD alloc] initWithFrame:screenFrame withState:inGame withHUDConfiguration:hudconfig withControlData:&ctrldata];
		[self.view addSubview:hud.view];
		hud.view.multipleTouchEnabled = YES;
		
		menuSettings = [[SettingsMenu alloc] initWithFrame:screenFrame AndHUDConfiguration:hudconfig withDelegate:hud withControlData:&ctrldata];
		menuSettings.view.hidden = YES;
		menuSettings.view.multipleTouchEnabled = YES;
		[self.view addSubview:menuSettings.view];
		self.view.multipleTouchEnabled = YES;
		
		[self changeState:inGame];
		
		[self TimerHandler];
	}
	
	return self;
}

- (void)setWifiReachabled:(BOOL)reachabled
{
	ctrldata.wifiReachabled = reachabled;
}

-(void)setScreenOrientationRight:(BOOL)right
{
	screenOrientationRight = right;
}

- (void) update
{
	static bool_t old_getConfig = FALSE;
	static int previousEnemiesCount = 0;
	
	if(old_getConfig != ctrldata.needGetConfiguration)
	{
		if(!ctrldata.needGetConfiguration)
			[menuSettings configChanged];
		
		old_getConfig = ctrldata.needGetConfiguration;
	}
	
	if(hud.firePressed == YES) 
	{
		[gameEngine executeCommandOut:ARDRONE_COMMAND_FIRE withParameter:nil fromSender:self.view];
	}
	else if(hud.mainMenuPressed == YES)
	{
		hud.mainMenuPressed = NO;
		[gameEngine executeCommandOut:ARDRONE_COMMAND_PAUSE withParameter:nil fromSender:self.view];
	}
	else if(hud.settingsPressed == YES)
	{
		hud.settingsPressed = NO;
		[menuSettings performSelectorOnMainThread:@selector(switchDisplay) withObject:nil waitUntilDone:YES];
	}
	
	// Set velocities	
	navigationData.linearVelocity.x = -ctrlnavdata.vy;
	navigationData.linearVelocity.y = ctrlnavdata.vz;
	navigationData.linearVelocity.z = ctrlnavdata.vx;
	navigationData.angularPosition.x = -ctrlnavdata.pitch;
	navigationData.angularPosition.y = ctrlnavdata.yaw;
	navigationData.angularPosition.z = ctrlnavdata.roll;
	navigationData.navVideoNumFrames = ctrlnavdata.videoNumFrames;
	navigationData.videoNumFrames    = get_video_current_numframes();

	// Set flying state.
	if(navigationData.flyingState != ctrlnavdata.flyingState)
	{
		NSLog(@"Flying state switch to %d", ctrlnavdata.flyingState);
		if(hudconfig.enableBackToMainMenu)
			[hud showBackToMainMenu:!ctrlnavdata.flyingState];
	}
	
	navigationData.flyingState = ctrlnavdata.flyingState;
	navigationData.emergencyState = ctrlnavdata.emergencyLanding;
	navigationData.detection_type = (ARDRONE_CAMERA_DETECTION_TYPE)ctrlnavdata.detectionType;

	// Set detected ARDRONE_ENEMY_HUMAN enemies if detected.
	humanEnemiesData.count = ctrlnavdata.nbDetectedOpponent;
	
	//printf("enemies count : %d\n", humanEnemiesData.count);
	for(int i = 0 ; i < humanEnemiesData.count ; i++)
	{
		humanEnemiesData.data[i].width = 2 * ctrlnavdata.widthOpponent[i] / 1000.0;
		humanEnemiesData.data[i].height = 2 * ctrlnavdata.heightOpponent[i] / 1000.0;		
		humanEnemiesData.data[i].position.x = (2 * ctrlnavdata.xOpponent[i] / 1000.0) - 1.0;
		humanEnemiesData.data[i].position.y = -(2 * ctrlnavdata.yOpponent[i] / 1000.0) + 1.0;
		humanEnemiesData.data[i].position.z = ctrlnavdata.distOpponent[i];
	}
	
	// Set Detection Camera
	vp_os_memcpy(detectionCamera.rotation, ctrlnavdata.detectCameraRot, sizeof(float) * 9);
	vp_os_memcpy(detectionCamera.translation, ctrlnavdata.detectCameraTrans, sizeof(float) * 3);
	detectionCamera.tag_index = ctrlnavdata.detectTagIndex;
	
	// Set Drone Camera rotation
	vp_os_memcpy(droneCamera.rotation, ctrlnavdata.droneCameraRot, sizeof(float) * 9);
	
	// Set Drone Camera translation
	// Get enemies data
	ARDroneEnemiesData AIEnemiesData;
	vp_os_memset(&AIEnemiesData, 0x0, sizeof(ARDroneEnemiesData));
	[gameEngine AIEnemiesData:&AIEnemiesData];		

	if((previousEnemiesCount == 0) && ((humanEnemiesData.count + AIEnemiesData.count) > 0))
		vp_os_memcpy(originDroneCameraTrans, ctrlnavdata.droneCameraTrans, sizeof(float) * 3);

	for(int i = 0 ; i < 3 ; i++)
		droneCamera.translation[i] = ctrlnavdata.droneCameraTrans[i] - originDroneCameraTrans[i];
	
	previousEnemiesCount = humanEnemiesData.count + AIEnemiesData.count;
	
	ctrldata.bootstrap = ctrlnavdata.bootstrap;
	
	// Set battery level in hud view
	[hud setBattery:(int)ctrlnavdata.remainingBattery];
	
	// Set  all texts in Hud view
	if((strlen(ctrldata.error_msg) != 0) && (ctrldata.framecounter >= (kFPS / 2.0)))
		[hud setMessageBox:[[NSBundle mainBundle] localizedStringForKey :[NSString stringWithCString:ctrldata.error_msg encoding:NSUTF8StringEncoding] value:@"" table:@"languages"]];
	else
		[hud setMessageBox:@""];
	
	[hud performSelectorOnMainThread:@selector(setTakeOff:) withObject:[NSString stringWithFormat:@"%s", ctrldata.takeoff_msg] waitUntilDone:YES];
	[hud performSelectorOnMainThread:@selector(setEmergency:) withObject:[NSString stringWithFormat:@"%s", ctrldata.emergency_msg] waitUntilDone:YES];
}

- (void) TimerHandler {
	NSTimeInterval currenttime, timeinterval;
	NSTimeInterval lasttime = [NSDate timeIntervalSinceReferenceDate];
	
	if(self.view.hidden == NO)
	{
		[navdata_delegate parrotNavdata:&ctrlnavdata];
		[self update];
		checkErrors();
	}
	
	currenttime = [NSDate timeIntervalSinceReferenceDate];
	timeinterval = (NSTimeInterval)(1.0 / kFPS) - (currenttime - lasttime);
	
	if(timeinterval < (NSTimeInterval)0)
		timeinterval = (NSTimeInterval)0;
	
/*	static int numsample = 0;
	if(numsample++ > 64)
	{
		numsample = 0;
		NSLog(@"Time Interval : %d ms, diff_time : %d ms\n", (int)((float)timeinterval * 1000), (int)((float)(currenttime - lasttime) * 1000));
	}
*/	
	ctrldata.framecounter = (ctrldata.framecounter + 1) % kFPS;

	[NSTimer scheduledTimerWithTimeInterval:timeinterval target:self selector:@selector(TimerHandler) userInfo:nil repeats:NO];
}

- (ARDroneNavigationData*)navigationData
{
	return &navigationData;
}

- (ARDroneDetectionCamera*)detectionCamera
{
	return &detectionCamera;
}

- (ARDroneCamera*)droneCamera
{
	return &droneCamera;
}

- (ARDroneEnemiesData*)humanEnemiesData
{
	return &humanEnemiesData;
}

- (void)changeState:(BOOL)inGame
{
	// Check whether there is a change of state
	self.view.hidden = !inGame;
	[hud changeState:inGame];
}

- (void)executeCommandIn:(ARDRONE_COMMAND_IN)commandId withParameter:(void*)parameter fromSender:(id)sender
{
	switch (commandId) {
		case ARDRONE_COMMAND_ISCLIENT:
			ctrldata.is_client = (int)parameter;
			ctrldata.needSetFrequency = TRUE;
			break;
			
		case ARDRONE_COMMAND_DRONE_ANIM:
			switch((ARDRONE_ANIMATION)parameter)
			{
				case ARDRONE_ANIMATION_PHI_M30_DEG:
				case ARDRONE_ANIMATION_PHI_30_DEG:
				case ARDRONE_ANIMATION_THETA_M30_DEG:
				case ARDRONE_ANIMATION_THETA_30_DEG:
				case ARDRONE_ANIMATION_THETA_20DEG_YAW_200DEG:
				case ARDRONE_ANIMATION_THETA_20DEG_YAW_M200DEG:
				case ARDRONE_ANIMATION_TURNAROUND:
				case ARDRONE_ANIMATION_TURNAROUND_GODOWN:
				case ARDRONE_ANIMATION_YAW_SHAKE:
					ctrldata.needAnimation = (int)((ARDRONE_ANIMATION)parameter);
					break;
			
				default:
					// Animations didn't implement
					NSLog(@"The ARDRONE_ANIMATION %d didn't implement !", (ARDRONE_ANIMATION)parameter);
					break;
			}
			break;
			
		case ARDRONE_COMMAND_VIDEO_CHANNEL:
			switch((ARDRONE_VIDEO_CHANNEL)parameter)
			{
				case ARDRONE_VIDEO_CHANNEL_HORI:
				case ARDRONE_VIDEO_CHANNEL_VERT:
				case ARDRONE_VIDEO_CHANNEL_VERT_IN_HORI:
				case ARDRONE_VIDEO_CHANNEL_HORI_IN_VERT:
					switchVideoChannel((int)((ARDRONE_VIDEO_CHANNEL)parameter));
					break;
				
				default:
					// Animations didn't implement
					NSLog(@"The ARDRONE_VIDEO_CHANNEL %d didn't implement !", (ARDRONE_VIDEO_CHANNEL)parameter);
					break;
			}
			break;
			
		case ARDRONE_COMMAND_CAMERA_DETECTION:
			switch((ARDRONE_CAMERA_DETECTION_TYPE)parameter)
			{
				case ARDRONE_CAMERA_VISION_DETECT:
				case ARDRONE_CAMERA_NONE:
					changeCameraDetection((int)(ARDRONE_CAMERA_DETECTION_TYPE)parameter);
					break;
					
				default:
					// Animations didn't implement
					NSLog(@"The ARDRONE_CAMERA_DETECTION_TYPE %d didn't implement !", (ARDRONE_CAMERA_DETECTION_TYPE)parameter);
					break;
			}
			break;
			
		case ARDRONE_COMMAND_ENEMY_SET_PARAM:
			switch(((ARDRONE_ENEMY_PARAM*)parameter)->color)
			{
				case ARDRONE_ENEMY_COLOR_ORANGE_BLUE:
				case ARDRONE_ENEMY_COLOR_ORANGE_GREEN:
				case ARDRONE_ENEMY_COLOR_ORANGE_YELLOW:
					changeEnemyColor((int)((ARDRONE_ENEMY_PARAM*)parameter)->color);
					break;
					
				default:
					// Animations didn't implement
					NSLog(@"The ARDRONE_ENEMY_COLOR %d didn't implement !", (int)((ARDRONE_ENEMY_PARAM*)parameter)->color);
					break;
			}
			
			changeEnemyOutdoorShell(((ARDRONE_ENEMY_PARAM*)parameter)->outdoor_shell);
			break;

		case ARDRONE_COMMAND_DRONE_LED_ANIM:
			switch(((ARDRONE_LED_ANIMATION_PARAM*)parameter)->led_anim)
			{
				case ARDRONE_LED_ANIMATION_BLINK_GREEN_RED:
				case ARDRONE_LED_ANIMATION_BLINK_RED:
				case ARDRONE_LED_ANIMATION_SNAKE_GREEN_RED:
				case ARDRONE_LED_ANIMATION_FIRE:
				case ARDRONE_LED_ANIMATION_STANDARD:
				case ARDRONE_LED_ANIMATION_RED:
				case ARDRONE_LED_ANIMATION_GREEN:
				case ARDRONE_LED_ANIMATION_RED_SNAKE:
				case ARDRONE_LED_ANIMATION_BLANK:
					changeLedAnimation((int)((ARDRONE_LED_ANIMATION_PARAM*)parameter)->led_anim, ((ARDRONE_LED_ANIMATION_PARAM*)parameter)->frequency, ((ARDRONE_LED_ANIMATION_PARAM*)parameter)->duration);
					break;
			
				default:
					// Animations didn't implement
					NSLog(@"The ARDRONE_LED_ANIMATION %d didn't implement !", ((ARDRONE_LED_ANIMATION_PARAM*)parameter)->led_anim);
					break;
			}
			break;

		default:
			// Commands didn't implement
			NSLog(@"The ARDRONE_COMMAND_IN %d didn't implement !");
			break;
	}
}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
	return (toInterfaceOrientation == UIInterfaceOrientationLandscapeRight || toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft);
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}

 
- (void)dealloc {
	[self changeState:NO];
	bye = TRUE;
	
	[hud release];
	[menuSettings release];
	
	[super dealloc];
}

@end
