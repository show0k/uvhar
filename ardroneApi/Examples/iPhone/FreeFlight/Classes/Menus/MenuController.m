#import "MenuController.h"
#import "MenuUpdater.h"

@implementation MenuController
@synthesize delegate;
@synthesize menuCurrent;

- (void)viewDidLoad
{
	NSLog(@"view did load");
	[super viewDidLoad];
	
	// Initialize the menu class history and set the very first menu
	menuClassHistory = [[NSMutableArray arrayWithCapacity:5] retain];
	menuNextClass = [MenuUpdater class];
	self.menuCurrent = [[menuNextClass alloc] initWithController:self];
}

- (void)changeState:(BOOL)inGame
{	
	self.view.hidden = NO;
	NSLog(@"menu view hidden = %i",  self.view.hidden);
	
	if(inGame)
	{
		[menuCurrent.view removeFromSuperview];
	}
	else
	{
		if([menuCurrent class] != menuNextClass)
		{
			// Unload the current menu
			[menuCurrent.view removeFromSuperview];
			[menuCurrent release];
			
			// Load the next menu
			menuCurrent = [[menuNextClass alloc] initWithController:self];
			[self.view addSubview:menuCurrent.view];
		}
		else 
		{
			[self.view addSubview:menuCurrent.view];
		}
	}
}


- (void)changeMenu:(Class)menuClass
{
	// Make sure there isn't a change in progress
	if([menuCurrent class] != menuNextClass)
	{
		return;
	}

	// Disable user interactions in the current menu to prevent its buttons to react to touch events
	menuCurrent.view.userInteractionEnabled = NO;

	// Take note of the class of the next menu
	// Note: the menu isn't instantly changed, modifications are performed when the sliders are closed
	if(menuClass)
	{
		[menuClassHistory addObject:menuNextClass];
		menuNextClass = menuClass;

		if([menuCurrent class] != menuNextClass)
		{
			// Unload the current menu
			[menuCurrent.view removeFromSuperview];
			
			// Load the next menu
			self.menuCurrent = [[menuNextClass alloc] initWithController:self];
			[self.view addSubview:menuCurrent.view];
		}
	}
	else
	{
		NSLog(@"move back to previous menu");
		if([menuClassHistory count] > 0)
		{
			menuNextClass = [menuClassHistory lastObject];
			[menuClassHistory removeLastObject];
		}
		else
		{
			// Unload the current menu
			[delegate changeState:YES];
			[menuCurrent.view removeFromSuperview];
		}
	}
}

- (void)executeCommandIn:(ARDRONE_COMMAND_IN)commandId withParameter:(void*)parameter fromSender:(id)sender
{
	
}

- (BOOL)checkState
{
	return YES;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{	
	if([delegate checkState] == NO)
		return UIInterfaceOrientationIsLandscape(interfaceOrientation);
	else
		return FALSE;
}

- (void)dealloc
{
	// Release the current menu
	[menuCurrent release];

	// Release the menu class history
	[menuClassHistory release];

	// Deallocate everthing else
	[super dealloc];
}

@end
