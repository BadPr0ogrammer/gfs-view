/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <opencv2/core/core.hpp>

#include <QtWidgets>

#include "defs.h"
#include "griblatlon.h"
#include "shaperead.h"
#include "cntrs.h"
#include "sets.h"

#define DASH_PATTERN \
if (line_dash) { \
  QVector<qreal> dashes; \
  if (line_dash == 1) dashes << 10 << 4; \
  else dashes << 10 << 3 << 1 << 3; \
  pen.setDashPattern(dashes); \
}

cntrs_c::~cntrs_c()
{
  if (m_parts) for (int i = 0; i < m_parts->size(); i++) delete (*m_parts)[i];
  delete m_parts;
  delete m_group;
}

const pair<float, float> cntrs_star[] = {
  {0.0, -0.5}, {0.14, -0.14}, {0.5, -0.14}, {0.22, 0.1}, {0.34, 0.5}, {0.0, 0.22}, {-0.34, 0.5}, {-0.22, 0.1}, {-0.22, 0.1}, {-0.5, -0.14}, {-0.14, -0.14}, { 0.0, -0.5 },
};
const pair<float, float> cntrs_triangle[] = {
  { 0.0, -0.5 },{ 0.5, 0.3 },{ -0.5, 0.3 },{ 0.0, -0.5 },
};
const pair<float, float> cntrs_cyrcle[] = {
  { 0, -0.3 }, { 0.15, -0.866 * 0.3 }, { 0.707 * 0.3, -0.707 * 0.3 }, { 0.866 * 0.3, -0.15 },
  { 0.3, 0 }, { 0.866 * 0.3, 0.15 }, { 0.707 * 0.3, 0.707 * 0.3 }, { 0.15, 0.866 * 0.3 },
  { 0, 0.3 }, { -0.15, 0.866 * 0.3 }, { -0.707 * 0.3, 0.707 * 0.3 }, { -0.866 * 0.3, 0.15, },
  { -0.3, 0 }, { -0.866 * 0.3, -0.15, }, { -0.707 * 0.3, -0.707 * 0.3 },{ -0.15, -0.866 * 0.3 },
  { 0, -0.3 },
};

cntrs_c::cntrs_c(vector<shape_c*> *shape, QGraphicsItemGroup *parent,
  const QColor& text_rgba, const QColor& line_rgba, float line_width, int line_dash, const QString& font_name, int font_size, const dlatlon_c& rect, double scale, int nx, int ny)
  : m_parts(0)
{
  QPen pen(line_rgba);
  pen.setWidthF(line_width);
  QFont font1(font_name, font_size);
  QFont font2(font_name, 1.25 * font_size);
  QFont font3(font_name, 1.75 * font_size);
  QFontMetrics fm1(font1);
  QFontMetrics fm2(font2);
  QFontMetrics fm3(font3);
  DASH_PATTERN;
  int w = scale * nx * rect.m_delta;
  int h = scale * ny * rect.m_delta;

  m_group = new QGraphicsItemGroup(parent);

  double lon_east = rect.east(nx);
  double lat_north = rect.north(ny);

  for (int i = 0; i < shape->size(); i++) {
    shape_c *shp = shape->at(i);
    if (!shp || !shp->pline()) {
      my_log("err #%s\n", "0xa5c95c89");
      continue;
    }
    QFontMetrics& pfm = shp->popul() < 1E5 ? fm1 : shp->popul() < 1E6 ? fm2 : fm3;

    vector<vector<CvPoint2D32f>*>::const_iterator it1;
    for (it1 = shp->pline()->begin(); it1 != shp->pline()->end(); it1++) {
      const vector<CvPoint2D32f> *a = *it1;
      vector<CvPoint2D32f>::const_iterator it2;
      for (it2 = a->begin(); it2 != a->end(); it2++) {
        const CvPoint2D32f &pt = *it2;
        if (pt.x >= rect.m_lon_ll && pt.x <= lon_east && pt.y >= rect.m_lat_ll && pt.y <= lat_north)
          break;
      }
      if (it2 != a->end())
        break;
    }

    if (it1 != shp->pline()->end()) {
      parts_c *parts = new parts_c();

      QPointF pt1;
      vector<CvPoint2D32f>::const_iterator it2;
      for (; it1 != shp->pline()->end(); it1++) {
        const vector<CvPoint2D32f> *a = *it1;

        for (it2 = a->begin(); it2 != a->end(); it2++) {
          CvPoint2D32f pt = *it2;
          if (pt.x >= rect.m_lon_ll && pt.x <= lon_east && pt.y >= rect.m_lat_ll && pt.y <= lat_north)
            break;
        }
        QVector<QPointF> tmp;
        for (; it2 != a->end(); it2++) {
          CvPoint2D32f pt = *it2;
          if (pt.x < rect.m_lon_ll || pt.x > lon_east || pt.y < rect.m_lat_ll || pt.y > lat_north)
          {
            if (tmp.size()) {
              QPolygonF poly(tmp);
              QPainterPath path;
              path.addPolygon(poly);
              QGraphicsPathItem *item = new QGraphicsPathItem(path);
              item->setPen(pen);
              if (!parts->m_path_lst)
                parts->m_path_lst = new QList<QGraphicsPathItem*>();
              parts->m_path_lst->append(item);

              m_group->addToGroup(item);

              tmp.clear();
            }
          }
          else {
            pt1 = QPointF(scale * (pt.x - rect.m_lon_ll), scale * (lat_north - pt.y));
            tmp.append(pt1);
          }
        }
        if (tmp.size()) {
          if (tmp.size() == 1) {
            QPointF p0 = tmp[0];
            tmp.clear();
            if (shp->popul() < 1E5) {
              for (int i = 0; i < array_countof(cntrs_cyrcle); i++) {
                QPointF p(p0.x() + 12 * cntrs_cyrcle[i].first, p0.y() + 12 * cntrs_cyrcle[i].second);
                tmp.append(p);
              }
            }
            else if (shp->popul() < 1E6) {
              for (int i = 0; i < array_countof(cntrs_triangle); i++) {
                QPointF p(p0.x() + 12 * cntrs_triangle[i].first, p0.y() + 12 * cntrs_triangle[i].second);
                tmp.append(p);
              }
            }
            else {
              for (int i = 0; i < array_countof(cntrs_star); i++) {
                QPointF p(p0.x() + 12 * cntrs_star[i].first, p0.y() + 12 * cntrs_star[i].second);
                tmp.append(p);
              }
            }
          }
          QPolygonF poly(tmp);
          QPainterPath path;
          path.addPolygon(poly);
          QGraphicsPathItem *item = new QGraphicsPathItem(path);
          item->setPen(pen);

          if (!parts->m_path_lst)
            parts->m_path_lst = new QList<QGraphicsPathItem*>();
          parts->m_path_lst->append(item);

          m_group->addToGroup(item);
        }
      }
      if (!shp->lab().isEmpty()) {
        if (pfm.width(shp->lab()) < w - pt1.x() && pfm.height() < h - pt1.y()) {
          parts->m_lab = new QGraphicsTextItem(shp->lab());
          parts->m_lab->setPos(pt1.x(), pt1.y() - pfm.height());
          parts->m_lab->setDefaultTextColor(text_rgba);
          parts->m_lab->setFont(shp->popul() < 1E5 ? font1 : shp->popul() < 1E6 ? font2 : font3);

          m_group->addToGroup(parts->m_lab);
        }
      }
      if (!m_parts)
        m_parts = new vector<parts_c*>();
      m_parts->push_back(parts);
    }
  }
}

bool cntrs_c::make_geogrid(QGraphicsItemGroup *group, const dlatlon_c& rect, double scale, int nx, int ny, const QColor &color, double pen_width, bool label_f)
{
  double step = 30;
  while (rect.width(nx) / step < 10) {
    if (step == 30)
      step = 10;
    else
      step /= 2;
  }
  {
    QPen pen(color);
    pen.setWidthF(pen_width);

    double x = step * ((int)(rect.m_lon_ll / step) + (fmod(rect.m_lon_ll, step) != 0));
    QPainterPath path;
    for (; x <= rect.east(nx); x += step) {
      qreal x2 = scale * (x - rect.m_lon_ll);
      path.moveTo(x2, 0);
      path.lineTo(x2, scale * rect.height(ny));
    }
    double y = step * ((int)(rect.m_lat_ll / step) + (fmod(rect.m_lat_ll, step) != 0));
    for (; y <= rect.north(ny); y += step) {
      qreal y2 = scale * (rect.north(ny) - y);
      path.moveTo(0, y2);
      path.lineTo(scale * rect.width(nx), y2);
    }
    QGraphicsPathItem *itemx = new QGraphicsPathItem(path);
    itemx->setPen(pen);
    group->addToGroup(itemx);
  }
  if (label_f) {
    QFont font;
    font.setPointSize(DEF_FONT_SIZE);
    QFontMetrics fm(font);

    double y = step * ((int)(rect.m_lat_ll / step) + (fmod(rect.m_lat_ll, step) != 0));
    for (; y <= rect.north(ny); y += step) {
      qreal y2 = scale * (rect.north(ny) - y);
      double x = step * ((int)(rect.m_lon_ll / step) + (fmod(rect.m_lon_ll, step) != 0));
      for (; x <= rect.east(nx); x += step) {
        qreal x2 = scale * (x - rect.m_lon_ll);
        QGraphicsTextItem *text = new QGraphicsTextItem(QString::number(x));
        text->setPos(x2, y2);
        text->setDefaultTextColor(Qt::gray);
        text->setFont(font);
        group->addToGroup(text);
      }
    }
  }
  return true;
}

void cntrs_c::set_line(QColor color, float line_width, int line_dash)
{
  if (!m_parts || !(*m_parts->begin())->m_path_lst) {
    my_log("err #%s\n", "0xdbecb1aa");

    return;
  }
  parts_c *parts = *m_parts->begin();
  QGraphicsPathItem *item = *parts->m_path_lst->begin();
  QAbstractGraphicsShapeItem *aitem = (QAbstractGraphicsShapeItem *)item;
  QPen pen;
  if (aitem)
    pen = aitem->pen();
  pen.setColor(color);
  pen.setWidth(line_width);
  DASH_PATTERN;

  vector<parts_c*>::iterator it;
  for (it = m_parts->begin(); it != m_parts->end(); it++) {
    parts_c *parts = *it;
    if (parts && parts->m_path_lst) {
      QList<QGraphicsPathItem*>::iterator it1;
      for (it1 = parts->m_path_lst->begin(); it1 != parts->m_path_lst->end(); it1++) {
        QGraphicsPathItem *p = *it1;
        QAbstractGraphicsShapeItem *aitem1 = (QAbstractGraphicsShapeItem *)p;
        if (aitem1)
          aitem1->setPen(pen);
      }
    }
    else {
      my_log("err #%s\n", "0xe34b92d7");
    }
  }
}

void cntrs_c::set_text_color(QColor color)
{
  if (!m_parts) {
    my_log("err #%s\n", "0x678d0f4b");
    return;
  }
  parts_c *parts = *m_parts->begin();
  if (!parts || !parts->m_lab) {
    return;
  }
  for_each(m_parts->begin(), m_parts->end(), [&](parts_c *parts) {
    if (parts && parts->m_lab)
      parts->m_lab->setDefaultTextColor(color);
  });
}

void cntrs_c::set_font(QString family, int size)
{
  if (!m_parts) {
    my_log("err #%s\n", "0x736d148");
    return;
  }
  parts_c *parts = *m_parts->begin();
  if (!parts || !parts->m_lab) {
    return;
  }
  QFont font(family, size);
  for_each(m_parts->begin(), m_parts->end(), [&](parts_c *parts) {
    if (parts && parts->m_lab)
      parts->m_lab->setFont(font);
  });
}
