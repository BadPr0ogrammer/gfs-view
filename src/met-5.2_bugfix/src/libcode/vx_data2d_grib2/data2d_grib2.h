// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*


////////////////////////////////////////////////////////////////////////


#ifndef  __MET_VX_DATA_2D_GRIB2_H__
#define  __MET_VX_DATA_2D_GRIB2_H__


////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <vector>

#include "data_plane.h"
#include "data_class.h"
#include "var_info_grib2.h"

extern "C" {
  #include "grib2.h"
}


////////////////////////////////////////////////////////////////////////


typedef struct Grib2Record {
   long ByteOffset;
   int Index;
   int NumFields;
   int RecNum;
   int FieldNum;
   int Discipline;
   int PdsTmpl;
   string ParmName;
   int ParmCat;
   int Parm;
   int LvlTyp;
   double LvlVal1;
   double LvlVal2;
   int RangeTyp;
   int RangeVal;
   int ResCompFlag;
   bool ProbFlag;
   double ProbLower;
   double ProbUpper;
   int ProbType;
   unixtime InitTime;
   unixtime ValidTime;
   int LeadTime;
   int Accum;
   int  ens_type;
   int ens_number;
   
   // _bp_
   int ipdtmpl_ptr[200];
} Grib2Record;


////////////////////////////////////////////////////////////////////////


class MetGrib2DataFile : public Met2dDataFile {

   private:

      void grib2_init_from_scratch();

      MetGrib2DataFile(const MetGrib2DataFile &);
      MetGrib2DataFile & operator=(const MetGrib2DataFile &);

      FILE *FileGrib2;

      map<string,string> PairMap;
      map<string,Grib2Record*> NameRecMap;

      int ScanMode;


      //
      //  utilities to read a GRIB2 information
      //


      void read_grib2_grid(gribfield *gfld);

      long read_grib2_record( long offset,
                              g2int unpack,
                              g2int ifld,
                              gribfield* &gfld,
                              unsigned char* &cgrib,
                              g2int &numfields
                            );

      void read_grib2_record_list();

      DataPlane check_uv_rotation( VarInfoGrib2 *vinfo,
                                   Grib2Record *rec,
                                   DataPlane plane
                                 );

      DataPlaneArray check_derived( VarInfoGrib2 *vinfo );


   public:
     // _bp_ was private
     void find_record_matches(VarInfoGrib2* vinfo,
       vector<Grib2Record*> &listMatchExact,
       vector<Grib2Record*> &listMatchRange
     );
     bool read_grib2_record_data_plane(Grib2Record *rec, DataPlane &plane);
     vector<Grib2Record*> RecList;

      MetGrib2DataFile();
     ~MetGrib2DataFile();

         //
         //  set stuff
         //

         //
         //  get stuff
         //

      GrdFileType file_type() const;

         //  retrieve the first matching data plane

      bool data_plane(VarInfo &, DataPlane &);

         //  retrieve all matching data planes

      int data_plane_array(VarInfo &, DataPlaneArray &);

         //  retrieve the index of the first matching record

      int index(VarInfo &);

         //
         //  do stuff
         //
#ifdef _WINDOWS
      bool open  (const wchar_t * filename);
#else
      bool open  (const char * filename);
#endif
      void close ();

      /// _bp_ void dump(ostream &, int = 0) const;

      static ConcatString build_magic(Grib2Record *rec);

};


////////////////////////////////////////////////////////////////////////


inline GrdFileType MetGrib2DataFile::file_type () const { return ( FileType_Gb2 ); }


////////////////////////////////////////////////////////////////////////


#endif   /*  __MET_VX_DATA_2D_GRIB2_H__  */


////////////////////////////////////////////////////////////////////////


