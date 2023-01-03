/*
// Decided not to use this.
#include "PythonInterface.h"

void PythonInterface::CallPython(std::string PythonModuleName, std::string PythonFunctionName, PyObject* pArgs)
{
	char* funcname = new char[PythonFunctionName.length() + 1];
	strcpy_s(funcname, PythonFunctionName.length() + 1, PythonFunctionName.c_str());

	char* modname = new char[PythonModuleName.length() + 1];
	strcpy_s(modname, PythonModuleName.length() + 1, PythonModuleName.c_str());

	/*
	printf("Hit any key to initialize the Python Interpreter\n");
	system("pause");
	

	Py_Initialize();

	TCHAR cwd[2048];
	GetCurrentDirectory(sizeof(cwd), cwd);

	/*
	printf("Hit any key to load the Python Module %ws - %s\n", cwd, modname);
	system("pause");
	

	PyObject* my_module = PyImport_ImportModule(modname);

	PyErr_Print();

	printf("Module found\n");
	/*
	printf("Hit any key to find function %ws from Python module %ws - %s\n", funcname, cwd);
	system("pause");
	

	printf("Getting address of %s in Python module\n", funcname);
	PyObject* my_function = PyObject_GetAttrString(my_module, funcname);

	PyErr_Print();

	printf("Function found\n");
	/*
	printf("Hit any key to call function %ws from Python module %ws - %s\n", funcname, cwd);
	system("pause");
	*/

	/*
	printf("Creating test arguments and calling function\n");
	
	

	PyObject* my_result;
	if (pArgs != NULL)
		my_result = PyObject_CallFunction(my_function, "(O)", pArgs);
	else
		my_result = PyObject_CallObject(my_function, NULL);

	if (my_result != NULL)
		printf("Function result: %d\n", PyLong_AsLong(my_result));
	else
		printf("Function call failed, returned NULL.\n");

	PyErr_Print();

	system("pause");

	Py_Finalize();

	delete[] funcname;
	delete[] modname;
}

PyObject* PythonInterface::GetTestArgs()
{
	PyObject* pArgs, *pValue;
	pArgs = PyTuple_New(3); // create a new tuple of size 3

	pValue = PyLong_FromLong(1);
	PyTuple_SetItem(pArgs, 0, pValue);
	pValue = PyLong_FromLong(2);
	PyTuple_SetItem(pArgs, 1, pValue);
	pValue = PyLong_FromLong(3);
	PyTuple_SetItem(pArgs, 2, pValue);

	//delete[] pValue; // Getting error when implementing this line, but shouldn't this be here?

	return pArgs;
}
*/
