#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include "OutputHandler.h"
#include "PyHandler.h"
#include <string>
#include <vector>

using namespace std;


class InputHandler
{
public:
  // Preset menu options
  enum menuOptions
  {
    NONE      = 0,
    ITEMIZED  = 1,
    FREQUENCY = 2,
    HISTOGRAM = 3,
    EXIT      = 4
  };
  
  InputHandler(); // Default constructor
  // Public member methods
  void getMenuInput(); // Prompt user for menu action
  string getItemNameInput(); // Prompt user for item name
  vector<pair<int, string>> ReadDataFile(); // Read in data from .dat file

private:
  // Declared private member variables
  OutputHandler m_OutputHandle;
  PyHandler m_PyHandle;
};
#endif
