/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <QtCore>

#include "defs.h"
#include "datefcst.h"
#include "staticstr.h"

const QRegularExpression datefc_c::m_regex_gfs(GFS_025_PATTERN);

int datefc_c::date_only_cmp(const datefc_c &p1, const datefc_c &p2)
{
  if (p1.m_year != p2.m_year)
    return p1.m_year - p2.m_year;
  if (p1.m_mon != p2.m_mon)
    return p1.m_mon - p2.m_mon;
  if (p1.m_day != p2.m_day)
    return p1.m_day - p2.m_day;
  if (p1.m_hour != p2.m_hour)
    return p1.m_hour - p2.m_hour;
  return 0;
}

int datefc_c::datefc_cmp(const datefc_c &p1, const datefc_c &p2)
{
  int d0 = p1.hour_range(p2, nullptr, false, 1);
  int d1 = -d0 + p1.m_fcst - p2.m_fcst;
  return !d1 ? date_only_cmp(p1, p2) : d1;
}

datefc_c::datefc_c(int year, int mon, int day, int hour, int fcst)
{
  m_year = year;
  m_mon = mon;
  m_day = day;
  m_hour = hour;
  m_fcst = fcst;
}

datefc_c::datefc_c(const datefc_c &date)
{
  m_year = date.m_year;
  m_mon = date.m_mon;
  m_day = date.m_day;
  m_hour = date.m_hour;
  m_fcst = date.m_fcst;
}

datefc_c::datefc_c(const datefc_c &date, int fcst)
{
  m_year = date.m_year;
  m_mon = date.m_mon;
  m_day = date.m_day;
  m_hour = date.m_hour;
  m_fcst = fcst;
}

bool datefc_c::parse(const char *date)
{
  int n = strlen(date);
  if (n == 10 || n == 8 || n == 12 || n == 13) {
    m_year = scanf_atoi(date, 4);
    date += 4;
    m_mon = scanf_atoi(date, 2);
    date += 2;
    m_day = scanf_atoi(date, 2);
    if (n >= 10) {
      date += 2;
      m_hour = scanf_atoi(date, 2);
      if (n >= 12) {
        date += 2;
        m_fcst = atoi(date);
      }
    }
    if (m_day < 1 || m_day > 31 || m_mon < 1 || m_mon > 12) {
      m_hour = m_day = m_mon = m_year = m_fcst = 0;
      my_log("err #%s\n", "0x3d2883a0");
      return false;
    }
    return true;
  }
  else {
    m_hour = m_day = m_mon = m_year = m_fcst = 0;
    my_log("err #%s\n", "0x17eadf9f");
  }
  return false;
}

datefc_c::datefc_c(const char *date, int fcst) : m_fcst(fcst), m_year(-1), m_mon(-1), m_day(-1), m_hour(-1)
{
  if (date)
    parse(date);
}

datefc_c::datefc_c(const char *date) : m_fcst(-1), m_year(-1), m_mon(-1), m_day(-1), m_hour(-1)
{
  if (date)
    parse(date);
}

string datefc_c::run_str() const
{
  string ret;
  char buf[SMALLBUF] = { 0 };
  sprintf(buf, RUN_FORMAT, m_year, m_mon, m_day, m_hour);
  return ret = buf;
}

string datefc_c::runfc_str() const
{
  string ret;
  char buf[SMALLBUF] = { 0 };
  sprintf(buf, RUN_FORMAT "%03d", m_year, m_mon, m_day, m_hour, m_fcst);
  return ret = buf;
}

int datefc_c::scanf_atoi(const char *buf, int sz)
{
  int i, ret = 0;
  for (i = 0; i < sz; i++) {
    if (!buf[i]) {
      my_log("err #%s\n", "0xea5729db");
      break;
    }
    ret += (buf[i] - '0') * ((int)pow(10, sz - 1 - i));
  }
  return ret;
}

bool datefc_c::parse_img_name(const char *name, string& role_str)
{
  int ret = false;

  QRegularExpressionMatch match = m_regex_gfs.match(name);
  int role = GFS_025_IDX;
  if (match.hasMatch()) {
    QString date = match.captured(1);
    QByteArray b = date.toLatin1();
    if (parse(b.constData())) {
      if (role == GFS_025_IDX) {
        QString adv = match.captured(3);
        int fcst = 0;
        QByteArray b1 = adv.toLatin1();
        if (sscanf(b1.constData(), "%03d", &fcst) == 1) {
          m_fcst = fcst;
          role_str = GFS_025;
          ret = true;
        }
      }
      else {
        m_hour = m_day = m_mon = m_year = m_fcst = 0;
        my_log("err #%s\n", "0xfbd85ad8");
      }
    }
  }
  return ret;
}

QString datefc_c::get_img_name(const string& role) const
{
  QString ret;
  char buf[LARGEBUF] = { 0 };
  if (role == GFS_025)
    sprintf(buf, GFS_025_FNAME_FORMAT, m_year, m_mon, m_day, m_hour, m_hour, m_fcst);
  else {
    my_log("err #%s\n", "0xa29f689f");
  }
  return ret = buf;
}

int datefc_c::hour_range(const datefc_c &stop, vector<datefc_c> *ret, bool incl, int hour_step) const
{
  datefc_c t0, t1;
  int s;
  if (date_only_cmp(*this, stop) > 0) {
    t0 = stop;
    t1 = *this;
    s = -1;
  }
  else {
    t1 = stop;
    t0 = *this;
    s = 1;
  }
  int i = 0, j = 0, k = 0, hh = 0, num = 0;
  for (i = t0.m_year; i <= t1.m_year; i++) {
    for (j = (i == t0.m_year ? t0.m_mon : 1); j <= (i == t1.m_year ? t1.m_mon : 12); j++) {
      for (k = (j == t0.m_mon ? t0.m_day : 1); k <= (j == t1.m_mon ? t1.m_day : day_in_mon(i, j)); k++) {
        for (hh = (k == t0.m_day ? t0.m_hour : 0);
          incl ? (hh <= (k == t1.m_day ? t1.m_hour : 23))
          : (hh < (k == t1.m_day ? t1.m_hour : 23)); hh += hour_step) {
          if (ret) {
            datefc_c tmp(i, j, k, hh, 0);
            ret->push_back(tmp);
          }
          num++;
        }
      }
    }
  }
  return s * num;
}

int datefc_c::day_of_week(int y, int m, int d)
{
  static int t[12] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
  y -= m < 3;
  return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

int datefc_c::day_in_mon(int year, int mon)
{
  static int dmonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  if (mon == 2) {
    if (!(year % 4)) {
      if (!(year % 100)) {
        if (!(year % 400))
          dmonth[1]++;
      }
      else
        dmonth[1]++;
    }
  }
  return dmonth[mon - 1];
}
