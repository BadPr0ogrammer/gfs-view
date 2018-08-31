/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <stdio.h>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtNetwork>

#include <opencv2/core/core.hpp>

#include "defs.h"
#include "local_str.h"
#include "color_tables.h"
#include "staticstr.h"
#include "datefcst.h"
#include "griblatlon.h"
#include "gribini.h"
#include "shaperead.h"
#include "cntrs.h"
#include "infodata.h"
#include "winc.h"
#include "mgrb2.h"
#include "gribitem.h"
#include "dlgdatetime.h"
#include "sets.h"

#include "grib_strings.h"

dlg_datetime_c::dlg_datetime_c(win_c *pwin) : QDialog(pwin), m_win(pwin),
m_dbl_date_ui(nullptr, DEF_RUNTIME_LAG, GFS_025, pwin), m_fcst_ui(pwin, GFS_025), m_tag_tree(pwin), m_reg_ui(this, true, pwin)
{
  string role;
  {
    data_tmpl_t tmpl;
    m_win->m_data->data_tmpl(tmpl);
    role = !tmpl.empty() ? tmpl.begin()->second.begin()->first : GFS_025;
  }
  m_tag_tree.tree_add(true, false, false, role, nullptr, true);
  m_tag_tree.expand_all(0);

  sets_c *sets = m_win->m_sets;

  setWindowTitle(IDSTR_DOWNLOAD_GRIB2_DATA);

  QVBoxLayout *vbox = new QVBoxLayout;
  setLayout(vbox);
  setContentsMargins(12, 12, 12, 12);

  vbox->addWidget(&m_reg_ui);

  vbox->addWidget(&m_dbl_date_ui);

  bool gfs = role == GFS_025;
  m_fcst_ui.m_fcst0->setEnabled(gfs);
  m_fcst_ui.m_fcst1->setEnabled(gfs);
  m_fcst_ui.m_step->setEnabled(gfs);
  vbox->addWidget(&m_fcst_ui);
  {
    QGroupBox *gb = new QGroupBox(IDSTR_RUNTIME_LAG_EXPLAIN);
    QHBoxLayout *hbox = new QHBoxLayout;
    gb->setLayout(hbox);

    QSpinBox *sb = new QSpinBox;
    sb->setRange(0, 60);
    sb->setValue(sets->m_runtime_lag);

    sb->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    QWidget::connect(sb, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int i) { m_win->m_sets->m_runtime_lag = i; });
    hbox->addWidget(sb);
    hbox->insertStretch(-1, 0);

    vbox->addWidget(gb);
  }
  {
    QComboBox *cb = new QComboBox;
    for (int i = 0; i < array_countof(my_roles); i++)
      cb->addItem(my_roles[i]);
    cb->setCurrentIndex(GFS_025_IDX);

    vbox->addWidget(cb);

    QWidget::connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [&](int i) {
      m_fcst_ui.m_fcst0->setEnabled(i == GFS_025_IDX);
      m_fcst_ui.m_fcst1->setEnabled(i == GFS_025_IDX);
      m_fcst_ui.m_step->setEnabled(i == GFS_025_IDX);

      m_tag_tree.tree_add(true, false, true, GFS_025, nullptr, true);
      m_dwnl_all->setChecked(true);
      m_tag_tree.expand_all(0);
    });
  }
  vbox->addWidget(m_tag_tree.m_view);
  m_tag_tree.expand_all(0);

  m_dwnl_all = new QCheckBox(IDSTR_ALL_CODES_AND_LEVELS);
  QWidget::connect(m_dwnl_all, static_cast<void(QCheckBox::*)(int)>(&QCheckBox::stateChanged), [&](int state) {
    m_tag_tree.check_all(state == Qt::Checked);
  });
  m_dwnl_all->setChecked(true);
  vbox->addWidget(m_dwnl_all);

  QDialogButtonBox *bbox = new QDialogButtonBox(QDialogButtonBox::Open | QDialogButtonBox::Cancel);
  bbox->button(QDialogButtonBox::Open)->setText(IDSTR_DOWNLOAD);

  vbox->addWidget(bbox);
  connect(bbox->button(QDialogButtonBox::Open), SIGNAL(clicked()), this, SLOT(download()));
  connect(bbox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
}

void dlg_datetime_c::download()
{
  data_tmpl_t tmpl;
  grib_ini_c *grib_ini = new grib_ini_c(m_win, &m_dbl_date_ui, &m_fcst_ui, &m_tag_tree, tmpl);

  if (!grib_ini || !tmpl.size()) {
    m_win->message_box(QMessageBox::Warning, IDSTR_SELECT_DATA_PLANES IDSTR_OR_SELECT " \"" IDSTR_ALL_CODES_AND_LEVELS "\"", QMessageBox::Ok);
    delete grib_ini;
    QDialog::close();
    return;
  }
  hide();
  grib_ini->show_prog_dlg(m_win, tmpl.size());

  grib_ini->download(m_reg_ui.m_reg_lat->value(), m_reg_ui.m_reg_lon->value(), m_reg_ui.m_reg_height->value(), m_reg_ui.m_reg_width->value(),
    m_dwnl_all->checkState() == Qt::CheckState::Checked, tmpl);

  if (!grib_ini->dwnl_szsum()) {
    delete grib_ini;
    m_win->set_msg(IDSTR_OP_FAILED, true, false);
    QDialog::close();
    return;
  }
  grib_ini->prog_dlg_delete();

  if (m_win->message_box(QMessageBox::Question, IDSTR_DATA_LOADED_OPEN_Q, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
    grib_ini->show_prog_dlg(m_win, tmpl.size());
    if (grib_ini->open_data(tmpl)) {
      data_tmpl_t otmpl;
      if (m_win->m_data->data_tmpl(otmpl))
        m_win->m_sets->save_ini(SETS_GROUP_GRIB_STARTUP | SETS_GROUP_DATE_TIME, &otmpl);

      m_win->m_tag_tree.tree_from_gribs(m_win->m_data->gribs());
      m_win->m_tag_tree.expand_all(0);
      m_win->set_cur(true, true);

      m_win->grib_done(true);
    }
    else
      m_win->set_msg(IDSTR_OP_FAILED, true, false);
  }
  delete grib_ini;

  m_reg_ui.quick_map_cleanup();

  QDialog::close();
}

void dlg_datetime_c::reject()
{
  m_reg_ui.quick_map_cleanup();
  m_win->m_dlg_datetime = nullptr;

  QDialog::reject();
}
