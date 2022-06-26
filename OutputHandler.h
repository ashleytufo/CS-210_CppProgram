#ifndef OUTPUTHANDLER_H
#define OUTPUTHANDLER_H
#include "PyHandler.h"
#include <string>
#include <vector>

using namespace std;


class OutputHandler
{
public:
  // Preset console color options
  enum colorOptions
  {
    BLUE         = 1,
    GREEN        = 2,
    BABYBLUE     = 3,
    RED          = 4,
    PURPLE       = 5,
    YELLOW       = 6,
    WHITE        = 7,
    BRIGHTBLUE   = 9,
    BRIGHTGREEN  = 10,
    AQUA         = 11,
    BRIGHTRED    = 12,
    BRIGHTPURPLE = 13,
    PALEYELLOW   = 14,
    BRIGHTWHITE  = 15,
  };

  OutputHandler(); // Default constructor
  // Public member methods
  string formatText( int size, char c );
  void setConsoleColor( int foreground = WHITE ); // Default to white
  void printItemFrequency( string itemName );
  void printHistogram( vector<pair<int, string>>& purchases );
  void printMenu();

private:
  // Declared private member variables
  PyHandler m_PyHandle;
};
#endif
