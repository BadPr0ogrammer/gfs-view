// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

////////////////////////////////////////////////////////////////////////

#ifndef  __TEMP_FILE_H__
#define  __TEMP_FILE_H__

////////////////////////////////////////////////////////////////////////

#include "concat_string.h"

////////////////////////////////////////////////////////////////////////

extern ConcatString make_temp_file_name(const char *, const char *);

extern void         remove_temp_file(const char *);

////////////////////////////////////////////////////////////////////////

#endif   /*  __TEMP_FILE_H__  */

////////////////////////////////////////////////////////////////////////
