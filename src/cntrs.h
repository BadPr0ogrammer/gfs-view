/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

using namespace std;
using namespace cv;

class shape_c;
class ll_region_c;
class dlatlon_c;

class parts_c
{
public:
  QList<QGraphicsPathItem*>   *m_path_lst;
  QGraphicsTextItem           *m_lab;

  parts_c() : m_path_lst(0), m_lab(0) {}
  ~parts_c() {
    if (m_path_lst) for (int i = 0; i < m_path_lst->size(); i++) delete m_path_lst->at(i);
    delete m_path_lst;
    delete m_lab;
  }
};

class cntrs_c
{
  vector<parts_c*>            *m_parts;
  QGraphicsItemGroup          *m_group;
  vector<pair<QString, int>>  m_dbf_idx;

public:
  cntrs_c() : m_parts(0), m_group(0) {}
  cntrs_c(vector<shape_c*> *shape, QGraphicsItemGroup *parent,
    const QColor& text_rgba, const QColor& line_rgba, float line_width, int line_dash, const QString& font_name, int font_size, const dlatlon_c& rect, double scale, int nx, int ny);
  ~cntrs_c();

  void set_line(QColor color, float line_width, int line_dash);
  void set_text_color(QColor color);
  void set_font(QString family, int size);
  bool make_geogrid(QGraphicsItemGroup *group, const dlatlon_c& rect, double scale, int nx, int ny, const QColor &color, double pen_width, bool label_f);

  const vector<parts_c*>*   parts() { return m_parts; }
  vector<parts_c*>*         parts_ptr() { return m_parts; }
  QGraphicsItemGroup*       group_ptr() { return m_group; }
};
