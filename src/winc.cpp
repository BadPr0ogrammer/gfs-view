/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <time.h>
#ifdef _WINDOWS
#include <shobjidl.h>
#endif

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtNetwork>
#include <QQmlApplicationEngine> 
#include <QQuickWindow>
#include <QQmlComponent>
#include <QtLocation>

#include "defs.h"
#include "local_str.h"
#include "staticstr.h"
#include "color_tables.h"
#include "datefcst.h"
#include "shaperead.h"
#include "griblatlon.h"
#include "cntrs.h"
#include "infocntrs.h"
#include "winc.h"
#include "mgrb2.h"
#include "gribitem.h"
#include "graphicsvw.h"
#include "tvdelegate.h"
#include "sets.h"
#include "gribini.h"
#include "ramppal.h"
#include "infodata.h"
#include "dlgstartup.h"
#include "dlgiface.h"
#include "dlgdatetime.h"

#include "map_qml.h"

#include "table_lookup.h"
#include "grib_strings.h"

const char *my_roles[LAST_ROLE + 1] = {
  GFS_025,
};

win_c::~win_c()
{
  my_log("OK\n");
  clear_pixmaps();

  delete m_engine;
  delete m_sets;
  delete m_data;
  delete m_netman;
  delete m_dlg_startup;
  delete m_dlg_datetime;
}

bool win_c::set_grib_table()
{
  GribTable = new TableFlatFile();
  if (!GribTable) {
    my_log("Memory error [%s]\n", "0xe1fe0a4f");
    return false;
  }
  GribTable->N_grib1_elements = array_countof(grib1_ncep_2_7);
  GribTable->N_grib2_elements = array_countof(grib2_all);
  GribTable->g1e = new Grib1TableEntry *[GribTable->N_grib1_elements];
  if (!GribTable->g1e) {
    my_log("Memory error [%s]\n", "0x3d2b194f");
    return false;
  }
  for (int i = 0; i < GribTable->N_grib1_elements; i++) {
    GribTable->g1e[i] = new Grib1TableEntry;
    if (!GribTable->g1e[i]) {
      my_log("err #%s\n", "0xd27fa002");
      return false;
    }
    GribTable->g1e[i]->code = grib1_ncep_2_7[i].code;
    GribTable->g1e[i]->table_number = grib1_ncep_2_7[i].table_number;
    GribTable->g1e[i]->center = grib1_ncep_2_7[i].center;
    GribTable->g1e[i]->parm_name = grib1_ncep_2_7[i].parm_name;
    GribTable->g1e[i]->full_name = grib1_ncep_2_7[i].full_name;
    GribTable->g1e[i]->units = grib1_ncep_2_7[i].units;
  }
  GribTable->g2e = new Grib2TableEntry *[GribTable->N_grib2_elements];
  if (!GribTable->g2e) {
    my_log("err #%s\n", "0x77f00eb7");
    return false;
  }

  for (int i = 0; i < 1018; i++) {
    grib2_all[i].my_id = i;
  }
  for (int i = 0; i < GribTable->N_grib2_elements; i++) {
    GribTable->g2e[i] = new Grib2TableEntry;
    if (!GribTable->g2e[i]) {
      my_log("err #%s\n", "0xabb5ec84");
      return false;
    }
    GribTable->g2e[i]->index_a = grib2_all[i].index_a;
    GribTable->g2e[i]->mtab_set = grib2_all[i].mtab_set;
    GribTable->g2e[i]->mtab_low = grib2_all[i].mtab_low;
    GribTable->g2e[i]->mtab_high = grib2_all[i].mtab_high;
    GribTable->g2e[i]->cntr = grib2_all[i].cntr;
    GribTable->g2e[i]->ltab = grib2_all[i].ltab;
    GribTable->g2e[i]->index_b = grib2_all[i].index_b;
    GribTable->g2e[i]->index_c = grib2_all[i].index_c;
    GribTable->g2e[i]->parm_name = grib2_all[i].parm_name;
    GribTable->g2e[i]->full_name = grib2_all[i].full_name;
    GribTable->g2e[i]->units = grib2_all[i].units;
  }
  return true;
}

win_c::win_c(QQmlApplicationEngine *engine) :
  m_sets(nullptr), m_data(nullptr), m_engine(engine), m_map_win(nullptr), m_netman(nullptr), m_timer(nullptr), m_prev_pix(nullptr),
  m_prev_pal(nullptr), m_pal_pxm(nullptr), m_dlg_startup(nullptr), m_dlg_datetime(nullptr), m_reg_ui(nullptr), m_dbl_date_ui(nullptr),
  m_fcst_ui(nullptr), m_tag_tree_ui(nullptr), m_dwnl_all(nullptr), m_tag_tree(this, true), m_canview(nullptr)
{
  QString s = QStandardPaths::writableLocation(
#ifdef _WINDOWS
        QStandardPaths::DocumentsLocation
#else
        QStandardPaths::HomeLocation
#endif
        );
  if (s.isEmpty()) {
    QMessageBox msg_box;
    msg_box.setWindowIcon(QPixmap(":/icons/gfs_view16.png"));
    msg_box.setIcon(QMessageBox::Critical);
    msg_box.setTextFormat(Qt::RichText);
    msg_box.setText("<span style='font-size:14pt; font-weight:600; color:#990000;'>Fatal error</span><br>");
    msg_box.setInformativeText("Unable to get Personal Folder location (something like C:/Users/<name>/Documents)");
    msg_box.exec();
    return;
  }
  m_persfold = s + DIR_SEPARATOR_S PROGRAM_NAME;
  m_timestamp = timestamp();
  init_log();

  if (!set_grib_table())
    return;

  m_sets = new sets_c(this);
  m_data = new info_data_c(this);

  m_netman = new QNetworkAccessManager(this);

  setWindowTitle(PROGRAM_NAME);
  setWindowIcon(QPixmap(":/icons/gfs_view32.png"));

  m_canvas = new QGraphicsScene;
  m_canview = new graphics_view_c(m_canvas, this);
  m_canview->setMouseTracking(true);
  m_canview->setRenderHint(QPainter::Antialiasing);
  connect(m_canview->horizontalScrollBar(), &QScrollBar::valueChanged, this, &win_c::scroll_changed);
  connect(m_canview->verticalScrollBar(), &QScrollBar::valueChanged, this, &win_c::scroll_changed);

  m_cntrs_group = m_canvas->createItemGroup(QList<QGraphicsItem *>());
  m_cntrs_group->setZValue(2);
  m_geogr_group = m_canvas->createItemGroup(QList<QGraphicsItem *>());
  m_geogr_group->setZValue(1);

  m_slider = new QSlider(Qt::Horizontal);
  m_slider->setTickPosition(QSlider::TicksAbove);
  m_slider->setMinimum(0);
  m_slider->setMaximum(0);

  m_slide_lb = new QLabel;
  m_slide_lb->setFrameStyle(QFrame::Box);
  m_splitter = new QSplitter;
  m_splitter->setChildrenCollapsible(false);

  QFrame *vbox = new QFrame;
  QVBoxLayout *vlay = new QVBoxLayout;
  vbox->setLayout(vlay);
  vlay->setContentsMargins(0, 0, 6, 0);
  QFrame *hbox = new QFrame;
  QHBoxLayout *hlay = new QHBoxLayout;
  hbox->setLayout(hlay);

  init_toolbar();

  m_splitter->addWidget(m_tag_tree.m_view);
  m_splitter->addWidget(vbox);

  hlay->addWidget(m_slider);
  hlay->addWidget(m_slide_lb);
  vlay->addWidget(hbox);
  vlay->addWidget(m_canview);

  setCentralWidget(m_splitter);

  statusBar()->addPermanentWidget(m_tag_lb = new QLabel(this), 130);
  m_tag_lb->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  statusBar()->addPermanentWidget(m_level_lb = new QLabel(this), 65);
  m_level_lb->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  statusBar()->addPermanentWidget(m_codes_lb = new QLabel(this), 75);
  m_codes_lb->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  statusBar()->addPermanentWidget(m_units_lb = new QLabel(this), 55);
  m_units_lb->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  statusBar()->addPermanentWidget(m_latlon = new QLabel(this), 75);
  m_latlon->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  statusBar()->addPermanentWidget(m_reg_cp = new QLabel(this), 75);
  m_reg_cp->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  statusBar()->addPermanentWidget(m_mes_lb = new QLabel(this), 300);
  m_mes_lb->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(slider_changed(int)));

  m_timer = new QTimer(this);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(play_next_clicked()));

  add_menu();
}

void win_c::add_menu()
{
  QMenu *menu = menuBar()->addMenu(IDSTR_MENU_FILE);

  QAction *action = menu->addAction(QPixmap(":/icons/open.png"), IDSTR_OPEN);
  connect(action, SIGNAL(triggered()), this, SLOT(open_clicked()));

  action = menu->addAction(QPixmap(":/icons/load_by_date.png"), IDSTR_LOAD_BY_DATE);
  connect(action, SIGNAL(triggered()), this, SLOT(date_time_clicked()));

  action = menu->addAction(QPixmap(":/icons/load_wizard.png"), IDSTR_DATA_LOADING_WIZARD);
  connect(action, SIGNAL(triggered()), this, SLOT(wizard_clicked()));

  action = menu->addAction(QPixmap(":/icons/sc_deleteallannotation_g.png"), IDSTR_CLOSE_ALL);
  connect(action, SIGNAL(triggered()), this, SLOT(close_all_clicked()));

  menu = menuBar()->addMenu(IDSTR_ZOOM);

  action = menu->addAction(QPixmap(":/icons/zoom_in.png"), IDSTR_ZOOM_IN);
  connect(action, SIGNAL(triggered()), this, SLOT(zoom_in_clicked()));

  action = menu->addAction(QPixmap(":/icons/zoom_out.png"), IDSTR_ZOOM_OUT);
  connect(action, SIGNAL(triggered()), this, SLOT(zoom_out_clicked()));

  action = menu->addAction(QPixmap(":/icons/zoom_fit.png"), IDSTR_ZOOM_TO_FIT);
  connect(action, SIGNAL(triggered()), this, SLOT(zoom_fit_clicked()));

  menu = menuBar()->addMenu(IDSTR_TIME_SLOT);

  action = menu->addAction(QPixmap(":/icons/all_left.png"), IDSTR_GO_FIRST);
  connect(action, SIGNAL(triggered()), this, SLOT(play_first_clicked()));

  action = menu->addAction(QPixmap(":/icons/one_left.png"), IDSTR_GO_PREV);
  connect(action, SIGNAL(triggered()), this, SLOT(play_prev_clicked()));

  action = menu->addAction(QPixmap(":/icons/one_right.png"), IDSTR_GO_NEXT);
  connect(action, SIGNAL(triggered()), this, SLOT(play_next_clicked()));

  action = menu->addAction(QPixmap(":/icons/all_right.png"), IDSTR_GO_LAST);
  connect(action, SIGNAL(triggered()), this, SLOT(play_last_clicked()));

  action = menu->addAction(QPixmap(":/icons/arrow.png"), IDSTR_PLAY);
  connect(action, SIGNAL(triggered()), this, SLOT(play_clicked()));

  menu = menuBar()->addMenu(IDSTR_COMPOSE);

  action = menu->addAction(QPixmap(":/icons/windy.png"), IDSTR_UV_WIND);
  action->setCheckable(true);
  action->setChecked(m_sets->m_wind_compose ? Qt::Checked : Qt::Unchecked);
  connect(action, SIGNAL(changed()), this, SLOT(draw_wind_clicked()));

  action = menu->addAction(QPixmap(":/icons/temperature_warm.png"), IDSTR_TEMPERATURE);
  action->setCheckable(true);
  action->setChecked(m_sets->m_tempre_compose ? Qt::Checked : Qt::Unchecked);
  connect(action, SIGNAL(changed()), this, SLOT(draw_temp_clicked()));

  action = menu->addAction(QPixmap(":/icons/speedometer.png"), IDSTR_PRESSURE);
  action->setCheckable(true);
  action->setChecked(m_sets->m_geopt_compose ? Qt::Checked : Qt::Unchecked);
  connect(action, SIGNAL(changed()), this, SLOT(draw_geopt_clicked()));

  action = menu->addAction(QPixmap(":/icons/weather_rain.png"), IDSTR_PRECIPITATION);
  action->setCheckable(true);
  action->setChecked(m_sets->m_precip_compose ? Qt::Checked : Qt::Unchecked);
  connect(action, SIGNAL(changed()), this, SLOT(draw_precip_clicked()));

  action = menu->addAction(QPixmap(":/icons/lc_helplinesvisible.png"), IDSTR_GEOGRAPHIC_GRID);
  action->setCheckable(true);
  action->setChecked(m_sets->m_geogrid ? Qt::Checked : Qt::Unchecked);
  connect(action, SIGNAL(changed()), this, SLOT(draw_geogr_clicked()));

  menu = menuBar()->addMenu(IDSTR_OPTIONS);

  action = menu->addAction(QPixmap(":/icons/wheel.png"), IDSTR_SETTINGS);
  connect(action, SIGNAL(triggered()), this, SLOT(settings_clicked()));

  action = menu->addAction(QPixmap(":/icons/info_16.png"), IDSTR_ABOUT);
  connect(action, SIGNAL(triggered()), this, SLOT(information()));
}

void win_c::move_ramppal()
{
  if (!m_data->gdata_empty()) {
    double s = 1 / m_canview->transform().m11();
    if (m_prev_pal) {
      m_prev_pal->setScale(s);
      if (m_prev_pix && m_pal_pxm) {
        QPointF br = m_prev_pix->boundingRect().bottomRight();
        QPoint pt = { int(br.x() - s * m_pal_pxm->width()), int(br.y() - s * m_pal_pxm->height()) };
        m_prev_pal->setPos(pt);
      }
      else {
        my_log("err #%s\n", "0xf2200997");
      }
    }
  }
}

void win_c::grib_done(bool pal_rebuild)
{
  if (m_prev_pix) {
    m_canvas->removeItem(m_prev_pix);
    delete m_prev_pix;
    m_prev_pix = nullptr;
  }
  grib_data_c *grib_data = m_data->cur_gdata_ptr();
  if (!grib_data || !grib_data->pixmap()) {
#ifdef DEBUG
    my_log("err #%s\n", "0x1ee75bc1");
#endif
    return;
  }
  m_prev_pix = m_canvas->addPixmap(*grib_data->pixmap());
  if (!m_prev_pix) {
    my_log("err #%s\n", "0x89a3409");
    return;
  }
  if (pal_rebuild) {
    const rtlv_c &par = m_data->cur_param();
    m_data->clear_pal(par);

    if (m_prev_pal) {
      m_canvas->removeItem(m_prev_pal);
      delete m_prev_pal;
      m_prev_pal = nullptr;
    }
    delete m_pal_pxm;
    m_pal_pxm = nullptr;

    ramp_c &pal = m_data->ramp_ptr()[par.m_role][par.m_tag][par.m_lev][par.m_lval];
    m_pal_pxm = pal.make_pal(m_sets, par.m_role, par.m_tag);

    if (!m_pal_pxm) {
      my_log("err #%s\n", "0xc635440a");
      return;
    }
    m_prev_pal = m_canvas->addPixmap(*m_pal_pxm);
    if (!m_prev_pal) {
      my_log("err #%s\n", "0xbe76b007");
      return;
    }
    m_prev_pal->setZValue(3);

    move_ramppal();
    const QMatrix &m = m_canview->matrix();
    if (m.m11() >= numeric_limits<double>::max() || m.m22() >= numeric_limits<double>::max())
      m_canview->resetMatrix();
  }
}

void win_c::set_cur(bool block, bool expand)
{
  const grib_c *grib = m_data->cur_grib();
  const rtlv_c& par = m_data->cur_param();
  if (!grib || par.is_empty()) {
    my_log("err #%s\n", "0x568a2d80");
    return;
  }

  rtlv_rf_c params(par.m_role, par.m_tag, par.m_lev, par.m_lval,
    expand ? grib->datefc().run_str() : string(), expand ? grib->datefc().fc_str() : string());
  QModelIndex idx = m_tag_tree.tree_get_sel(params);
  if (!idx.isValid()) {
    my_log("err #%s\n", "0xe12e9d79");
    return;
  }
  if (block)
    m_tag_tree.m_view->selectionModel()->blockSignals(true);
  m_tag_tree.m_view->setCurrentIndex(idx);
  if (block)
    m_tag_tree.m_view->selectionModel()->blockSignals(false);
  m_tag_tree.m_view->setExpanded(idx, false);
  m_tag_tree.m_view->scrollTo(idx);
  m_tag_tree.m_view->setFocus();

  if (block)
    m_slider->blockSignals(true);
  m_slider->setMaximum(m_data->gribs()->size() - 1);
  m_slider->setValue(m_data->slot_idx(m_data->cur_grib()));
  if (block)
    m_slider->blockSignals(false);

  set_slider_lb();
  set_labels();
}

void win_c::scroll_changed(int)
{
  move_ramppal();
}

void win_c::settings_clicked()
{
  dlg_iface_c dlg(this);
  dlg.exec();
}

void win_c::open_clicked()
{
  stop_play();

  m_dlg_startup = new dlg_startup_c(this);
  m_dlg_startup->show();
}

void win_c::date_time_clicked()
{
  stop_play();
  init_proxy();

  m_dlg_datetime = new dlg_datetime_c(this);
  m_dlg_datetime->show();
}

void win_c::init_toolbar()
{
  QToolBar *toolbar = new QToolBar(this);
  addToolBar(toolbar);

  for (int i = 0; i < CMD_NUM; i++) {
    m_cmd[i] = new QToolButton;
    toolbar->addWidget(m_cmd[i]);
    switch (i) {
    case CMD_OPEN:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(open_clicked()));
      m_cmd[i]->setIcon(QPixmap(":/icons/open.png"));
      m_cmd[i]->setToolTip(IDSTR_OPEN);
      break;
    case CMD_NEW_BY_SLOT:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(date_time_clicked()));
      m_cmd[i]->setIcon(QPixmap(":/icons/load_by_date.png"));
      m_cmd[i]->setToolTip(IDSTR_DOWNLOAD);
      break;
    case CMD_WIZARD:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(wizard_clicked()));
      m_cmd[i]->setIcon(QPixmap(":/icons/load_wizard.png"));
      m_cmd[i]->setToolTip(IDSTR_DATA_LOADING_WIZARD);
      break;
    case CMD_ZOOM_IN:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(zoom_in_clicked()));
      m_cmd[i]->setIcon(QPixmap(":/icons/zoom_in.png"));
      m_cmd[i]->setToolTip(IDSTR_USE_MOUSE_WHELL);
      break;
    case CMD_ZOOM_OUT:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(zoom_out_clicked()));
      m_cmd[i]->setIcon(QPixmap(":/icons/zoom_out.png"));
      m_cmd[i]->setToolTip(IDSTR_USE_MOUSE_WHELL);
      break;
    case CMD_ZOOM_FIT:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(zoom_fit_clicked()));
      m_cmd[i]->setIcon(QPixmap(":/icons/zoom_fit.png"));
      m_cmd[i]->setToolTip(IDSTR_FIT_ZOOM_TO_WINDOW);
      break;
    case CMD_GO_FIRST:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(play_first_clicked()));
      m_cmd[i]->setIcon(QPixmap(":/icons/all_left.png"));
      m_cmd[i]->setToolTip(IDSTR_GO_FIRST);
      break;
    case CMD_GO_PREV:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(play_prev_clicked()));
      m_cmd[i]->setIcon(QPixmap(":/icons/one_left.png"));
      m_cmd[i]->setToolTip(IDSTR_GO_PREV);
      break;
    case CMD_GO_NEXT:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(play_next_clicked()));
      m_cmd[i]->setIcon(QPixmap(":/icons/one_right.png"));
      m_cmd[i]->setToolTip(IDSTR_GO_NEXT);
      break;
    case CMD_GO_LAST:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(play_last_clicked()));
      m_cmd[i]->setIcon(QPixmap(":/icons/all_right.png"));
      m_cmd[i]->setToolTip(IDSTR_GO_LAST);
      break;
    case CMD_GO_PLAY:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(play_clicked()));
      m_cmd[i]->setCheckable(true);
      m_cmd[i]->setIcon(QPixmap(":/icons/arrow.png"));
      m_cmd[i]->setToolTip(IDSTR_GO_PLAY);
      break;
    case CMD_SETTINGS:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(settings_clicked()));
      m_cmd[i]->setIcon(QPixmap(":/icons/wheel.png"));
      m_cmd[i]->setToolTip(IDSTR_SETTINGS);
      break;
    case CMD_COMPOSE_WIND:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(draw_wind_clicked()));
      m_cmd[i]->setCheckable(true);
      m_cmd[i]->setChecked(m_sets->m_wind_compose);
      m_cmd[i]->setIcon(QPixmap(":/icons/windy.png"));
      m_cmd[i]->setToolTip(IDSTR_WIND_VECTORS);
      break;
    case CMD_COMPOSE_TEMPRE:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(draw_temp_clicked()));
      m_cmd[i]->setCheckable(true);
      m_cmd[i]->setChecked(m_sets->m_tempre_compose);
      m_cmd[i]->setIcon(QPixmap(":/icons/temperature_warm.png"));
      m_cmd[i]->setToolTip(IDSTR_TEMPERATURE);
      break;
    case CMD_COMPOSE_GEOPT:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(draw_geopt_clicked()));
      m_cmd[i]->setCheckable(true);
      m_cmd[i]->setChecked(m_sets->m_geopt_compose);
      m_cmd[i]->setIcon(QPixmap(":/icons/speedometer.png"));
      m_cmd[i]->setToolTip(IDSTR_PRESSURE);
      break;
    case CMD_COMPOSE_PRECIP:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(draw_precip_clicked()));
      m_cmd[i]->setCheckable(true);
      m_cmd[i]->setChecked(m_sets->m_precip_compose);
      m_cmd[i]->setIcon(QPixmap(":/icons/weather_rain.png"));
      m_cmd[i]->setToolTip(IDSTR_PRECIPITATION);
      break;
    case CMD_GEOGRID:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(draw_geogr_clicked()));
      m_cmd[i]->setCheckable(true);
      m_cmd[i]->setChecked(m_sets->m_geogrid);
      m_cmd[i]->setIcon(QPixmap(":/icons/lc_helplinesvisible.png"));
      m_cmd[i]->setToolTip(IDSTR_GEOGRAPHIC_GRID);
      break;
    case CMD_INFORMATION:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(information()));
      m_cmd[i]->setIcon(QPixmap(":/icons/info_16.png"));
      m_cmd[i]->setToolTip(IDSTR_ABOUT);
      break;
    case CMD_CLOSE_ALL:
      connect(m_cmd[i], SIGNAL(clicked()), this, SLOT(close_all_clicked()));
      m_cmd[i]->setIcon(QPixmap(":/icons/sc_deleteallannotation_g.png"));
      m_cmd[i]->setToolTip(IDSTR_CLOSE_ALL);
      break;
    }

    if (i == CMD_WIZARD || i == CMD_ZOOM_FIT || i == CMD_GO_PLAY || i == CMD_INFORMATION || i == CMD_GEOGRID) {
      QWidget *spacer = new QWidget;
      spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      spacer->setMinimumWidth(6);
      toolbar->addWidget(spacer);
    }
  }
}

void win_c::zoom_in_clicked()
{
  if (m_canview)
    m_canview->zooming(5);
}

void win_c::zoom_out_clicked()
{
  if (m_canview)
    m_canview->zooming(-5);
}

void win_c::zoom_fit_clicked()
{
  if (m_canview) {
    QRectF rcf = m_canview->sceneRect();
    const QRect size = geometry();
    double s = MIN(size.width() / rcf.width(), size.height() / rcf.height());
    m_canview->scale(s, s);
  }
}

bool win_c::update_canv(const tag_grib_c& next)
{
  bool ret = false;

  grib_c *grib = m_data->get_grib(next.m_gr->datefc().run_str(), next.m_gr->datefc().fc_str());
  grib_data_c *grib_data = grib->grib_data_ptr(next.m_rtlv);
  if (!grib_data) {
#ifdef DEBUG
    my_log("err #%s\n", "0x2ba58a6b");
#endif
    goto end;
  }
  wait_cursor(true);
  bool clear = false;
  if (!grib_data || !grib_data->plane() || !grib_data->pixmap()) {
    if (!grib_data->make_grib(clear)) {
#ifdef DEBUG
      my_log("err #%s\n", "0x53053e92");
#endif
      goto end;
    }
  }
  m_data->set_cur_grib(next);
  grib_done(clear);
  ret = true;
end:
  if (!ret)
    set_msg(IDSTR_NO_DATA_B, true, true);
  wait_cursor(false);
  return ret;
}

void win_c::play_first_clicked()
{
  stop_play();
  m_slider->triggerAction(QAbstractSlider::SliderToMinimum);
}

void win_c::play_prev_clicked()
{
  stop_play();
  int cur = m_data->slot_idx(m_data->cur_grib());
  if (cur <= 0)
    return;
  m_slider->triggerAction(QAbstractSlider::SliderSingleStepSub);
}

void win_c::play_next_clicked()
{
  int cur = m_data->slot_idx(m_data->cur_grib());
  m_slider->triggerAction(cur < m_data->gribs()->size() - 1 ? QAbstractSlider::SliderSingleStepAdd : QAbstractSlider::SliderToMinimum);
}

void win_c::play_last_clicked()
{
  stop_play();
  m_slider->triggerAction(QAbstractSlider::SliderToMaximum);
}

void  win_c::play_clicked()
{
  if (!m_start) {
    if (m_data->gribs()->size() > 1) {
      m_start = true;
      m_timer->start(m_sets->m_play_timeout);
      m_cmd[CMD_GO_PLAY]->setChecked(true);
    }
  }
  else
    stop_play();
}

void win_c::slider_changed(int val)
{
  grib_c *grib = m_data->get_slot(val);
  if (!grib) {
    set_msg(IDSTR_NO_DATA_B, true, true);
    my_log("err #%s\n", "0x169d1aa6");
    return;
  }

  if (update_canv(tag_grib_c(m_data->cur_param(), grib)))
    set_slider_lb();
}

void win_c::set_slider_lb()
{
  const char *month[12] = { "Jan","Feb","Mar","Apr","May","June","July","Aug","Sept","Oct","Nov","Dec" };
  const char *dayweek[7] = { "Sun","Mon","Tue","Wed","Thu","Fri","Sat" };
  char buf[100] = { 0 };
  if (m_data->cur_grib()) {
    const datefc_c &date = m_data->cur_grib()->datefc();
    int year = date.m_year;
    int mon = date.m_mon;
    int day = date.m_day;
    int hour = date.m_hour + date.m_fcst;
    while (hour >= 24) {
      day++;
      hour -= 24;
      if (datefc_c::day_in_mon(year, mon) < day) {
        day = 1;
        mon++;
        if (mon > 12) {
          mon = 1;
          year++;
          break;
        }
      }
    }
    int dw = datefc_c::day_of_week(year, mon, day);
    sprintf(buf, "<b>%s %02d %s %02d:00UTC</b>  Run %02dz %03d", dayweek[dw], day, month[mon - 1], hour, date.m_hour, date.m_fcst);
  }
  m_slide_lb->setText(buf);
  QPalette palette = m_slide_lb->palette();
  palette.setColor(m_slide_lb->foregroundRole(), Qt::darkBlue);
  m_slide_lb->setPalette(palette);
}

void  win_c::clear_pixmaps()
{
  delete m_prev_pix;
  m_prev_pix = nullptr;

  if (m_data->gribs()) {
    for (auto itg = m_data->gribs()->begin(); itg != m_data->gribs()->end(); itg++) {
      grib_c *p = *itg;
      for (auto it = p->data_map().begin(); it != p->data_map().end(); it++) {
        for (auto it0 = (*it).second.begin(); it0 != (*it).second.end(); it0++) {
          for (auto it1 = (*it0).second.begin(); it1 != (*it0).second.end(); it1++) {
            for (auto it2 = (*it1).second.begin(); it2 != (*it1).second.end(); it2++) {
              grib_data_c *grib_data = (*it2).second;
              delete grib_data->pixmap_ptr();
              grib_data->set_pixmap(nullptr);
            }
          }
        }
      }
    }
  }
}

void  win_c::draw_wind_clicked()
{
  m_sets->m_wind_compose = m_cmd[CMD_COMPOSE_WIND]->isChecked();
  clear_pixmaps();
  update_canv(m_data->tag_grib());
}

void  win_c::draw_temp_clicked()
{
  m_sets->m_tempre_compose = m_cmd[CMD_COMPOSE_TEMPRE]->isChecked();

  grib_data_c *grib_data = m_data->cur_gdata_ptr();
  if (grib_data)
    grib_data->clear_pix_iso();

  update_canv(m_data->tag_grib());
}

void  win_c::draw_geopt_clicked()
{
  m_sets->m_geopt_compose = m_cmd[CMD_COMPOSE_GEOPT]->isChecked();
  clear_pixmaps();
  update_canv(m_data->tag_grib());
}

void  win_c::draw_precip_clicked()
{
  m_sets->m_precip_compose = m_cmd[CMD_COMPOSE_PRECIP]->isChecked();
  clear_pixmaps();
  update_canv(m_data->tag_grib());
}

void win_c::draw_geogr_clicked()
{
  m_sets->m_geogrid = !m_sets->m_geogrid;
  if (!m_sets->m_geogrid) {
    QList<QGraphicsItem *> list = m_geogr_group->childItems();
    for (QGraphicsItem *item : list) {
      delete item;
    }
    m_data->clear_geogr();
  }
  else if (!m_data->geogr()) {
    m_data->set_geogr(new info_cntrs_c(this));
    m_data->geogr_ptr()->make_geogr();
  }
}

void win_c::mes_lb_clear()
{
  m_mes_lb->clear();
}

void win_c::set_msg(const char *msg, bool red, bool clear)
{
  m_mes_lb->setText(msg);
  if (red) {
    QPalette palette = m_mes_lb->palette();
    palette.setColor(m_mes_lb->foregroundRole(), Qt::red);
    m_mes_lb->setPalette(palette);
  }
  if (clear)
    QTimer::singleShot(5000, this, SLOT(mes_lb_clear()));
  else
    QTimer::singleShot(50000, this, SLOT(mes_lb_clear()));
}

void win_c::close_all_clicked()
{
  clear_w();

  delete m_data;
  m_data = new info_data_c(this);
}

void win_c::clear_w()
{
  m_data->clear_grib();

  m_sets->m_wind_compose = m_sets->m_tempre_compose = m_sets->m_geopt_compose = m_sets->m_precip_compose = false;

  delete m_prev_pix;
  m_prev_pix = nullptr;
  delete m_prev_pal;
  m_prev_pal = nullptr;
  delete m_pal_pxm;
  m_pal_pxm = nullptr;

  m_tag_tree.tree_done();

  m_slider->setMaximum(0);
  m_slide_lb->setText("");
  m_tag_lb->setText("");
  m_level_lb->setText("");
  m_units_lb->setText("");
  m_codes_lb->setText("");
  m_reg_cp->setText("");
}

void win_c::set_labels()
{
  if (!m_data->cur_grib())
    return;
  const rtlv_c& par = m_data->cur_param();
  const string& role = par.m_role;
  int idx = tag2level_idx(role, par.m_tag, par.m_lev, par.m_lval);
  if (idx == -1) {
    my_log("err #%s\n", "0x3d0ac5e9");
    return;
  }
  const char *descr = role == string(GFS_025) ? grib2_all[gfs_tag2level[idx].idx].short_name : grib2_all[idx].full_name;
  const char *unit = role == string(GFS_025) ? grib2_all[gfs_tag2level[idx].idx].units : grib2_all[idx].units;

  const char *tag = par.m_tag.c_str();
  m_tag_lb->setText(QString(descr) + " (" + tag + ")");

  QString s(IDSTR_UNITS);
  if (TEMPERATURE_CODES(tag))
    s += TEMPERATURE_LABEL(m_sets->m_units_temp);
  else if (GEOPOTENTIAL_CODES(tag))
    s += GEOPOTENTIAL_LABEL(m_sets->m_units_geopt);
  else if (SPEED_CODES(tag))
    s += SPEED_LABEL(m_sets->m_units_speed);
  else if (PRECIPITATION_CODES(tag))
    s += PRECIPITATION_LABEL(m_sets->m_units_precip);
  else if (PRECIP_RATE_CODES(tag))
    s += PRECIP_RATE_LABEL(m_sets->m_units_prerate);
  else if (PRESSURE_CODES(tag))
    s += PRESSURE_LABEL(m_sets->m_units_press);
  else
    s += unit;
  m_units_lb->setText(s);

  int j = grib_level_idx(par.m_lev);
  QString s1(grib_level_list[j].name);
  s1 += " ";
  s1 += par.m_lval.c_str();
  s1 += " (";
  s1 += grib_level_list[j].abbr;
  s1 += ")";
  m_level_lb->setText(s1);

  const grib_data_c *gd = m_data->cur_gdata();
  if (gd) {
    m_codes_lb->setText(gd->codes_str().c_str());

    const dlatlon_c& reg = m_data->cur_gdata()->dlatlon();
    double x = reg.m_lon_ll + gd->plane_nx() * reg.m_delta / 2;
    double y = reg.m_lat_ll + gd->plane_ny() * reg.m_delta / 2;
    int x1 = (x - int(x)) * 60;
    int y1 = (y - int(y)) * 60;
    QString s2 = QString("Pos:%1°%2'N,%3°%4'E  Ny:%5,Nx:%6 Δ:%7°").arg(
      QString::number(int(y)), QString::number(y1),
      QString::number(int(x)), QString::number(x1),
      QString::number(gd->plane_ny()), QString::number(gd->plane_nx()),
      QString::number(reg.m_delta));
    m_reg_cp->setText(s2);
  }
  else
    m_reg_cp->setText("");
}

void win_c::fit_desktop()
{
  QSize size = 0.8 * QApplication::desktop()->availableGeometry().size();
  setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size, QApplication::desktop()->availableGeometry()));
  QList<int> sizes = { 200, size.width() - 200 };
  m_splitter->setSizes(sizes);
  if (m_canview) {
    QRectF rcf = m_canview->sceneRect();
    double s = MIN(size.width() / rcf.width(), size.height() / rcf.height());
    m_canview->scale(s, s);
  }
}

void win_c::stop_play()
{
  m_start = false;
  m_timer->stop();
  m_cmd[CMD_GO_PLAY]->setChecked(false);
}

void win_c::information()
{
  stop_play();

  QMessageBox msg_box;
  QPixmap pixmap(":/icons/gfs_view32.png");
  msg_box.setWindowTitle(PROGRAM_NAME);
  msg_box.setIconPixmap(pixmap);
  msg_box.setWindowIcon(pixmap);
  msg_box.setTextFormat(Qt::RichText);
  const char *prog_title = "<span style='font-size:14pt; font-weight:600; color:#224488;'>" PROGRAM_NAME "</span> version  " \
    PROGRAM_MAJ_VERSION "." PROGRAM_MIN_VERSION;
  char buf[XLARGEBUF];

  strcpy(buf, prog_title);
  strcat(buf,
    "<hr><span style='font-size:11pt;'>"
    "The program to generate and display weather forecast maps<hr>"
    "(C) 2017-2018, Bad Programmer<br>"
    "<a href='mailto:badpr0ogrammer@gmail.com'>badpr0ogrammer@gmail.com</a><br>"
    "<a href='https://gfs-view.firebaseapp.com/'>gfs-view.firebaseapp.com</a>"
    "</span>");
  msg_box.setText(buf);

  msg_box.setContentsMargins(12, 12, 12, 12);
  msg_box.exec();
}

void win_c::init_proxy()
{
  if (m_sets->m_proxy && !m_sets->m_proxy_host.isEmpty()) {
    QNetworkProxy proxy;
    proxy.setHostName(m_sets->m_proxy_host);
    proxy.setPort(quint16(m_sets->m_proxy_port));
    proxy.setType(QNetworkProxy::HttpProxy);
    if (!m_sets->m_proxy_user.isEmpty()) {
      proxy.setUser(m_sets->m_proxy_user);
      if (!m_sets->m_proxy_passwd.isEmpty())
        proxy.setPassword(m_sets->m_proxy_passwd);
    }
    QNetworkProxy::setApplicationProxy(proxy);
    m_netman->setProxy(proxy);
  }
  else {
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    m_netman->setProxy(QNetworkProxy());
  }
}

void win_c::wait_cursor(bool set)
{
  if (set)
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
  else
    QGuiApplication::restoreOverrideCursor();
  QApplication::processEvents();
}

void win_c::wizard_finish()
{
  data_tmpl_t tmpl;
  grib_ini_c *grib_ini = new grib_ini_c(this, m_dbl_date_ui, m_fcst_ui, m_tag_tree_ui, tmpl);

  if (!grib_ini || !tmpl.size()) {
    message_box(QMessageBox::Warning, IDSTR_SELECT_DATA_PLANES, QMessageBox::Ok);
    delete grib_ini;
    return;
  }
  grib_ini->show_prog_dlg(this, tmpl.size());

  grib_ini->download(m_reg_ui->m_reg_lat->value(), m_reg_ui->m_reg_lon->value(), m_reg_ui->m_reg_height->value(), m_reg_ui->m_reg_width->value(),
    m_dwnl_all->checkState() == Qt::CheckState::Checked, tmpl);

  if (!grib_ini->dwnl_szsum()) {
    delete grib_ini;
    set_msg(IDSTR_OP_FAILED, true, false);
    return;
  }
  grib_ini->prog_dlg_delete();

  delete m_data;
  m_data = new info_data_c(this);

  grib_ini->show_prog_dlg(this, tmpl.size());
  if (grib_ini->open_data(tmpl)) {
    data_tmpl_t out;
    if (m_data->data_tmpl(out))
      m_sets->save_ini(SETS_GROUP_GRIB_STARTUP | SETS_GROUP_DATE_TIME, &out);

    m_tag_tree.tree_from_gribs(m_data->gribs());
    set_cur(true, true);
    grib_done(true);

    m_sets->read_cntrs();
  }
  else
    set_msg(IDSTR_OP_FAILED, true, false);

  delete grib_ini;
}

#define WIZARD_TITLE_STYLE      "<span style='color:#47c829;font-size:13pt;'>"
#define WIZARD_SUBTITLE_STYLE   "<span style='color:#c82947;font-size:12pt;'>"
#define WIZARD_LABEL_STYLE      "<span style='color:#152464;font-size:10pt;'>"

#define WIZARD_LABEL(_lb) \
_lb->setWordWrap(true); \
_lb->setMargin(24); \
_lb->setStyleSheet("background-color:#fefefe;"); \
_lb->setAutoFillBackground(true)

void win_c::wizard_clicked()
{
  init_proxy();

  QWizard *wizard = new QWizard;
  wizard->setParent(nullptr);
  wizard->setWizardStyle(QWizard::ModernStyle);
  wizard->setWindowTitle(IDSTR_DATA_LOADING_WIZARD);
  wizard->setPixmap(QWizard::BannerPixmap, QPixmap(":/icons/banner_gfs.png"));
  wizard->setTitleFormat(Qt::RichText);
  wizard->setSubTitleFormat(Qt::RichText);
  wizard->setWindowIcon(QPixmap(":/icons/gfs_view16.png"));

  char title[100] = { 0 }, no = 1;
  {
    QWizardPage *page2 = new QWizardPage;
    sprintf(title, WIZARD_TITLE_STYLE "Step %d</span>", no++);
    page2->setTitle(title);
    page2->setSubTitle(WIZARD_SUBTITLE_STYLE "The City & Extent selection</span>");

    QVBoxLayout *vbox2 = new QVBoxLayout;
    page2->setLayout(vbox2);
    page2->setContentsMargins(6, 6, 6, 6);

    m_reg_ui = new reg_ui_c(page2, false, this);

    vbox2->addWidget(m_reg_ui);
    wizard->addPage(page2);

    QLabel *label2 = new QLabel;
    WIZARD_LABEL(label2);
    label2->setText(WIZARD_LABEL_STYLE "<p>Select a City from the list<p>Adjust region extent.<p>(latitude/longitude in decimal degrees)</span>");
    vbox2->addWidget(label2);
  }
  {
    QWizardPage *page3 = new QWizardPage;
    sprintf(title, WIZARD_TITLE_STYLE "Step %d</span>", no++);
    page3->setTitle(title);
    page3->setSubTitle(WIZARD_SUBTITLE_STYLE "The Date selection</span>");

    QVBoxLayout *vbox3 = new QVBoxLayout;
    page3->setLayout(vbox3);
    page3->setContentsMargins(6, 6, 6, 6);

    m_dbl_date_ui = new dbl_date_ui_c(nullptr, DEF_RUNTIME_LAG, GFS_025, this);
    vbox3->addWidget(m_dbl_date_ui);
    wizard->addPage(page3);

    QLabel *label3 = new QLabel;
    label3->setText(WIZARD_LABEL_STYLE "<p>Select the Starting/Ending Date<p>Each day in the range will be processed,\
<p>starting from the first Run time and ending by the last Run time.<br>(See next page)</ul></span>");
    WIZARD_LABEL(label3);
    vbox3->addWidget(label3);
  }
  {
    class MyPage4 : public QWizardPage
    {
      win_c *m_win;
    public:
      MyPage4(win_c *win) : QWizardPage(win), m_win(win) {}
    protected:
      void initializePage() Q_DECL_OVERRIDE {
        int role = field("roleComboBox").toInt();
        if (role < 0 && role >= array_countof(my_roles)) {
          my_log("err #%s\n", "0xf708a21a");
          return;
        }
        QVBoxLayout *vbox4 = new QVBoxLayout;
        setLayout(vbox4);
        setContentsMargins(6, 6, 6, 6);

        m_win->m_fcst_ui = new fcst_ui_c(m_win, my_roles[role]);
        vbox4->addWidget(m_win->m_fcst_ui);

        QLabel *label4 = new QLabel;
        label4->setText(WIZARD_LABEL_STYLE "Each day the NCEP runs the GFS numerical model four times (at 0000,0600,1200,1800).\
<p>For each Run time there are many Forecasts with one hour Step.\
<p>Select the starting Run time for the starting day and the ending Run time for the ending day.\
<p>All the forecast times from the first to the last, with the Step selected will be downloaded.\
</span>");
        WIZARD_LABEL(label4);
        label4->setMargin(6);
        vbox4->addWidget(label4);
      }
    };
    MyPage4 *page4 = new MyPage4(this);
    sprintf(title, WIZARD_TITLE_STYLE "Step %d</span>", no++);
    page4->setTitle(title);
    page4->setSubTitle(WIZARD_SUBTITLE_STYLE "The Run & Forecast selection</span>");

    wizard->addPage(page4);
  }
  {
    class MyPage5 : public QWizardPage
    {
      win_c *m_win;
    public:
      MyPage5(win_c *win) : QWizardPage(win), m_win(win) {}
    protected:

      void initializePage() Q_DECL_OVERRIDE {
        int role = field("roleComboBox").toInt();
        if (role < 0 && role >= array_countof(my_roles)) {
          my_log("err #%s\n", "0xeb897603");
          return;
        }
        QVBoxLayout *vbox5 = new QVBoxLayout;
        setLayout(vbox5);
        setContentsMargins(6, 6, 6, 6);

        m_win->m_tag_tree_ui = new tag_tree_ui_c(m_win);
        m_win->m_tag_tree_ui->tree_add(true, false, false, my_roles[role], nullptr, true);

        vbox5->addWidget(m_win->m_tag_tree_ui->m_view);
        m_win->m_tag_tree_ui->expand_all(3);

        m_win->m_dwnl_all = new QCheckBox(IDSTR_ALL_CODES_AND_LEVELS);
        QWidget::connect(m_win->m_dwnl_all, static_cast<void(QCheckBox::*)(int)>(&QCheckBox::stateChanged), [&](int state) {
          m_win->m_tag_tree_ui->check_all(state == Qt::Checked);
        });
        m_win->m_dwnl_all->setChecked(true);
        vbox5->addWidget(m_win->m_dwnl_all);
      }
    };
    MyPage5 *page5 = new MyPage5(this);
    sprintf(title, WIZARD_TITLE_STYLE "Step %d</span>", no++);
    page5->setTitle(title);
    page5->setSubTitle(WIZARD_SUBTITLE_STYLE "The Grib Values selection</span>");

    wizard->addPage(page5);
  }
  connect(wizard->button(QWizard::FinishButton), SIGNAL(clicked()), this, SLOT(wizard_finish()));

  wizard->exec();

  m_reg_ui = nullptr;
  m_dbl_date_ui = nullptr;
  m_fcst_ui = nullptr;
  m_tag_tree_ui = nullptr;
  m_dwnl_all = nullptr;

  delete wizard;
}

int win_c::message_box(QMessageBox::Icon icon, const char *msg, QMessageBox::StandardButtons flags)
{
  QMessageBox msg_box;
  msg_box.setIcon(icon);
  msg_box.setText(msg);
  msg_box.setStandardButtons(flags);
  msg_box.setWindowTitle(PROGRAM_NAME);
  msg_box.setWindowIcon(QPixmap(":/icons/gfs_view16.png"));
  return msg_box.exec();
}

string win_c::timestamp()
{
  char buf[15];
  time_t t = time(nullptr);
  struct tm *stm = gmtime(&t);
  //struct tm *stm = localtime(&t);
  snprintf(buf, sizeof(buf), "%04d%02d%02d%02d%02d%02d", stm->tm_year + 1900, stm->tm_mon + 1, stm->tm_mday, stm->tm_hour, stm->tm_min, stm->tm_sec);
  return string(buf);
}

static win_c *my_win;

void my_log(const char *frmt, const char *s)
{
  FILE *fd = nullptr;
#ifndef DEBUG
#ifdef _WINDOWS
  fd = _wfopen(my_win->m_log_fname, L"at");
#else
  fd = fopen(my_win->m_log_fname, "at");
#endif
#endif
  if (!fd)
    fd = stdout;
  fprintf(fd, frmt, s);
#ifndef DEBUG  
  fclose(fd);
#endif
}

void win_c::init_log()
{
  my_win = this;

  FILE *fd = nullptr;
  QString log_fname = m_persfold + DIR_SEPARATOR_S PROGRAM_NAME ".log";
#ifdef _WINDOWS
  log_fname.toWCharArray(m_log_fname);
  m_log_fname[log_fname.length()] = 0;

  fd = _wfopen(m_log_fname, L"rb");
#else
  strcpy(m_log_fname, log_fname.toUtf8().data());
  fd = fopen(m_log_fname, "rb");
#endif
  if (fd) {
    char ok_str[3] = { 0, 0, 0 };
    if (!fseek(fd, -4, SEEK_END) && fread(ok_str, 1, 2, fd) && !strncmp(ok_str, "OK", 3))
      m_term_ok = true;
    fclose(fd);
#ifdef _WINDOWS
    fd = _wfopen(m_log_fname, L"w");
#else
    fd = fopen(m_log_fname, "w");
#endif

    if (fd)
      fclose(fd);
  }
  my_log("Start %s\n", m_timestamp.c_str());
}
