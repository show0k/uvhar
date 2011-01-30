#include <python2.6/Python.h>

int
main(int argc, char *argv[])
{
    PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue, *pValue2, *pValue3;
    PyObject *methodName, *methodArgument1;

    int i;

    if (argc < 3) {
        fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
        return 1;
    }

    Py_Initialize();
    pName = PyString_FromString(argv[1]);
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    // start of python method call and data conversion from c to python

    if (pModule != NULL) 
    {
        pFunc = PyObject_GetAttrString(pModule, argv[2]);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) 
        {
            pArgs = PyTuple_New(argc - 3);
            for (i = 0; i < argc - 3; ++i) 
            {
                pValue = PyInt_FromLong(atoi(argv[i + 3]));
                if (!pValue) 
                {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }

            pValue = PyObject_CallObject(pFunc, pArgs);

            pValue2 = PyObject_CallMethod(pValue, "printieprint", NULL);

            methodName = PyString_FromString("argumentsMethod");
            methodArgument1 = PyString_FromString("argumenties!");

            pValue2 = PyObject_CallMethod(pValue, "printieprint", NULL);
            pValue3 = PyObject_CallMethodObjArgs(pValue, methodName, methodArgument1, NULL);

            Py_DECREF(pArgs);
            if (pValue != NULL) 
            {
                printf("Result of call 1: %ld\n", PyInt_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else 
            {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
            if (pValue2 != NULL)
            {
                printf("Result of call 2: %ld\n", PyInt_AsLong(pValue2));
                Py_DECREF(pValue2);
            }
            else
            {
                printf("Call 2 was null\n");
            }
        }
        else 
        {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else 
    {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        return 1;
    }

    // end here


    Py_Finalize();
    return 0;
}
