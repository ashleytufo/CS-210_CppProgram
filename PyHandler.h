#ifndef PYHANDLER_H
#define PYHANDLER_H
#include <Python.h>
#include <string>

using namespace std;


class PyHandler
{
public:
  PyHandler(); // Default constructor
  // Public member methods to call python functions
  void CallProcedure( string pName );
  int callIntFunc( string proc, string param );
  int callIntFunc( string proc, int param );
};
#endif
