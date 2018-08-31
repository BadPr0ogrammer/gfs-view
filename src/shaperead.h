/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

#include <opencv2/core/types_c.h>

using namespace std;
using namespace cv;

class shape_c
{
  vector<vector<CvPoint2D32f>*> *m_pline;
  QString                       m_lab;
  int                           m_shp_type;
  int                           m_popul;

public:
  shape_c() : m_pline(0), m_shp_type(0), m_popul(0) {}
  ~shape_c() {
    if (m_pline) for_each(m_pline->begin(), m_pline->end(), [](vector<CvPoint2D32f> *p) { delete p; });
    delete m_pline;
  }

  static vector<shape_c*>*              shape_load(QString name, int dbf_name, int dbf_pop, int charset);
  static vector<pair<QString, int>>*    get_dbf_fld_names(QString fname);
  static int                            dbf_fld_idx(const QStringRef& attr, vector<pair<QString, int>>* flds);

  const vector<vector<CvPoint2D32f>*>*  pline() { return m_pline; }
  const QString&                        lab() const { return m_lab; }
  int                                   popul() { return m_popul; }
};
