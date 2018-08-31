/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <opencv2/core/core.hpp>

#include <QtCore>
#include <QTextCodec>

#include <shapefil.h>

#include "defs.h"
#include "staticstr.h"
#include "shaperead.h"

#include <QtWidgets>
#include "places.h"

vector<shape_c*>* shape_c::shape_load(QString fname, int dbf_name, int dbf_pop, int charset)
{
  vector<shape_c*>* ret = new vector<shape_c*>();
  QTextCodec *codec = QTextCodec::codecForName(char_set_names[charset]);

#ifdef _WINDOWS
  wchar_t name[MAX_PATH];
  fname.toWCharArray(name);
  name[fname.length()] = 0;

  SHPHandle shp = SHPOpen(name, "rb");
#else
  SHPHandle shp = SHPOpen(fname.toUtf8().data(), "rb");
#endif
  if (!shp || !shp->nRecords) {
    my_log("err #%s\n", "0xdd4ea77e");

    if (shp)
      SHPClose(shp);
    return ret;
  }

  DBFHandle dbf = nullptr;
  int field_num = 0;
  if (dbf_name >= 0) {
#ifdef _WINDOWS
    dbf = DBFOpen(name, "rb");
#else
    dbf = DBFOpen(fname.toUtf8().data(), "rb");
#endif
    field_num = DBFGetFieldCount(dbf);
  }


  for (int i = 0; i < shp->nRecords; i++) {
    shape_c *shape = new shape_c();

    SHPObject *obj = SHPReadObject(shp, i);
    if (!obj) {
      my_log("err #%s\n", "0x9593cfa4");
      continue;
    }
    shape->m_shp_type = obj->nSHPType;

    int j;
    for (j = 0; j < MAX(1, obj->nParts); j++) {
      int k0 = obj->nParts > 1 ? obj->panPartStart[j] : 0;
      int k1 = obj->nParts > 1 && j < obj->nParts - 1 ? obj->panPartStart[j + 1] : obj->nVertices;
      vector<CvPoint2D32f> *tmp = new vector<CvPoint2D32f>();

      int k;
      for (k = k0; k < k1; k++) {
        CvPoint2D32f pt = cvPoint2D32f(obj->padfX[k], obj->padfY[k]);
        tmp->push_back(pt);
      }
      if (tmp->size()) {
        if (!shape->m_pline)
          shape->m_pline = new vector<vector<CvPoint2D32f>*>();
        shape->m_pline->push_back(tmp);
      }
      else {
        my_log("err #%s\n", "0xe4759d10");
        delete tmp;
      }
    }
    if (shape->m_pline && dbf && field_num > dbf_name) {
      const char *str = DBFReadStringAttribute(dbf, i, dbf_name);
      shape->m_lab = codec->toUnicode(str);
      int n = DBFReadIntegerAttribute(dbf, i, dbf_pop);
      shape->m_popul = n;
    }

    SHPDestroyObject(obj);
    if (shape->m_pline)
      ret->push_back(shape);
    else {
      my_log("err #%s\n", "0x44ace496");
      delete shape;
    }
  }
  if (dbf)
    DBFClose(dbf);


  if (shp)
    SHPClose(shp);
  return ret;
}

vector<pair<QString, int>>* shape_c::get_dbf_fld_names(QString fname)
{
#ifdef _WINDOWS
  wchar_t name[LARGEBUF];
  fname.toWCharArray(name);
  name[fname.length()] = 0;

  DBFHandle dbf = DBFOpen(name, "rb");
#else
  DBFHandle dbf = DBFOpen(fname.toUtf8().data(), "rb");
#endif
  vector<pair<QString, int>>* ret = nullptr;
  if (dbf) {
    char buf[SMALLBUF];
    int i, num = DBFGetFieldCount(dbf);

    for (i = 0; i < num; i++) {
      memset(buf, 0, SMALLBUF);
      if (DBFGetFieldInfo(dbf, i, buf, nullptr, nullptr) != FTInvalid && buf[0]) {
        pair<QString, int> b;
        //        buf[31] = 0;
        b.first = buf;
        b.second = i;
        if (!ret)
          ret = new vector<pair<QString, int>>();
        ret->push_back(b);
      }
    }
    DBFClose(dbf);
  }
  return ret;
}

int shape_c::dbf_fld_idx(const QStringRef& attr, vector<pair<QString, int>>* flds)
{
  int ret = -1;
  if (flds && !attr.isEmpty()) {
    for (int i = 0; i < flds->size(); i++) {
      pair<QString, int> &b = (*flds)[i];
      if (!QString::compare(b.first, attr, Qt::CaseInsensitive)) {
        ret = b.second;
        break;
      }
    }
  }
  return ret;
}
