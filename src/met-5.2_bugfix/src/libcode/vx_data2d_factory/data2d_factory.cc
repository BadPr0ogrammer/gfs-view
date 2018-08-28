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
#include <exception>

#ifdef _WINDOWS
#include <float.h>
#include <math.h>
#define isnan _isnan
#define strcasecmp strcmpi
#endif
void my_log(const char *frmt, const char *s = NULL);

#include "data2d_factory.h"
#include "data2d_factory_utils.h"
#include "grdfiletype_to_string.h"
#include "data2d_grib.h"
#include "data2d_grib2.h"
/*
#include "data2d_nc_met.h"
#include "data2d_nc_pinterp.h"
#include "data2d_nccf.h"
*/
#ifdef WITH_GRIB2
   #include "data2d_grib2.h"
#endif

#include "vx_log.h"

////////////////////////////////////////////////////////////////////////
//
//  Code for class Met2dDataFileFactory
//
////////////////////////////////////////////////////////////////////////

Met2dDataFile * Met2dDataFileFactory::new_met_2d_data_file(GrdFileType type) {
   Met2dDataFile *mtddf = (Met2dDataFile *) 0;

   //
   // Switch on file type and instantiate the appropriate class.
   // The Met2dDataFile object is allocated and needs to be deleted by caller.
   //

   switch(type) {
     /* _bp_
      case FileType_Gb1:
         mtddf = new MetGrib1DataFile;
         break;
     */
      case FileType_Gb2:
         mtddf = new MetGrib2DataFile;
         break;
         /*
      case FileType_NcMet:
         mtddf = new MetNcMetDataFile;
         break;

      case FileType_NcPinterp:
         mtddf = new MetNcPinterpDataFile;
         break;

      case FileType_NcCF:
         mtddf = new MetNcCFDataFile;
         break;

      case FileType_HdfEos:

         mlog << Error << "\nMet2dDataFileFactory::new_met_2d_data_file() -> "
              << "Support for GrdFileType = \"" << grdfiletype_to_string(type)
              << "\" not yet implemented!\n\n";
         exit(1);
         break;

      case FileType_Bufr:

         mlog << Error << "\nMet2dDataFileFactory::new_met_2d_data_file() -> "
              << "cannot use this factory to read files of type \""
              << grdfiletype_to_string(type) << "\"\n\n";
         exit(1);
         break;
         */
      case FileType_None:
         // For FileType_None, silently return a NULL pointer
         mtddf = (Met2dDataFile *) 0;
         break;

      default:
        my_log("err #%s\n", "0x4596e699");

        return NULL;

   } // end switch

   return(mtddf);
}

////////////////////////////////////////////////////////////////////////
#ifdef _WINDOWS
Met2dDataFile * Met2dDataFileFactory::new_met_2d_data_file(const wchar_t *filename) {
#else
Met2dDataFile * Met2dDataFileFactory::new_met_2d_data_file(const char *filename) {
#endif
   GrdFileType type;
   Met2dDataFile *mtddf = (Met2dDataFile *) 0;

   //
   // Determine the file type
   //
   type = grd_file_type(filename);

   //
   // Create a new data file object and call open if the point is non-zero
   //
   if((mtddf = new_met_2d_data_file(type))) {
      if(!(mtddf->open(filename))) {
        my_log("err #%s\n", "0x2696490");

         return NULL;
      }
   }

   return(mtddf);
}

////////////////////////////////////////////////////////////////////////
#ifdef _WINDOWS
Met2dDataFile * Met2dDataFileFactory::new_met_2d_data_file(const wchar_t *filename, GrdFileType type) {
#else
Met2dDataFile * Met2dDataFileFactory::new_met_2d_data_file(const char *filename, GrdFileType type) {
#endif
   Met2dDataFile *mtddf = (Met2dDataFile *) 0;

   //
   // Use the file type, if valid
   //
   if(type != FileType_None) {
      if((mtddf = new_met_2d_data_file(type))) {
         if(!(mtddf->open(filename))) {
           my_log("err #%s\n", "0xe52fe2ed");

           return NULL;
         }
      }
   }

   //
   // Otherwise determine the type from the file name
   //
   else {
      mtddf = new_met_2d_data_file(filename);
   }

   return(mtddf);
}

///////////////////////////////////////////////////////////////////////////////
