// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

///////////////////////////////////////////////////////////////////////////////
//
//   Filename:   level_info.cc
//
//   Description:
//
//   Mod#   Date      Name           Description
//   ----   ----      ----           -----------
//
///////////////////////////////////////////////////////////////////////////////

using namespace std;

#include <map>

#ifdef _WINDOWS
#include <float.h>
#include <math.h>
#define isnan _isnan
#define strcasecmp strcmpi
#endif

#include "is_bad_data.h"

#include "level_info.h"
#include "leveltype_to_string.h"

///////////////////////////////////////////////////////////////////////////////
//
//  Code for class LevelInfo
//
///////////////////////////////////////////////////////////////////////////////

LevelInfo::LevelInfo() {

   init_from_scratch();
}

///////////////////////////////////////////////////////////////////////////////

LevelInfo::~LevelInfo() {

   clear();
}

///////////////////////////////////////////////////////////////////////////////

LevelInfo::LevelInfo(const LevelInfo &f) {

   init_from_scratch();

   assign(f);
}

///////////////////////////////////////////////////////////////////////////////

LevelInfo & LevelInfo::operator=(const LevelInfo &f) {

   if ( this == &f )  return ( *this );

   assign(f);

   return ( *this );
}

///////////////////////////////////////////////////////////////////////////////

void LevelInfo::init_from_scratch() {

   clear();

   return;
}

///////////////////////////////////////////////////////////////////////////////

void LevelInfo::assign(const LevelInfo &l) {

   clear();

   // Copy
   Type    = l.type();
   TypeNum = l.type_num();
   ReqName = l.req_name();
   Name    = l.name();
   Units   = l.units();
   Upper   = l.upper();
   Lower   = l.lower();

   return;
}

///////////////////////////////////////////////////////////////////////////////

void LevelInfo::clear() {

   // Initialize
   Type = LevelType_None;
   TypeNum = bad_data_int;
   ReqName.clear();
   Name.clear();
   Units.clear();
   Upper  = 0.0;
   Lower  = 0.0;

   return;
}

///////////////////////////////////////////////////////////////////////////////
/* _bp_
void LevelInfo::dump(ostream &out) const {

   // Dump out the contents
   out << "LevelInfo::dump():\n"
       << "  Type    = " << leveltype_to_string(Type) << "\n"
       << "  TypeNum = " << TypeNum << "\n"
       << "  ReqName = " << (ReqName ? ReqName.text() : "(nul)") << "\n"
       << "  Name    = " << (Name ? Name.text() : "(nul)") << "\n"
       << "  Units   = " << (Units ? Units.text() : "(nul)") << "\n"
       << "  Upper   = " << Upper << "\n"
       << "  Lower   = " << Lower << "\n";

   return;
}
*/
///////////////////////////////////////////////////////////////////////////////

void LevelInfo::set_type(LevelType lt) {
   Type = lt;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void LevelInfo::set_type_num(int i) {
   TypeNum = i;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void LevelInfo::set_req_name(const char *str) {
   ReqName = str;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void LevelInfo::set_name(const char *str) {
   Name = str;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void LevelInfo::set_units(const char *str) {
   Units = str;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void LevelInfo::set_upper(double u) {
   Upper = u;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void LevelInfo::set_lower(double l) {
   Lower = l;
   return;
}

///////////////////////////////////////////////////////////////////////////////

void LevelInfo::set_range(double l, double u) {
   Lower = l;
   Upper = u;
   return;
}

///////////////////////////////////////////////////////////////////////////////
