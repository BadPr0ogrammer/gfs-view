// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*


////////////////////////////////////////////////////////////////////////


   //
   //  Warning:
   //
   //     This file is machine generated.
   //
   //     Do not edit by hand.
   //
   //
   //     Created by enum_to_string from file "data_file_type.h"
   //
   //     on January 26, 2017   2:48 pm MST
   //


////////////////////////////////////////////////////////////////////////


using namespace std;


#include <string.h>

#include "grdfiletype_to_string.h"


////////////////////////////////////////////////////////////////////////


ConcatString grdfiletype_to_string(const GrdFileType t)

{

const char * s = (const char *) 0;

switch ( t )  {

   case FileType_None:             s = "FileType_None";             break;
   case FileType_Gb1:              s = "FileType_Gb1";              break;
   case FileType_Gb2:              s = "FileType_Gb2";              break;
   case FileType_NcMet:            s = "FileType_NcMet";            break;
   case FileType_General_Netcdf:   s = "FileType_General_Netcdf";   break;

   case FileType_NcPinterp:        s = "FileType_NcPinterp";        break;
   case FileType_NcCF:             s = "FileType_NcCF";             break;
   case FileType_HdfEos:           s = "FileType_HdfEos";           break;
   case FileType_Bufr:             s = "FileType_Bufr";             break;

   default:
      s = "(bad value)";
      break;

}   //  switch


return ( ConcatString (s) );

}


////////////////////////////////////////////////////////////////////////


bool string_to_grdfiletype(const char * text, GrdFileType & t)

{

     if ( strcmp(text, "FileType_None"          ) == 0 )   { t = FileType_None;             return ( true ); }
else if ( strcmp(text, "FileType_Gb1"           ) == 0 )   { t = FileType_Gb1;              return ( true ); }
else if ( strcmp(text, "FileType_Gb2"           ) == 0 )   { t = FileType_Gb2;              return ( true ); }
else if ( strcmp(text, "FileType_NcMet"         ) == 0 )   { t = FileType_NcMet;            return ( true ); }
else if ( strcmp(text, "FileType_General_Netcdf") == 0 )   { t = FileType_General_Netcdf;   return ( true ); }

else if ( strcmp(text, "FileType_NcPinterp"     ) == 0 )   { t = FileType_NcPinterp;        return ( true ); }
else if ( strcmp(text, "FileType_NcCF"          ) == 0 )   { t = FileType_NcCF;             return ( true ); }
else if ( strcmp(text, "FileType_HdfEos"        ) == 0 )   { t = FileType_HdfEos;           return ( true ); }
else if ( strcmp(text, "FileType_Bufr"          ) == 0 )   { t = FileType_Bufr;             return ( true ); }
   //
   //  nope
   //

return ( false );

}


////////////////////////////////////////////////////////////////////////
