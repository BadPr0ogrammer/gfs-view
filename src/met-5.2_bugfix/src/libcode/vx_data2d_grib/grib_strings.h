// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

////////////////////////////////////////////////////////////////////////

#ifndef __GRIB_STRINGS_H__
#define __GRIB_STRINGS_H__

////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////

#include "vx_util.h"

////////////////////////////////////////////////////////////////////////

static const char missing_str[]        = "MISSING";
static const char ugrd_abbr_str[]      = "UGRD";
static const char vgrd_abbr_str[]      = "VGRD";
static const char ugrd_vgrd_abbr_str[] = "UGRD_VGRD";
static const char wind_abbr_str[]      = "WIND";
static const char wdir_abbr_str[]      = "WDIR";

////////////////////////////////////////////////////////////////////////

//
// List of GRIB1 precipitation variable names
//
static const char *grib_precipitation_abbr[] = {
   "PWAT",    // Code  54, Precipitable water, kg/m^2
   "PRATE",   // Code  59, Precipitation rate, kg/m^2/s
   "APCP",    // Code  61, Total precipitation, kg/m^2
   "NCPCP",   // Code  62, Large scale precipitation (non-conv.), kg/m^2
   "ACPCP",   // Code  63, Convective precipitation, kg/m^2
   "CPRAT",   // Code 214, Convective Precipitation rate, kg/m^2/s
   "RPRATE",  // Code 223, Rain Precipitation Rate, kg/m^2/s
   "SPRATE",  // Code 224, Snow Precipitation Rate, kg/m^2/s
   "FPRATE",  // Code 225, Freezing Rain Precipitation Rate, kg/m^2/s
   "IPRATE",  // Code 226, Ice Pellets Precipitation Rate, kg/m^2/s
   "PSPCP",   // Code 252, Pseudo-Precipitation, kg/m^2
   "LSPA",    // Code 154, Land Surface Precipitation Accumulation (LSPA), kg/m^2
   "ASNOW",   // Code 161, Frozen precipitation (e.g. snowfall), kg/m^2
   "ARAIN",   // Code 162, Liquid precipitation (rainfall), kg/m^2
   "ASNOW",   // Code 161, Frozen precipitation (e.g. snowfall), kg/m^2
   "ARAIN",   // Code 162, Liquid precipitation (rainfall), kg/m^2
   "APCPN",   // Code 202, Total precipitation (nearest grid point), kg/m^2
   "ACPCPN",  // Code 206, Convective precipitation (nearest grid point), kg/m^2
   "CPRAT"    // Code 214, Convective precip. rate, kg/m^2/s
};

//
// Number of GRIB1 precipitation variable names
//
static const int n_grib_precipitation_abbr =
                     sizeof(grib_precipitation_abbr)/
                    sizeof(*grib_precipitation_abbr);

////////////////////////////////////////////////////////////////////////

//
// List of GRIB1 specific humidity abbreviations
//
static const char *grib_specific_humidity_abbr[] = {
   "SPFH",  // Code  51, Specific humidity, kg/kgSpecific humidity, kg kg-1
   "QZ0",   // Code 195, Specific humidity at top of viscous sublayer, kg/kg
   "QMAX",  // Code 204, Maximum specific humidity at 2m, kg/kg
   "QMIN"   // Code 205, Minimum specific humidity at 2m, kg/kg
};

//
// Number of GRIB1 specific humidity abbreviations
//
static const int n_grib_specific_humidity_abbr =
                     sizeof(grib_specific_humidity_abbr)/
                    sizeof(*grib_specific_humidity_abbr);

////////////////////////////////////////////////////////////////////////
//
// NCEP Office Note 388 - Table 2
// Grib Code to Units and Abbreviations
//
// (1) GRIB Code
// (2) GRIB Code Name
// (3) GRIB Code Units
// (4) GRIB Code Abbreviation
//
////////////////////////////////////////////////////////////////////////

struct GribCodeData {
   int         code;
   const char *name;
   const char *unit;
   const char *abbr;
};

////////////////////////////////////////////////////////////////////////
//
// NCEP Office Note 388 - Table 3
// Level Types and Values
//
// (1) Level Value
// (2) Level Type:
//     0 -> no specific level
//     2 -> vertical level type
//     3 -> pressure level type
// (3) Level Flag:
//     Octets 11 and 12 in the PDS:
//     0 -> ignore them
//     1 -> combine them to form one number
//     2 -> octet 11 contains the beginning of a range and
//          octet 12 contains the end of a range
// (4) Level Name
// (5) Level Abbreviation
//
////////////////////////////////////////////////////////////////////////

struct GribLevelData {
   int         level;
   int         type;
   int         flag;
   const char *name;
   const char *abbr;
};

////////////////////////////////////////////////////////////////////////
//
// GRIB tables: http://www.nco.ncep.noaa.gov/pmb/docs/on388/table3.html
// Last Updated 05/11/2009
//
////////////////////////////////////////////////////////////////////////

static const GribLevelData grib_level_list[] = {
   {   1, 2, 0, "Surface", "SFC" }, /// Ground or water surface
   {   2, 0, 0, "Cloud base", "CBL" }, 
   {   3, 0, 0, "Cloud top", "CTL" }, 
   {   4, 0, 0, "0 deg isotherm", "0DEG" }, 
   {   5, 0, 0, "Adiabatic condensation lifted", "ADCL" }, ///  Level of      from the surface
   {   6, 0, 0, "Maximum wind", "MWSL" }, 
   {   7, 0, 0, "Tropopause", "TRO" }, 
   {   8, 0, 0, "Nominal top of atmosphere", "NTAT" },
   {   9, 0, 0, "Sea bottom", "SEAB" }, 
   {  20, 0, 2, "Isothermal", "TMPL" }, ///  (temperature in 1/100 K in octets 11 and 12)  level
   { 100, 3, 1, "Isobaric", "ISBL" }, 
   { 101, 3, 2, "Between two isobaric", "ISBY" }, 
   { 102, 2, 0, "Mean sea level", "MSL" }, 
   { 103, 2, 1, "Altitude above MSL", "GPML" }, 
   { 104, 2, 2, "Between two altitudes above MSL", "GPMY" },
   { 105, 2, 1, "Height above ground", "HTGL" }, 
   { 106, 2, 2, "Between two height above ground", "HTGY" }, 
   { 107, 0, 1, "Sigma", "SIGL" }, 
   { 108, 0, 2, "Between two sigma", "SIGY" }, 
   { 109, 0, 1, "Hybrid", "HYBL" }, 
   { 110, 0, 2, "Between two hybrid", "HYBY" },
   { 111, 2, 1, "Depth below land surface", "DBLL" }, 
   { 112, 2, 2, "Between two depths below land surface", "DBLY" }, 
   { 113, 0, 1, "Isentropic", "THEL" }, 
   { 114, 0, 2, "Between two isentropic", "THEY" }, 
   { 115, 0, 1, "At pressure difference from ground", "SPDL" }, /// Level at specified pressure difference from ground to level
   { 116, 0, 2, "Between two pressure difference from ground", "SPDY" }, /// Layer   to level levels at specified 
   { 117, 0, 1, "Potential vorticity surface", "PVL" }, 
   { 119, 0, 1, "NAM", "NAML" }, 
   { 120, 0, 2, "Between two NAM", "NAMY" }, 
   { 121, 0, 2, "Between two isobaric (high precision)", "IBYH" }, /// Layer surfaces
   { 125, 2, 1, "Above ground (high precision)", "HGLH" }, /// Height Specified level 
   { 126, 3, 1, "Isobaric level", "ISBP" }, 
   { 128, 0, 2, "Between two sigma (high precision)", "SGYH" }, 
   { 141, 0, 2, "Between two isobaric (mixed precision)", "IBYM" }, ///Layer  surfaces 
   { 160, 2, 1, "Depth below sea level", "DBSL" }, 
   { 200, 0, 0, "Entire atmosphere", "EATM" }, /// (considered as a single layer)
   { 201, 0, 0, "Entire ocean", "EOCN" }, ///  (considered as a single layer)
   { 204, 0, 0, "Highest tropospheric freezing", "HTFL" },
   { 206, 0, 0, "Grid scale cloud bottom", "GCBL" }, 
   { 207, 0, 0, "Grid scale cloud top", "GCTL" }, 
   { 209, 0, 0, "Boundary layer cloud bottom", "BCBL" },
   { 210, 0, 0, "Boundary layer cloud top", "BCTL" }, 
   { 211, 0, 0, "Boundary layer cloud", "BCY" }, 
   { 212, 0, 0, "Low cloud bottom", "LCBL" }, 
   { 213, 0, 0, "Low cloud top", "LCTL" }, 
   { 214, 0, 0, "Low cloud", "LCY" }, 
   { 215, 0, 0, "Cloud ceiling", "CEIL" }, 
   { 216, 0, 0, "Cumulonimbus base", "CBB" }, /// (m)
   { 217, 0, 0, "Cumulonimbus top", "CBT" }, /// (m)
   { 220, 0, 0, "Planetary boundary layer", "PBLRI" }, //   Layer (derived from Richardson number)
   { 222, 0, 0, "Middle cloud bottom", "MCBL" }, 
   { 223, 0, 0, "Middle cloud top", "MCTL" }, 
   { 224, 0, 0, "Middle cloud", "MCY" }, 
   { 232, 0, 0, "High cloud bottom", "HCBL" }, 
   { 233, 0, 0, "High cloud top", "HCTL" }, 
   { 234, 0, 0, "High cloud", "HCY" }, 
   { 235, 0, 0, "Ocean Isotherm 1/10C", "OITL" }, ///Level  deg 
   { 236, 0, 2, "Between two depths below ocean surface", "OLYR" },
   { 237, 0, 0, "Bottom of ocean mixed", "OBML" }, /// Layer (m)
   { 238, 0, 0, "Bottom of ocean isothermal", "OBIL" },/// Layer (m)
   { 239, 0, 0, "Ocean surface and 26C", "S26CY" }, ///Layer  Ocean Isothermal Level
   { 240, 0, 0, "Ocean mixed", "OMXL" }, 
   { 241, 0, 0, "Ordered Sequence of Data", "OSEQD" },
   { 242, 0, 0, "Convective cloud bottom", "CCBL" }, 
   { 243, 0, 0, "Convective cloud top", "CCTL" }, 
   { 244, 0, 0, "Convective cloud", "CCY" }, 
   { 245, 0, 0, "Lowest level of the wet bulb zero", "LLTW" }, 
   { 246, 0, 0, "Maximum equivalent potential temperature", "MTHE" },
   { 247, 0, 0, "Equilibrium level", "EHLT" }, 
   { 248, 0, 0, "Shallow convective cloud bottom", "SCBL" }, 
   { 249, 0, 0, "Shallow convective cloud top", "SCTL" }, 
   { 251, 0, 0, "Deep convective cloud bottom", "DCBL" }, 
   { 252, 0, 0, "Deep convective cloud top", "DCTL" }, 
   { 253, 0, 0, "Lowest bottom of supercooled liquid water", "LBLSW"}, 
   { 254, 0, 0, "Highest top of supercooled liquid water", "HTLSW" }, 
   { 255, 0, 0, "Missing", "NA" }, 
};
static const int n_grib_level_list = sizeof(grib_level_list)/sizeof(GribLevelData);

////////////////////////////////////////////////////////////////////////

ConcatString get_grib_code_name(int, int);
ConcatString get_grib_code_unit(int, int);
ConcatString get_grib_code_abbr(int, int);

ConcatString get_grib_level_name(int);
ConcatString get_grib_level_abbr(int);
ConcatString get_grib_level_str(int, unsigned char *);

int str_to_grib_code(const char *);
int str_to_grib_code(const char *, int);

int str_to_grib_code(const char *, int &, double &, double &);
int str_to_grib_code(const char *, int &, double &, double &, int);

int str_to_prob_info(const char *, double &, double &, int);

void get_grib_code_list(int, int, int &, const GribCodeData *&);
ConcatString get_grib_code_list_str(int, int, int);
ConcatString get_grib_level_list_str(int, int);

///////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////

#endif   //  __GRIB_STRINGS_H__

////////////////////////////////////////////////////////////////////////

