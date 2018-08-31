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
#include "infocntrs.h"
#include "infodata.h"
#include "sets.h"
#include "winc.h"

info_cntrs_c::~info_cntrs_c()
{
  if (m_shp) for_each(m_shp->begin(), m_shp->end(), [](shape_c *p) { delete p; });
  delete m_shp;
  delete m_cntrs;
  delete m_dbf_flds;
}

bool info_cntrs_c::make_cntrs()
{
  info_data_c *data = m_win->m_data;
  if (!data)
    return false;
  const grib_data_c *pdata = data->cur_gdata();
  if (!pdata)
    return false;

  QString fname = m_win->m_persfold + DIR_SEPARATOR_S MY_SHP_IMPORT DIR_SEPARATOR_S + m_name;
  m_dbf_flds = shape_c::get_dbf_fld_names(fname);
  m_shp = shape_c::shape_load(fname,
    m_dbf_flds && m_dbf_flds->size() > 0 && m_dbf_plref.size() > 0 ? shape_c::dbf_fld_idx(m_dbf_plref[0], m_dbf_flds) : -1,
    m_dbf_flds && m_dbf_flds->size() > 1 && m_dbf_plref.size() > 1 ? shape_c::dbf_fld_idx(m_dbf_plref[1], m_dbf_flds) : -1,
    m_win->m_sets->m_charset);

  if (!m_shp->empty())
    m_cntrs = new cntrs_c(m_shp, m_win->m_cntrs_group, m_text_c, m_line_c, m_line_w, m_dash, m_font_face, m_font_size,
      pdata->dlatlon(), CANVAS_MAG, pdata->plane_nx(), pdata->plane_ny());
  if (m_shp && m_cntrs && m_cntrs->parts()) {
    if (!data->vec())
      data->set_vec(new list<info_cntrs_c*>());
    data->vec_ptr()->push_back(this);
    return true;
  }
  else {
    my_log("err #%s\n", "0x92bcbc5");
    delete this;
  }
  return false;
}

bool info_cntrs_c::make_geogr()
{
  info_data_c *data = m_win->m_data;
  if (!data)
    return false;
  const grib_data_c *pdata = data->cur_gdata();
  if (!pdata)
    return false;

  m_cntrs = new cntrs_c();
  if (m_cntrs->make_geogrid(m_win->m_geogr_group, pdata->dlatlon(), CANVAS_MAG,
    pdata->plane_nx(), pdata->plane_ny(),
    m_win->m_sets->m_geogrid_color, m_win->m_sets->m_geogrid_width, m_win->m_sets->m_geogrid_label))
    return true;
  else {
    my_log("err #%s\n", "0xe89d9f4f");
    delete this;
  }
  return false;
}
