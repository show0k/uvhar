/**
 *  \brief    gamepad handling implementation
 *  \author   Sylvain Gaeremynck <sylvain.gaeremynck@parrot.fr>
 *  \version  1.0
 *  \date     04/06/2007
 *  \warning  Subject to completion
 */

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#include <linux/joystick.h>

#include <ardrone_api.h>
#include <VP_Os/vp_os_print.h>
#include "gamepad.h"

input_device_t gamepad = { 
  "Gamepad",
  open_gamepad,
  update_gamepad,
  close_gamepad
};



C_RESULT open_gamepad(void)
{

     return C_OK;
}

C_RESULT update_gamepad(void)
{
     
     return C_OK;
}

C_RESULT close_gamepad(void)
{

     return C_OK;
}










