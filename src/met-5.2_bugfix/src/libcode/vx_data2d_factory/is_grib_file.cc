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
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cmath>

#include <sys/types.h>  
#include <sys/stat.h>  
#include <stdio.h>  

#ifdef _WINDOWS
#include <io.h>
#include <float.h>
#include <math.h>
#define strcasecmp strcmpi
#define isnan _isnan
#endif
void my_log(const char *frmt, const char *s = NULL);

#include "is_grib_file.h"
#include "vx_util.h"
#include "vx_log.h"


////////////////////////////////////////////////////////////////////////


static const char grib_magic [] = "GRIB";

static const int grib_magic_len = strlen(grib_magic);

static const int buf_size = 256;


////////////////////////////////////////////////////////////////////////

#ifdef _WINDOWS
static bool check_grib(const wchar_t * filename, int & version);


bool is_grib1_file(const wchar_t * filename)
{

int version;
bool status = check_grib(filename, version);

if ( !status )  return ( false );

   //
   //  done
   //

return ( version == 1 );

}


////////////////////////////////////////////////////////////////////////


bool is_grib2_file(const wchar_t * filename)

{

int version;
bool status = check_grib(filename, version);

if ( !status )  return ( false );

   //
   //  done
   //

return ( version == 2 );

}


////////////////////////////////////////////////////////////////////////


   //
   //  searches the buffer for the "GRIB" magic cookie
   //

bool check_grib(const wchar_t * filename, int & version)

{

int fd = -1;
char buf[buf_size];
bool found = false;
int i;
long long size, read_size;

   //
   //  open file
   //
  if ( (fd = _wopen(filename, _O_RDONLY | _O_BINARY)) < 0 )  {
    my_log("err #%s\n", "0x7490092d");

   return false;
  }

   //
   //  determine the number of bytes to read
   //
size      = file_size(filename);
read_size = ( buf_size < size ? buf_size : size );

   //
   //  read into the buffer
   //
  if ( _read(fd, buf, read_size) != read_size )  {
    my_log("err #%s\n", "0xaf4a71ed");

   return false;
  }

   //
   //  close file
   //
_close(fd);  fd = -1;

   //
   //  search buffer for grib magic cookie
   //

for ( i=0; i<(read_size - grib_magic_len); ++i)  {
  
   if ( strncmp(&buf[i], grib_magic, grib_magic_len) == 0 )  {
      found = true;
      break;
   }
}

if( !found ) return ( false );

   //
   //  grab version number
   //

version = (int) (buf[i+7]);

   //
   //  done
   //

return ( true );

}
#else

static bool check_grib(const char * filename, int & version);


bool is_grib1_file(const char * filename)
{

int version;
bool status = check_grib(filename, version);

if ( !status )  return ( false );

   //
   //  done
   //

return ( version == 1 );

}


////////////////////////////////////////////////////////////////////////


bool is_grib2_file(const char * filename)

{

int version;
bool status = check_grib(filename, version);

if ( !status )  return ( false );

   //
   //  done
   //

return ( version == 2 );

}


////////////////////////////////////////////////////////////////////////


   //
   //  searches the buffer for the "GRIB" magic cookie
   //

bool check_grib(const char * filename, int & version)

{

int fd = -1;
char buf[buf_size];
bool found = false;
int i;
long long size, read_size;

   //
   //  open file
   //
  if ( (fd = open(filename, O_RDONLY)) < 0 )  {
    my_log("err #%s\n", "0x6df877d1");

   return false;
  }

   //
   //  determine the number of bytes to read
   //
size      = file_size(filename);
read_size = ( buf_size < size ? buf_size : size );

   //
   //  read into the buffer
   //
  if ( read(fd, buf, read_size) != read_size )  {
    my_log("err #%s\n", "0x9c9fffbb");

   return false;
  }

   //
   //  close file
   //
close(fd);  fd = -1;

   //
   //  search buffer for grib magic cookie
   //

for ( i=0; i<(read_size - grib_magic_len); ++i)  {

   if ( strncmp(&buf[i], grib_magic, grib_magic_len) == 0 )  {
      found = true;
      break;
   }
}

if( !found ) return ( false );

   //
   //  grab version number
   //

version = (int) (buf[i+7]);

   //
   //  done
   //

return ( true );

}

#endif

////////////////////////////////////////////////////////////////////////
