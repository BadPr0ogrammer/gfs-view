/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

#include "gribitem.h"

class win_c;
class grib_data_c;
class grib_c;
class grib_lval_cmp_c;

#include "mgrb2.h" // for data_map_t
#include "gribini.h" // for data_tmpl_t

class tag_tree_ui_c : public QObject
{
  Q_OBJECT
public:
  QStandardItemModel * m_model;
  QTreeView           *m_view;
  win_c               *m_win;

  tag_tree_ui_c(win_c *pwin, bool sig = true);
  ~tag_tree_ui_c() { delete m_model; delete m_view; }

  void  tree_from_gribs(const list<grib_c*>* grib_list);
  bool  tree_add(bool check, bool abbr, bool clear, const string& role, const data_tmpl_t* tmpl, bool force_check);
  void  tree_done() { m_model->removeRows(0, m_model->rowCount()); }

  void check_all(bool f);
  bool tree_map(slot_tmpl_t &tmpl);
  QModelIndex tree_get_sel(const rtlv_rf_c& params);
  void expand_all(int level);
  bool append(QStandardItem **p_item, QStandardItem *parent, const rtlv_rf_c& obj, const char *name, bool sel);

public slots:
  void tree_sel_changed(const QModelIndex &current, const QModelIndex &);
};
