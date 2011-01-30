#include <python2.6/Python.h>

void exec_interactive_interpreter(int argc, char** argv)
{
	Py_Initialize();
	Py_Main(argc, argv);
	// PyRun_SimpleString("print \"whattup\"");
	Py_Finalize();
}

int main(int argc, char** argv)
{
	//printf("hello, world!\n");
	exec_interactive_interpreter(argc, argv); 
}
