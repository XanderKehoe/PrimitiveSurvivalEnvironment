#pragma once
#include <string>
#include <Windows.h>

// work around "cannot open file 'python38_d.lib'" error from https://stackoverflow.com/questions/17028576/using-python-3-3-in-c-python33-d-lib-not-found
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

class PythonInterface
{
public:
	/*
	Calls the Py_Initialize function which must be called before using any other Python/C API functions (https://docs.python.org/3/c-api/init.html)
	This function must be called before using any other functions in this class.
	*/
	static void Initialize() { Py_Initialize(); };

	/*
	Calls a Python function from the given file name.
	PythonModuleName: a string of the name of the file containing the function.
	PythonFunctionName: a string of the name of the function to call
	pArgs: a PyObject pointer to a list of arguments to pass in
	*/
	static void CallPython(std::string PythonModuleName, std::string PythonFunctionName, PyObject* pArgs);

	/*
	Returns a set of 3 numbers (1, 2, 3) for testing purposes
	*/
	static PyObject* GetTestArgs();
};

