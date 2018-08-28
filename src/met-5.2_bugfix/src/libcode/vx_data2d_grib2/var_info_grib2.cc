
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

///////////////////////////////////////////////////////////////////////////////
//
//   Filename:   var_info_grib2.cc
//
//   Description:
//
//   Mod#   Date      Name           Description
//   ----   ----      ----           -----------
//
///////////////////////////////////////////////////////////////////////////////

using namespace std;

#include <map>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/// _bp_ #include <regex.h>
void my_log(const char *frmt, const char *s = NULL);

#ifdef _WINDOWS
#include <float.h>
#include <math.h>
#define isnan _isnan
#define strcasecmp strcmpi
#endif

#include "var_info_grib2.h"

#include "grib_strings.h"
#include "math_constants.h"
#include "vx_math.h"
#include "vx_log.h"
#include "vx_util.h"
#include "vx_data2d.h"
#include "vx_config.h"


///////////////////////////////////////////////////////////////////////////////
//
//  Code for class VarInfoGrib2
//
///////////////////////////////////////////////////////////////////////////////

VarInfoGrib2::VarInfoGrib2() {

   init_from_scratch();
}

///////////////////////////////////////////////////////////////////////////////

VarInfoGrib2::~VarInfoGrib2() {

   clear();
}

///////////////////////////////////////////////////////////////////////////////

VarInfoGrib2::VarInfoGrib2(const VarInfoGrib2 &f) {

   init_from_scratch();

   assign(f);
}

///////////////////////////////////////////////////////////////////////////////

VarInfoGrib2 & VarInfoGrib2::operator=(const VarInfoGrib2 &f) {

   if ( this == &f )  return ( *this );

   assign(f);

   return ( *this );
}

///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::init_from_scratch() {

   // First call the parent's initialization
   VarInfo::init_from_scratch();

   clear();

   Discipline = 0;

   return;
}

///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::assign(const VarInfoGrib2 &v) {

   // First call the parent's assign
   VarInfo::assign(v);

   // Copy
   Record     = v.record();
   Discipline = v.discipline();
   MTable     = v.m_table();
   LTable     = v.l_table();
   Tmpl       = v.tmpl();
   ParmCat    = v.parm_cat();
   Parm       = v.parm();
   Process    = v.process();
   EnsType    = v.ens_type();
   DerType    = v.der_type();

   return;
}

///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::clear() {

   // First call the parent's clear
   VarInfo::clear();

   // Initialize
   Record     = bad_data_int;
   Discipline = bad_data_int;
   MTable     = bad_data_int;
   LTable     = bad_data_int;
   Tmpl       = bad_data_int;
   ParmCat    = bad_data_int;
   Parm       = bad_data_int;
   Process    = bad_data_int;
   EnsType    = bad_data_int;
   DerType    = bad_data_int;

   return;
}

///////////////////////////////////////////////////////////////////////////////
/* _bp_
void VarInfoGrib2::dump(ostream &out) const {

   // Dump out the contents
   out << "VarInfoGrib2::dump():\n"
       << "  Record     = " << Record     << "\n"
       << "  Discipline = " << Discipline << "\n"
       << "  MTable     = " << MTable     << "\n"
       << "  LTable     = " << LTable     << "\n"
       << "  Tmpl       = " << Tmpl       << "\n"
       << "  ParmCat    = " << ParmCat    << "\n"
       << "  Parm       = " << Parm       << "\n"
       << "  Process    = " << Process    << "\n"
       << "  EnsType    = " << EnsType    << "\n"
       << "  DerType    = " << DerType    << "\n";

   return;
}
*/
///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::set_record(int v) {
   Record = v;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::set_discipline(int v) {
   Discipline = v;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::set_m_table(int v) {
   MTable = v;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::set_l_table(int v) {
   LTable = v;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::set_tmpl(int v) {
   Tmpl = v;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::set_parm_cat(int v) {
   ParmCat = v;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::set_parm(int v) {
   Parm = v;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::set_process(int v) {
   Process = v;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::set_ens_type(int v) {
   EnsType = v;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::set_der_type(int v) {
   DerType = v;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::set_magic(const ConcatString &s) {

   // Validate the magic_string
   VarInfo::set_magic(s);

   // Store the magic string
   MagicStr = s;

}

///////////////////////////////////////////////////////////////////////////////

void VarInfoGrib2::set_dict(Dictionary & dict) {

   VarInfo::set_dict(dict);

   int tab_match = -1;
   Grib2TableEntry tab;
   ConcatString field_name = dict.lookup_string(conf_key_name,            false);
   ConcatString ens        = dict.lookup_string (conf_key_GRIB_ens,       false);
   int field_disc          = dict.lookup_int   (conf_key_GRIB2_disc,      false);
   int field_parm_cat      = dict.lookup_int   (conf_key_GRIB2_parm_cat,  false);
   int field_parm          = dict.lookup_int   (conf_key_GRIB2_parm,      false);
   int cntr                = dict.lookup_int   (conf_key_GRIB2_cntr,      false);
   int ltab                = dict.lookup_int   (conf_key_GRIB2_ltab,      false);
   int mtab                = dict.lookup_int   (conf_key_GRIB2_mtab,      false);

   //  if the name is specified, use it
   if( !field_name.empty() ){

      set_name( field_name );
      set_req_name( field_name );

      //  look up the name in the grib tables
      if( !GribTable->lookup_grib2(field_name, field_disc, field_parm_cat, field_parm, mtab, cntr, ltab,
                                  tab, tab_match) &&
          field_name != "PROB" ){
        my_log("err #%s\n", "0xa64778c9");

        return;
      }

   }

   //  if the field name is not specified, look for and use indexes
   else {

      //  if either the field name or the indices are specified, bail
      if( bad_data_int == field_disc ||
          bad_data_int == field_parm_cat ||
          bad_data_int == field_parm ){
        my_log("err #%s\n", "0x27421c3");

        return;
      }

      //  use the specified indexes to look up the field name
      if( !GribTable->lookup_grib2(field_disc, field_parm_cat, field_parm, mtab, cntr, ltab,tab) ){
        my_log("err #%s\n", "0xfe3e2fc8");

         return;
      }

      //  use the lookup parameter name
      field_name = tab.parm_name;
   }
   set_ens         (ens);
   //  set the matched parameter lookup information
   set_name      ( field_name    );
   set_req_name  ( field_name    );
   if( field_name != "PROB" ){
      set_discipline( tab.index_a   );
      set_parm_cat  ( tab.index_b   );
      set_parm      ( tab.index_c   );
      set_units     ( tab.units     );
      set_long_name ( tab.full_name );
   }

   //  call the parent to set the level information
   set_level_info_grib(dict);

   //  if the level type is a record number, set the data member
   set_record( Level.type() == LevelType_RecNumber ? nint(Level.lower()) : -1 );

   //  if the field is not probabilistic, work is done
   if( field_name != "PROB" ) return;

   //  check for a probability dictionary setting
   Dictionary* dict_prob;
   if( NULL == (dict_prob = dict.lookup_dictionary(conf_key_prob, false)) ){
     my_log("err #%s\n", "0x3d293930");

     return;
   }

   //  gather information from the prob dictionary
   ConcatString prob_name = dict_prob->lookup_string(conf_key_name);
   field_disc       = dict_prob->lookup_int   (conf_key_GRIB2_disc,     false);
   field_parm_cat   = dict_prob->lookup_int   (conf_key_GRIB2_parm_cat, false);
   field_parm       = dict_prob->lookup_int   (conf_key_GRIB2_parm,     false);
   double thresh_lo = dict_prob->lookup_double(conf_key_thresh_lo,      false);
   double thresh_hi = dict_prob->lookup_double(conf_key_thresh_hi,      false);

   //  look up the probability field abbreviation
   if( !GribTable->lookup_grib2(prob_name, field_disc, field_parm_cat, field_parm, mtab, cntr, ltab,
                               tab, tab_match) ){
     my_log("err #%s\n", "0x175de450");

     return;
   }

   set_discipline ( tab.index_a );
   set_parm_cat   ( tab.index_b );
   set_parm       ( tab.index_c );
   set_p_flag     ( true        );
   set_p_units    ( tab.units   );
   set_units      ( tab.units   );

   set_prob_info_grib(prob_name, thresh_lo, thresh_hi);

}

///////////////////////////////////////////////////////////////////////////////

bool VarInfoGrib2::is_precipitation() const {
   return Discipline == 0 &&
          ParmCat    == 1 &&
          (
             Parm == 3  ||       //  PWAT
             Parm == 7  ||       //  PRATE
             Parm == 8  ||       //  APCP
             Parm == 9  ||       //  NCPCP
             Parm == 10          //  ACPCP
          );
}

///////////////////////////////////////////////////////////////////////////////

bool VarInfoGrib2::is_specific_humidity() const {
   return Discipline == 0 &&
          ParmCat    == 1 &&
          Parm       == 0;
}

///////////////////////////////////////////////////////////////////////////////

bool VarInfoGrib2::is_u_wind() const {
   return(ReqName == ugrd_abbr_str ||
          (Discipline == 0 &&
           ParmCat    == 2 &&
           Parm       == 2));
}

///////////////////////////////////////////////////////////////////////////////

bool VarInfoGrib2::is_v_wind() const {
   return(ReqName == vgrd_abbr_str ||
          (Discipline == 0 &&
           ParmCat    == 2 &&
           Parm       == 3));
}

///////////////////////////////////////////////////////////////////////////////

bool VarInfoGrib2::is_wind_speed() const {
   return(ReqName == wind_abbr_str ||
          (Discipline == 0 &&
           ParmCat    == 2 &&
           Parm       == 1));
}

///////////////////////////////////////////////////////////////////////////////

bool VarInfoGrib2::is_wind_direction() const {
   return(ReqName == wdir_abbr_str ||
          (Discipline == 0 &&
           ParmCat    == 2 &&
           Parm       == 0));
}

////////////////////////////////////////////////////////////////////////

LevelType VarInfoGrib2::g2_lty_to_level_type(int lt) {
   LevelType t = LevelType_None;

   //  from: http://www.nco.ncep.noaa.gov/pmb/docs/grib2/grib2_table4-5.shtml
   switch( lt ){
      case   8: // Nominal Top of the Atmosphere
         t = LevelType_Accum;
         break;

      case 100: // Isobaric Surface
      case 108: // Level at Specified Pressure Difference from Ground to Level
         t = LevelType_Pres;
         break;

      case   1: // Ground or Water Surface
      case 101: // Mean Sea Level
      case 102: // Specific Altitude Above Mean Sea Level
      case 103: // Specified Height Level Above Ground
         t = LevelType_Vert;
         break;

      default:
         t = LevelType_None;
         break;
   }

   return t;
}

////////////////////////////////////////////////////////////////////////

double VarInfoGrib2::g2_time_range_unit_to_sec(int ind) {

   //  from: http://www.nco.ncep.noaa.gov/pmb/docs/grib2/grib2_table4-4.shtml
   double sec_per_unit = -1;
   switch(ind) {
      case 0:     sec_per_unit = sec_per_minute;             break;    // minute
      case 1:     sec_per_unit = sec_per_hour;               break;    // hour
      case 2:     sec_per_unit = sec_per_day;                break;    // day
      case 3:     sec_per_unit = sec_per_day*30.0;           break;    // month
      case 4:     sec_per_unit = sec_per_day*365.0;          break;    // year
      case 5:     sec_per_unit = sec_per_day*365.0*10.0;     break;    // decade
      case 6:     sec_per_unit = sec_per_day*365.0*30.0;     break;    // normal (30 years)
      case 7:     sec_per_unit = sec_per_day*365.0*100.0;    break;    // century
      case 10:    sec_per_unit = sec_per_hour*3.0;           break;    // 3 hours
      case 11:    sec_per_unit = sec_per_hour*6.0;           break;    // 6 hours
      case 12:    sec_per_unit = sec_per_hour*12.0;          break;    // 12 hours
      case 13:    sec_per_unit = 1.0;                        break;    // second
   }
   return sec_per_unit;
}

////////////////////////////////////////////////////////////////////////
