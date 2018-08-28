// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*




////////////////////////////////////////////////////////////////////////

using namespace std;

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <cmath>

#include "file_linecount.h"
#include "empty_string.h"
#include "vx_log.h"

void my_log(const char *frmt, const char *s = NULL);

////////////////////////////////////////////////////////////////////////


int file_linecount(const char * path)

{

int count = 0;
ifstream in;
char c;

in.open(path);

if ( !in )  {
  my_log("err #%s\n", "0xdc5fe2");

   return 0;
}

count = 0;

while ( in.get(c) )  {

   if ( c == '\n' )  ++count;

}


in.close();

return ( count );

}


////////////////////////////////////////////////////////////////////////
