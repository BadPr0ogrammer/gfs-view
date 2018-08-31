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

class win_c;

class fcst_ui_c : public QGroupBox
{
  Q_OBJECT
public:
  QComboBox       *m_time0;
  QComboBox       *m_time1;
  QSpinBox        *m_fcst0;
  QSpinBox        *m_fcst1;
  QSpinBox        *m_step;
  win_c           *m_win;

  fcst_ui_c(win_c *win, const string& role);
};
