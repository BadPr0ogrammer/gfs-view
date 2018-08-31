/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <opencv2/core/core.hpp>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "defs.h"
#include "local_str.h"
#include "staticstr.h"
#include "color_tables.h"
#include "datefcst.h"
#include "shaperead.h"
#include "griblatlon.h"
#include "cntrs.h"
#include "mgrb2.h"
#include "graphicsvw.h"
#include "sets.h"
#include "infodata.h"
#include "winc.h"

void graphics_view_c::mouseMoveEvent(QMouseEvent *event)
{
  const info_data_c *data;
  const grib_data_c *pdata;
  const grib_c *grib;
  if (event && (data = m_win->m_data) && data->gribs() && (grib = data->cur_grib()) && (pdata = data->cur_gdata())) {
    m_mouse_pos = event->pos();
    m_scene_pos = mapToScene(event->pos());
    double x = pdata->dlatlon().m_delta + m_scene_pos.rx() / CANVAS_MAG;
    double y = pdata->dlatlon().north(pdata->plane_ny()) - m_scene_pos.ry() / CANVAS_MAG;
    double x1 = (x - (int)x) * 60;
    double y1 = (y - (int)y) * 60;
    static char buf[SMALLBUF];
    sprintf(buf, "%02d\302\260%02d'N  %02d\302\260%02d'E", (int)y, (int)y1, (int)x, (int)x1);
    m_win->m_latlon->setText(buf);
  }
}

void graphics_view_c::zooming(int angle)
{
  double factor = qPow(m_zoom, angle);
  scale(factor, factor);

  QPointF delta = m_mouse_pos - QPointF(viewport()->width() / 2.0, viewport()->height() / 2.0);
  QPointF center = mapFromScene(m_scene_pos) - delta;
  centerOn(mapToScene(center.toPoint()));

  m_win->move_ramppal();
}

void graphics_view_c::wheelEvent(QWheelEvent *event)
{
  if (event) {
    int angle = event->angleDelta().y();
    zooming(angle);
  }
}
