/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

#include "gribini.h"
#include "griblatlon.h"

#define DEF_FONT_SIZE           6
#define DEF_ISO_STEP            3
#define DEF_LINE_WIDTH          1
#define DEF_WIND_ARROW_SIZE     (2.)
#define DEF_WIND_ARROW_WIDTH    1
#define DEF_ISO_SMOOTH_VAL      30
#define DEF_ISO_LABEL_DENSITY   10
#define DEF_GEOGRID_WIDTH       1
#define DEF_PLAY_TIMEOUT        1000
#define DEF_RUNTIME_LAG         30

class info_data_c;
class win_c;
class sets_c;
class datefc_c;

#include "gribini.h" // for data_tmpl_t

class sets_c
{
public:
  QString           m_ini_path;

  int               m_charset;

  bool              m_proxy;
  QString           m_proxy_host;
  int               m_proxy_port;
  QString           m_proxy_user;
  QString           m_proxy_passwd;

  QString           m_iso_font_family;
  int               m_iso_font_size;
  int               m_iso_font_weight;
  int               m_iso_step;
  QColor            m_iso_color;
  double            m_iso_width;
  bool              m_iso_lab_bgerase;
  bool              m_iso_smooth;
  int               m_iso_smooth_val;
  int               m_iso_label_density;

  bool              m_wind_compose;
  QColor            m_wind_color;
  QString           m_wind_lval;
  double            m_wind_arrow_size;
  double            m_wind_arrow_width;

  bool              m_tempre_compose;
  QColor            m_tempre_color;
  QString           m_tempre_lval;

  bool              m_geopt_compose;
  QColor            m_geopt_color;
  QString           m_geopt_lval;

  bool              m_precip_compose;
  QColor            m_precip_color;
  QString           m_precip_tag;

  int               m_units_temp;
  int               m_units_geopt;
  int               m_units_speed;
  int               m_units_precip;
  int               m_units_prerate;
  int               m_units_press;

  map<string, map<string, int>>  m_pal_idx;

  int               m_place_idx;

  int               m_fcst_from;
  int               m_fcst_to;
  int               m_fcst_step;

  bool              m_geogrid;
  QColor            m_geogrid_color;
  double            m_geogrid_width;
  bool              m_geogrid_label;

  int               m_play_timeout;
  int               m_runtime_lag;

  win_c             *m_win;

  sets_c(win_c *win) : m_win(win),
    m_charset(0),
    m_proxy(false), m_proxy_port(65535),
    m_iso_font_size(DEF_FONT_SIZE), m_iso_font_weight(QFont::Normal), m_iso_step(DEF_ISO_STEP),
    m_iso_width(DEF_LINE_WIDTH), m_iso_lab_bgerase(false), m_iso_smooth(true), m_iso_smooth_val(DEF_ISO_SMOOTH_VAL), m_iso_label_density(DEF_ISO_LABEL_DENSITY),
    m_wind_compose(false), m_wind_lval("1000"), m_wind_arrow_size(DEF_WIND_ARROW_SIZE), m_wind_arrow_width(DEF_WIND_ARROW_WIDTH),
    m_tempre_compose(false), m_tempre_lval("1000"),
    m_geopt_compose(false), m_geopt_lval("1000"),
    m_precip_compose(false), m_precip_tag("CWAT"),
    m_units_temp(1), m_units_geopt(1), m_units_speed(0), m_units_precip(0), m_units_prerate(1), m_units_press(),
    m_place_idx(-1),
    m_fcst_from(0), m_fcst_to(0), m_fcst_step(1),
    m_geogrid(false), m_geogrid_color(Qt::gray), m_geogrid_width(DEF_GEOGRID_WIDTH), m_geogrid_label(true),
    m_play_timeout(DEF_PLAY_TIMEOUT),
    m_runtime_lag(DEF_RUNTIME_LAG)
  {};

  grib_ini_c* read_ini(data_tmpl_t& tmpl);
  void read_proxy(QSettings& settings);
  bool save_ini(int group_idx, const data_tmpl_t *p_tmpl);
  bool read_cntrs();
};

#define DEF_DBF_PLS_PARAM       "name;pop_max"

#define DATA_TMPL_KEY           "data-tmpl"
#define CONT_FILE_KEY           "cont-file"
#define CONT_ATTR_KEY           "cont-attr"

#define CHARSET_KEY             "charset"

#define PROXY_ENABLED_KEY       "proxy-enabled"
#define PROXY_HOST_KEY          "proxy-host"
#define PROXY_PORT_KEY          "proxy-port"
#define PROXY_USER_KEY          "proxy-user"
#define PROXY_PASSWD_KEY        "proxy-passwd"

#define ISO_FONT_FAMILY_KEY     "iso-font-family"
#define ISO_FONT_SIZE_KEY       "iso-font-size"
#define ISO_FONT_WEIGHT_KEY     "iso-font-weight"
#define ISO_STEP_KEY            "iso-step"
#define ISO_COLOR_KEY           "iso-color"
#define ISO_WIDTH_KEY           "iso-width"
#define ISO_LAB_BGERASE_KEY     "iso-lab-bgerase"
#define ISO_SMOOTH_KEY          "iso-smooth"
#define ISO_SMOOTH_VAL_KEY      "iso-smooth-val"
#define ISO_LABEL_DENSITY_KEY   "iso-label-density"

#define WIND_COMPOSE_KEY        "wind-compose"
#define WIND_ARROW_SIZE_KEY     "wind-arrow-size"
#define WIND_ARROW_WIDTH_KEY    "wind-arrow-width"
#define WIND_LVAL_KEY           "wind-lval"
#define WIND_COLOR_KEY          "wind-color"

#define TEMPRE_COMPOSE_KEY      "tempre-compose"
#define TEMPRE_COLOR_KEY        "tempe-color"
#define TEMPRE_LVAL_KEY         "tempre-lval"

#define GEOPT_COMPOSE_KEY       "geopt-compose"
#define GEOPT_COLOR_KEY         "geopt-color"
#define GEOPT_LVAL_KEY          "geopt-lval"

#define PRECIP_COMPOSE_KEY      "precip-compose"
#define PRECIP_COLOR_KEY        "precip-color"
#define PRECIP_TAG_KEY          "precip-tag"

#define UNITS_TEMPRE_KEY        "units-tempre"
#define UNITS_GEOPT_KEY         "units-geopt"
#define UNITS_SPEED_KEY         "units-speed"
#define UNITS_PRECIP_KEY        "units-precip"
#define UNITS_PRERATE_KEY       "units-preprate"
#define UNITS_PRESS_KEY         "units-press"

#define PAL_IDX_KEY             "pal-idx"

#define REGION_CITY_KEY         "region-city"

#define FCST_FROM_KEY           "fcst-from"
#define FCST_TO_KEY             "fcst-to"
#define FCST_STEP_KEY           "fcst-step"

#define GEOGRID_KEY             "geogrid"
#define GEOGRID_COLOR_KEY       "geogrid-color"
#define GEOGRID_WIDTH_KEY       "geogrid-width"
#define GEOGRID_LABEL_KEY       "geogrid-label"

#define PLAY_TIMEOUT_KEY        "play-timeout"
#define RUNTIME_LAG_KEY         "runtime-lag"

#define CNTRS_NAME_KEY            "name"
#define CNTRS_DBF_PLS_PARAM_KEY   "dbf-pls-param"
#define CNTRS_LINE_WIDTH_KEY      "line-width"
#define CNTRS_LINE_DASH_KEY       "line-dash"
#define CNTRS_FONT_NAME_KEY       "font-name"
#define CNTRS_FONT_SIZE_KEY       "font-size"
#define CNTRS_TEXT_COLOR_KEY      "text-color"
#define CNTRS_LINE_COLOR_KEY      "line-color"

#define SETS_GROUP_INTERFACE          1
#define SETS_GROUP_GRIB_STARTUP       2
#define SETS_GROUP_CNTRS_STARTUP      4
#define SETS_GROUP_DATE_TIME          8
