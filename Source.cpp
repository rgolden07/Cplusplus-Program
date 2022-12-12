#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>


using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}

void displayMenu() {
	cout << "          Corner Grocer Item-tracking" << endl;
	cout << endl;
	cout << "_______________________________________________" << endl;
	cout << "| Please choose from the menu items below:    |" << endl;
	cout << "| 1 : Daily purchased items list.             |" << endl;
	cout << "| 2 : Purchase frequency of particular Item.  |" << endl;
	cout << "| 3 : Histogram of daily purchased items.     |" << endl;
	cout << "| 4 : Exit program.                           |" << endl;
	cout << "|_____________________________________________|" << endl;
}

/*
validates user input is an integer via while loop then returns input once condition is true.
*/
int integerCheck() { 
	int input;	

	while (!(cin >> input)){
		cout << "Invalid input: Integer required" << endl;
		cin.clear();
		cin.ignore(100, '\n');
	}
	return input;
}

/*
Function is called in main.  Takes user input and validates that it is in the acceptable range.  Returns menuSelection as an int.
*/
int menuSelection() { 
	int menuSelection;
	bool valid = false;

	do
	{
		menuSelection = integerCheck();
		if (menuSelection >= 1 && menuSelection <= 4) {
			valid = true;
		}
		else {
			cout << endl;
			cout << "Please input corresponding menu selection's number." << endl;
			cout << "Valid entries are 1, 2, 3, or 4" << endl;
			cout << endl;
			displayMenu();
			cout << endl;
		}

	} while (!valid);

	return menuSelection;	
}

/*
Function takes user input as a string.  Then alters the user string to match case format of items in the resource file.

Passes the altred user input to PythonCode where it evaluates if input is in the list of items. 

If input is in the recourse file it returns the frequency of the item, and if not it returns 0.

Depending on what number is returned changes the output.
*/
void getItem() {
	string itemOutput;
	string item;
	int frequency;
	
	cout << "Please enter desired item to check purchase frequency." << endl;
	cout << endl;
	cin >> item;
	cout << endl;

	for (int i = 0; i < item.size(); ++i){
		if (i == 0) {
			item.at(i) = toupper(item.at(i));
		}
		else {
			item.at(i) = tolower(item.at(i));
		}
	}

	frequency = callIntFunc("determineFrequency", item);

	if (frequency == 0) {
		itemOutput = "Sorry " + item + " has not been purchased recently.";
		cout << itemOutput << endl;
		cout << endl;
	}
	else {
		itemOutput = item + " were purchased " + to_string(frequency) + " times today.";
		cout << itemOutput << endl;
		cout << endl;
	}

}

/*
Function is passsed two pointer vectors from main, then calls file generation python function.  The resulting file is opened, 

ieterated through while adding the values to the parallel pointer vectors.  

Then for astetic value the longest item string length is found to aid in making the output more readable.
*/
void displayHistogram(vector<string> &items, vector<int> &frequencys) {
	ifstream file;
	int frequency;
	string item;
	int size = 0;
	
	CallProcedure("output");

	file.open("frequency.dat");

	if (!file.is_open()) { // checks if file is open
		cout << "Error in opening frequency.dat" << endl;
	}
	else {
		file >> item >> frequency;
		items.push_back(item);
		frequencys.push_back(frequency);
		while (!file.eof()) // adds values to vectors
		{		
			file >> item >> frequency;
			items.push_back(item);
			frequencys.push_back(frequency);
		}
	}
	file.close();

	for (int i = 0; i < items.size(); ++i) {
		if (items[i].size() > size) {
			size = items[i].size();
		}
	}

	for (int i = 0; i < items.size(); ++i) { // outputs formatted histogram
		cout << setw(size) << items.at(i) << "| ";
		for (int x = 0; x < frequencys.at(i); ++x) {
			cout << ">";
		}
		cout << endl;
	}
	cout << endl;
}

void main()
{
	const int TOTAL_INVENTORY_MENU_CHOICE = 1;
	const int ITEM_FREQUENCY_MENU_CHOICE = 2;
	const int HISTOGRAM_MENU_CHOICE = 3;
	const int EXIT_PROGRAM_MENU_CHOICE = 4;
	int menuChoice = -1;
	vector<string> items;
	vector<int> frequencys;

	do // loops while for as long as the user wants to.
	{
		displayMenu();
		cout << endl;
		menuChoice = menuSelection();
		cout << endl;

		switch (menuChoice) // switch statement for usermenu choice
		{
		case TOTAL_INVENTORY_MENU_CHOICE:
			cout << "   TOTAL INVENTORY" << endl;
			CallProcedure("determineInventory");
			cout << endl;
			break;
		case ITEM_FREQUENCY_MENU_CHOICE:
			getItem();
			break;
		case HISTOGRAM_MENU_CHOICE:
			displayHistogram(items, frequencys);
			break;
		default:
			break;
		}

	} while (menuChoice != EXIT_PROGRAM_MENU_CHOICE);
}
	
	
	
	
	
	
	
	
	
