#include <ardrone_tool/Navdata/ardrone_navdata_client.h>

#include <Navdata/navdata.h>

#include <stdio.h>
#define CTRL_STATES_STRING
#include "control_states.h"

int printNavData;

const char* ctrl_state_str(uint32_t ctrl_state)
{
  #define MAX_STR_CTRL_STATE 256
  static char str_ctrl_state[MAX_STR_CTRL_STATE];

  ctrl_string_t* ctrl_string;
  uint32_t major = ctrl_state >> 16;
  uint32_t minor = ctrl_state & 0xFFFF;

  if( strlen(ctrl_states[major]) < MAX_STR_CTRL_STATE )
  {
    vp_os_memset(str_ctrl_state, 0, sizeof(str_ctrl_state));

    strcat(str_ctrl_state, ctrl_states[major]);
    ctrl_string = control_states_link[major];

    if( ctrl_string != NULL && (strlen(ctrl_states[major]) + strlen(ctrl_string[minor]) < MAX_STR_CTRL_STATE) )
    {
      strcat( str_ctrl_state, " | " );
      strcat( str_ctrl_state, ctrl_string[minor] );
    }
  }
  else
  {
    vp_os_memset( str_ctrl_state, '#', sizeof(str_ctrl_state) );
  }

  return str_ctrl_state;
}



/* Initialization local variables before event loop  */
inline C_RESULT navdata_client_init( void* data )
{
     // your code to initliazie local variables
     return C_OK;
}

/* Receving navdata during the event loop */
inline C_RESULT navdata_client_process( const navdata_unpacked_t* const navdata )
{

     const navdata_demo_t* const nd = &navdata->navdata_demo;
     if (printNavData == 1)
     {

     printf("\n=====================\nNavdata for flight demonstrations\n=====================\n\n");

     printf("Control state : %s                                      \n",ctrl_state_str(nd->ctrl_state));
     printf("Battery level : %i/100          \n",nd->vbat_flying_percentage);
     printf("Orientation   : [Theta] %4.3f  [Phi] %4.3f  [Psi] %4.3f          \n",nd->theta,nd->phi,nd->psi);
     printf("Altitude      : %i          \n",nd->altitude);
     printf("Speed         : [vX] %4.3f  [vY] %4.3f  [vZ] %4.3f          \n",nd->vx,nd->vy,nd->vz);

     printf("\033[10A");
     }


  return C_OK;
}

/* Relinquish the local resources after the event loop exit */
inline C_RESULT navdata_client_release( void )
{
     // free local variables
     return C_OK;
}

/* Registering to navdata client */
BEGIN_NAVDATA_HANDLER_TABLE
  NAVDATA_HANDLER_TABLE_ENTRY(navdata_client_init, navdata_client_process, navdata_client_release, NULL)
END_NAVDATA_HANDLER_TABLE

