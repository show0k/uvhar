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
// our own exit indicator
int exitOnNextUpdate = 0;

extern int imageCounter, videoSwitch;
int counter = 0;
extern float batteryLevel, theta, phi, psi, altitude, vx, vy, vz;
float roll, pitch, gaz, yaw;

int isTrim = 0;

// variables needed for python
PyObject *pArgument, *pClassInstance, *pResult, *pMethodName;



C_RESULT python_init()
{
    PyObject *pName, *pFunc, *pModule;

   	Py_Initialize();

    // create a python string object for the module name
   	pName = PyString_FromString("uvhar");
 
	if (pName == NULL)
    {
         printf("\tPython name is null.\n");
		 return C_FAIL;
    }

	pModule = PyImport_Import(pName);

	Py_DECREF(pName);
	if (pModule == NULL)
    {
         printf("\tPython module is null.\n");
	     PyErr_Print();
		 return C_FAIL;
    }

    // create a callable python object 
    pFunc = PyObject_GetAttrString(pModule, "Uvhar");

    // check for succes
    if (pFunc && PyCallable_Check(pFunc))
    {
         // create the class
         pClassInstance = PyObject_CallObject(pFunc, NULL);
         if (pClassInstance == NULL)
         {
             printf("\tPython class constructor FAILED!!!\n");
             PyErr_Print();
             return C_FAIL;
         }
         printf("\tPython class contructor is done\n");
         // now we do not need the module and func any longer

         pMethodName = PyString_FromString("update");

         Py_XDECREF(pFunc);
         Py_XDECREF(pModule);
    }
    else
    {
         Py_DECREF(pModule);
         Py_XDECREF(pFunc);
         printf("\tpFunc not true or not callable\n");
         return C_FAIL;
    }

    printf("\tPython initialized\n");
     
	return C_OK;
}

C_RESULT python_update()
{
     if (exitOnNextUpdate)
         return C_OK;

     // putting the last saved image in a python object 
     pArgument = PyTuple_New(9); //PyInt_FromLong(imageCounter);
     PyTuple_SetItem(pArgument, 0, PyInt_FromLong(imageCounter));
     PyTuple_SetItem(pArgument, 1, PyFloat_FromDouble(batteryLevel));
     PyTuple_SetItem(pArgument, 2, PyFloat_FromDouble(theta));
     PyTuple_SetItem(pArgument, 3, PyFloat_FromDouble(phi));
     PyTuple_SetItem(pArgument, 4, PyFloat_FromDouble(psi));
     PyTuple_SetItem(pArgument, 5, PyFloat_FromDouble(altitude));
     PyTuple_SetItem(pArgument, 6, PyFloat_FromDouble(vx));
     PyTuple_SetItem(pArgument, 7, PyFloat_FromDouble(vy));
     PyTuple_SetItem(pArgument, 8, PyFloat_FromDouble(vz));

     // pResult will be the fly commands
     pResult = PyObject_CallMethodObjArgs(pClassInstance, pMethodName, pArgument, NULL);

     if (pResult == NULL)
     {
         printf("\tPython update has failed. Printing error and calling signal_exit():\n");
         PyErr_Print();
         exitOnNextUpdate = 1;
         Py_XDECREF(pArgument);
         return C_FAIL;
     }
     else
     {
         pResult = PyList_AsTuple(pResult);
         if (PyInt_AsLong(PyTuple_GetItem(pResult, 0))) // the first entry indicates if we want to quit
         {
             printf("\tc knows python indicates quitin'\n");
	     printf("Let's land this thing!");
	     ardrone_tool_set_ui_pad_start(0);
             exitOnNextUpdate = 1;
         }
         else
         {
             roll = PyFloat_AsDouble(PyTuple_GetItem(pResult, 1));
             pitch = PyFloat_AsDouble(PyTuple_GetItem(pResult, 2));
             gaz = PyFloat_AsDouble(PyTuple_GetItem(pResult, 3));
             yaw = PyFloat_AsDouble(PyTuple_GetItem(pResult, 4));
	     if(PyInt_AsLong(PyTuple_GetItem(pResult, 5)))
	     	videoSwitch *= -1;
         }
     }
     Py_XDECREF(pResult);     
     Py_XDECREF(pArgument);

     return C_OK; 
}

// finalizing python. 
C_RESULT python_exit()
{
     printf("\tc python exit called\n");

     PyObject *pExitResult;

     if (pClassInstance != NULL)
     {
         pExitResult = PyObject_CallMethod(pClassInstance, "exit", NULL); 
         if (pExitResult == NULL)
             PyErr_Print();
         else
             Py_DECREF(pExitResult);
     }

     printf("\tDecfrefin' ...\n");
     Py_XDECREF(pClassInstance);
     Py_XDECREF(pMethodName);
     Py_XDECREF(pArgument); // this one is already NULL
     Py_XDECREF(pResult);
     printf("\tFinalizin' ...\n");
	 Py_Finalize();
     printf("\tDone finalizin' ...\n");
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

 	C_RESULT pythonCResult = python_init();

	// reset the drone (there is no emergency) and set it to 
	// land once at the start... also trim to flat
	ardrone_tool_set_ui_pad_select(0);
	ardrone_tool_set_ui_pad_start(0);
    if (isTrim)
        ardrone_at_set_flat_trim();
    // and now launch this thing
    else
        ardrone_tool_set_ui_pad_start(1);
	return pythonCResult;
}

C_RESULT ardrone_tool_update_custom()
{
    if (isTrim)
        return C_OK;
    /*
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
        printf("\tScript has run, exiting?\n");
		//counter = 0;
        signal_exit(); 
	}
    */
     
     ardrone_at_set_progress_cmd(1, roll, pitch, gaz, yaw);
     //ardrone_at_set_progress_cmd(1, 0, 0, 0, yaw);
     
     if (exitOnNextUpdate)
         signal_exit();
     
     counter ++;
     if (counter > 2000)
     {
         printf("\t1500 updates, we're stoppin'!\n");
         signal_exit();
     }
    

     python_update();

	 return C_OK;
}

/* The delegate object calls this method when the event loop exit */
C_RESULT ardrone_tool_shutdown_custom()
{
	 printf("\tardrone_tool_shutdown_custom called\n");

	 python_exit();

     printf("\tbefore set uit pad start (0)\n");
     printf("\tafter set uit pad start (0) and before join thread video stage \n");
	 /* Relinquish all threads of your application */
	 JOIN_THREAD( video_stage );
     printf("\tafter join thread video stage\n");


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

