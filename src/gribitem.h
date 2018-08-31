/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

#include "defs.h"
#include "datefcst.h"

class rtlv_c
{
public:
  string  m_role;
  string  m_tag;
  string  m_lev;
  string  m_lval;
  rtlv_c() {}
  rtlv_c(const rtlv_c& param)
    : m_role(param.m_role), m_tag(param.m_tag), m_lev(param.m_lev), m_lval(param.m_lval) {}
  rtlv_c(const string& role, const string& tag, const string& lev, const string& lval)
    : m_role(role), m_tag(tag), m_lev(lev), m_lval(lval) {}
  void clear() { m_role.clear(); m_tag.clear(); m_lev.clear(); m_lval.clear(); }
  bool is_empty() const { return m_role.empty() || m_tag.empty() || m_lev.empty() || m_lval.empty(); }
  bool operator== (const rtlv_c& rhs) const {
    if (m_role == rhs.m_role && m_tag == rhs.m_tag && m_lev == rhs.m_lev && m_lval == rhs.m_lval) return true;
    return false;
  }
};
Q_DECLARE_METATYPE(rtlv_c);

class rtlv_rf_c : public rtlv_c
{
public:
  string  m_run;
  string  m_fc;

  rtlv_rf_c() {}
  rtlv_rf_c(const rtlv_rf_c& param) : rtlv_c(param), m_run(param.m_run), m_fc(param.m_fc) {}
  rtlv_rf_c(const rtlv_c& param, const string& run, const string& fc) : rtlv_c(param), m_run(run), m_fc(fc) {}
  rtlv_rf_c(const string& role, const string& tag, const string& lev, const string& lval, const string& run, const string& fc)
    : rtlv_c(role, tag, lev, lval), m_run(run), m_fc(fc) {}
  rtlv_rf_c(map<string, map<string, map<string, vector<string>>>>::const_iterator it1, const datefc_c& datefc) {
    m_role = (*it1).first;
    m_tag = (*it1).second.begin()->first;
    m_lev = (*it1).second.begin()->second.begin()->first;
    m_lval = (*it1).second.begin()->second.begin()->second.begin()[0];
    m_run = datefc.run_str();
    m_fc = datefc.fc_str();
  }
  void clear() { m_role.clear(); m_tag.clear(); m_lev.clear(); m_lval.clear(); m_run.clear(); m_fc.clear(); }
  bool operator== (const rtlv_rf_c& rhs) const {
    if (m_role == rhs.m_role && m_tag == rhs.m_tag && m_lev == rhs.m_lev && m_lval == rhs.m_lval && m_run == rhs.m_run && m_fc == rhs.m_fc) return true;
    return false;
  }
};
Q_DECLARE_METATYPE(rtlv_rf_c);

class grib_c;

class tag_grib_c
{
public:
  rtlv_c      m_rtlv;
  grib_c      *m_gr;

  tag_grib_c() : m_gr(0) {}
  tag_grib_c(const string& role, const string& tag, const string& lev, const string& lval, grib_c *grib = nullptr) : m_rtlv(role, tag, lev, lval), m_gr(grib) {}
  tag_grib_c(const rtlv_c& p, grib_c *grib) : m_rtlv(p), m_gr(grib) {}
};
Q_DECLARE_METATYPE(tag_grib_c);
