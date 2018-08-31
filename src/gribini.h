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

#include "datefcst.h"
#include "gribitem.h"

class win_c;
class dbl_date_ui_c;
class fcst_ui_c;
class tag_tree_ui_c;

typedef map<string, map<string, map<string, map<string, vector<string>>>>> data_tmpl_t;
typedef             map<string, map<string, map<string, vector<string>>>>  slot_tmpl_t;

class grib_ini_c : public QObject
{
  Q_OBJECT

    pair<string, string>  m_two_str;
  QEventLoop            *m_loop;
  QNetworkRequest       m_request;
  int                   m_counter;
  int                   m_dwnl_szsum;
  QNetworkReply         *m_reply;
  QProgressDialog       *m_prog_dlg;
  win_c                 *m_win;

public:
  grib_ini_c(win_c *win) : m_loop(nullptr), m_counter(0), m_dwnl_szsum(0), m_reply(nullptr), m_prog_dlg(nullptr), m_win(win) {}
  grib_ini_c(win_c *win, dbl_date_ui_c *dbl_date_ui, fcst_ui_c *fcst_ui, tag_tree_ui_c *tag_tree, data_tmpl_t& tmpl);

  ~grib_ini_c() { delete m_prog_dlg; }

  bool open_data(const data_tmpl_t& tmpl);
  vector<pair<string, string>> make_url_ncep(double lat, double lon, double h, double w, bool dwnl_all, const data_tmpl_t& tmpl);
  void download(double lat, double lon, double h, double w, bool dwnl_all, const data_tmpl_t& tmpl);
  void show_prog_dlg(QWidget *win, int tmpl_size);

  void                          prog_dlg_delete() { delete m_prog_dlg; m_prog_dlg = nullptr; }
  int                           dwnl_szsum() { return m_dwnl_szsum; }
  QProgressDialog*              prog_dlg_ptr() { return m_prog_dlg; }

private slots:
  void reply_progress(qint64 read, qint64 total);
  void reply_finished();
};
