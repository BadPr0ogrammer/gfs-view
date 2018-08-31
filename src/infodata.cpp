/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "defs.h"
#include "color_tables.h"
#include "datefcst.h"
#include "griblatlon.h"
#include "shaperead.h"
#include "cntrs.h"
#include "mgrb2.h"
#include "ramppal.h"
#include "infocntrs.h"
#include "gribitem.h"
#include "infodata.h"
#include "winc.h"
#include "sets.h"
#include "staticstr.h"

#include "data2d_grib2.h"
#include "data2d_factory.h"

void info_data_c::clear_grib()
{
  m_cur_grib = tag_grib_c();
}

void info_data_c::set_cur_grib(const tag_grib_c& tag_grib)
{
  m_cur_grib = tag_grib;
}

void info_data_c::set_cur_grib(const rtlv_c& param, grib_c* grib)
{
  m_cur_grib = tag_grib_c(param, grib);
}

void info_data_c::set_cur_param(const rtlv_c& param)
{
  m_cur_grib.m_rtlv = param;
}

void info_data_c::vec_delete()
{
  if (m_vec) for_each(m_vec->begin(), m_vec->end(), [](info_cntrs_c *p) { delete p; });
  delete m_vec;
  m_vec = nullptr;
}

info_data_c::~info_data_c()
{
  vec_delete();

  if (m_gribs) for_each(m_gribs->begin(), m_gribs->end(), [](grib_c *p) { delete p; });
  delete m_gribs;

  for (auto it = m_files.begin(); it != m_files.end(); it++)
    delete (*it).m_file;

  delete m_geogr;
}

grib_c* info_data_c::add_slot(const datefc_c& date_id, bool *inplace)
{
  if (inplace)
    *inplace = false;
  if (!m_gribs) {
    m_gribs = new list<grib_c*>();
    grib_c *slot = new grib_c(date_id, m_win);
    m_gribs->push_back(slot);
    return slot;
  }

  list<grib_c*>::iterator it = find_if(m_gribs->begin(), m_gribs->end(), [=](grib_c *other) {
    return !datefc_c::datefc_cmp(date_id, other->datefc());
  });
  if (it == m_gribs->end()) {
    grib_c *slot = new grib_c(date_id, m_win);
    m_gribs->push_back(slot);
    m_gribs->sort([](grib_c *p1, grib_c *p2) { return datefc_c::datefc_cmp(p1->datefc(), p2->datefc()) < 0; });
    return slot;
  }
  else {
    if (inplace)
      *inplace = true;
    return *it;
  }
  return nullptr;
}

int info_data_c::slot_idx(const grib_c *slot)
{
  if (!m_gribs) {
    my_log("err #%s\n", "0x778459a2");
    return -1;
  }
  int ret = distance(m_gribs->begin(), find(m_gribs->begin(), m_gribs->end(), slot));
  return ret;
}

grib_c* info_data_c::get_slot(int cur)
{
  if (!m_gribs || !m_gribs->size() || cur < 0 || cur >= m_gribs->size()) {
    return nullptr;
  }
  list<grib_c*>::iterator it = m_gribs->begin();
  advance(it, cur);
  return it != m_gribs->end() ? *it : nullptr;
}

void info_data_c::clear_pal(const rtlv_c& params)
{
  for (auto it = m_gribs->begin(); it != m_gribs->end(); it++) {
    grib_c* grib = *it;
    if (grib && grib != m_cur_grib.m_gr) {
      grib_data_c *grib_data = grib->grib_data_ptr(params);
      if (grib_data)
        grib_data->clear_pix_iso();
    }
  }
}

int info_data_c::data_tmpl(data_tmpl_t &tmpl)
{
  tmpl.clear();
  int n = 0;
  if (m_gribs) {
    for (auto its = m_gribs->begin(); its != m_gribs->end(); its++) {
      grib_c *p = *its;
      string slot = p->m_datefc.runfc_str();
      for (auto itr = p->m_data_map.begin(); itr != p->m_data_map.end(); itr++) {
        const string& role = (*itr).first;
        for (auto itt = (*itr).second.begin(); itt != (*itr).second.end(); itt++) {
          const string& tag = (*itt).first;
          for (auto itl = (*itt).second.begin(); itl != (*itt).second.end(); itl++) {
            const string& lev = (*itl).first;
            for (auto itv = (*itl).second.begin(); itv != (*itl).second.end(); itv++) {
              const string& lval = (*itv).first;
              grib_data_c *grib_data = (*itv).second;
              if (grib_data->get_plane()) {
                tmpl[slot][role][tag][lev].push_back(lval);
                n++;
              }
            }
          }
        }
      }
    }
  }
  return n;
}

grib_c* info_data_c::get_grib(string run, string fc)
{
  grib_c *grib = nullptr;
  auto it = find_if(m_gribs->begin(), m_gribs->end(), [&](grib_c *other) {
    const datefc_c& p = other->datefc();
    bool ret = run == p.run_str() && fc == p.fc_str();
    if (ret)
      grib = other;
    return ret;
  });
  return grib;
}

bool data_tmpl_exists(const data_tmpl_t &tmpl, const rtlv_c& params)
{
  if (tmpl.empty() || params.is_empty())
    return false;
  bool ret = false;
  for (auto its = tmpl.begin(); its != tmpl.end(); its++) {
    auto itr = (*its).second.find(params.m_role);
    if (itr != (*its).second.end()) {
      auto itt = (*itr).second.find(params.m_tag);
      if (itt != (*itr).second.end()) {
        auto itl = (*itt).second.find(params.m_lev);
        if (itl != (*itt).second.end()) {
          auto itv = find_if((*itl).second.begin(), (*itl).second.end(), [&params](const string& a) {return a == params.m_lval; });
          if (itv != (*itl).second.end()) {
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

