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
#include <unistd.h>
#include <stdlib.h>
#include <cmath>

#include "file_exists.h"


////////////////////////////////////////////////////////////////////////

#ifdef _WINDOWS
#include "windows.h"
bool file_exists(const wchar_t * path)
#else
bool file_exists(const char * path)
#endif
{
#ifdef _WINDOWS
  DWORD attr = GetFileAttributesW(path);
  return  attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY);
#else

int status;

status = access(path, F_OK);

if ( status == 0 )  return ( true );

return ( false );
#endif
}


////////////////////////////////////////////////////////////////////////
