// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

////////////////////////////////////////////////////////////////////////

using namespace std;

#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "temp_file.h"
#include "file_exists.h"
#include "vx_log.h"

////////////////////////////////////////////////////////////////////////
#ifdef _WINDOWS
#include "windows.h"
#endif
void my_log(const char *frmt, const char *s = NULL);

ConcatString make_temp_file_name(const char *prefix, const char *suffix) {

#ifdef _WINDOWS
  char buf[300], buf1[300];
  GetTempPathA(300, buf);
  GetTempFileNameA(buf, prefix, 0, buf1);
  ConcatString s(buf1);

#else
   int i, pid;
   ConcatString s;
   const int max_tries = 1000;

   //
   // Retrieve the current process id
   //
   pid = (int) getpid();

   i = -1;

   do {
      i++;

      if(i > max_tries) {
        my_log("warn #%s\n", "0x36d304c2");

        return s;
      }

      //
      // Initialize
      //
      s.erase();

      //
      // Build the file name
      //
      if(prefix) s << prefix << '_';
      s << pid << '_' << i;
      if(suffix) s << '_' << suffix;

   } while(file_exists(s));
#endif
   return(s);
}

////////////////////////////////////////////////////////////////////////

void remove_temp_file(const char *file_name) {
   int errno;

   //
   // Attempt to remove the file and print out any error message
   //
   if((errno = remove(file_name)) != 0) {
     my_log("err #%s\n", "0x11228965");

   }

   return;
}

////////////////////////////////////////////////////////////////////////
