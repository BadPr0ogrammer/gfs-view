/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <opencv2/core/core.hpp>

#include "griblatlon.h"

using namespace std;
using namespace cv;

class QQuickWindow;
class win_c;

class reg_ui_c : public QGroupBox
{
  Q_OBJECT
public:
  QComboBox           *m_pl_combo;
  QTreeView           *m_pl_view;
  QStandardItemModel  *m_pl_model;
  QDoubleSpinBox      *m_reg_lon;
  QDoubleSpinBox      *m_reg_lat;
  QDoubleSpinBox      *m_reg_width;
  QDoubleSpinBox      *m_reg_height;
  QQuickWindow        *m_quick_map;
  QPushButton         *m_osm_btn;

  win_c           *m_win;

  reg_ui_c(QWidget *parent, bool osm, win_c *win);

public slots:
  void quick_map_clicked();
  void quick_map_done(QVariant rectObj);
  void quick_map_cleanup();
};

