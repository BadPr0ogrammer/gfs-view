/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtNetwork>

#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

#include "defs.h"
#include "gribini.h"
#include "regui.h"
#include "dbldateui.h"
#include "fcstui.h"
#include "tagtreeui.h"

class win_c;

class dlg_datetime_c : public QDialog
{
  Q_OBJECT
public:
  reg_ui_c            m_reg_ui;
  dbl_date_ui_c       m_dbl_date_ui;
  fcst_ui_c           m_fcst_ui;
  tag_tree_ui_c       m_tag_tree;
  QCheckBox           *m_dwnl_all;

  win_c               *m_win;

  dlg_datetime_c(win_c *win);

public slots:
  void download();
  void reject();
};
