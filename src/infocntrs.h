/**
Copyright Andrej Barkhatov, RPA Typhoon (Obninsk) (c) 2009 - 2018
*/
#pragma once

using namespace std;
using namespace cv;

class shape_c;
class cntrs_c;
class info_data_c;
class win_c;

class info_cntrs_c
{
  vector<shape_c*>            *m_shp;
  cntrs_c                     *m_cntrs;
  QString                     m_name;
  QString                     m_dbf_plparam;
  QVector<QStringRef>         m_dbf_plref;
  vector<pair<QString, int>>  *m_dbf_flds;
  QString                     m_font_face;
  int                         m_font_size;
  float                       m_line_w;
  QColor                      m_line_c;
  int                         m_dash;
  QColor                      m_text_c;
  win_c                       *m_win;

public:
  ~info_cntrs_c();
  info_cntrs_c(win_c *win) : m_win(win), m_shp(0), m_cntrs(0), m_font_size(5), m_line_w(1), m_dash(0), m_dbf_flds(0) {
    QFont font;
    m_font_face = font.defaultFamily();
  }

  bool make_cntrs();
  bool make_geogr();

  const cntrs_c*              cntrs() { return m_cntrs; }
  cntrs_c*                    cntrs_ptr() { return m_cntrs; }
  void                        set_cntrs(cntrs_c* cntrs) { m_cntrs = cntrs; }
  const QString&              dbf_plparam() const { return  m_dbf_plparam; }
  QString&                    dbf_plparam_ptr() { return  m_dbf_plparam; }
  void                        set_dbf_plparam(const QString& field) { m_dbf_plparam = field; }
  void                        set_dbf_plref(QVector<QStringRef> dbf_pls_ref) { m_dbf_plref = dbf_pls_ref; }
  const QVector<QStringRef>&  dbf_plref() const { return m_dbf_plref; }
  const QString&              name() const { return m_name; }
  void                        set_name(const QString& name) { m_name = name; }
  const QString&              font_face() const { return m_font_face; }
  void                        set_font_face(const QString& font_face) { m_font_face = font_face; }
  int                         font_size() { return  m_font_size; }
  void                        set_font_size(int font_size) { m_font_size = font_size; }
  float                       line_w() { return  m_line_w; }
  void                        set_line_w(float line_w) { m_line_w = line_w; }
  const QColor&               line_c() const { return m_line_c; }
  void                        set_line_c(const QColor& line_c) { m_line_c = line_c; }
  int                         dash() { return  m_dash; }
  void                        set_dash(int dash) { m_dash = dash; }
  const QColor&               text_c() const { return m_text_c; }
  void                        set_text_c(const QColor& text_c) { m_text_c = text_c; }
  const vector<shape_c*>      *shp() { return m_shp; }
  vector<shape_c*>            *shp_ptr() { return m_shp; }
};
Q_DECLARE_METATYPE(info_cntrs_c*);
