// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*


///////////////////////////////////////////////////////////////////////////////

#ifndef  __INTERP_MTHD_H__
#define  __INTERP_MTHD_H__

///////////////////////////////////////////////////////////////////////////////

#include "concat_string.h"

///////////////////////////////////////////////////////////////////////////////

//
// Enumeration for interpolation methods
//
enum InterpMthd {
   InterpMthd_None,
   InterpMthd_Min,
   InterpMthd_Max,
   InterpMthd_Median,
   InterpMthd_UW_Mean,
   InterpMthd_DW_Mean,
   InterpMthd_LS_Fit,
   InterpMthd_Nbrhd,
   InterpMthd_Bilin, 
   InterpMthd_Nearest,
   InterpMthd_Budget,
   InterpMthd_Force
};

//
// String corresponding to the enumerated values above
//
static const char interpmthd_none_str[]    = "NA";
static const char interpmthd_min_str[]     = "MIN";
static const char interpmthd_max_str[]     = "MAX";
static const char interpmthd_median_str[]  = "MEDIAN";
static const char interpmthd_uw_mean_str[] = "UW_MEAN";
static const char interpmthd_dw_mean_str[] = "DW_MEAN";
static const char interpmthd_ls_fit_str[]  = "LS_FIT";
static const char interpmthd_nbrhd_str[]   = "NBRHD";
static const char interpmthd_bilin_str[]   = "BILIN";
static const char interpmthd_nearest_str[] = "NEAREST";
static const char interpmthd_budget_str[]  = "BUDGET";
static const char interpmthd_force_str[]   = "FORCE";

///////////////////////////////////////////////////////////////////////////////

extern ConcatString interpmthd_to_string(const InterpMthd);
extern InterpMthd   string_to_interpmthd(const char *);

///////////////////////////////////////////////////////////////////////////////

#endif   //  __INTERP_MTHD_H__

///////////////////////////////////////////////////////////////////////////////
