#include "InputHandler.h"
#include <fstream>
#include <iostream>
#include <stdexcept>


InputHandler::InputHandler()
{
  // Initialize handlers
  m_OutputHandle = OutputHandler();
  m_PyHandle     = PyHandler();
  // Get user input
  getMenuInput();
}

void InputHandler::getMenuInput()
{
  int menuChoice = NONE; // Default menu option
  // Loop until user chooses to exit
  while ( menuChoice != EXIT )
  {
    try
    {
      string itemName;
      vector<pair<int, string>> purchases;

      m_OutputHandle.printMenu(); // Print menu options

      cin >> menuChoice; // Get input
      cout << endl << flush;

      // Check if input is bad (like a char instead of an int)
      if ( !cin )
      {
        throw runtime_error( "Invalid input! Entered value must be a number." );
      }

      // Switch statement to determine action taken based on input
      switch ( menuChoice )
      {
        case ITEMIZED: // Input = 1
        {
          // Set console color for itemized report
          m_OutputHandle.setConsoleColor( OutputHandler::colorOptions::AQUA );
          // Call python function to print each item and its frequency
          m_PyHandle.CallProcedure( "GetItemizedReport" );
          m_OutputHandle.setConsoleColor(); // Reset console color
        }
        break;
        case FREQUENCY: // Input = 2
        {
          // Set console color for item frequency
          m_OutputHandle.setConsoleColor( OutputHandler::colorOptions::BRIGHTGREEN );
          itemName = getItemNameInput();                 // Get item name
          m_OutputHandle.printItemFrequency( itemName ); // Output item frequency
        }
        break;
        case HISTOGRAM: // Input = 3
        {
          // Call python function to create the .dat file
          m_PyHandle.CallProcedure( "WriteItemizedData" );
          purchases = ReadDataFile();                 // Read .dat file and return item|frequency pairs
          m_OutputHandle.printHistogram( purchases ); // Print data
        }
        break;
        case EXIT: // Input = 4
        {
          cout << "Exiting..." << endl;
        }
        break;
        default: // Input = any other number
        {
          throw runtime_error(
            "Menu choice not recognized. Enter your selection as a number 1, 2, 3, or 4." );
        }
      }
    }
    catch ( runtime_error& excpt )
    {
      // Set console color for the error message
      m_OutputHandle.setConsoleColor( OutputHandler::colorOptions::RED );
      // Clear + ignore input
      cin.clear();
      // Ignore any other value on the line until you get to the next line
      cin.ignore( numeric_limits<streamsize>::max(), '\n' );
      menuChoice = NONE;            // Reset menu choice
      cout << excpt.what() << endl; // Output error
      // Return to beginning of loop
      m_OutputHandle.setConsoleColor(); // Reset console color
    }
  }
}

string InputHandler::getItemNameInput()
{
  string itemName;
  cout << "Enter the name of the item: ";
  cin >> itemName; // Get name of item

  // Check if input is bad
  if ( !cin )
  {
    throw runtime_error( "Invalid input! Entered value must be a string." );
  }
  cout << endl << flush; // For spacing

  return itemName;
}

vector<pair<int, string>> InputHandler::ReadDataFile()
{
  const string inventoryFile = "frequency.dat"; // Default .dat file name
  ifstream inFile;
  vector<pair<int, string>> purchases;

  inFile.open( inventoryFile );
  if ( !inFile.is_open() ) // Report error if file failed to open
  {
    throw runtime_error( "Could not find or open " + inventoryFile + "." );
  }

  // Loop while no input problems are encountered
  while ( !inFile.fail() )
  {
    string itemPurchased;
    inFile >> itemPurchased; // Read in item name - part 1 of line

    if ( inFile.fail() ) // Report error if there was a problem with the item name
    {
      throw runtime_error( "Could not retrieve line from file." );
    }
    else
    {
      int itemFrequency;
      inFile >> itemFrequency; // Get frequency number - part 2 of line
      // Pair the values and add to vector
      purchases.push_back( pair<int, string>( itemFrequency, itemPurchased ) );
      inFile.ignore(); // Skip to next line
    }
  }
  if ( !inFile.eof() ) // Report error if unable to get through the entire file
  {
    throw runtime_error( "Something went wrong before reaching the end of " + inventoryFile + "." );
  }
  inFile.close();

  return purchases;
}
