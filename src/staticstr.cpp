/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <stdio.h>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtNetwork>

#include <opencv2/core/core.hpp>

#include "defs.h"
#include "griblatlon.h"
#include "datefcst.h"
#include "mgrb2.h"
#include "staticstr.h"

#include "grib_strings.h"

const char* dash_line_names[DASH_LINE_NAME_NUM] = {
  "continuous", "dashed", "dashdot"
};

const char* char_set_names[CHAR_SET_NUM] = {
  "WINDOWS-1252", "WINDOWS-1250", "WINDOWS-1251", "WINDOWS-1253", "WINDOWS-1254", "WINDOWS-1255", "WINDOWS-1256", "WINDOWS-1257", "WINDOWS-1258",
  "ISO 8859-1", "ISO 8859-2", "ISO 8859-3", "ISO 8859-4", "ISO 8859-5", "ISO 8859-7", "ISO 8859-8", "ISO 8859-9", "ISO 8859-10",
  "ISO 8859-13", "ISO 8859-14", "ISO 8859-15", "ISO 8859-16",
  "UTF-8", "UTF-16", "UTF-32"
};

tag_to_level_t gfs_tag2level[TAG_TO_LEVEL_NUM] = {
  { "4LFTX",  "SFC",  { "0" },  0 },
  { "ABSV",   "ISBL", { "10", "20", "30", "50", "70", "100", "150", "200", "250", "300", "350", "400", "450", "500", "550",
  "600", "650", "700", "750", "800", "850", "900", "925", "950", "975", "1000" }, 11 },
  { "CAPE",   "SFC",  { "0" } , 88 },
  { "CAPE",   "SIGY", { "180", "255" }, 88 },
  { "CIN",    "SFC",  { "0" }, 121 },
  { "CIN",    "SIGY", { "180" }, 121 }, 
  { "CLWMR",  "ISBL", {
    "350", "400", "450", "500", "550", "600", "650",
    "700", "750", "800", "850", "900", "925", "950", "975", "1000" }, 128 },
  { "CWAT",   "EATM", { "0" }, 193 }, 
  { "DPT",    "GPML", { "2" }, 219 },
  { "FLDCP",  "SFC",  { "0" }, 274 },
  { "GUST",   "SFC",  { "0" }, 301 },
  { "HGT",    "0DEG", { "0" }, 318 },
  { "HGT",    "HTFL", { "0" }, 318 },
  { "HGT",    "HYBL", { "20" }, 318 },
  { "HGT",    "ISBL", {
    "1", "2", "3", "5", "7", "10", "20", "30", "50", "70", "100",
    "150", "200", "250", "300", "350", "400", "450", "500", "550",
    "600", "650", "700", "750", "800", "850", "900", "925", "950", "975", "1000" }, 318 },
  { "HGT",    "SFC",  { "0" }, 318 },
  { "HGT",    "MWSL", { "0" }, 318 },
  { "HGT",    "SFC",  { "0" }, 318 },
  { "HGT",    "TRO",  { "0" }, 318 },
  { "HLCY",   "GPML", { "30" }, 323 },
  { "HPBL",   "SFC",  { "0" }, 324 },
  { "ICAHT",  "MWSL", { "0" }, 332 },
  { "ICAHT",  "TRO",  { "0" }, 332 },
  { "LFTX",   "SFC",  { "0" }, 381 },
  { "MSLET",  "ISBY", { "0" }, 447 },//Pa// "Mean sea level pressure (ETA model)"
  { "O3MR",   "ISBL", { "1", "2", "3", "5", "7", "10", "20", "30", "50", "70", "100", "150", "200", "250", "300", "350", "400" }, 498 },
  { "PRES",   "GPML", { "80" }, 552 }, //Pa},
  { "PRES",   "HYBL", { "20" }, 552 },
  { "PRES",   "MWSL", { "0" }, 552 },
  { "PRES",   "SFC",  { "0" }, 552 },
  { "PWAT",   "EATM", { "0" }, 567 },//kg/m^2},
  { "RH",     "0DEG", { "0" }, 615 },
  { "RH",     "EATM", { "0" }, 615 },
  { "RH",     "GPML", { "2" }, 615 },
  { "RH",     "GPMY", { "33", "99" }, 615 },
  { "RH",     "HTFL", { "0" }, 615 },
  { "RH",     "ISBL", { "1", "2", "3", "5", "7", "10", "20", "30", "50", "70", "100", "150", "200", "250", "300", "350", "400", "450", "500", "550", "600",
  "650", "700", "750", "800", "850", "900", "925", "950", "975", "1000" }, 615 },
  { "RH",     "SIGY", { "30" }, 615 },
  { "SPFH",   "GPML",  { "2" }, 747 },
  { "TMP",    "GPML", { "2" }, 853 },
  { "TMP",    "GPMY", { "99" }, 853 },
  { "TMP",    "HYBL", { "20" }, 853 },
  { "TMP",    "ISBL", { "1", "2", "3", "5", "7", "10", "20", "30", "50", "70", "100", "150", "200", "250", "300", "350", "400", "450", "500", "550",
  "600", "650", "700", "750", "800", "850", "900", "925", "950", "975", "1000" }, 853 },
  { "TMP",    "MSL",  { "18", "27", "36", }, 853 },
  { "TMP",    "MWSL", { "0" }, 853 },
  { "TMP",    "SFC",  { "0" }, 853 },
  { "TMP",    "SIGY", { "30" }, 853 },
  { "TMP",    "TRO",  { "0" }, 853 },
  { "TOZNE",  "EATM", { "0" }, 861 },
  { "UGRD",   "GPML", { "1", "10" }, 898 },
  { "UGRD",   "GPMY", { "99", }, 898 },
  { "UGRD",   "HYBL", { "20" }, 898 },
  { "UGRD",   "ISBL", { "1", "2", "3", "5", "7", "10", "20", "30", "50", "70", "100", "150", "200", "250", "300", "350", "400", "450", "500", "550",
  "600", "650", "700", "750", "800", "850", "900", "925", "950", "975", "1000" }, 898 },
  { "UGRD",   "MSL",  { "18", "27", "36" }, 898 },
  { "UGRD",   "MWSL", { "0" }, 898 },
  { "UGRD",   "PBLRI",{ "0" }, 898 },
  { "UGRD",   "SIGY", { "30" }, 898 },
  { "UGRD",   "TRO",  { "0" }, 898 },
  { "USTM",   "GPML", { "60" }, 911 },
  { "VIS",    "SFC",   { "0" }, 943 },
  { "VRATE",  "PBLRI",{ "0" }, 952 },
  { "VVEL",  "GPMY",  { "99" }, 963 },
  { "VVEL",   "ISBL", { "100", "150", "200", "250", "300", "350", "400", "450", "500", "550", "600", "650", "700", "750", "800", "850", "900", "925", "950", "975", "1000" }, 963 },
  { "VWSH",   "HYBL", { "20" },  965 },
  { "VWSH",   "TRO",  { "0" }, 965 },
  { "WILT",   "SFC",  { "0" }, 984 }
};

int gfs_tag2level_idx(const string& tag, const string& lev, const string& lval)
{
  int i;
  for (i = 0; i < TAG_TO_LEVEL_NUM; i++) {
    if (gfs_tag2level[i].tag != tag)
      continue;
    if (lev.empty() && lval.empty())
      break;
    if (gfs_tag2level[i].lev != lev)
      continue;
    if (lval.empty())
      break;
    for (int j = 0; j < TAG_TO_LEVEL_SIZE; j++) {
      if (!gfs_tag2level[i].lval[j])
        break;
      if (gfs_tag2level[i].lval[j] == lval)
        goto end;
    }
  }
end:
  return i < TAG_TO_LEVEL_NUM ? i : -1;
}

int grib_level_idx(const string& key)
{
  int i;
  for (i = 0; i < n_grib_level_list; ++i) {
    if (key == grib_level_list[i].abbr)
      break;
  }
  return i < n_grib_level_list ? i : -1;
}

int tag2level_idx(const string&role, const string& tag, const string& lev, const string& lval)
{
  int ret = -1;
  if (role == string(GFS_025))
    ret = gfs_tag2level_idx(tag, lev, lval);
  return ret;
}
