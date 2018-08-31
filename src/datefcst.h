/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

#include <QtCore>
#include <QRegularExpression> 

#include <opencv2/core/core.hpp>

#include "defs.h"

using namespace std;
using namespace cv;

class datefc_c
{
public:
  int       m_year;
  int       m_mon;
  int       m_day;
  int       m_hour;
  int       m_fcst;

  static const QRegularExpression m_regex_gfs;

  datefc_c(int year = 0, int mon = 0, int day = 0, int hour = 0, int fcst = 0);
  datefc_c(const datefc_c &date);
  datefc_c(const datefc_c &date, int fcst);
  datefc_c(const char *date, int fcst);
  datefc_c(const char *date);

  bool            parse(const char *date);
  string          run_str() const;
  string          runfc_str() const;
  string          fc_str() const { return to_string(m_fcst); }
  int             scanf_atoi(const char *buf, int sz);
  bool            parse_img_name(const char *name, string& role);
  QString         get_img_name(const string& role) const;

  static int  day_in_mon(int year, int mon);
  static int  day_of_week(int y, int m, int d);
  int         hour_range(const datefc_c &stop, vector<datefc_c> *ret, bool incl, int hour_step) const;
  static int  date_only_cmp(const datefc_c &p1, const datefc_c &p2);
  static int  datefc_cmp(const datefc_c &p1, const datefc_c &p2);
};
