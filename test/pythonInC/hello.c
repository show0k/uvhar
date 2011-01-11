#include "python2.6/Python.h"
//#include <stdio.h

int main(int argc, char** argv)
{
     //printf("hello, world!\n");
     exec_interactive_interpreter(argc, argv); 
}
/*
void exec_interactive_interpreter(int argc, char** argv)
{
  Py_Initialize();
  Py_Main(argc, argv);
  Py_Finalize();
}
*/
