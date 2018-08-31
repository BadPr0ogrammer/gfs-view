/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <opencv2/core/core.hpp>
#include <set>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtNetwork>

#include <shapefil.h>

#include "defs.h"
#include "color_tables.h"
#include "datefcst.h"
#include "shaperead.h"
#include "griblatlon.h"
#include "cntrs.h"
#include "infodata.h"
#include "winc.h"
#include "mgrb2.h"
#include "sets.h"
#include "gribini.h"
#include "local_str.h"
#include "staticstr.h"
#include "dbldateui.h"
#include "fcstui.h"
#include "tagtreeui.h"

#include "grib_strings.h"

bool grib_ini_c::open_data(const data_tmpl_t& tmpl)
{
  bool ret = true, first = true;
  info_data_c *data = m_win->m_data;
  tag_grib_c cur_grib;

  set<string> id_set;
  if (m_prog_dlg)
    m_prog_dlg->setMaximum(tmpl.size());

  for (auto it = tmpl.begin(); it != tmpl.end(); it++) {
    const string& slot = (*it).first;
    datefc_c datefc(slot.c_str());

    for (auto it1 = (*it).second.begin(); it1 != (*it).second.end(); it1++) {
      const string& role = (*it1).first;
      rtlv_rf_c params(it1, datefc);

      QString name = datefc.get_img_name(role);
      QString fname = m_win->m_persfold + DIR_SEPARATOR_S MY_NWP_IMPORT DIR_SEPARATOR_S + name;
      QFileInfo finfo(fname);

      if (finfo.exists()) {
        string id = datefc.runfc_str() + role;
        if (id_set.find(id) != id_set.end())
          continue;
        id_set.insert(id);

        if (role == GFS_025) {
          grib_c *grib = m_win->m_data->add_slot(datefc, nullptr);
          if (!grib) {
            my_log("err #%s\n", "0x960da87a");
            return false;
          }
          rtlv_rf_c par;
          if (!grib->read_grib(fname, data->files_ptr(), role, par)) {
            my_log("err #%s\n", "0x27e9739b");
            delete grib;
            ret = false;
            continue;
          }
          if (first) {
            grib_data_c *pdata = grib->grib_data_ptr(params);
            par.m_run = datefc.run_str();
            par.m_fc = datefc.fc_str();
            cur_grib.m_rtlv = !pdata ? par : params;
            cur_grib.m_gr = grib;
          }
        }
        if (first) {
          first = false;
          grib_c *grib = cur_grib.m_gr;
          grib_data_c *pdata = grib->grib_data_ptr(cur_grib.m_rtlv);
          if (!grib || !pdata) {
            my_log("err #%s\n", "0x9e9111d0");
            return false;
          }
          bool bb;
          if (!pdata->make_grib(bb)) {
            my_log("err #%s\n", "0x9abab097");
            ret = false;
            continue;
          }
          data->set_cur_grib(cur_grib);
        }
      }
    }
    if (m_prog_dlg) {
      if (m_prog_dlg->wasCanceled())
        return false;
      static int prog;
      m_prog_dlg->setLabelText(slot.c_str());
      m_prog_dlg->setValue(++prog);
      QApplication::processEvents();
    }
  }
  return ret && !first;
}

#define GFS_025_URL_BASE        "http://nomads.ncep.noaa.gov/cgi-bin/filter_gfs_0p25.pl?"
#define GFS_025_URL_PREF        "gfs."
#define GFS_025_URL_SUFF0       "gfs.t"
#define GFS_025_URL_SUFF1       "z.pgrb2.0p25.f"
#define GFS_025_URL_SUFF2       ""

vector<pair<string, string>> grib_ini_c::make_url_ncep(double lat, double lon, double h, double w, bool dwnl_all, const data_tmpl_t& tmpl)
{
  string reg_str;
  if (lon < 0)
    lon = 360 + lon;
  {
    char buf0[16], buf1[16], buf2[16], buf3[16];
    sprintf(buf0, "%.3f", lon - w / 2);
    sprintf(buf1, "%.3f", lon + w / 2);
    sprintf(buf2, "%.3f", lat + h / 2);
    sprintf(buf3, "%.3f", lat - h / 2);
    reg_str = string("subregion=&leftlon=") + buf0 + string("&rightlon=") + buf1 + string("&toplat=") + buf2 + string("&bottomlat=") + buf3 + string("&");
  }

  vector<pair<string, string>> ret;
  auto it3 = tmpl.begin();
  for (auto it2 = (*it3).second.begin(); it2 != (*it3).second.end(); it2++) {
    const string& role = (*it2).first;

    string pref, suff0, suff1, suff2, url_base, fc0, fc1;
    if (role == string(GFS_025)) {
      url_base = GFS_025_URL_BASE;
      pref = GFS_025_URL_PREF;
      suff2 = GFS_025_URL_SUFF2;
      fc0 = "00";
      suff0 = GFS_025_URL_SUFF0;
      suff1 = GFS_025_URL_SUFF1;
      fc1 = "0";
    }
    string code_str, level_str;
    if (dwnl_all) {
      code_str = "all_var=on&";
      level_str = "all_lev=on&";
    }
    else {
      set<string> level_set;
      set<string> abbr_set;
      set<pair<string, string>> height_set;

      for (auto it0 = (*it2).second.begin(); it0 != (*it2).second.end(); it0++) {
        const string& tag = (*it0).first;
        abbr_set.insert(tag);

        for (auto it1 = (*it0).second.begin(); it1 != (*it0).second.end(); it1++) {
          const string& level = (*it1).first;
          if (level == "SFC" || level == "MSL" || level == "EATM" || level == "TRO" || level == "CCY" || level == "PBLRI")
            level_set.insert(level);
          else if (level == "ISBL" || level == "GPML" || level == "HTGL") {
            for (int k = 0; k < (*it1).second.size(); k++) {
              const string& height = (*it1).second[k];
              height_set.insert(make_pair(level, height));
            }
          }
          else {
            my_log("err #%s\n", "0x82d1835f");
          }
        }
      }
      for (auto it = level_set.begin(); it != level_set.end(); it++) {
        const string& level = *it;
        if (level == "SFC")
          level_str += "lev_surface=on&";
        else if (level == "MSL")
          level_str += "lev_mean_sea_level=on&";
        else if (level == "EATM")
          level_str += "lev_entire_atmosphere_%5C%28considered_as_a_single_layer%5C%29=on&";
        else if (level == "TRO")
          level_str += "lev_tropopause=on&";
        else if (level == "CCY")
          level_str += "lev_convective_cloud_layer=on&";
        else if (level == "PBLRI")
          level_str += "lev_planetary_boundary_layer=on&";
      }
      for (auto it = height_set.begin(); it != height_set.end(); it++) {
        const string& level = (*it).first;
        const string& height = (*it).second;
        if (level == "ISBL") {
          level_str += "lev_";
          level_str += height;
          level_str += "_mb=on&";
        }
        else if (level == "GPML") {
          level_str += "lev_";
          level_str += height;
          level_str += "_m_above_ground=on&";
        }
        else if (level == "HTGL") {
          level_str += "lev_";
          level_str += height;
          level_str += "-0_m_above_ground=on&"; //?????? 3000-0m
          break;
        }
        else {
          level_str += "&";
          my_log("err #%s\n", "0x110d3b6d");
        }
      }
      for (auto it = abbr_set.begin(); it != abbr_set.end(); it++) {
        const string& abbr = *it;
        code_str += "var_";
        code_str += abbr;
        code_str += "=on&";

        if (VELOCITY_CODE(abbr)) {
          if (abbr == "UGRD")
            code_str += "var_VGRD=on&";
          else if (abbr == "UFLX")
            code_str += "var_VFLX=on&";
          else if (abbr == "USTM")
            code_str += "var_VSTM=on&";
        }
      }
    }

    for (auto it = tmpl.begin(); it != tmpl.end(); it++) {
      const string& slot = (*it).first;
      datefc_c datefc(slot.c_str());

      string file_str(suff0);
      if (role == string(GFS_025)) {
        if (datefc.m_hour < 10)
          file_str += "0" + to_string(datefc.m_hour);
        else
          file_str += to_string(datefc.m_hour);
      }
      file_str += suff1;

      if (role == string(GFS_025)) {
        if (datefc.m_fcst < 10)
          file_str += fc0 + to_string(datefc.m_fcst);
        else if (datefc.m_fcst < 100)
          file_str += fc1 + to_string(datefc.m_fcst);
        else
          file_str += to_string(datefc.m_fcst);
      }
      file_str += suff2;

      string tmp(url_base);
      tmp += "file=" + file_str + "&";

      tmp += level_str;
      tmp += code_str;

      tmp += reg_str;

      tmp += "dir=%2F";
      tmp += pref;
      if (role == string(GFS_025)) {
        QByteArray b = datefc.run_str().c_str();
        tmp += b.constData();
      }

      string s = datefc.get_img_name(role).toLatin1().data();
      ret.push_back(make_pair(tmp, s));
    }
  }
  return ret;
}

void grib_ini_c::download(double lat, double lon, double h, double w, bool dwnl_all, const data_tmpl_t& tmpl)
{
  {
    QString s = m_win->m_persfold + DIR_SEPARATOR_S MY_NWP_IMPORT;
    QDir dir(s);
    if (!dir.exists()) {
      dir.mkpath(s);
      my_log("err #%s\n", "0x473577cf");
    }
  }
  vector<pair<string, string>> url_name = make_url_ncep(lat, lon, h, w, dwnl_all, tmpl);
  if (!url_name.size()) {
    m_win->set_msg(IDSTR_OP_FAILED, true, false);
    my_log("err #%s\n", "0xd2b5474c");
    return;
  }
  m_dwnl_szsum = 0;
  if (m_prog_dlg)
    m_prog_dlg->setMaximum(url_name.size());

  for (int i = 0; i < url_name.size(); i++) {
    m_two_str = url_name[i];
    if (m_prog_dlg) {
      if (m_prog_dlg->wasCanceled())
        break;
      m_prog_dlg->setLabelText(m_two_str.second.c_str());
      {
        static int prog;
        m_prog_dlg->setValue(++prog);
      }
      QApplication::processEvents();
    }
    m_request = *new QNetworkRequest(QUrl(m_two_str.first.c_str()));
    m_reply = m_win->m_netman->get(m_request);
    m_reply->setReadBufferSize(0);
    connect(m_reply, SIGNAL(finished()), this, SLOT(reply_finished()));
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(reply_progress(qint64, qint64)));

    m_loop = new QEventLoop(m_win);
    m_loop->exec();

    delete m_loop;
    m_loop = nullptr;
    m_counter = 0;
  }

  if (m_prog_dlg) {
    if (m_dwnl_szsum / 1024 > 0)
      m_prog_dlg->setLabelText(QString::number(m_dwnl_szsum / 1024) + " kB for " + QString::number(url_name.size()) + " file(s).");
    else
      m_prog_dlg->setLabelText(IDSTR_NO_DATA_B);
    QApplication::processEvents();
  }
}

void grib_ini_c::reply_progress(qint64 read, qint64 total)
{
  if (m_prog_dlg) {
    if (m_prog_dlg->wasCanceled()) {
      m_loop->quit();      
      return;
    }
    m_prog_dlg->setLabelText(QString(m_two_str.second.c_str()) + ": " + QString::number((read +1023) / 1024) + " kB...");
    QApplication::processEvents();
  }
}

void grib_ini_c::reply_finished()
{
  int ret = FALSE;
  if (m_reply->isFinished()) {
    if (m_prog_dlg->wasCanceled()) {
      m_reply->deleteLater();
      m_reply = nullptr;
      return;
    }
    QByteArray data = m_reply->readAll();
    int sz = data.size();
    if (sz > 0 && !m_two_str.second.empty()) {
      if (m_reply->error() == QNetworkReply::NoError) {
        QString fname = m_win->m_persfold + DIR_SEPARATOR_S MY_NWP_IMPORT DIR_SEPARATOR_S + m_two_str.second.c_str();
        QFileInfo finfo(fname);
        if (!finfo.absoluteDir().exists() && !QDir().mkpath(finfo.absolutePath())) {
          my_log("err #%s\n", "0xc244086b");
          return;
        }
        QFile file(fname);
        if (file.open(QIODevice::WriteOnly)) {
          file.write(data);
          file.close();
          m_dwnl_szsum += sz;
          if (m_prog_dlg) {
            m_prog_dlg->setLabelText(QString(m_two_str.second.c_str()) + ": " + QString::number(sz / 1024) + " kB received!");
            QApplication::processEvents();
          }
          ret = TRUE;
        }
      }
      else {
        my_log("err #%s\n", "0x8e887316");
        my_log("%s", data.data());
      }
    }
    else {
      QThread::sleep(5);
      my_log("err #%s\n", "0x606a21ea");
    }
  }
  else {
    my_log("err #%s\n", "0x2b773442");
  }
  if (!ret)
    m_win->set_msg(IDSTR_OP_FAILED, true, false);
  QThread::sleep(5);

  m_loop->quit();
  m_reply->deleteLater();
  m_reply = nullptr;
}

void grib_ini_c::show_prog_dlg(QWidget *win, int tmpl_size)
{
  delete m_prog_dlg;
  m_prog_dlg = new QProgressDialog(win);
  m_prog_dlg->setWindowModality(Qt::WindowModal);
  m_prog_dlg->setWindowTitle(IDSTR_LOADING_P);
  m_prog_dlg->setLabelText(IDSTR_LOADING_GRIB_DATA_P);
  m_prog_dlg->setMinimum(0);
  m_prog_dlg->setMinimumDuration(0);
  m_prog_dlg->setAutoClose(false);

  m_prog_dlg->show();
  m_prog_dlg->raise();
  m_prog_dlg->activateWindow();
  QApplication::processEvents();
}

grib_ini_c::grib_ini_c(win_c *win, dbl_date_ui_c *dbl_date_ui, fcst_ui_c *fcst_ui, tag_tree_ui_c *tag_tree, data_tmpl_t& tmpl)
  : grib_ini_c(win)
{
  QDate	date0 = dbl_date_ui->m_datetime0->date();
  QDate	date1 = dbl_date_ui->m_datetime1->date();
  int	time0 = 6 * MAX(0, fcst_ui->m_time0->currentIndex());
  int	time1 = 6 * MAX(0, fcst_ui->m_time1->currentIndex());
  int fcst0 = fcst_ui->m_fcst0 ? fcst_ui->m_fcst0->value() : 0;
  int fcst1 = fcst_ui->m_fcst0 ? fcst_ui->m_fcst1->value() : 0;
  if (fcst0 > fcst1) {
    int f;
    CV_SWAP(fcst0, fcst1, f);
  }
  int step = fcst_ui->m_step ? fcst_ui->m_step->value() : 1;

  datefc_c start(date0.year(), date0.month(), date0.day(), time0, 0);
  datefc_c stop(date1.year(), date1.month(), date1.day(), time1, 0);

  slot_tmpl_t stmpl;
  if (!tag_tree->tree_map(stmpl)) {
    my_log("err #%s\n", "0x46b75cec");
    return;
  }

  vector<datefc_c> vdatefc;
  if (start.hour_range(stop, &vdatefc, true, 6) < 0 || !vdatefc.size()) {
    my_log("err #%s\n", "0x858813d7");
    return;
  }

  for (int i = 0; i < vdatefc.size(); i++) {
    datefc_c datefc = vdatefc[i];
    for (int fc = fcst0; fc <= fcst1; fc += step) {
      datefc.m_fcst = fc;
      string slot = datefc.runfc_str();
      tmpl[slot] = stmpl;
    }
  }
}
