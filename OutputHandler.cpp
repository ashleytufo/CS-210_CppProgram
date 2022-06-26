#include "OutputHandler.h"
#include <Windows.h>
#include <algorithm>
#include <iomanip>
#include <iostream>


OutputHandler::OutputHandler()
{
  // Initialize Python caller
  m_PyHandle = PyHandler();
  // Resize console window so lists will be fully visible
  HWND window = GetConsoleWindow(); // Console object
  RECT winSize;                     // to store window rectangle size
  const int width  = 650;
  const int height = 700;
  GetWindowRect( window, &winSize );
  // Set width and height of window
  MoveWindow( window, winSize.left, winSize.top, width, height, TRUE );
}

string OutputHandler::formatText( int size, char c )
{
  string lineFill;

  // Add the provided character to the line filler
  for ( int i = 0; i < size; i++ )
  {
    lineFill += c;
  }
  return lineFill;
}

void OutputHandler::setConsoleColor( int foreground )
{
  HANDLE console = GetStdHandle( STD_OUTPUT_HANDLE ); // Console object
  // Set the text color of the console. If no color is passed, set to white
  SetConsoleTextAttribute( console, foreground );
}

void OutputHandler::printItemFrequency( string itemName )
{
  setConsoleColor( BRIGHTGREEN ); // Set console color for frequency results

  // Call python function to return this item's frequency
  int itemCount = m_PyHandle.callIntFunc( "GetItemSoldFrequency", itemName );

  if ( itemCount == 0 ) // Report that this item isn't in the list
  {
    cout << "'" << itemName << "' not found in today's sales." << endl;
  }
  else if ( itemCount == -1 ) // Report that the entered name is invalid (like only numbers)
  {
    throw runtime_error( "Invalid input: " + itemName + "! Item names may only contain uppercase letters,"
                         + " lowercase letters, spaces, and hyphens." );
  }
  else
  {
    // Output the number of times the item was sold today
    cout << "Total " << itemName << " sold today: " << itemCount << endl;
  }
  cout << flush;
  setConsoleColor(); // Reset console color
}

void OutputHandler::printHistogram( vector<pair<int, string>>& purchases )
{
  // Set constants for printing
  const int lineLen         = 33;
  const int lineSpaceLen    = 10;
  const string lineSpacer   = formatText( lineSpaceLen, ' ' );
  const string headFootLine = formatText( lineLen, '-' );

  // Sort the pairs of items-frequencies in order of highest frequency first
  sort( purchases.rbegin(), purchases.rend() );
  setConsoleColor( PALEYELLOW ); // Set console color for histogram results table
  // Print title
  cout << lineSpacer << "TODAY'S SALES" << lineSpacer << endl;
  cout << headFootLine << endl;

  int color = BLUE; // Default value for text color
  unsigned int i;
  for ( i = 0; i < purchases.size(); i++ ) // Loop through pairs in the list
  {
    // Create a copy of the pair
    pair<int, string> nextItem = purchases.at( i );
    int itemQuantity           = nextItem.first;  // Pull the first part of the pair as frequency
    string itemName            = nextItem.second; // Pull the second part of the pair as name

    // Get a string of asterisks (* X quantity)
    string histoBarStr = formatText( itemQuantity, '*' );
    setConsoleColor( color ); // Set console text color for this histogram line
    // Print this item
    cout << setfill( ' ' ) << setw( 15 ) << right << itemName << " | " << histoBarStr;
    cout << endl << flush;

    // Loop through colors from blue (1) to white (7) so items display in different colors
    if ( color >= WHITE )
    {
      color = BLUE; // Reset back to 1
    }
    else
    {
      ++color;
    }
  }
  setConsoleColor( PALEYELLOW ); // Set console color for histogram results table
  cout << headFootLine << endl;  // Print footer line of table
  setConsoleColor();             // Rest console color
  cout << endl;
}

void OutputHandler::printMenu()
{
  setConsoleColor(); // Reset console color - always start with a white menu
  cout << endl;
  // Print menu options
  cout << "1. Show daily purchases itemized report" << endl;
  cout << "2. Show daily purchase frequency for an item" << endl;
  cout << "3. Show daily purchases histogram" << endl;
  cout << "4. Exit" << endl;
  cout << "What would you like to do? " << flush;
}
