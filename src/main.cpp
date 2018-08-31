/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <stdio.h>

#include <QtWidgets>
#include <QApplication>
#include <QtQuick>

#include <opencv2/core/core.hpp>

#include "defs.h"
#include "color_tables.h"
#include "datefcst.h"
#include "shaperead.h"
#include "griblatlon.h"
#include "cntrs.h"
#include "mgrb2.h"
#include "gribitem.h"
#include "infodata.h"
#include "sets.h"
#include "gribini.h"
#include "winc.h"
#include "local_str.h"

#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
  setlocale(LC_NUMERIC, "C");

  QApplication *app = nullptr;
  QQmlApplicationEngine *engine = nullptr;
  {
#ifdef _WINDOWS
    QString wdir = QDir::currentPath();
    wdir.chop(3);
#else
    QString wdir = getenv("QTDIR");
#endif
    QCoreApplication::addLibraryPath(wdir + "plugins");

    app = new QApplication(argc, argv);
    if (!app) {
      printf("%s\n", "0xdd981ec1");
      return 0;
    }
    app->setStyle("fusion");
    engine = new QQmlApplicationEngine;
    if (!engine) {
      printf("%s\n", "0x3c396810");
      return 0;
    }
    engine->addImportPath(wdir + "qml");
    engine->addImportPath(wdir + "plugins");
    engine->addPluginPath(wdir + "plugins");
  }
  win_c *win = new win_c(engine);
  if (!win) {
    printf("%s\n", "0x8164bc1a");
    return 0;
  }

  if (!win->m_sets || !win->m_data) {
    my_log("err #%s\n", "0x33a269e");
    delete app;
    delete win;
    return 0;
  }
  win->show();
  win->raise();
  win->activateWindow();
  win->fit_desktop();

  if (win->m_term_ok ||
    win->message_box(QMessageBox::Warning, IDSTR_PROGRAM_TERMINATED, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
    data_tmpl_t tmpl;
    grib_ini_c *grib_ini = win->m_sets->read_ini(tmpl);
    if (grib_ini && tmpl.size()) {
      grib_ini->show_prog_dlg(win, tmpl.size());
      bool ret = grib_ini->open_data(tmpl);

      if (ret) {
        win->m_tag_tree.tree_from_gribs(win->m_data->gribs());
        win->m_tag_tree.expand_all(0);
        win->set_cur(true, true);

        win->grib_done(true);
        win->m_sets->read_cntrs();
      }
      else
        win->set_msg(IDSTR_OP_FAILED, false, true);
    }
    delete grib_ini;
  }
  win->move_ramppal();
  if (win->m_sets->m_wind_compose)
    win->m_cmd[CMD_COMPOSE_WIND]->setChecked(true);
  if (win->m_sets->m_tempre_compose)
    win->m_cmd[CMD_COMPOSE_TEMPRE]->setChecked(true);
  if (win->m_sets->m_geopt_compose)
    win->m_cmd[CMD_COMPOSE_GEOPT]->setChecked(true);
  if (win->m_sets->m_precip_compose)
    win->m_cmd[CMD_COMPOSE_PRECIP]->setChecked(true);
  if (win->m_sets->m_geogrid) {
    win->m_cmd[CMD_GEOGRID]->setChecked(true);
    win->m_sets->m_geogrid = false;
    win->draw_geogr_clicked();
  }

  app->exec();

  delete win;
  delete app;
  return 0;
}
