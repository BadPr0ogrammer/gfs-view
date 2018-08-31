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

#include "gribitem.h"
#include "gribini.h"
#include "tagtreeui.h"

#define CMD_OPEN            0
#define CMD_NEW_BY_SLOT     1
#define CMD_WIZARD          2
#define CMD_ZOOM_IN         3
#define CMD_ZOOM_OUT        4
#define CMD_ZOOM_FIT        5
#define CMD_GO_FIRST        6
#define CMD_GO_PREV         7
#define CMD_GO_NEXT         8
#define CMD_GO_LAST         9
#define CMD_GO_PLAY         10
#define CMD_COMPOSE_WIND    11
#define CMD_COMPOSE_TEMPRE  12
#define CMD_COMPOSE_GEOPT   13
#define CMD_COMPOSE_PRECIP  14
#define CMD_GEOGRID         15
#define CMD_SETTINGS        16
#define CMD_INFORMATION     17
#define CMD_CLOSE_ALL       18
#define CMD_NUM             (CMD_CLOSE_ALL + 1)

class win_c;
class sets_c;
class info_data_c;
class grib_c;
class graphics_view_c;
class grib_item_c;
class grib_data_c;
class grib_lval_cmp_c;
class dlg_startup_c;
class dlg_datetime_c;
class reg_ui_c;
class dbl_date_ui_c;
class fcst_ui_c;
class tag_tree_ui_c;

class QNetworkAccessManager;
class QQmlApplicationEngine;
class QQuickWindow;

class win_c : public QMainWindow
{
  Q_OBJECT
public:
  sets_c                    *m_sets;
  info_data_c               *m_data;
  QQmlApplicationEngine     *m_engine;
  QQuickWindow              *m_map_win;
  QNetworkAccessManager     *m_netman;
  QTimer                    *m_timer;
  QGraphicsItem             *m_prev_pix;
  QGraphicsItem             *m_prev_pal;
  QPixmap                   *m_pal_pxm;
  dlg_startup_c             *m_dlg_startup;
  dlg_datetime_c            *m_dlg_datetime;
  reg_ui_c                  *m_reg_ui;
  dbl_date_ui_c             *m_dbl_date_ui;
  fcst_ui_c                 *m_fcst_ui;
  tag_tree_ui_c             *m_tag_tree_ui;
  QCheckBox                 *m_dwnl_all;
  tag_tree_ui_c             m_tag_tree;
  bool                      m_start;
  bool                      m_term_ok;
  string                    m_timestamp;
  QString                   m_persfold;
  QGraphicsItemGroup        *m_cntrs_group;
  QGraphicsItemGroup        *m_geogr_group;
  QToolButton               *m_cmd[CMD_NUM];
  QGraphicsScene            *m_canvas;
  graphics_view_c           *m_canview;
  QSplitter                 *m_splitter;
  QSlider                   *m_slider;
  QLabel                    *m_slide_lb;
  QLabel                    *m_tag_lb;
  QLabel                    *m_level_lb;
  QLabel                    *m_codes_lb;
  QLabel                    *m_units_lb;
  QLabel                    *m_latlon;
  QLabel                    *m_reg_cp;
  QLabel                    *m_mes_lb;
#ifdef _WINDOWS
  wchar_t                   m_log_fname[LARGEBUF];
#else
  char                      m_log_fname[LARGEBUF];
#endif

  ~win_c();
  win_c(QQmlApplicationEngine *engine);

  bool              set_grib_table();
  void              init_toolbar();
  void              set_slider_lb();
  void              set_cur(bool block, bool expand);
  bool              update_canv(const tag_grib_c& next);
  void              set_msg(const char *msg, bool red, bool clear);
  void              set_labels();
  void              fit_desktop();
  void              grib_done(bool pal_rebuild);
  void              move_ramppal();
  void              stop_play();
  void              add_menu();
  void              clear_w();
  void              clear_pixmaps();
  void              init_proxy();
  static void       wait_cursor(bool set);
  int               message_box(QMessageBox::Icon icon, const char *msg, QMessageBox::StandardButtons flags);
  string            timestamp();
  void              init_log();

private slots:
  void  slider_changed(int);
  void  settings_clicked();
  void  open_clicked();
  void  date_time_clicked();
  void  mes_lb_clear();
  void  scroll_changed(int);
  void  play_first_clicked();
  void  play_last_clicked();
  void  play_prev_clicked();
  void  play_next_clicked();
  void  information();
  void  draw_wind_clicked();
  void  draw_temp_clicked();
  void  draw_geopt_clicked();
  void  draw_precip_clicked();
  void  play_clicked();
  void  wizard_finish();
  void  zoom_in_clicked();
  void  zoom_out_clicked();
  void  zoom_fit_clicked();

public slots:
  void close_all_clicked();
  void wizard_clicked();
  void draw_geogr_clicked();
};
