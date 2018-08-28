

// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*




////////////////////////////////////////////////////////////////////////


#ifndef  __IS_GRIB_FILE_H__
#define  __IS_GRIB_FILE_H__


////////////////////////////////////////////////////////////////////////

#ifdef _WINDOWS
extern bool is_grib1_file(const wchar_t * filename);
extern bool is_grib2_file(const wchar_t * filename);
#else
extern bool is_grib1_file(const char * filename);
extern bool is_grib2_file(const char * filename);
#endif

////////////////////////////////////////////////////////////////////////


#endif   /*  __IS_GRIB_FILE_H__  */


////////////////////////////////////////////////////////////////////////


