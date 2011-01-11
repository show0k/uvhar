#include "python2.6/Python.h"
//#include <stdio.h





// the global variable that holds a pointer to the "python function object"
// that is the python function. This is found by the python program 
static PyObject *my_callback;

static PyObject *
my_set_callback(PyObject *dummy, PyObject *args)
{
    PyObject *result = NULL;
    PyObject *temp;


    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(my_callback);  /* Dispose of previous callback */
        my_callback = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

static PyObject *
spam_system(PyObject *self, PyObject *args)
{
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);
    return Py_BuildValue("i", sts);
    //return 2132;
}


static PyMethodDef SpamMethods[] = {
    {"mySetCallback", my_set_callback, METH_VARARGS, 
     "calling something back?!"},
    {"system",  spam_system, METH_VARARGS,
     "Execute a shell command."},
    //...
    {NULL, NULL, 0, NULL}        /* Sentinel */
};



PyMODINIT_FUNC
initspam(void)
{
    (void) Py_InitModule("spam", SpamMethods);
}


int
main(int argc, char *argv[])
{
    /* Pass argv[0] to the Python interpreter */
    Py_SetProgramName(argv[0]);

    /* Initialize the Python interpreter.  Required. */
    Py_Initialize();

    /* Add a static module */
    initspam();


    // it is time to call the function? 
    int arg;
    PyObject *arglist;
    PyObject *result;

    arg = 123;

    // returns a tuple when its format string consists of zero or more 
    // code formats between parentheses. 
    arglist = Py_BuildValue("(i)", arg);
    // the python function and the argument list, returns the return value
    // of the python function
    result = PyEval_CallObject(my_callback, arglist);
    Py_DECREF(arglist); 

     return 1;
}
