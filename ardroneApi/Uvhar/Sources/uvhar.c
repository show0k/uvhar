/**
 * @file main.c
 * @author sylvain.gaeremynck@parrot.com
 * @date 2009/07/01
 */
#include <uvhar.h>
#include <python2.6/Python.h>

//ARDroneLib
#include <ardrone_tool/ardrone_time.h>
#include <ardrone_tool/Navdata/ardrone_navdata_client.h>
#include <ardrone_tool/Control/ardrone_control.h>
#include <ardrone_tool/UI/ardrone_input.h>

//Common
#include <config.h>
#include <ardrone_api.h>

//VP_SDK
#include <ATcodec/ATcodec_api.h>
#include <VP_Os/vp_os_print.h>
#include <VP_Api/vp_api_thread_helper.h>
#include <VP_Os/vp_os_signal.h>

//Local project
#include <UI/gamepad.h>
#include <Video/video_stage.h>
#include <Navdata/navdata.h>

static int32_t exit_ihm_program = 1;

int counter = 0;
// variables needed for python
PyObject *pName, *pModule, *pDict, *pFunc;
PyObject *pArgs, *pValue;



C_RESULT python_init()
{

   	Py_Initialize();

   	pName = PyString_FromString("uvhar");

	if (pName == NULL)
		return C_FAIL;

	pModule = PyImport_Import(pName);

	Py_DECREF(pName);
	if (pModule == NULL)
    {
	     PyErr_Print();
		 return C_FAIL;
    }

    printf("\tPython initialized\n");
     
	return C_OK;
}


C_RESULT python_update()
{
     printf("\tAttempting a python call\n");

     if (pModule == NULL)
         return C_FAIL;

	pFunc = PyObject_GetAttrString(pModule, "update");

	if (pFunc && PyCallable_Check(pFunc))
	{
		pArgs = PyTuple_New(1);//"HERE GOES THE NUMBER OF ARGUMENTS AS INT");

		pValue = PyString_FromString("testietest");
		// repeat for more arguments

        /*
		// we are going to be awesome and not have to check if the arguments
          //     correctly convert! 
                if (!pValue)
                {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    printf("Cannot convert argument\n");
                    return C_FAIL;
                }   
        */
		
		PyTuple_SetItem(pArgs, 0, pValue);
		pValue = PyObject_CallObject(pFunc, pArgs);
		Py_DECREF(pArgs);


		if (pValue != NULL) 
		{
			// HERE WE RETRIEVE THE RETURN VALUE AS INTEGER FROM THE PYTHON FUNCTION
			int result = PyInt_AsLong(pValue); 
            printf("result of python call: %d\n", result); 
			Py_DECREF(pValue);
		}
		else
		{
			Py_DECREF(pFunc);
			//Py_DECREF(pModule);
			PyErr_Print();
			printf("\tPYTHON METHOD FAILED:(\n");
			return C_FAIL;
		}
	}
	else
	{
		if (PyErr_Occurred())
			PyErr_Print();

		printf("\tPYTHON METHOD FAILED, could not find function\n");
	}
	Py_XDECREF(pFunc);
	//Py_DECREF(pModule);

return C_OK; 
}

// finalizing python. 
C_RESULT python_exit()
{
	Py_Finalize();
	return C_OK;
}



/* The delegate object calls this method during initialization of an ARDrone application */
C_RESULT ardrone_tool_init_custom(int argc, char **argv)
{
	/* Registering for a new device of game controller */
	//ardrone_tool_input_add( &gamepad );

	printf("\tardrone_tool_init_custom called\n");

	/* Start all threads of your application */
	START_THREAD( video_stage, NULL );

 //	C_RESULT pythonCResult = python_init();

	// reset the drone (there is no emergency) and set it to 
	// land once at the start
	ardrone_tool_set_ui_pad_select(0);
	ardrone_tool_set_ui_pad_start(0);

	//return pythonCResult;
     return C_OK;
}

C_RESULT ardrone_tool_update_custom()
{

     printf("\t tool update\n");
	//* 
	counter ++;     
	if (counter == 1)
	{
		// take off
		ardrone_tool_set_ui_pad_start(1);
	}
	else if (counter == 200 )   
	{        
		// start rotatin' 
		ardrone_at_set_progress_cmd(1, 0, 0, 0, 0.5);
	}
	else if (counter == 600)
	{
		// set to hoover and land
		ardrone_at_set_progress_cmd(0, 0, 0, 0, 0);
		ardrone_tool_set_ui_pad_start(0); 
	}
	else if (counter == 800)
	{
		counter = 0;
	}
	//*/ 

    // python_update();


     /*
	char c = getchar();
	if (c == -1) // quit on ctrl + c
		signal_exit();
     */

	return C_OK;
}

/* The delegate object calls this method when the event loop exit */
C_RESULT ardrone_tool_shutdown_custom()
{
	printf("\tardrone_tool_shutdown_custom called\n");

//	python_exit();

	ardrone_tool_set_ui_pad_start(0);

	/* Relinquish all threads of your application */
	JOIN_THREAD( video_stage );


	/* Unregistering for the current device */
	//ardrone_tool_input_remove( &gamepad );

	return C_OK;
}

/* The event loop calls this method for the exit condition */
bool_t ardrone_tool_exit()
{
	//printf("\tardrone_tool_exit called\n");

	return exit_ihm_program == 0;
}

C_RESULT signal_exit()
{
	printf("\tsignal_exit called\n");

	exit_ihm_program = 0;

	return C_OK;
}

/* Implementing thread table in which you add routines of your application and those provided by the SDK */
BEGIN_THREAD_TABLE
THREAD_TABLE_ENTRY( ardrone_control, 20 )
THREAD_TABLE_ENTRY( navdata_update, 20 )
THREAD_TABLE_ENTRY( video_stage, 20 )
END_THREAD_TABLE
