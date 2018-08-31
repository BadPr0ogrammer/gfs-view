/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

#include <set>

#include "infocntrs.h"
#include "gribitem.h"
#include "mgrb2.h"
#include "ramppal.h"
#include "gribini.h"

using namespace std;
using namespace cv;

class dlatlon_c;
class datefc_c;
class grib_plane_c;
class grib_data_c;
class grib_c;
class info_cntrs_c;
class info_data_c;
class win_c;

class MetGrib2DataFile;

class gribf_fname_c
{
public:
  MetGrib2DataFile * m_file;
  string            m_fname;

  gribf_fname_c() : m_file(0) {}
  gribf_fname_c(MetGrib2DataFile* file, const string& fname) : m_file(file), m_fname(fname) {}
  bool operator< (const gribf_fname_c& other) const { return m_fname == other.m_fname; }
};

class info_data_c
{
  list<grib_c*>             *m_gribs;
  list<info_cntrs_c*>       *m_vec;
  info_cntrs_c              *m_geogr;
  tag_grib_c                m_cur_grib;
  map<string,
    map<string,
    map<string,
    map<string,
    ramp_c>>>>              m_ramp;
  set<gribf_fname_c>        m_files;
  win_c                     *m_win;

public:
  info_data_c(win_c *win) : m_gribs(0), m_vec(0), m_win(win), m_geogr(0) { }
  ~info_data_c();

  grib_c*       add_slot(const datefc_c& date_id, bool *inplace);
  int           data_tmpl(data_tmpl_t &tmpl);

  int       slot_idx(const grib_c *slot);
  grib_c*   get_slot(int cur);
  bool      read_info_grib(QString filename, const datefc_c& datefc, const string& role, rtlv_c& pparam, set<string>& strid);
  void      vec_delete();

  const list<grib_c*>*        gribs() const { return m_gribs; }
  list<grib_c*>*              gribs_ptr() { return m_gribs; }
  const grib_c*               cur_grib() const { return m_cur_grib.m_gr; }
  grib_c*                     cur_grib_ptr() { return m_cur_grib.m_gr; }
  const rtlv_c&               cur_param() const { return m_cur_grib.m_rtlv; }
  const tag_grib_c&           tag_grib() { return m_cur_grib; }
  const grib_data_c*          cur_gdata() const { return m_cur_grib.m_gr ? m_cur_grib.m_gr->grib_data(m_cur_grib.m_rtlv) : nullptr; }
  grib_data_c*                cur_gdata_ptr() { return m_cur_grib.m_gr ? m_cur_grib.m_gr->grib_data_ptr(m_cur_grib.m_rtlv) : nullptr; }
  void                        set_cur_grib(const tag_grib_c& tag_grib);
  void                        set_cur_grib(const rtlv_c& param, grib_c* grib);
  void                        set_cur_param(const rtlv_c& param);
  void                        clear_grib();
  const list<info_cntrs_c*>*  vec() { return m_vec; }
  list<info_cntrs_c*>*        vec_ptr() { return m_vec; }
  void                        set_vec(list<info_cntrs_c*>* vec) { m_vec = vec; }
  const info_cntrs_c*         geogr() { return m_geogr; }
  info_cntrs_c*               geogr_ptr() { return m_geogr; }
  void                        clear_geogr() { delete m_geogr; m_geogr = nullptr; }
  void                        set_geogr(info_cntrs_c *geogr) { m_geogr = geogr; }
  grib_c*                     get_grib(string run, string fc);
  void                        clear_pal(const rtlv_c& params);

  map<string,
    map<string,
    map<string,
    map<string,
    ramp_c>>>>&ramp_ptr() { return m_ramp; }
  set<gribf_fname_c>&         files_ptr() { return m_files; }
  bool                        gdata_empty() { return !m_cur_grib.m_gr || !m_cur_grib.m_gr->data_map().size() ? true : false; }
};

bool   data_tmpl_exists(const data_tmpl_t &tmpl, const rtlv_c& params);