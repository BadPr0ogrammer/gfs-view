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

class win_c;

class dbl_date_ui_c : public QGroupBox
{
  Q_OBJECT
public:
  QDateTimeEdit   *m_datetime0;
  QDateTimeEdit   *m_datetime1;
  win_c           *m_win;

  dbl_date_ui_c(QDateTime *p_utc, int begin_off, const string& role, win_c *win);

};

QDateTime current_UTC(int *p_runtime, int runtime_pad, const string& role);
int currentRuntime(QDateTime& utc, int mlag, string role);
