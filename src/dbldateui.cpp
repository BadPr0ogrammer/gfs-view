/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtNetwork>

#include <opencv2/core/core.hpp>

#include "local_str.h"
#include "griblatlon.h"
#include "sets.h"
#include "winc.h"
#include "dbldateui.h"

int currentRuntime(QDateTime& utc, int mlag, string role)
{
  int runtime = 0;
  int hour = utc.time().hour();
  int minute = utc.time().minute();
  bool prev = false;
  if (role == GFS_025) {
    prev = hour <= 2 || hour == 3 && minute < mlag;
    runtime = (hour >= 22 || hour == 21 && minute >= mlag) || prev ? 18 :
      hour >= 16 || hour == 15 && minute >= mlag ? 12 :
      hour >= 10 || hour == 9 && minute >= mlag ? 6 : 0;
  }
  else {
    prev = hour <= 3 || hour == 4 && minute < mlag;
    runtime = (hour >= 23 || hour == 22 && minute >= mlag) || prev ? 18 :
      hour >= 17 || hour == 16 && minute >= mlag ? 12 :
      hour >= 11 || hour == 10 && minute >= mlag ? 6 : 0;
  }
  if (prev)
    utc.addDays(-1);
  return runtime;
}

QDateTime current_UTC(int *p_runtime, int runtime_pad, const string& role)
{
  QDateTime local(QDateTime::currentDateTime());
  QDateTime UTC(local.toTimeSpec(Qt::UTC));
  int runtime = currentRuntime(UTC, runtime_pad, role);
  if (p_runtime)
    *p_runtime = runtime;
  return UTC;
}

dbl_date_ui_c::dbl_date_ui_c(QDateTime *p_utc, int begin_off, const string& role, win_c *win) : m_win(win)
{
  setTitle(IDSTR_STARTING_ENDING_DATE);

  QHBoxLayout *hbox4 = new QHBoxLayout;
  setLayout(hbox4);

  m_datetime0 = new QDateTimeEdit;
  m_datetime0->setCalendarPopup(true);
  hbox4->addWidget(m_datetime0);

  m_datetime1 = new QDateTimeEdit;
  m_datetime1->setCalendarPopup(true);
  hbox4->addWidget(m_datetime1);

  QDateTime UTC = current_UTC(nullptr, 0, role);
  const QDateTime &utc = p_utc ? *p_utc : UTC;

  m_datetime0->setDate(utc.date());
  m_datetime1->setDate(utc.date());
  m_datetime0->setMaximumDate(utc.date());
  m_datetime1->setMaximumDate(utc.date());
  m_datetime0->setMinimumDate(utc.date().addDays(-begin_off));
  m_datetime1->setMinimumDate(utc.date().addDays(-begin_off));
}
