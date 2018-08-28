// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

////////////////////////////////////////////////////////////////////////

#ifndef  __UTIL_CONSTANTS_H__
#define  __UTIL_CONSTANTS_H__

#if HAVE_CONFIG_H
# include "config.h"
#endif

////////////////////////////////////////////////////////////////////////

static const char met_version[]          = "V5.2";
static const char met_base_str[]         = "MET_BASE";
static const char default_met_data_dir[] = "MET_BASE";
static const char txt_file_ext[]         = ".txt";
static const char stat_file_ext[]        = ".stat";
static const char tc_stat_file_ext[]     = ".tcst";
static const char full_domain_str[]      = "FULL";

static const char latlon_proj_type         [] = "LatLon";
static const char mercator_proj_type       [] = "Mercator";
static const char lambert_proj_type        [] = "Lambert Conformal";
static const char stereographic_proj_type  [] = "Polar Stereographic";

// String containing all valid PrepBufr message types
static const char vld_msg_typ_str[] =
   "ADPUPA AIRCAR AIRCFT ADPSFC ERS1DA GOESND GPSIPW MSONET \
    PROFLR QKSWND RASSDA SATEMP SATWND SFCBOG SFCSHP SPSSMI \
    SYNDAT VADWND ANYAIR ANYSFC ONLYSF";

// Array of valid PrepBufr message types
static const char *vld_msg_typ_list[] = {
   "ADPUPA", "AIRCAR", "AIRCFT", "ADPSFC", "ERS1DA", "GOESND",
   "GPSIPW", "MSONET", "PROFLR", "QKSWND", "RASSDA", "SATEMP",
   "SATWND", "SFCBOG", "SFCSHP", "SPSSMI", "SYNDAT", "VADWND" };

// Number of valid PrepBufr message types
static const int n_vld_msg_typ =
   sizeof(vld_msg_typ_list)/sizeof(*vld_msg_typ_list);

// Special PrepBufr message types
static const char anyair_str[]         = "ANYAIR";
static const char anyair_msg_typ_str[] = "AIRCAR AIRCFT";
static const char anysfc_str[]         = "ANYSFC";
static const char anysfc_msg_typ_str[] = "ADPSFC SFCSHP ADPUPA PROFLR";
static const char onlysf_str[]         = "ONLYSF";
static const char onlysf_msg_typ_str[] = "ADPSFC SFCSHP";

// String containing all interpolation methods
static const char all_interp_mthd_str[] =
   "MIN MAX MEDIAN UW_MEAN DW_MEAN LS_FIT BILIN";

// Commonly used regular expressions
static const char yyyymmdd_hhmmss_reg_exp[] =
   "[0-9]\\{8,8\\}_[0-9]\\{6,6\\}";
static const char ws_reg_exp[] = "[ \t\r\n]";
static const char ws_line_reg_exp[] = "^[ \t\r\n]*$";
static const char sep_str[] = "--------------------------------------------------------------------------------";

// Bootstrap methods
static const int boot_bca_flag  = 0;
static const int boot_perc_flag = 1;

////////////////////////////////////////////////////////////////////////

static const int max_line_len = 2048;
static const double grib_earth_radius_km = 6371.20;
static const int default_precision = 5;
static const double default_grid_weight = 1.0;

////////////////////////////////////////////////////////////////////////

// Constants used in deriving observation values
static const double const_a     = 1.0/273.15;
static const double const_b     = 2500000.0/461.5;
static const double const_c     = 6.11;

// Constants used in deriving RH
static const double const_pq0   = 379.90516;
static const double const_a2    = 17.2693882;
static const double const_a3    = 273.16;
static const double const_a4    = 35.86;

// Constants used in deriving PRMSL
static const double const_tmb   = 288.15;  // kelvin
static const double const_gop   = 9.80616; // from The Ceaseless Wind
static const double const_rd    = 287.0;   // kg/k dry gas constant

static const int vx_data2d_star = -12345;

////////////////////////////////////////////////////////////////////////

// Define isatty() function, if needed.  It used used by lex but is
// not defined on some systems
#ifndef HAVE_ISATTY
   inline int isatty(int fd) { return 0; }
#endif

////////////////////////////////////////////////////////////////////////

#endif   //  __UTIL_CONSTANTS_H__

////////////////////////////////////////////////////////////////////////
