/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <opencv2/core/core.hpp>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <QQmlApplicationEngine> 
#include <QQuickWindow>
#include <QQmlComponent>
#include <QtLocation>

#include "local_str.h"
#include "regui.h"
#include "sets.h"
#include "infodata.h"
#include "mgrb2.h"
#include "winc.h"
#include "places.h"

reg_ui_c::reg_ui_c(QWidget *parent, bool osm, win_c *win) : QGroupBox(parent),
m_quick_map(0), m_osm_btn(0), m_win(win)
{
  sets_c *sets = m_win->m_sets;
  const grib_data_c* pdata = m_win->m_data->cur_gdata();
  const dlatlon_c& reg = pdata ? pdata->dlatlon() : dlatlon_c();
  double w = pdata ? reg.width(pdata->plane_nx()) : 10;
  double h = pdata ? reg.height(pdata->plane_ny()) : 8;

  setTitle(IDSTR_GEOREFERENCING);

  QGridLayout *grid = new QGridLayout;
  setLayout(grid);
  grid->setSpacing(6);

  QLabel *label = new QLabel(IDSTR_SELECT_WORLD_REGION_OR);
  grid->addWidget(label, 0, 0);

  initializer_list<QString> list{ "Europe", "North America", "South West Pacific", "Asia" };
  QComboBox *cb = new QComboBox;
  cb->addItems(list);
  cb->setCurrentIndex(-1);
  grid->addWidget(cb, 0, 1, 1, 4);
  QWidget::connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [&](int i) {
    Rect_<double> rc[4] = { {14, 46, 52, 32}, {-98, 44, 73, 42}, {135, -9, 84, 57}, {103, 33, 113, 53} };
    i = MAX(0, MIN(3, i));
    m_reg_lon->setValue(rc[i].x);
    m_reg_lat->setValue(rc[i].y);
    m_reg_width->setValue(rc[i].width);
    m_reg_height->setValue(rc[i].height);
  });

  label = new QLabel(IDSTR_OR_SELECT_A_CITY);
  grid->addWidget(label, 1, 0);

  QModelIndex sel_idx;
  m_pl_model = places_create(sets->m_place_idx, sel_idx);
  m_pl_combo = new QComboBox;
  m_pl_combo->setModel(m_pl_model);
  m_pl_combo->setView(new QTreeView);
  m_pl_view = (QTreeView *)m_pl_combo->view();
  m_pl_view->header()->hide();

  grid->addWidget(m_pl_combo, 1, 1, 1, 4);

  m_pl_view->setCurrentIndex(sel_idx);
  m_pl_combo->setRootModelIndex(sel_idx.parent());
  m_pl_combo->setCurrentIndex(sel_idx.row());
  m_pl_combo->setRootModelIndex(QModelIndex());

  label = new QLabel(IDSTR_LONGITUDE);
  label->setAlignment(Qt::AlignVCenter);
  grid->addWidget(label, 2, 0);

  m_reg_lon = new QDoubleSpinBox;
  m_reg_lon->setRange(-180, 180);
  m_reg_lon->setSingleStep(reg.m_delta);
  m_reg_lon->setValue(reg.m_lon_ll + w / 2);
  grid->addWidget(m_reg_lon, 2, 1);

  label = new QLabel(IDSTR_LATITUDE);
  label->setAlignment(Qt::AlignVCenter);
  grid->addWidget(label, 2, 3);

  m_reg_lat = new QDoubleSpinBox;
  m_reg_lat->setRange(-90, 90);
  m_reg_lat->setSingleStep(reg.m_delta);
  m_reg_lat->setValue(reg.m_lat_ll + h / 2);
  grid->addWidget(m_reg_lat, 2, 4);

  label = new QLabel(IDSTR_WIDTH);
  label->setAlignment(Qt::AlignVCenter);
  grid->addWidget(label, 3, 0);

  m_reg_width = new QDoubleSpinBox;
  m_reg_width->setRange(0, 360);
  m_reg_width->setSingleStep(reg.m_delta);

  m_reg_width->setValue(w);
  grid->addWidget(m_reg_width, 3, 1);

  label = new QLabel(IDSTR_HEIGHT);
  label->setAlignment(Qt::AlignVCenter);
  grid->addWidget(label, 3, 3);

  m_reg_height = new QDoubleSpinBox;
  m_reg_height->setRange(0, 180);
  m_reg_height->setSingleStep(reg.m_delta);
  m_reg_height->setValue(h);
  grid->addWidget(m_reg_height, 3, 4);

  QWidget::connect(m_pl_view->selectionModel(),
    static_cast<void(QItemSelectionModel::*)(const QModelIndex&, const QModelIndex&)>(&QItemSelectionModel::currentChanged), [&](const QModelIndex &cur) {
    if (!cur.isValid() || !cur.data(Qt::UserRole + 1).isValid())
      return;
    int idx = cur.data(Qt::UserRole + 1).value<int>();
    if (idx < 0 || idx >= NE10M_PLACES_NUM)
      return;
    m_win->m_sets->m_place_idx = idx;
    name_ll_t *p = ne10m_places + idx;
    if (m_reg_width->value() < 0.25 || m_reg_height->value() < 0.25) {
      m_reg_width->setValue(10);
      m_reg_height->setValue(8);
    }
    m_reg_lon->setValue(p->lon);
    m_reg_lat->setValue(p->lat);
  });

  if (osm) {
    label = new QLabel(IDSTR_SELECT_REGION);
    label->setAlignment(Qt::AlignVCenter);
    grid->addWidget(label, 3, 5);

    m_osm_btn = new QPushButton;
    m_osm_btn->setIcon(QIcon(QPixmap(":/icons/globe.png")));
    QObject::connect(m_osm_btn, SIGNAL(clicked()), this, SLOT(quick_map_clicked()));
    grid->addWidget(m_osm_btn, 3, 6);
  }
}

void reg_ui_c::quick_map_cleanup()
{
  if (m_quick_map) {
    m_quick_map->hide();
    m_quick_map->deleteLater();
    m_quick_map = nullptr;
  }
}

#include "map_qml.h"

void reg_ui_c::quick_map_clicked()
{
  sets_c *sets = m_win->m_sets;

  double lon = m_reg_lon->value();
  double lat = m_reg_lat->value();
  double w0 = m_reg_width->value();
  double h0 = m_reg_height->value();

  m_win->m_engine->loadData(QByteArray(MAP_REG_QML), QUrl());
  m_quick_map = qobject_cast<QQuickWindow*>(m_win->m_engine->rootObjects().value(0));
  if (!m_quick_map) {
    m_win->set_msg(IDSTR_OP_FAILED, true, false);
    my_log("err #%s\n", "0x1eba66c5");
  }
  QQmlEngine::setObjectOwnership(m_quick_map, QQmlEngine::CppOwnership);
  m_quick_map->setTitle(PROGRAM_NAME " : " IDSTR_SELECT_RECTANGULAR_REGION);
  m_quick_map->setIcon(QIcon(QPixmap(":/icons/gfs_view16.png")));

  QObject::connect(m_quick_map, SIGNAL(appWinAccepted(QVariant)), this, SLOT(quick_map_done(QVariant)));
  QObject::connect(m_quick_map, SIGNAL(appWinRejected()), this, SLOT(quick_map_cleanup()));
  {
    QObject *rect = m_quick_map->findChild<QObject*>("redRect");
    if (!rect) {
      my_log("err #%s\n", "0x2a4e46d6");
    }
    else {
      QGeoCoordinate topLeft(0.5, -0.5), bottomRight(-0.5, 0.5);
      double w = MIN(300, MAX(1, w0));
      double h = MIN(120, MAX(1, h0));
      topLeft.setLongitude(MAX(-150, lon - w / 2));
      bottomRight.setLatitude(MAX(-60, lat - h / 2));
      bottomRight.setLongitude(lon + w / 2);
      topLeft.setLatitude(lat + h / 2);
      rect->setProperty("topLeft", QVariant::fromValue(topLeft));
      rect->setProperty("bottomRight", QVariant::fromValue(bottomRight));
    }
  }
  {
    QObject *map = m_quick_map->findChild<QObject*>("osmMap");
    if (!map) {
      my_log("err #%s\n", "0xcb3891d4");
    }
    else {
      QGeoCoordinate center(0, 0);
      double w = MAX(1, w0);
      double h = MAX(1, h0);
      center.setLongitude(lon + w / 2);
      center.setLatitude(lat + h / 2);
      map->setProperty("center", QVariant::fromValue(center));
    }
  }
  m_quick_map->raise();
  m_quick_map->setVisible(true);
  m_quick_map->show();
}

void reg_ui_c::quick_map_done(QVariant rectObj)
{
  sets_c *sets = m_win->m_sets;
  QObject *objRect = rectObj.value<QObject*>();
  if (!objRect) {
    m_win->set_msg(IDSTR_OP_FAILED, true, false);
    quick_map_cleanup();
    my_log("err #%s\n", "0xca906763");
    return;
  }

  QGeoCoordinate topLeft = objRect->property("topLeft").value<QGeoCoordinate>();
  QGeoCoordinate bottomRight = objRect->property("bottomRight").value<QGeoCoordinate>();

  double w = bottomRight.longitude() - topLeft.longitude();
  double h = topLeft.latitude() - bottomRight.latitude();

  m_reg_lon->setValue(topLeft.longitude() + w / 2);
  m_reg_lat->setValue(bottomRight.latitude() + h / 2);
  m_reg_width->setValue(w);
  m_reg_height->setValue(h);

  quick_map_cleanup();
}
