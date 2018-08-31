/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <opencv2/core/core.hpp>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "local_str.h"
#include "regui.h"
#include "datefcst.h"
#include "griblatlon.h"
#include "mgrb2.h"
#include "gribitem.h"
#include "gribini.h"
#include "infodata.h"
#include "fcstui.h"
#include "sets.h"
#include "winc.h"
#include "dbldateui.h"

fcst_ui_c::fcst_ui_c(win_c *win, const string& role) : m_win(win), m_fcst0(0), m_fcst1(0), m_step(0)
{
  setTitle(IDSTR_FORECAST_RUN_PARAMS);

  QHBoxLayout *hbox = new QHBoxLayout;
  setLayout(hbox);

  QLabel *label = new QLabel(IDSTR_RUN_TIME_FROM_C);
  label->setAlignment(Qt::AlignVCenter);
  hbox->addWidget(label);

  int run_time;
  current_UTC(&run_time, m_win->m_sets->m_runtime_lag, role);

  m_time0 = new QComboBox;
  QList<QString> hours_list = { "00", "06", "12", "18" };
  m_time0->addItems(hours_list);
  m_time0->setCurrentIndex(run_time / 6);
  hbox->addWidget(m_time0);

  label = new QLabel(IDSTR_TO_C);
  label->setAlignment(Qt::AlignVCenter);
  hbox->addWidget(label);

  m_time1 = new QComboBox;
  m_time1->addItems(hours_list);
  m_time1->setCurrentIndex(run_time / 6);
  hbox->addWidget(m_time1);

  label = new QLabel(IDSTR_FC_FROM_C);
  label->setAlignment(Qt::AlignVCenter);
  hbox->addWidget(label);

  if (role == GFS_025) {
    sets_c *sets = m_win->m_sets;

    m_fcst0 = new QSpinBox;
    m_fcst0->setRange(0, 384);
    m_fcst0->setValue(sets->m_fcst_from);
    hbox->addWidget(m_fcst0);
    QWidget::connect(m_fcst0, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&](int i) { m_win->m_sets->m_fcst_from = i; });

    label = new QLabel(IDSTR_TO_C);
    label->setAlignment(Qt::AlignVCenter);
    hbox->addWidget(label);

    m_fcst1 = new QSpinBox;
    m_fcst1->setRange(0, 384);
    m_fcst1->setValue(sets->m_fcst_to);
    hbox->addWidget(m_fcst1);
    QWidget::connect(m_fcst1, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&](int i) { m_win->m_sets->m_fcst_to = i; });

    label = new QLabel(IDSTR_STEP);
    label->setAlignment(Qt::AlignVCenter);
    hbox->addWidget(label);

    m_step = new QSpinBox;
    m_step->setRange(1, 24);
    m_step->setSingleStep(1);
    m_step->setValue(sets->m_fcst_step);
    hbox->addWidget(m_step);
    QWidget::connect(m_step, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&](int i) { m_win->m_sets->m_fcst_step = i; });
  }
}
