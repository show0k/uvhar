#include "ConstantsAndMacros.h" 
#import "ARDrone.h"

typedef enum CONTROL_MODE
{
	CONTROL_MODE3,
	CONTROL_MODE2,
	CONTROL_MODE1,
	CONTROL_MODE4,
	CONTROL_MODE_MAX,
} CONTROL_MODE;

typedef struct 
{
	BOOL  logsActivated;
} SettingsParams;

@protocol SettingsMenuDelegate<NSObject>

- (void)acceleroValueChanged:(BOOL)enabled;
- (void)controlModeChanged:(CONTROL_MODE)mode;

@end

@interface SettingsMenu : UIViewController <UIScrollViewDelegate> {
	// Settings field
	IBOutlet UISwitch *pairingSwitch;
	IBOutlet UISwitch *altituteLimitedSwitch;
    IBOutlet UISwitch *outdoorFlightSwitch;
    IBOutlet UISwitch *outdoorShellSwitch;

	IBOutlet UISwitch *acceleroDisabledSwitch;
    IBOutlet UISwitch *leftHandedSwitch;
  
	IBOutlet UIButton *clearButton0, *clearButton1;
	IBOutlet UIButton *flatTrimButton0, *flatTrimButton1;
	
    IBOutlet UILabel  *droneTiltMinLabel;
    IBOutlet UILabel  *droneTiltMaxLabel;
    IBOutlet UILabel  *droneTiltCurrentLabel;
    IBOutlet UISlider *droneTiltSlider;
	
	IBOutlet UILabel  *iPhoneTiltMinLabel;
    IBOutlet UILabel  *iPhoneTiltMaxLabel;
    IBOutlet UILabel  *iPhoneTiltCurrentLabel;
    IBOutlet UISlider *iPhoneTiltSlider;

	IBOutlet UILabel  *verticalSpeedMinLabel;
    IBOutlet UILabel  *verticalSpeedMaxLabel;
    IBOutlet UILabel  *verticalSpeedCurrentLabel;
	IBOutlet UISlider *verticalSpeedSlider;
	
	IBOutlet UILabel  *yawSpeedMinLabel;
    IBOutlet UILabel  *yawSpeedMaxLabel;
    IBOutlet UILabel  *yawSpeedCurrentLabel;
    IBOutlet UISlider *yawSpeedSlider;
	
	IBOutlet UITextField *droneSSIDTextField;
	
	IBOutlet UILabel  *droneTrimRollMinLabel;
    IBOutlet UILabel  *droneTrimRollMaxLabel;
    IBOutlet UILabel  *droneTrimRollCurrentLabel;
    IBOutlet UISlider *droneTrimRollSlider;

	IBOutlet UILabel  *droneTrimPitchMinLabel;
    IBOutlet UILabel  *droneTrimPitchMaxLabel;
    IBOutlet UILabel  *droneTrimPitchCurrentLabel;
    IBOutlet UISlider *droneTrimPitchSlider;

	IBOutlet UILabel  *softwareVersion;
	IBOutlet UILabel  *droneHardVersion, *droneSoftVersion;
	IBOutlet UILabel  *dronePicHardVersion, *dronePicSoftVersion;
	IBOutlet UILabel  *droneMotor1HardVersion, *droneMotor1SoftVersion, *droneMotor1SupplierVersion;
	IBOutlet UILabel  *droneMotor2HardVersion, *droneMotor2SoftVersion, *droneMotor2SupplierVersion;
	IBOutlet UILabel  *droneMotor3HardVersion, *droneMotor3SoftVersion, *droneMotor3SupplierVersion;
	IBOutlet UILabel  *droneMotor4HardVersion, *droneMotor4SoftVersion, *droneMotor4SupplierVersion;

#ifdef INTERFACE_WITH_DEBUG
	IBOutlet UIButton *clearLogsButton;
	IBOutlet UISwitch *logsSwitch;
	IBOutlet UISwitch *tvoutSwitch;
#endif
}

- (id)initWithFrame:(CGRect)frame AndHUDConfiguration:(ARDroneHUDConfiguration)configuration withDelegate:(id <SettingsMenuDelegate>)delegate withControlData:(ControlData*)data;
- (void)switchDisplay;
- (void)configChanged;

- (IBAction)valueChanged:(id)sender;
- (IBAction)clearButtonClick:(id)sender;
- (IBAction)flatTrimButtonClick:(id)sender;
- (IBAction)okButtonClick:(id)sender;
- (IBAction)sliderRelease:(id)sender;

#ifdef INTERFACE_WITH_DEBUG
- (IBAction)logsChanged:(id)sender;
- (IBAction)clearLogsButtonClick:(id)sender;
- (IBAction)tvoutChanged:(id)sender;
#endif
@end
