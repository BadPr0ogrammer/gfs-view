/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <math.h>
#include <array>  

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QRegularExpression> 

#include "defs.h"
#include "color_tables.h"
#include "cntrs.h"
#include "griblatlon.h"
#include "img.h"
#include "datefcst.h"
#include "ramppal.h"
#include "gribitem.h"
#include "infodata.h"
#include "winc.h"
#include "mgrb2.h"
#include "staticstr.h"
#include "sets.h"
#include "local_str.h"

#include "data_class.h"
#include "data2d_grib.h"
#include "data2d_grib2.h"
#include "data2d_factory.h"
#include "grib_strings.h"
#include "vx_data2d.h"
#include "table_lookup.h"
#include "grib1_ncep_2_7.h"
#include "grib2_all.h"
#include "data2d_grib_utils.h"
#include "data_plane.h"

bool grib_lval_cmp_c::operator()(const string& a, const string& b) const
{
  return stoi(a) < stoi(b);
}

grib_data_c::grib_data_c(win_c *win, const tag_grib_c &pgr, const dlatlon_c& dlatlon) :
  m_record(nullptr), m_plane(nullptr), m_datafile(nullptr), m_pixmap(nullptr),
  m_exp(0), m_unit(false), m_bad_val(0),
  m_tag_grib(pgr), m_dlatlon(dlatlon),
  m_isolns(nullptr), m_thr(nullptr),
  m_win(win)
{}

void grib_data_c::clear_pix_iso()
{
  delete m_pixmap;
  m_pixmap = nullptr;
  if (m_isolns) {
    for_each(m_isolns->begin(), m_isolns->end(), [](vector<vector<Point>> *v) { delete v; });
    delete m_isolns;
    m_isolns = nullptr;
  }
  if (m_thr) {
    delete m_thr;
    m_thr = nullptr;
  }
}

grib_data_c::~grib_data_c()
{
  delete m_record;
  delete m_plane;

  clear_pix_iso();
}

grib_c::~grib_c()
{
  delete m_cntrs;

  for (auto it = m_data_map.begin(); it != m_data_map.end(); it++)
    for (auto it0 = (*it).second.begin(); it0 != (*it).second.end(); it0++)
      for (auto it1 = (*it0).second.begin(); it1 != (*it0).second.end(); it1++)
        for (auto it2 = (*it1).second.begin(); it2 != (*it1).second.end(); it2++) {
          grib_data_c *pdata = (*it2).second;
          delete pdata;
        }
}

bool grib_c::read_grib(QString filename, set<gribf_fname_c>& files, const string& role, rtlv_c& ret)
{
  Met2dDataFileFactory *factory = new Met2dDataFileFactory();
  if (!factory) {
    my_log("err #%s\n", "0xc5318b04");
    return false;
  }

  MetGrib2DataFile *datafile = nullptr;
#ifdef _WINDOWS
  wchar_t buf[LARGEBUF];
  filename.toWCharArray(buf);
  buf[filename.length()] = 0;

  datafile = (MetGrib2DataFile *)factory->new_met_2d_data_file(buf);
#else
  datafile = (MetGrib2DataFile *)factory->new_met_2d_data_file(filename.toUtf8().data());
#endif
  delete factory;
  factory = nullptr;

  if (!datafile) {
    my_log("err #%s\n", "0xae3708fa");
    return false;
  }

  const Grid &grid = datafile->grid();
  const GridInfo &gi = grid.info();

  if (role == GFS_025 && !gi.ll) {
    delete datafile;
    my_log("err #%s\n", "0x78607351");
    return false;
  }
  dlatlon_c dlatlon;
  if (gi.ll) {
    dlatlon.m_lat_ll = gi.ll->lat_ll;
    dlatlon.m_lon_ll = -gi.ll->lon_ll;//WEST!!!
    dlatlon.m_delta = gi.ll->delta_lon;

    if (dlatlon.m_lon_ll == 0 && dlatlon.m_lon_ll + gi.ll->Nlon * dlatlon.m_delta == 360)
      dlatlon.m_lon_ll = -180;

    if (dlatlon.m_delta <= 0 || gi.ll->delta_lon != gi.ll->delta_lat) {
      delete datafile;
      my_log("err #%s\n", "0xeb6ee4d6");
      return false;
    }
  }
  gribf_fname_c gribf_fname(datafile, filename.toStdString());
  files.insert(gribf_fname);

  bool first = false;
  for (auto it = datafile->RecList.begin(); it != datafile->RecList.end(); it++) {
    Grib2Record *rec = *it;
    if (!rec || rec->ParmName.empty()) {
      my_log("err #%s\n", "0xf60ad187");
      continue;
    }
    string level, lval;
    if (!get_level(rec->LvlTyp, rec->LvlVal1, level, lval)) {
      continue;
    }
    if (!VELOCITY_VCODE(rec->ParmName) && tag2level_idx(role, rec->ParmName, level, lval) == -1) {
      continue;
    }
    rtlv_rf_c par(role, rec->ParmName, level, lval, m_datefc.run_str(), m_datefc.fc_str());
    grib_data_c *pdata = grib_data_ptr(par);
    if (!pdata) {
      pdata = new grib_data_c(m_win, tag_grib_c(par, this), dlatlon);
      pdata->m_record = rec;
      pdata->m_datafile = datafile;
      m_data_map[role][rec->ParmName][level][lval] = pdata;

      if (!first) {
        ret = par;
        first = true;
      }
    }
  }
  return true;
}

grib_data_c* grib_c::grib_data_ptr(const rtlv_c& par) const
{
  if (!m_data_map.size())
    return nullptr;
  if (par.is_empty()) {
    my_log("err #%s\n", "0xfbd4f649");
    return nullptr;
  }
  grib_data_c *grib = nullptr;
  auto it = m_data_map.find(par.m_role);
  if (it != m_data_map.end()) {
    auto it0 = (*it).second.find(par.m_tag);
    if (it0 != (*it).second.end()) {
      auto it1 = (*it0).second.find(par.m_lev);
      if (it1 != (*it0).second.end()) {
        auto it2 = (*it1).second.find(par.m_lval);
        if (it2 != (*it1).second.end())
          grib = (*it2).second;
      }
    }
  }
  return grib;
}

const grib_data_c* grib_c::grib_data(const rtlv_c& par) const
{
  return grib_data_ptr(par);
}

bool grib_c::iter_from(rtlv_c& par)
{
  bool ret = false;
  auto f = m_data_map.find(par.m_role);
  for (auto it = (f == m_data_map.end()) ? m_data_map.begin() : f; it != m_data_map.end(); it++) {
    string role = (*it).first;
    auto f0 = (*it).second.find(par.m_tag);
    for (auto it0 = (f0 == (*it).second.end() ? (*it).second.begin() : f0); it0 != (*it).second.end(); it0++) {
      string tag = (*it0).first;
      auto f1 = (*it0).second.find(par.m_lev);
      for (auto it1 = (f1 == (*it0).second.end() ? (*it0).second.begin() : f1); it1 != (*it0).second.end(); it1++) {
        string lev = (*it1).first;
        auto f2 = (*it1).second.find(par.m_lval);
        for (auto it2 = (f2 == (*it1).second.end() ? (*it1).second.begin() : f2); it2 != (*it1).second.end(); it2++) {
          if ((*it2).second) {
            string lval = (*it2).first;
            par.m_role = role;
            par.m_tag = tag;
            par.m_lev = lev;
            par.m_lval = lval;
            ret = true;
            goto end;
          }
        }
      }
    }
  }
end:
  return ret;
}

string grib_data_c::codes_str() const
{
  string ret;
  ret += "Disc:" + to_string(m_record->Discipline);
  ret += ",Cat:" + to_string(m_record->ParmCat);
  ret += ",Param:" + to_string(m_record->Parm);
  ret += ",Type:" + to_string(m_record->LvlTyp);
  ret += ",Lev:" + to_string((int)m_record->LvlVal1);
  return ret;
}

bool grib_c::get_level(int LvlTyp, double LvlVal1, string& level, string& lval)
{
  int i;
  for (i = 0; i < n_grib_level_list; i++) {
    if (LvlTyp == grib_level_list[i].level)
      break;
  }
  if (i == n_grib_level_list) {
    my_log("err #%s\n", "0xe79ef443");
    return false;
  }

  level = grib_level_list[i].abbr;
  if (fabs(LvlVal1) > FLT_EPSILON && fabs(LvlVal1) < 100)
    lval = to_string(abs((int)LvlVal1));
  else
    lval = to_string((int)(LvlVal1 / 100.0));

  return true;
}

void draw_isoline(win_c *win, int w, int h, QPainter *painter, QColor &color, const rtlv_c& par, const vector<vector<vector<Point>>*> *contours, const vector<double> *thresh)
{
  info_data_c *data = win->m_data;
  sets_c *sets = win->m_sets;
  bool f_hpa = GEOPOTENTIAL_CODES(par.m_tag) && sets->m_units_geopt;
  ramp_c &pal = data->ramp_ptr()[par.m_role][par.m_tag][par.m_lev][par.m_lval];//const ramp_c!!!!

  QFont font(!sets->m_iso_font_family.isEmpty() ? sets->m_iso_font_family : QFont().defaultFamily(), sets->m_iso_font_size, sets->m_iso_font_weight);
  painter->setFont(font);
  QFontMetrics fm(font);
  int fm_height = fm.height();

  int d0 = sets->m_iso_label_density * fm_height;
  int w0 = w / d0;
  int h0 = h / d0;
  vector<pair<string, Point>> *lab_cell = new vector<pair<string, Point>>();
  lab_cell->resize(w0 * h0);

  QPen pen(color);
  pen.setWidth(sets->m_iso_width);
  painter->setPen(pen);

  int num = contours->size() - 1;
  double prec;
  if (f_hpa)
    prec = pal.hpas() > 3 ? 0 : log10(pal.hpas());
  else
    prec = pal.step() > 3 ? 0 : log10(pal.step());
  int prec_n = ceil(-prec);
  char buf[SMALLBUF];

  int i = 0;
  for (auto it0 = contours->begin(); it0 != contours->end(); it0++, i++) {
    double v = (*thresh)[!f_hpa ? i : num - i];
    if (prec >= -3)
      sprintf(buf, "%.*f", prec_n, v);
    else
      sprintf(buf, "%.2e", v);
    pal.labs_ptr().push_back(buf);

    if (!*it0) {
      continue;
    }
    Point pt0 = { -1,-1 };
    double d = 0;
    for (auto it1 = (*it0)->begin(); it1 != (*it0)->end(); it1++) {
      QPolygon polygon;
      for (auto it2 = (*it1).begin(); it2 != (*it1).end(); it2++) {
        Point pt = *it2;
        polygon << QPoint(pt.x, pt.y);

        if (pt0.x > -1 && d > d0) {
          int x = MAX(0, MIN(w0 - 1, pt.x / d0));
          int y = MAX(0, MIN(h0 - 1, pt.y / d0));
          pair<string, Point> pr(pal.get_lab(pal.labs().size() - 1), pt);
          (*lab_cell)[x + y * w0] = pr;
          d = 0;
        }
        else
          d += abs(pt.x - pt0.x) + abs(pt.y - pt0.y);
        pt0 = pt;
      }
      painter->drawPolygon(polygon);
    }
  }

  for (int i = 0; i < w0 * h0; i++) {
    pair<string, Point> &pr = (*lab_cell)[i];
    if (!pr.first.empty()) {
      Point pt = pr.second;
      if (sets->m_iso_lab_bgerase) {
        int w1 = fm.width(buf);
        painter->eraseRect(QRect(pt.x, pt.y - 0.9 * fm_height, w1, fm_height));
      }
      painter->drawText(pt.x, pt.y, pr.first.c_str());
    }
  }
  delete lab_cell;
}

double* grib_data_c::get_plane()
{
  if (m_tag_grib.m_rtlv.m_role.empty() || m_tag_grib.m_rtlv.m_tag.empty()) {
    my_log("err #%s\n", "0x3f251cb");
    return nullptr;
  }
  if (m_plane && m_plane->data())
    return m_plane->data_ptr();

  m_plane = new DataPlane;
  if (!m_datafile->read_grib2_record_data_plane(m_record, *m_plane) || !m_plane->data()) {
    my_log("err #%s\n", "0x800e754b");
    return nullptr;
  }
  Mat *tmp = new Mat(m_plane->ny(), m_plane->nx(), CV_64F, m_plane->data_ptr());
  flip(*tmp, *tmp, 0);

  sets_c *sets = m_win->m_sets;
  const char *tag = m_tag_grib.m_rtlv.m_tag.c_str();
  if (!m_unit &&
    (TEMPERATURE_CODES(tag) && sets->m_units_temp
      || SPEED_CODES(tag) && sets->m_units_speed
      || PRECIPITATION_CODES(tag) && sets->m_units_precip
      || PRECIP_RATE_CODES(tag) && sets->m_units_prerate
      || PRESSURE_CODES(tag) && sets->m_units_press)) {
    m_unit = true;

    if (TEMPERATURE_CODES(tag)) {
      if (sets->m_units_temp == 1)
        *tmp = *tmp - 273.15;
      else if (sets->m_units_temp == 2)
        *tmp = *tmp * (9. / 5) - 459.67;
    }
    else if (SPEED_CODES(tag)) {
      if (sets->m_units_speed == 1)
        *tmp = *tmp * 3.6;
      else if (sets->m_units_speed == 2)
        *tmp = *tmp * 2.236936;
      else if (sets->m_units_speed == 3)
        *tmp = *tmp * 1.943844;
      else if (sets->m_units_speed == 4)
        *tmp = *tmp * 3.280840;
    }
    else if (PRECIPITATION_CODES(tag)) {
      if (sets->m_units_precip == 1)
        *tmp = *tmp / 25.4;
    }
    else if (PRECIP_RATE_CODES(tag)) {
      if (sets->m_units_prerate == 1)
        *tmp = *tmp / 3600;
      else if (sets->m_units_prerate == 2)
        *tmp = *tmp / 141.7323;
    }
    else if (PRESSURE_CODES(tag)) {
      if (sets->m_units_press == 1)
        *tmp = *tmp * 0.01;
      if (sets->m_units_press == 2)
        *tmp = *tmp * (0.01 * 0.750062);
      if (sets->m_units_press == 3)
        *tmp = *tmp * (0.01 * 0.0295386);
    }
  }
  tmp->data = nullptr;
  delete tmp;
  return m_plane->data_ptr();
}

bool grib_data_c::make_rgb(bool& clear_pal, Mat **p_rgb)
{
  sets_c *sets = m_win->m_sets;
  double dmin = FLT_MAX, dmax = -FLT_MAX;
  const rtlv_c& par = m_tag_grib.m_rtlv;
  ramp_c& pal = m_win->m_data->ramp_ptr()[par.m_role][par.m_tag][par.m_lev][par.m_lval];
  int ipal = MIN(NCL_PAL_NUM - 1, MAX(0, sets->m_pal_idx[par.m_role][par.m_tag]));
  float(*col_tab)[4] = ncl_pal_ctbl[ipal];
  bool f_hpa = GEOPOTENTIAL_CODES(par.m_tag) && sets->m_units_geopt;
  double us = f_hpa ? (sets->m_units_geopt == 2 ? 0.750062 : sets->m_units_geopt == 3 ? 0.0295386 : 1) : 1;
  Mat *tmp1 = nullptr;

  double *plane = get_plane();
  if (!plane) {
    my_log("err #%s\n", "0x95f896af");
    return false;
  }
  {
    Mat *tmp2 = new Mat(m_plane->ny(), m_plane->nx(), CV_64F, plane);
    if (VELOCITY_CODE(par.m_tag)) {
      rtlv_c par(par.m_role, VELOCITY_CORCODE(par.m_tag), par.m_lev, par.m_lval);
      grib_data_c *pdata = m_tag_grib.m_gr->grib_data_ptr(par);
      double *dbl_1 = pdata->get_plane();
      if (!dbl_1) {
        my_log("err #%s\n", "0xac7097c");
        return false;
      }
      Mat *ytmp = new Mat(tmp2->size(), CV_64F, dbl_1);

      Mat *tmp = new Mat(tmp2->size(), CV_64F);
      magnitude(*tmp2, *ytmp, *tmp);

      tmp2->data = nullptr;
      delete tmp2;
      ytmp->data = nullptr;
      delete ytmp;

      tmp2 = tmp;
    }

    if (par.m_role == GFS_025)
      minMaxLoc(*tmp2, &dmin, &dmax);
    if (fabs(dmax - dmin) < FLT_EPSILON) {
#ifdef DEBUG
      my_log("err #%s\n", "0x586b6b3e");
#endif
      m_win->set_msg(IDSTR_NO_DATA_B, true, true);
      return false;
    }
    tmp1 = new Mat(CANVAS_MAG * m_plane->ny() * m_dlatlon.m_delta, CANVAS_MAG * m_plane->nx() * m_dlatlon.m_delta, CV_64F);

    cv::resize(*tmp2, *tmp1, tmp1->size(), 0, 0, CV_INTER_LINEAR);

    if (!VELOCITY_CODE(par.m_tag))
      tmp2->data = nullptr;
    delete tmp2;
  }

  if (!pal.num() || dmax > pal.val1() || dmin < pal.val0()) {
    clear_pal = true;
    if (!pal.ramp_step(sets, dmin, dmax, par.m_role, par.m_tag)) {
      my_log("err #%s\n", "0xd6990940");
      return false;
    }
  }
  img_c *gray = nullptr;
  if (p_rgb)
    gray = new img_c(tmp1->size(), CV_8U, Scalar(0));

  Mat *mask = new Mat(tmp1->size(), CV_8U);

  clear_pix_iso();
  m_isolns = new vector<vector<vector<Point>>*>;
  m_thr = new vector<double>;

  m_isolns->push_back(nullptr);
  m_thr->push_back(!f_hpa ? pal.val0() : us * pal.hpa1());

  double thr = pal.val0() + pal.step();
  for (int i = 1; i < pal.num(); i++, thr += pal.step()) {
    compare(*tmp1, thr, *mask, CV_CMP_GT);
    if (gray)
      gray->setTo(i, *mask);

    if (!(i % sets->m_iso_step)) {
      vector<vector<Point>> *cont = new vector<vector<Point>>();
      findContours(*mask, *cont, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
      m_isolns->push_back(cont);
      m_thr->push_back(!f_hpa ? thr : us * (pal.hpa1() - i * pal.hpas()));
    }
  }
  delete mask;
  delete tmp1;

  if (p_rgb) {
    uchar(*tab)[3] = (uchar(*)[3]) malloc(pal.num() * 3);

    for (int j = 0; j < pal.num(); j++)
      for (int k = 0; k < 3; k++)
        tab[j][k] = (uchar)(255 * col_tab[j][k + 1]);

    Mat *rgb = gray->pal2rgb(tab, pal.num(), 1);
    free(tab);

    if (sets->m_iso_smooth)
      blur(*rgb, *rgb, Size(sets->m_iso_smooth_val, sets->m_iso_smooth_val));

    *p_rgb = rgb;
  }
  delete gray;
  return true;
}

bool grib_data_c::make_grib(bool& clear_pal, QPixmap **p_pixmap)
{
  bool ret = false;
  QPixmap *pixmap = nullptr;
  Mat *rgb = nullptr;
  QImage *img = nullptr;
  int w = 0, h = 0;

  clear_pal = false;
  if (m_pixmap) {
    my_log("#%s\n", "0xdf0ca080");
    return true;
  }

  if (!make_rgb(clear_pal, &rgb)) {
#ifdef DEBUG
    my_log("err #%s\n", "0xe5f0b681");
#endif
    goto end;
  }
  w = rgb->cols;
  h = rgb->rows;
  pixmap = new QPixmap;
  if (p_pixmap) {
    delete *p_pixmap;
    *p_pixmap = pixmap;
  }
  else {
    delete m_pixmap;
    m_pixmap = pixmap;
  }
  img = new QImage(rgb->data, w, h, QImage::Format_ARGB32);
  if (!img || !pixmap->convertFromImage(*img)) {
    my_log("err #%s\n", "0xd644086d");
    goto end;
  }
  if (!grib_paint(pixmap)) {
    my_log("err #%s\n", "0xbaff791d");
    goto end;
  }
  ret = true;
end:
  delete img;
  delete rgb;
  if (!ret) {
    if (p_pixmap) {
      delete pixmap;
      *p_pixmap = nullptr;
    }
    clear_pix_iso();
  }
  return ret;
}

bool grib_data_c::grib_paint(QPixmap *pixmap)
{
  sets_c *sets = m_win->m_sets;
  const rtlv_c& par = m_tag_grib.m_rtlv;
  const char *tag_str = par.m_tag.c_str();
  grib_c *gr = m_tag_grib.m_gr;

  QPainter *painter = new QPainter();
  if (!painter->begin(pixmap)) {
    delete painter;
    my_log("err #%s\n", "0x51b72750");
    return false;
  }
  int w = pixmap->width();
  int h = pixmap->height();
  painter->setClipRegion(QRegion(0, 0, w, h));
  painter->setRenderHint(QPainter::Antialiasing, true);

  draw_isoline(m_win, w, h, painter, sets->m_iso_color, par, m_isolns, m_thr);

  if (VELOCITY_CODE(tag_str) || sets->m_wind_compose && par.m_role == GFS_025) {
    QPen pen(sets->m_wind_color);
    pen.setWidth(sets->m_wind_arrow_width);
    painter->setPen(pen);

    rtlv_c params1 = par.m_role == GFS_025 ? rtlv_c(par.m_role, "UGRD", "ISBL", sets->m_wind_lval.toStdString())
      : rtlv_c(par.m_role, "UGRD", "SFC", "0");
    rtlv_c params2 = par.m_role == GFS_025 ? rtlv_c(par.m_role, "VGRD", "ISBL", sets->m_wind_lval.toStdString())
      : rtlv_c(par.m_role, "VGRD", "SFC", "0");
    rtlv_c params3 = rtlv_c(par.m_role, VELOCITY_CORCODE(tag_str), par.m_lev, par.m_lval);
    grib_data_c *pdata1 = gr->grib_data_ptr(params1);
    grib_data_c *pdata2 = gr->grib_data_ptr(params2);
    grib_data_c *pdata3 = gr->grib_data_ptr(params3);
    const double *dbl = sets->m_wind_compose ? pdata1->get_plane() : get_plane();
    const double *dbl_1 = sets->m_wind_compose ? pdata2->get_plane() : pdata3->get_plane();

    if (dbl && dbl_1) {
      for (int i = 0; i < m_plane->ny(); i++) {
        for (int j = 0; j < m_plane->nx(); j++) {
#if defined(WAVE_ROLE) || defined(HRRR_ROLE)
          if (dbl[j + i * m_plane->nx()] == m_bad_val) // test for role?
            continue;
#endif
          double x = j * CANVAS_MAG * m_dlatlon.m_delta;
          double y = i * CANVAS_MAG * m_dlatlon.m_delta;
          double X = sets->m_wind_arrow_size * dbl[j + i * m_plane->nx()];
          double Y = -sets->m_wind_arrow_size * dbl_1[j + i * m_plane->nx()];
          double x1 = x + X;
          double y1 = y + Y;
          double d = hypot(X, Y);
          double cs = X / d;
          double sn = Y / d;
          double x3 = x1 - 3 * (cs - sn);
          double y3 = y1 - 3 * (cs + sn);
          double x4 = x1 - 3 * (cs + sn);
          double y4 = y1 - 3 * (sn - cs);

          QVector<QPointF> vec;
          vec << QPointF(x, y) << QPointF(x1, y1);
          vec << QPointF(x3, y3) << QPointF(x1, y1);
          vec << QPointF(x4, y4) << QPointF(x1, y1);
          painter->drawLines(vec);
        }
      }
    }
#ifdef DEBUG
    else {
      my_log("err #%s\n", "0x69bb37bb"); //disable wind toggle
    }
#endif
  }
  if (par.m_role == GFS_025) {
    rtlv_c par3[3];
    grib_data_c* pdata[3] = { 0, 0, 0 };
    QColor color[3];
    if (sets->m_tempre_compose && (par.m_tag != "TMP" || par.m_lev != "ISBL" || par.m_lval != sets->m_tempre_lval.toStdString())) {
      par3[0] = rtlv_c(par.m_role, "TMP", "ISBL", sets->m_tempre_lval.toStdString());
      pdata[0] = gr->grib_data_ptr(par3[0]);
      color[0] = sets->m_tempre_color;
    }
    if (sets->m_geopt_compose && (par.m_tag != "HGT" || par.m_lev != "ISBL" || par.m_lval != sets->m_geopt_lval.toStdString())) {
      par3[1] = rtlv_c(par.m_role, "HGT", "ISBL", sets->m_geopt_lval.toStdString());
      pdata[1] = gr->grib_data_ptr(par3[1]);
      color[1] = sets->m_geopt_color;
    }
    if (sets->m_precip_compose && (par.m_tag != "CWAT" || par.m_lev != "EATM" || par.m_lval != "0")) {
      par3[2] = rtlv_c(par.m_role, "CWAT", "EATM", "0");
      pdata[2] = gr->grib_data_ptr(par3[2]);
      color[2] = sets->m_precip_color;
    }
    for (int i = 0; i < 3; i++) {
      if (!pdata[i])
        continue;
      bool bb;
      if (pdata[i]->make_rgb(bb))
        draw_isoline(m_win, w, h, painter, color[i], par3[i], pdata[i]->m_isolns, pdata[i]->m_thr);
      else {
        my_log("err #%s\n", "0x6e832730");

        painter->end();
        delete painter;
        return false;
      }
    }
  }

  painter->end();
  delete painter;
  return true;
}

int grib_data_c::plane_nx() const
{
  return m_plane->nx();
}
int grib_data_c::plane_ny() const
{
  return m_plane->ny();
}
