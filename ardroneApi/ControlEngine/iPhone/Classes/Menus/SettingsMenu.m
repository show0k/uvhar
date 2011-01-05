#import "SettingsMenu.h"
#ifdef INTERFACE_WITH_DEBUG
#import "TVOut.h"
#endif
#define REFRESH_TRIM_TIMEOUT	1
#define ALTITUDE_LIMITED		3000
#define NO_ALTITUDE				10000

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	void saveSettings(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

extern char iphone_mac_address[];

struct tm *settings_atm = NULL;

void saveSettings(void)
{
	struct timeval tv;
	// Save backups of Settings in text
	gettimeofday(&tv,NULL);
	settings_atm = localtime(&tv.tv_sec);
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); //creates paths so that you can pull the app's path from it
	NSString *documentsDirectory = [paths objectAtIndex:0]; // gets the applications directory on the users iPhone

	const char *filename = [[documentsDirectory stringByAppendingFormat:@"/settings_%04d%02d%02d_%02d%02d%02d.txt",
					   settings_atm->tm_year+1900, settings_atm->tm_mon+1, settings_atm->tm_mday,
					   settings_atm->tm_hour, settings_atm->tm_min, settings_atm->tm_sec] cStringUsingEncoding:NSUTF8StringEncoding];
	FILE *file = fopen(filename, "w");
	if(file)
	{
		fprintf(file, "iPhone section\n");
		fprintf(file, "Accelero disabled  : %s\n", ((ardrone_control_config.control_level >> CONTROL_LEVEL_ACCELERO_DISABLED) & 0x1) ? "ON" : "OFF");
		fprintf(file, "Combined yaw       : %s\n", ((ardrone_control_config.control_level >> CONTROL_LEVEL_COMBINED_YAW) & 0x1) ? "ON" : "OFF");
		fprintf(file, "Left - Handed      : %s\n", ((ardrone_control_config.control_level >> CONTROL_LEVEL_CONTROL_MODE) & (CONTROL_MODE_MAX - 1)) == CONTROL_MODE2 ? "ON" : "OFF");
		fprintf(file, "Iphone Tilt        : %0.2f\n", ardrone_control_config.control_iphone_tilt * RAD_TO_DEG);
		fprintf(file, "\n");
		fprintf(file, "AR.Drone section\n");
		fprintf(file, "Pairing            : %s\n", strcmp(ardrone_control_config.owner_mac, NULL_MAC) != 0 ? "ON" : "OFF");
		fprintf(file, "Drone Network SSID : %s\n", ardrone_control_config.ssid_single_player);
		fprintf(file, "Altitude Limited   : %s\n", (ardrone_control_config.altitude_max == ALTITUDE_LIMITED) ? "ON" : "OFF");
		fprintf(file, "Outdoor Shell      : %s\n", ardrone_control_config.flight_without_shell ? "ON" : "OFF");
		fprintf(file, "Outdoor Flight     : %s\n", ardrone_control_config.outdoor ? "ON" : "OFF");
		fprintf(file, "Yaw Speed          : %0.2f\n", ardrone_control_config.control_yaw * RAD_TO_DEG);				
		fprintf(file, "Vertical Speed     : %0.2f\n", (float)ardrone_control_config.control_vz_max);
		fprintf(file, "Drone Tilt         : %0.2f\n", ardrone_control_config.euler_angle_max * RAD_TO_DEG);
		fclose(file);
	}
	else 
	{
		NSLog(@"%s not found", filename);
	}		
}

@interface SettingsMenu ()
id <SettingsMenuDelegate> _delegate;
ControlData *controlData;
CONTROL_MODE controlMode;
#ifdef INTERFACE_WITH_DEBUG
TVOut *tvout;
#endif
BOOL ssidChangeInProgress;

- (void)refresh;
- (void)refreshTimeout;
- (BOOL)textFieldShouldReturn:(UITextField *)theTextField;
@end

@implementation SettingsMenu
- (id)initWithFrame:(CGRect)frame AndHUDConfiguration:(ARDroneHUDConfiguration)configuration withDelegate:(id <SettingsMenuDelegate>)delegate withControlData:(ControlData*)data
{
	NSLog(@"SettingsMenu frame => w : %f, h : %f", frame.size.width, frame.size.height);

#ifdef INTERFACE_WITH_DEBUG
	if(self = [super initWithNibName:@"SettingsMenuDebug" bundle:nil])
#else
	if(self = [super initWithNibName:@"SettingsMenu" bundle:nil])
#endif
	{
		controlData = data;
		ssidChangeInProgress = NO;
		_delegate = delegate;
		// Set parameters of scrollview
		UIScrollView *scrollView = (UIScrollView*)self.view;
		scrollView.alwaysBounceHorizontal = NO;
		scrollView.alwaysBounceVertical = YES;
		scrollView.clipsToBounds = YES;
		scrollView.contentSize = CGSizeMake(scrollView.frame.size.width, scrollView.frame.size.height);
		scrollView.frame = CGRectMake(frame.origin.x, frame.origin.y, frame.size.height, frame.size.width);		
		scrollView.delegate = self;
		scrollView.scrollEnabled = YES;
		scrollView.indicatorStyle = UIScrollViewIndicatorStyleWhite;
		
		yawSpeedMinLabel.text = [NSString stringWithFormat:@"%0.2f", yawSpeedSlider.minimumValue];
		yawSpeedMaxLabel.text = [NSString stringWithFormat:@"%0.2f", yawSpeedSlider.maximumValue];		
		verticalSpeedMinLabel.text = [NSString stringWithFormat:@"%0.2f", verticalSpeedSlider.minimumValue];
		verticalSpeedMaxLabel.text = [NSString stringWithFormat:@"%0.2f", verticalSpeedSlider.maximumValue];
		droneTiltMinLabel.text = [NSString stringWithFormat:@"%0.2f", droneTiltSlider.minimumValue];
		droneTiltMaxLabel.text = [NSString stringWithFormat:@"%0.2f", droneTiltSlider.maximumValue];		
		iPhoneTiltMinLabel.text = [NSString stringWithFormat:@"%0.2f", iPhoneTiltSlider.minimumValue];
		iPhoneTiltMaxLabel.text = [NSString stringWithFormat:@"%0.2f", iPhoneTiltSlider.maximumValue];		
		droneTrimRollMinLabel.text = [NSString stringWithFormat:@"%0.2f", droneTrimRollSlider.minimumValue];
		droneTrimRollMaxLabel.text = [NSString stringWithFormat:@"%0.2f", droneTrimRollSlider.maximumValue];		
		droneTrimPitchMinLabel.text = [NSString stringWithFormat:@"%0.2f", droneTrimPitchSlider.minimumValue];
		droneTrimPitchMaxLabel.text = [NSString stringWithFormat:@"%0.2f", droneTrimPitchSlider.maximumValue];		
		
		softwareVersion.text	= [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];

		acceleroDisabledSwitch.on = NO;
		leftHandedSwitch.on = NO;
		controlMode = CONTROL_MODE3;
		
		[_delegate acceleroValueChanged:(acceleroDisabledSwitch.on == NO)];
		[_delegate controlModeChanged:controlMode];
		
#ifdef INTERFACE_WITH_DEBUG
		// Create TV instance
		tvoutSwitch.on = NO;
		tvout = [[TVOut alloc] init];

		logsSwitch.on = NO;
#endif		
		
		[self refresh];
		
		[NSTimer scheduledTimerWithTimeInterval:REFRESH_TRIM_TIMEOUT target:self selector:@selector(refreshTimeout) userInfo:nil repeats:YES];
	}
	
	return self;
}

- (void)refreshTimeout
{
	BOOL enabled = (controlData->bootstrap) ? NO : YES; 
	float alpha = (controlData->bootstrap) ? 0.5 : 1.0;
	
#ifdef INTERFACE_WITH_DEBUG
	logsSwitch.enabled = enabled;
	logsSwitch.alpha = alpha;
#endif	

	acceleroDisabledSwitch.enabled = leftHandedSwitch.enabled = enabled;
	acceleroDisabledSwitch.alpha = leftHandedSwitch.alpha = alpha;
	pairingSwitch.enabled = enabled;
	pairingSwitch.alpha = alpha;
	altituteLimitedSwitch.enabled = outdoorFlightSwitch.enabled = outdoorShellSwitch.enabled =  enabled;
	altituteLimitedSwitch.alpha = outdoorFlightSwitch.alpha = outdoorShellSwitch.alpha = alpha;
	droneSSIDTextField.enabled = enabled;
	droneSSIDTextField.alpha = alpha;
	droneTiltSlider.enabled = iPhoneTiltSlider.enabled = verticalSpeedSlider.enabled = yawSpeedSlider.enabled = enabled;
	droneTiltSlider.alpha = iPhoneTiltSlider.alpha = verticalSpeedSlider.alpha = yawSpeedSlider.alpha = alpha;
	clearButton0.enabled = clearButton1.enabled = enabled;
	clearButton0.alpha = clearButton1.alpha = alpha;
	
	flatTrimButton0.enabled = flatTrimButton1.enabled = enabled;
	flatTrimButton0.alpha = flatTrimButton1.alpha = (enabled ? 1.0 : 0.5);

	[self refresh];
}

- (void)configChanged
{
	acceleroDisabledSwitch.on = ((ardrone_control_config.control_level >> CONTROL_LEVEL_ACCELERO_DISABLED) & 0x1);
	controlMode = ((ardrone_control_config.control_level >> CONTROL_LEVEL_CONTROL_MODE) & (CONTROL_MODE_MAX - 1));
	leftHandedSwitch.on = ((controlMode == CONTROL_MODE2) ? YES : NO);	
	altituteLimitedSwitch.on = (ardrone_control_config.altitude_max == ALTITUDE_LIMITED) ? YES : NO;	
	[_delegate acceleroValueChanged:acceleroDisabledSwitch.on == NO];
	[_delegate controlModeChanged:controlMode];
	
	pairingSwitch.on = strcmp(ardrone_control_config.owner_mac, NULL_MAC) != 0 ? YES : NO;
	outdoorFlightSwitch.on = ardrone_control_config.outdoor ? YES : NO;
	outdoorShellSwitch.on = ardrone_control_config.flight_without_shell ? YES : NO;
	droneTiltSlider.value = ardrone_control_config.euler_angle_max * RAD_TO_DEG;
	iPhoneTiltSlider.value = ardrone_control_config.control_iphone_tilt * RAD_TO_DEG;
	verticalSpeedSlider.value = ardrone_control_config.control_vz_max;
	yawSpeedSlider.value = ardrone_control_config.control_yaw * RAD_TO_DEG;
	
	// Update SSID AR.Drone network
	if(!ssidChangeInProgress && (strcmp(ardrone_control_config.ssid_single_player, "") != 0))
		droneSSIDTextField.text = [NSString stringWithCString:ardrone_control_config.ssid_single_player encoding:NSUTF8StringEncoding];
	
	// Update pic version number
	if(ardrone_control_config.pic_version != 0)
	{
		dronePicHardVersion.text = [NSString stringWithFormat:@"%.1f", (float)(ardrone_control_config.pic_version >> 24)];
		dronePicSoftVersion.text = [NSString stringWithFormat:@"%d.%d", (int)((ardrone_control_config.pic_version & 0xFFFFFF) >> 16),(int)(ardrone_control_config.pic_version & 0xFFFF)];
	}
	else
	{
		dronePicHardVersion.text = @"none";
		dronePicSoftVersion.text = @"none";
	}
	
	// update AR.Drone software version 
	if(strcmp(ardrone_control_config.num_version_soft, "") != 0)
		droneSoftVersion.text = [NSString stringWithFormat:@"%s", ardrone_control_config.num_version_soft];
	else
		droneSoftVersion.text = @"none";
	
	// update AR.Drone hardware version 
	if(ardrone_control_config.num_version_mb != 0)
		droneHardVersion.text = [NSString stringWithFormat:@"%x.0", ardrone_control_config.num_version_mb];
	else
		droneHardVersion.text = @"none";

	// Update motor 1 version (soft / hard / supplier)
	if(strcmp(ardrone_control_config.motor1_soft, "") != 0)
		droneMotor1SoftVersion.text = [NSString stringWithFormat:@"%s", ardrone_control_config.motor1_soft];
	else
		droneMotor1SoftVersion.text = [NSString stringWithString:@"none"];

	if(strcmp(ardrone_control_config.motor1_hard, "") != 0)
		droneMotor1HardVersion.text = [NSString stringWithFormat:@"%s", ardrone_control_config.motor1_hard];
	else
		droneMotor1HardVersion.text = [NSString stringWithString:@"none"];
	
	if(strcmp(ardrone_control_config.motor1_supplier, "") != 0)
		droneMotor1SupplierVersion.text = [NSString stringWithFormat:@"%s", ardrone_control_config.motor1_supplier];
	else
		droneMotor1SupplierVersion.text = [NSString stringWithString:@"none"];
	
	// Update motor 2 version (soft / hard / supplier)
	if(strcmp(ardrone_control_config.motor2_soft, "") != 0)
		droneMotor2SoftVersion.text = [NSString stringWithFormat:@"%s", ardrone_control_config.motor2_soft];
	else
		droneMotor2SoftVersion.text = [NSString stringWithString:@"none"];
	
	if(strcmp(ardrone_control_config.motor2_hard, "") != 0)
		droneMotor2HardVersion.text = [NSString stringWithFormat:@"%s", ardrone_control_config.motor2_hard];
	else
		droneMotor2HardVersion.text = [NSString stringWithString:@"none"];
	
	if(strcmp(ardrone_control_config.motor2_supplier, "") != 0)
		droneMotor2SupplierVersion.text = [NSString stringWithFormat:@"%s", ardrone_control_config.motor2_supplier];
	else
		droneMotor2SupplierVersion.text = [NSString stringWithString:@"none"];
	
	// Update motor 3 version (soft / hard / supplier)
	if(strcmp(ardrone_control_config.motor3_soft, "") != 0)
		droneMotor3SoftVersion.text = [NSString stringWithFormat:@"%s", ardrone_control_config.motor3_soft];
	else
		droneMotor3SoftVersion.text = [NSString stringWithString:@"none"];
	
	if(strcmp(ardrone_control_config.motor3_hard, "") != 0)
		droneMotor3HardVersion.text = [NSString stringWithFormat:@"%s", ardrone_control_config.motor3_hard];
	else
		droneMotor3HardVersion.text = [NSString stringWithString:@"none"];
	
	if(strcmp(ardrone_control_config.motor3_supplier, "") != 0)
		droneMotor3SupplierVersion.text = [NSString stringWithFormat:@"%s", ardrone_control_config.motor3_supplier];
	else
		droneMotor3SupplierVersion.text = [NSString stringWithString:@"none"];
	
	// Update motor 4 version (soft / hard / supplier)
	if(strcmp(ardrone_control_config.motor4_soft, "") != 0)
		droneMotor4SoftVersion.text = [NSString stringWithFormat:@"%s", ardrone_control_config.motor4_soft];
	else
		droneMotor4SoftVersion.text = [NSString stringWithString:@"none"];
	
	if(strcmp(ardrone_control_config.motor4_hard, "") != 0)
		droneMotor4HardVersion.text = [NSString stringWithFormat:@"%s", ardrone_control_config.motor4_hard];
	else
		droneMotor4HardVersion.text = [NSString stringWithString:@"none"];
	
	if(strcmp(ardrone_control_config.motor4_supplier, "") != 0)
		droneMotor4SupplierVersion.text = [NSString stringWithFormat:@"%s", ardrone_control_config.motor4_supplier];
	else
		droneMotor4SupplierVersion.text = [NSString stringWithString:@"none"];

	[self refresh];
}

- (void)refresh
{
#ifdef INTERFACE_WITH_DEBUG
	if(logsSwitch.on == YES)
	{
		clearLogsButton.enabled = NO;
		clearLogsButton.alpha = 0.50;
	}
	else 
	{
		clearLogsButton.enabled = YES;
		clearLogsButton.alpha = 1.0;
	}
#endif
	
	yawSpeedCurrentLabel.text = [NSString stringWithFormat:@"%0.2f", yawSpeedSlider.value];
	verticalSpeedCurrentLabel.text = [NSString stringWithFormat:@"%0.2f", verticalSpeedSlider.value];
	droneTiltCurrentLabel.text = [NSString stringWithFormat:@"%0.2f", droneTiltSlider.value];
	iPhoneTiltCurrentLabel.text = [NSString stringWithFormat:@"%0.2f", iPhoneTiltSlider.value];
	droneTrimPitchCurrentLabel.text = [NSString stringWithFormat:@"%0.2f", ((droneTrimPitchSlider.value < -0.01) || (droneTrimPitchSlider.value > 0.01)) ? droneTrimPitchSlider.value : 0.0];
	droneTrimRollCurrentLabel.text = [NSString stringWithFormat:@"%0.2f", ((droneTrimRollSlider.value < -0.01) || (droneTrimRollSlider.value > 0.01)) ? droneTrimRollSlider.value : 0.0];
	
	droneTrimRollSlider.enabled = droneTrimPitchSlider.enabled = NO;
	droneTrimRollSlider.alpha = droneTrimPitchSlider.alpha = 0.5;
}

- (IBAction)valueChanged:(id)sender
{
	if(sender == acceleroDisabledSwitch)
	{
		[_delegate acceleroValueChanged:acceleroDisabledSwitch.on == NO];
		ARDRONE_CONFIGURATION_SET(control_level, (controlMode << 2) | (0 << 1) | acceleroDisabledSwitch.on);
	}
	else if(sender == leftHandedSwitch)
	{
		controlMode = (leftHandedSwitch.on == YES) ? CONTROL_MODE2 : CONTROL_MODE3;
		[_delegate controlModeChanged:controlMode];
		ARDRONE_CONFIGURATION_SET(control_level, (controlMode << 2) | (0 << 1) | acceleroDisabledSwitch.on);
	}
	else if(sender == pairingSwitch)
	{
		strcpy(controlData->pairing_mac_address, (pairingSwitch.on == YES) ? iphone_mac_address : NULL_MAC);
		strcpy(ardrone_control_config.owner_mac, controlData->pairing_mac_address);
		controlData->needSetPairing = YES;
		
	}
	else if(sender == altituteLimitedSwitch)
	{
		uint32_t value = (altituteLimitedSwitch.on == YES) ? ALTITUDE_LIMITED : NO_ALTITUDE;
		ardrone_control_config.altitude_max = value;
		ARDRONE_CONFIGURATION_SET(altitude_max, ardrone_control_config.altitude_max);
	}
	else if(sender == outdoorFlightSwitch)
	{
		bool_t enabled = (outdoorFlightSwitch.on == YES);
		ardrone_control_config.outdoor = enabled;
		ARDRONE_CONFIGURATION_SET(outdoor, enabled);
		controlData->needGetConfiguration = TRUE;
	}
	else if(sender == outdoorShellSwitch)
	{
		bool_t enabled = (outdoorShellSwitch.on == YES);
		ardrone_control_config.flight_without_shell = enabled;
		ARDRONE_CONFIGURATION_SET(flight_without_shell, ardrone_control_config.flight_without_shell);
	}
	else
	{
		[self refresh];		
	}
}

- (IBAction)sliderRelease:(id)sender
{
	if(sender == droneTiltSlider)
	{
		float value = droneTiltSlider.value;
		ardrone_control_config.euler_angle_max = value * DEG_TO_RAD;
		ARDRONE_CONFIGURATION_SET(euler_angle_max, ardrone_control_config.euler_angle_max);
	}
	else if(sender == iPhoneTiltSlider)
	{
		float value = iPhoneTiltSlider.value;
		ardrone_control_config.control_iphone_tilt = value * DEG_TO_RAD;
		ARDRONE_CONFIGURATION_SET(control_iphone_tilt, ardrone_control_config.control_iphone_tilt);
	}
	else if(sender == verticalSpeedSlider)
	{
		float value = verticalSpeedSlider.value;
		ardrone_control_config.control_vz_max = value;
		ARDRONE_CONFIGURATION_SET(control_vz_max, ardrone_control_config.control_vz_max);
	}
	else if(sender == yawSpeedSlider)
	{
		float value = yawSpeedSlider.value;
		ardrone_control_config.control_yaw = value * DEG_TO_RAD;
		ARDRONE_CONFIGURATION_SET(control_yaw, ardrone_control_config.control_yaw);
	}
}

#ifdef INTERFACE_WITH_DEBUG
- (IBAction)logsChanged:(id)sender
{
	if(logsSwitch.on)
		saveSettings();
	setNavdataDemo(logsSwitch.on == NO);
	[self refresh];
}

- (IBAction)tvoutChanged:(id)sender
{
	if(tvoutSwitch.on)
		[tvout startTVOut];
	else
		[tvout stopTVOut];	
}

- (IBAction)clearLogsButtonClick:(id)sender
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
	if([paths count] > 0)
	{
		NSString *documentsDirectory = [paths objectAtIndex:0];
		NSFileManager *fileManager = [[NSFileManager alloc] init];
		NSArray *documentsDirectoryContents = [fileManager contentsOfDirectoryAtPath:documentsDirectory error:nil];
		for(int i = 0 ; i < [documentsDirectoryContents count] ; i++)
		{
			if([[documentsDirectoryContents objectAtIndex:i] hasPrefix:@"mesures"] || [[documentsDirectoryContents objectAtIndex:i] hasPrefix:@"settings"])
			{
				char filename[256];
				sprintf(filename, "%s/%s", [documentsDirectory cStringUsingEncoding:NSUTF8StringEncoding], [[documentsDirectoryContents objectAtIndex:i] cStringUsingEncoding:NSUTF8StringEncoding]);
				NSLog(@"- Remove %s", filename);
				remove(filename);
			}
		}
		[fileManager release];
	}
}
#endif

- (IBAction)clearButtonClick:(id)sender
{
//	NSLog(@"%s", __FUNCTION__);
	ardrone_control_config.euler_angle_max = ardrone_control_config_default.euler_angle_max;
	droneTiltSlider.value = ardrone_control_config.euler_angle_max * RAD_TO_DEG;
	ARDRONE_CONFIGURATION_SET(euler_angle_max, ardrone_control_config.euler_angle_max);
	
	ardrone_control_config.control_iphone_tilt = ardrone_control_config_default.control_iphone_tilt;
	iPhoneTiltSlider.value = ardrone_control_config.control_iphone_tilt * RAD_TO_DEG;
	ARDRONE_CONFIGURATION_SET(control_iphone_tilt, ardrone_control_config.control_iphone_tilt);
	
	ardrone_control_config.control_vz_max = ardrone_control_config_default.control_vz_max;
	verticalSpeedSlider.value = ardrone_control_config.control_vz_max;
	ARDRONE_CONFIGURATION_SET(control_vz_max, ardrone_control_config.control_vz_max);
	
	ardrone_control_config.control_yaw = ardrone_control_config_default.control_yaw;
	yawSpeedSlider.value = ardrone_control_config.control_yaw * RAD_TO_DEG;
	ARDRONE_CONFIGURATION_SET(control_yaw, ardrone_control_config.control_yaw);
	
	ardrone_control_config.flight_without_shell = ardrone_control_config_default.flight_without_shell;
	outdoorShellSwitch.on = (ardrone_control_config.flight_without_shell ? YES : NO);
	ARDRONE_CONFIGURATION_SET(flight_without_shell, ardrone_control_config.flight_without_shell);

	ardrone_control_config.outdoor = ardrone_control_config_default.outdoor;
	outdoorFlightSwitch.on = (ardrone_control_config.outdoor ? YES : NO);
	ARDRONE_CONFIGURATION_SET(outdoor, ardrone_control_config.outdoor);
	
	ardrone_control_config.altitude_max = ardrone_control_config_default.altitude_max;
	altituteLimitedSwitch.on = ((ardrone_control_config.altitude_max == ALTITUDE_LIMITED) ? YES : NO);
	ARDRONE_CONFIGURATION_SET(altitude_max, ardrone_control_config.altitude_max);

	ardrone_control_config.control_level = ardrone_control_config_default.control_level;
	acceleroDisabledSwitch.on = ((ardrone_control_config.control_level >> CONTROL_LEVEL_ACCELERO_DISABLED) & 0x1);
	controlMode = ((ardrone_control_config.control_level >> CONTROL_LEVEL_CONTROL_MODE) & (CONTROL_MODE_MAX - 1));
	leftHandedSwitch.on = ((controlMode == CONTROL_MODE2) ? YES : NO);
	[_delegate acceleroValueChanged:acceleroDisabledSwitch.on == NO];
	[_delegate controlModeChanged:controlMode];
	ARDRONE_CONFIGURATION_SET(control_level, ardrone_control_config.control_level);
}

- (IBAction)flatTrimButtonClick:(id)sender
{
//	NSLog(@"%s", __FUNCTION__);
	ardrone_at_set_flat_trim();
}

- (IBAction)okButtonClick:(id)sender
{
//	NSLog(@"%s", __FUNCTION__);
	self.view.hidden = YES;
}

- (void)switchDisplay
{
	self.view.hidden = !self.view.hidden;
}

- (void)textFieldDidBeginEditing:(UITextField *)theTextField           // became first responder
{
//	NSLog(@"%s", __FUNCTION__);
	if(theTextField == droneSSIDTextField)
	{
		ssidChangeInProgress = NO;
	}
}
- (BOOL)textFieldShouldReturn:(UITextField *)theTextField 
{
//	NSLog(@"%s", __FUNCTION__);
	if(theTextField == droneSSIDTextField)
	{
		NSString *str = [NSString stringWithFormat:@"Your changes will be applied after rebooting the AR.Drone !\n" \
						 "\t- Quit application\n" \
						 "\t- Reboot your AR.Drone\n" \
						 "\t- Connect your iPhone on %s network\n" \
						 "\t- Launch application\n", [droneSSIDTextField.text cStringUsingEncoding:NSUTF8StringEncoding]];
		strcpy(ardrone_control_config.ssid_single_player, [droneSSIDTextField.text cStringUsingEncoding:NSUTF8StringEncoding]);
		ARDRONE_CONFIGURATION_SET(ssid_single_player, ardrone_control_config.ssid_single_player);
		
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"AR.Drone network SSID" message:str delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil, nil];
		[alert show];
		[alert release];
	}

	[theTextField resignFirstResponder];
	return YES;
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	NSLog(@"%s", __FUNCTION__);
	ssidChangeInProgress = YES;
}

- (void)dealloc
{
#ifdef INTERFACE_WITH_DEBUG
	[tvout release];
#endif
	[super dealloc];	
}
@end
