/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

#include <opencv2/core/core.hpp>
#include <set>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "griblatlon.h"
#include "datefcst.h"
#include "gribitem.h"

using namespace std;
using namespace cv;

typedef struct grib1_tbl_t {
  int   code;
  int   table_number;
  int   center;
  int   subcenter;
  char  *parm_name;
  char  *full_name;
  char  *units;
} grib1_tbl_t;

typedef struct grib2_tbl_t {
  int index_a;
  int mtab_set;
  int mtab_low;
  int mtab_high;
  int cntr;
  int ltab;
  int index_b;
  int index_c;
  char *parm_name;
  char *full_name;
  char *units;
  char *short_name;
  int my_id;
} grib2_tbl_t;

extern grib1_tbl_t grib1_ncep_2_7[1024];
#define GRIB2_ALL_NUM 1018
extern grib2_tbl_t grib2_all[GRIB2_ALL_NUM];

class cntrs_c;
class win_c;
class griblatlon_c;
class grib_data_c;
class grib_c;
class ramp_c;

class DataPlane;
struct Grib2Record;
class MetGrib2DataFile;

class grib_lval_cmp_c
{
public:
  bool operator()(const string& a, const string& b) const;
};

typedef map<string, map<string, map<string, map<string, grib_data_c*, grib_lval_cmp_c>>>> data_map_t;

class grib_data_c
{
  Grib2Record       *m_record;
  DataPlane         *m_plane;
  MetGrib2DataFile  *m_datafile;
  QPixmap           *m_pixmap;
  int               m_exp;
  bool              m_unit;
  double            m_bad_val;
  tag_grib_c        m_tag_grib;
  dlatlon_c         m_dlatlon;

  vector<vector<vector<Point>>*>  *m_isolns;
  vector<double>                  *m_thr;

  win_c             *m_win;

  friend class grib_c;
  friend class info_data_c;
public:

  grib_data_c(win_c *win, const tag_grib_c &pgr, const dlatlon_c& dlatlon);
  ~grib_data_c();

  string  codes_str() const;
  double* get_plane();
  bool    make_rgb(bool& clear_pal, Mat **p_rgb = nullptr);
  bool    make_grib(bool& clear_pal, QPixmap **pixmap = nullptr);
  bool    grib_paint(QPixmap *pixmap);
  void    clear_pix_iso();

  const DataPlane*    plane() const { return m_plane; }
  DataPlane*          plane_ptr() { return m_plane; }
  int                 plane_nx() const;
  int                 plane_ny() const;
  const QPixmap*      pixmap() { return m_pixmap; }
  QPixmap*            pixmap_ptr() { return m_pixmap; }
  void                set_pixmap(QPixmap *pixmap) { m_pixmap = pixmap; }
  const tag_grib_c&   tag_grib() { return m_tag_grib; }
  const dlatlon_c&    dlatlon() const { return m_dlatlon; }
};

class gribf_fname_c;

class grib_c
{
  data_map_t            m_data_map;
  cntrs_c               *m_cntrs;
  datefc_c              m_datefc;

  win_c                 *m_win;

  friend class info_data_c;
public:
  grib_c(const datefc_c& date_id, win_c *win) : m_cntrs(nullptr), m_datefc(date_id), m_win(win) {}
  ~grib_c();

  bool              read_grib(QString filename, set<gribf_fname_c>& files, const string& role, rtlv_c& ret);
  bool              iter_from(rtlv_c& par);
  const grib_data_c* grib_data(const rtlv_c& par) const;
  grib_data_c*       grib_data_ptr(const rtlv_c& par) const;
  static bool       get_level(int LvlTyp, double LvlVal1, string& level, string& lval);

  const data_map_t&     data_map() const { return m_data_map; }
  data_map_t&           data_map_ptr() { return m_data_map; }
  const datefc_c&       datefc() const { return m_datefc; }
  rtlv_c                first_param() {
    return rtlv_c(m_data_map.begin()->first,
      m_data_map.begin()->second.begin()->first,
      m_data_map.begin()->second.begin()->second.begin()->first,
      m_data_map.begin()->second.begin()->second.begin()->second.begin()->first);
  }
};
