/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef array_countof
#define array_countof(_a) sizeof(_a)/sizeof(_a[0])
#endif
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#define SMALLBUF    72
#define LARGEBUF    256
#define XLARGEBUF   (8*1024)

#define PROGRAM_NAME            "GFS-view-OS"
#define PROGRAM_MAJ_VERSION     "0.1"
#define PROGRAM_MIN_VERSION     "1"

#ifdef __linux__
#define strcmpi strcasecmp
#else
#define strcasecmp strcmpi
#endif

#define DIR_SEPARATOR_S "/"

#define BOX_MIN_MAX(_x, _y, _x_min, _x_max, _y_min, _y_max) \
if (_x > _x_max) _x_max = _x; \
if (_x < _x_min) _x_min = _x; \
if (_y > _y_max) _y_max = _y; \
if (_y < _y_min) _y_min = _y

#define BAD_VALUE_m9999         (-9999.)
#define BAD_VALUE_m999          (-999.)

#define RUN_FORMAT              "%04d%02d%02d%02d"

#define GFS_025                 "GFS_025"
#define GFS_025_FNAME_FORMAT    "gfs.%04d%02d%02d%02d/gfs.t%02dz.pgrb2.0p25.f%03d"
#define GFS_025_PATTERN         "gfs.(\\d{10})/gfs.t(\\d{2})z.pgrb2.0p25.f(\\d{3})"
#define GFS_025_IDX             0
#define LAST_ROLE               GFS_025_IDX

extern const char *my_roles[LAST_ROLE + 1];

#define MY_NWP_IMPORT           "import" DIR_SEPARATOR_S "NWP_data"
#define MY_SHP_IMPORT           "import" DIR_SEPARATOR_S "Shp"

#define TAG_TO_LEVEL_SIZE      40
#define TAG_TO_LEVEL_NUM       66

#define CANVAS_MAG              120.

#define PAL_FACET_SZ				    15

#define GPT_PRES_TABLE_NUM      54

#define TEMPERATURE_CODES(_temp)    (_temp == string("TMP") || _temp == string("DPT") || _temp == string("TMIN") || _temp == string("TMAX"))
#define TEMPERATURE_LABEL(_tmode)   (_tmode == 1 ? "\302\260C" : _tmode == 2 ? "\302\260F" : "K")

#define GEOPOTENTIAL_CODES(_gp)     (_gp == string("HGT") || _gp == string("5WAVH"))
#define GEOPOTENTIAL_LABEL(_gpm)    (_gpm == 1 ? "mbar" : _gpm == 2 ? "mm_Hg" : _gpm == 3 ? "in_Hg": "gpm")

#define VELOCITY_CODE(_ucomp)       (_ucomp == string("UGRD") || _ucomp == string("UFLX") || _ucomp == string("USTM"))
#define VELOCITY_VCODE(_ucomp)      (_ucomp == string("VGRD") || _ucomp == string("VFLX") || _ucomp == string("VSTM"))
#define VELOCITY_CORCODE(_ucomp)    (_ucomp == string("UFLX") ? "VFLX" : _ucomp == string("USTM") ? "VSTM" : "VGRD")
#define VELOCITY_PAIRCODE(_u1,_u2)  (_u1 == string("UGRD") && _u2 == string("VGRD") || _u1 == string("UFLX") \
                                  && _u2 == string("VFLX") || _u1 == string("USTM") && _u2 == string("VSTM"))

#define SPEED_CODES(_speed)         (_speed == string("GUST") || _speed == string("UGRD") || _speed == string("USTM"))
#define SPEED_LABEL(_smode)         (_smode == 1 ? "km/h" : _smode == 2 ? "mph" : _smode == 3 ? "kts" : _smode == 4 ? "ft/s" : "m/s")

#define PRECIPITATION_CODES(_prec)  (_prec == string("ACPCP") || _prec == string("APCP") || _prec == string("CWAT") || _prec == string("PWAT"))
#define PRECIPITATION_LABEL(_precm) (_precm == 1 ? "in" : "mm")

#define PRECIP_RATE_CODES(_prate)   (_prate == string("CPRAT") || _prate == string("PRATE"))
#define PRECIP_RATE_LABEL(_pratem)  (_pratem == 1 ? "mm/h" : _pratem == 2 ? "in/h" : "mm/s")

#define PRESSURE_CODES(_press)      (_press == string("MSLET") || _press == string("PRES"))
#define PRESSURE_LABEL(_pressm)     (_pressm == 1 ? "mbar" : _pressm == 2 ? "mm_Hg" : _pressm == 3 ? "in_Hg" : "Pa")

#define SET_GRAY(w) \
{ \
  QPalette palette = w->palette(); \
  palette.setColor(QPalette::Base, Qt::lightGray); \
  palette.setColor(QPalette::Text, Qt::white); \
  w->setAutoFillBackground(true); \
  w->setPalette(palette); \
}

void my_log(const char *frmt, const char *s = nullptr);
