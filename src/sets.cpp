/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <stdio.h>
#include <wchar.h>
#ifdef DEBUG
#include <sstream>
#endif

#include <opencv2/core/core.hpp>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <shapefil.h>

#include "defs.h"
#include "local_str.h"
#include "color_tables.h"
#include "staticstr.h"
#include "datefcst.h"
#include "places.h"
#include "shaperead.h"
#include "griblatlon.h"
#include "cntrs.h"
#include "infodata.h"
#include "infocntrs.h"
#include "sets.h"
#include "mgrb2.h"
#include "winc.h"
#include "gribini.h"

void sets_c::read_proxy(QSettings& settings)
{
  m_proxy = settings.value(PROXY_ENABLED_KEY, false).toBool();
  m_proxy_host = settings.value(PROXY_HOST_KEY).toString();
  m_proxy_port = settings.value(PROXY_PORT_KEY, 65535).toInt();
  m_proxy_user = settings.value(PROXY_USER_KEY).toString();
  m_proxy_passwd = settings.value(PROXY_PASSWD_KEY).toString();
}

bool sets_c::save_ini(int group_idx, const data_tmpl_t *p_tmpl)
{
  bool ret = true;
  info_data_c *data = m_win->m_data;

  if (m_ini_path.isEmpty())
    m_ini_path = m_win->m_persfold + DIR_SEPARATOR_S "config" DIR_SEPARATOR_S PROGRAM_NAME ".ini";

  QSettings settings(m_ini_path, QSettings::IniFormat);
  if (settings.status() != QSettings::NoError) {
    my_log("err #%s\n", "0x66703dc7");
    return false;
  }
  if (group_idx & SETS_GROUP_INTERFACE) {
    settings.beginGroup(IDSTR_INTERFACE);
    settings.remove("");
    settings.endGroup();
    settings.beginGroup(IDSTR_INTERFACE);

    settings.setValue(CHARSET_KEY, m_charset);

    settings.setValue(PROXY_ENABLED_KEY, m_proxy);
    settings.setValue(PROXY_HOST_KEY, m_proxy_host);
    settings.setValue(PROXY_PORT_KEY, m_proxy_port);
    settings.setValue(PROXY_USER_KEY, m_proxy_user);
    settings.setValue(PROXY_PASSWD_KEY, m_proxy_passwd);

    settings.setValue(ISO_FONT_FAMILY_KEY, m_iso_font_family);
    settings.setValue(ISO_FONT_SIZE_KEY, m_iso_font_size);
    settings.setValue(ISO_FONT_WEIGHT_KEY, m_iso_font_weight);
    settings.setValue(ISO_STEP_KEY, m_iso_step);
    settings.setValue(ISO_COLOR_KEY, m_iso_color.name());
    settings.setValue(ISO_WIDTH_KEY, m_iso_width);
    settings.setValue(ISO_LAB_BGERASE_KEY, m_iso_lab_bgerase);
    settings.setValue(ISO_SMOOTH_KEY, m_iso_smooth);
    settings.setValue(ISO_SMOOTH_VAL_KEY, m_iso_smooth_val);
    settings.setValue(ISO_LABEL_DENSITY_KEY, m_iso_label_density);

    settings.setValue(WIND_COMPOSE_KEY, m_wind_compose);
    settings.setValue(WIND_ARROW_SIZE_KEY, m_wind_arrow_size);
    settings.setValue(WIND_ARROW_WIDTH_KEY, m_wind_arrow_width);
    settings.setValue(WIND_LVAL_KEY, m_wind_lval);
    settings.setValue(WIND_COLOR_KEY, m_wind_color.name());

    settings.setValue(TEMPRE_COMPOSE_KEY, m_tempre_compose);
    settings.setValue(TEMPRE_COLOR_KEY, m_tempre_color.name());
    settings.setValue(TEMPRE_LVAL_KEY, m_tempre_lval);

    settings.setValue(GEOPT_COMPOSE_KEY, m_geopt_compose);
    settings.setValue(GEOPT_COLOR_KEY, m_geopt_color.name());
    settings.setValue(GEOPT_LVAL_KEY, m_geopt_lval);

    settings.setValue(PRECIP_COMPOSE_KEY, m_precip_compose);
    settings.setValue(PRECIP_COLOR_KEY, m_precip_color.name());
    settings.setValue(PRECIP_TAG_KEY, m_precip_tag);

    settings.setValue(UNITS_TEMPRE_KEY, m_units_temp);
    settings.setValue(UNITS_GEOPT_KEY, m_units_geopt);
    settings.setValue(UNITS_SPEED_KEY, m_units_speed);
    settings.setValue(UNITS_PRECIP_KEY, m_units_precip);
    settings.setValue(UNITS_PRERATE_KEY, m_units_prerate);
    settings.setValue(UNITS_PRESS_KEY, m_units_press);

    settings.setValue(GEOGRID_KEY, m_geogrid);
    settings.setValue(GEOGRID_COLOR_KEY, m_geogrid_color.name());
    settings.setValue(GEOGRID_WIDTH_KEY, m_geogrid_width);
    settings.setValue(GEOGRID_LABEL_KEY, m_geogrid_label);

    settings.setValue(PLAY_TIMEOUT_KEY, m_play_timeout);
    settings.setValue(RUNTIME_LAG_KEY, m_runtime_lag);

    if (m_pal_idx.size()) {
      settings.beginWriteArray(PAL_IDX_KEY);
      int i = 0;
      for (auto it = m_pal_idx.begin(); it != m_pal_idx.end(); it++) {
        const string& role = (*it).first;
        for (auto it0 = (*it).second.begin(); it0 != (*it).second.end(); it0++) {
          const string& tag = (*it0).first;
          int idx = (*it0).second;
          if (!tag.empty() && idx > 0 && idx < NCL_PAL_NUM) { //tag_to_level_exist(tag) && 
            settings.setArrayIndex(i++);
            settings.setValue("role", role.c_str());
            settings.setValue("tag", tag.c_str());
            settings.setValue("value", idx);
          }
        }
      }
      settings.endArray();
    }

    settings.endGroup();
  }

  if (group_idx & SETS_GROUP_GRIB_STARTUP) {
    settings.beginGroup(IDSTR_STARTUP_FILES);
    settings.remove("");
    settings.endGroup();
    settings.beginGroup(IDSTR_STARTUP_FILES);

    if (!data->gdata_empty() && p_tmpl && p_tmpl->size()) {
      QMap<QString, QVariant> vmap;
      for (auto it = p_tmpl->begin(); it != p_tmpl->end(); it++) {
        QString slot = (*it).first.c_str();
        QMap<QString, QVariant> vmap0;
        for (auto it0 = (*it).second.begin(); it0 != (*it).second.end(); it0++) {
          QString role = (*it0).first.c_str();
          QMap<QString, QVariant> vmap1;
          for (auto it1 = (*it0).second.begin(); it1 != (*it0).second.end(); it1++) {
            QString tag = (*it1).first.c_str();
            QMap<QString, QVariant> vmap2;
            for (auto it2 = (*it1).second.begin(); it2 != (*it1).second.end(); it2++) {
              QString lev = (*it2).first.c_str();
              QList<QString> vec;
              for (auto it3 = (*it2).second.begin(); it3 != (*it2).second.end(); it3++) {
                QString lval = (*it3).c_str();
                vec.push_back(lval);
              }
              vmap2[lev] = QVariant(vec);
            }
            vmap1[tag] = QVariant(vmap2);
          }
          vmap0[role] = QVariant(vmap1);
        }
        vmap[slot] = QVariant(vmap0);
      }
      settings.setValue(DATA_TMPL_KEY, vmap);
    }
    settings.endGroup();
  }
  if (group_idx & SETS_GROUP_CNTRS_STARTUP) {
    settings.beginGroup(IDSTR_CONTOURS);
    settings.remove("");
    settings.endGroup();
    settings.beginGroup(IDSTR_CONTOURS);

    if (data->vec()) {
      settings.beginWriteArray(CONT_FILE_KEY);
      int i = 0;
      for (auto it = data->vec()->begin(); it != data->vec()->end(); it++) {
        info_cntrs_c *info = *it;
        settings.setArrayIndex(i++);

        settings.setValue(CNTRS_NAME_KEY, info->name());
        settings.setValue(CNTRS_DBF_PLS_PARAM_KEY, info->dbf_plparam());
        settings.setValue(CNTRS_LINE_WIDTH_KEY, QString::number(info->line_w()));
        settings.setValue(CNTRS_LINE_DASH_KEY, QString::number(info->dash()));
        settings.setValue(CNTRS_FONT_NAME_KEY, info->font_face());
        settings.setValue(CNTRS_FONT_SIZE_KEY, info->font_size());
        settings.setValue(CNTRS_TEXT_COLOR_KEY, info->text_c().name());
        settings.setValue(CNTRS_LINE_COLOR_KEY, info->line_c().name());
      }
      settings.endArray();
    }
    settings.endGroup();
  }
  if (group_idx & SETS_GROUP_DATE_TIME) {
    settings.beginGroup(IDSTR_DATE_TIME_DWNL);
    settings.remove("");
    settings.endGroup();
    settings.beginGroup(IDSTR_DATE_TIME_DWNL);

    settings.setValue(FCST_FROM_KEY, m_fcst_from);
    settings.setValue(FCST_TO_KEY, m_fcst_to);
    settings.setValue(FCST_STEP_KEY, m_fcst_step);

    settings.setValue(REGION_CITY_KEY, m_place_idx > -1 ? QString::fromWCharArray((const wchar_t*)ne10m_places[m_place_idx].city) : "");

    settings.endGroup();
  }
  settings.sync();
  return ret;
}

grib_ini_c* sets_c::read_ini(data_tmpl_t& tmpl)
{
  grib_ini_c *grib_ini = new grib_ini_c(m_win);
  if (m_ini_path.isEmpty())
    m_ini_path = m_win->m_persfold + DIR_SEPARATOR_S "config" DIR_SEPARATOR_S PROGRAM_NAME ".ini";

  QFileInfo finfo(m_ini_path);
  if (!finfo.exists()) {
    QString s = m_win->m_persfold + DIR_SEPARATOR_S "config";
    QDir dir(s);
    if (!dir.exists())
      dir.mkpath(s);

    QFile file(m_ini_path);
    file.open(QIODevice::WriteOnly | QFile::Text);
    file.close();

    my_log("err #%s\n", "0xf2d83968");
    delete grib_ini;
    return nullptr;
  }

  QSettings settings(m_ini_path, QSettings::IniFormat);
  if (settings.status() != QSettings::NoError) {
    my_log("err #%s\n", "0x4756d653");
    delete grib_ini;
    return nullptr;
  }
  {
    settings.beginGroup(IDSTR_INTERFACE);

    m_charset = settings.value(CHARSET_KEY, 0).toInt();

    read_proxy(settings);

    m_iso_font_family = settings.value(ISO_FONT_FAMILY_KEY, QFont().defaultFamily()).toString();
    m_iso_font_size = settings.value(ISO_FONT_SIZE_KEY, DEF_FONT_SIZE).toInt();
    m_iso_font_size = MAX(1, MIN(28, m_iso_font_size));
    m_iso_font_weight = settings.value(ISO_FONT_WEIGHT_KEY, QFont::Normal).toInt();
    m_iso_step = settings.value(ISO_STEP_KEY, DEF_ISO_STEP).toInt();
    m_iso_step = MAX(1, MIN(15, m_iso_step));
    m_iso_color = QColor(settings.value(ISO_COLOR_KEY).toString());
    m_iso_width = settings.value(ISO_WIDTH_KEY, DEF_LINE_WIDTH).toDouble();
    m_iso_width = MAX(0.1, MIN(30, m_iso_width));
    m_iso_lab_bgerase = settings.value(ISO_LAB_BGERASE_KEY, false).toBool();
    m_iso_smooth = settings.value(ISO_SMOOTH_KEY, true).toBool();
    m_iso_smooth_val = settings.value(ISO_SMOOTH_VAL_KEY, DEF_ISO_SMOOTH_VAL).toInt();
    m_iso_smooth_val = MAX(3, MIN(300, m_iso_smooth_val));
    m_iso_label_density = settings.value(ISO_LABEL_DENSITY_KEY, DEF_ISO_LABEL_DENSITY).toInt();
    m_iso_label_density = MAX(1, MIN(1000, m_iso_label_density));

    m_wind_compose = settings.value(WIND_COMPOSE_KEY, false).toBool();
    m_wind_arrow_size = settings.value(WIND_ARROW_SIZE_KEY, DEF_WIND_ARROW_SIZE).toDouble();
    m_wind_arrow_size = MAX(0.1, MIN(10, m_wind_arrow_size));
    m_wind_arrow_width = settings.value(WIND_ARROW_WIDTH_KEY, DEF_WIND_ARROW_WIDTH).toDouble();
    m_wind_arrow_width = MAX(1, MIN(10, m_wind_arrow_width));

    m_wind_lval = settings.value(WIND_LVAL_KEY, "1000").toString();
    m_wind_color = QColor(settings.value(WIND_COLOR_KEY).toString());

    m_tempre_compose = settings.value(TEMPRE_COMPOSE_KEY, false).toBool();
    m_tempre_color = QColor(settings.value(TEMPRE_COLOR_KEY).toString());
    m_tempre_lval = settings.value(TEMPRE_LVAL_KEY, "1000").toString();

    m_geopt_compose = settings.value(GEOPT_COMPOSE_KEY, false).toBool();
    m_geopt_color = QColor(settings.value(GEOPT_COLOR_KEY).toString());
    m_geopt_lval = settings.value(GEOPT_LVAL_KEY, "1000").toString();

    m_precip_compose = settings.value(PRECIP_COMPOSE_KEY, false).toBool();
    m_precip_color = QColor(settings.value(PRECIP_COLOR_KEY).toString());
    m_precip_tag = settings.value(PRECIP_TAG_KEY, "CWAT").toString();

    m_units_temp = settings.value(UNITS_TEMPRE_KEY, 1).toInt();
    m_units_geopt = settings.value(UNITS_GEOPT_KEY, 1).toInt();
    m_units_speed = settings.value(UNITS_SPEED_KEY, 0).toInt();
    m_units_precip = settings.value(UNITS_PRECIP_KEY, 0).toInt();
    m_units_prerate = settings.value(UNITS_PRERATE_KEY, 1).toInt();
    m_units_press = settings.value(UNITS_PRESS_KEY, 1).toInt();

    m_geogrid = settings.value(GEOGRID_KEY, false).toBool();
    m_geogrid_color = QColor(settings.value(GEOGRID_COLOR_KEY, QColor(Qt::gray)).toString());
    m_geogrid_width = settings.value(GEOGRID_WIDTH_KEY, DEF_GEOGRID_WIDTH).toDouble();
    m_geogrid_width = MAX(1, MIN(10, m_geogrid_width));
    m_geogrid_label = settings.value(GEOGRID_LABEL_KEY, true).toBool();

    m_play_timeout = settings.value(PLAY_TIMEOUT_KEY, DEF_PLAY_TIMEOUT).toInt();
    m_play_timeout = MAX(100, MIN(10000, m_play_timeout));

    m_runtime_lag = settings.value(RUNTIME_LAG_KEY, DEF_RUNTIME_LAG).toInt();
    m_runtime_lag = MAX(0, MIN(60, m_runtime_lag));

    int pal_len = settings.beginReadArray(PAL_IDX_KEY);
    pal_len = MIN(pal_len, NCL_PAL_NUM);
    for (int i = 0; i < pal_len; i++) {
      settings.setArrayIndex(i);
      QString role = settings.value("role").toString();
      QString tag = settings.value("tag").toString();
      int j = settings.value("value").toInt();
      j = MIN(j, NCL_PAL_NUM - 1);
      m_pal_idx[role.toStdString()][tag.toStdString()] = j;
    }
    settings.endArray();

    settings.endGroup();
  }

  settings.beginGroup(IDSTR_PRO_USER);
  settings.endGroup();

  settings.beginGroup(IDSTR_STARTUP_FILES);
  {
    QMap<QString, QVariant> vmap = settings.value(DATA_TMPL_KEY).toMap();
    for (auto it = vmap.begin(); it != vmap.end(); it++) {
      string slot = it.key().toStdString();
      QMap<QString, QVariant> vmap0 = it.value().toMap();
      for (auto it0 = vmap0.begin(); it0 != vmap0.end(); it0++) {
        string role = it0.key().toStdString();
        QMap<QString, QVariant> vmap1 = it0.value().toMap();
        for (auto it1 = vmap1.begin(); it1 != vmap1.end(); it1++) {
          string tag = it1.key().toStdString();
          QMap<QString, QVariant> vmap2 = it1.value().toMap();
          for (auto it2 = vmap2.begin(); it2 != vmap2.end(); it2++) {
            string lev = it2.key().toStdString();
            QStringList vec = it2.value().toStringList();
            for (auto it3 = vec.begin(); it3 != vec.end(); it3++) {
              string lval = (*it3).toStdString();
              if (tag2level_idx(role, tag, lev, lval) != -1)
                tmpl[slot][role][tag][lev].push_back(lval);
            }
          }
        }
      }
    }
    settings.endGroup();
    {
#ifdef DEBUG
      if (!tmpl.empty()) {
        std::ostringstream oss;
        set<string> roles;
        for (auto it = tmpl.begin(); it != tmpl.end(); it++) {
          string r = (*it).second.begin()->first;
          r += "(";
          r += (*it).second.begin()->second.begin()->first;
          r += ")";
          roles.insert(r.c_str());
        }
        for (auto it = roles.begin(); it != roles.end(); it++)
          oss << (*it).c_str() << " ";
        my_log("info #%s\n", oss.str().c_str());
      }
#endif
    }
  }
  {
    settings.beginGroup(IDSTR_DATE_TIME_DWNL);

    m_fcst_from = settings.value(FCST_FROM_KEY, 0).toInt();
    m_fcst_to = settings.value(FCST_TO_KEY, 0).toInt();
    m_fcst_step = settings.value(FCST_STEP_KEY, 1).toInt();
    {
      QString name = settings.value(REGION_CITY_KEY, "").toString();
      if (!name.isEmpty()) {
        const name_ll_t *p = ne10m_places;
        wchar_t buf[LARGEBUF];
        name.toWCharArray(buf);
        buf[name.length()] = 0;
        int i;
        for (i = 0; i < NE10M_PLACES_NUM; i++, p++)
          if (!wcscmp(p->city, buf))
            break;
        if (i < NE10M_PLACES_NUM)
          m_place_idx = i;
        else {
          my_log("err #%s\n", "0x30aa042f");
        }
      }
    }
    settings.endGroup();
  }
  settings.sync();

  return grib_ini;
}

bool sets_c::read_cntrs()
{
  if (m_ini_path.isEmpty())
    m_ini_path = m_win->m_persfold + DIR_SEPARATOR_S "config" DIR_SEPARATOR_S PROGRAM_NAME ".ini";

  QSettings settings(m_ini_path, QSettings::IniFormat);
  if (settings.status() != QSettings::NoError) {
    my_log("err #%s\n", "0x653dfaf4");
    return false;
  }
  {
    settings.beginGroup(IDSTR_CONTOURS);
    {
      int file_num = settings.beginReadArray(CONT_FILE_KEY);
      for (int i = 0; i < file_num; i++) {
        settings.setArrayIndex(i);

        QString name = settings.value(CNTRS_NAME_KEY).toString();
        if (name.isEmpty()) {
          my_log("err #%s\n", "0x8995c743");
          continue;
        }

        QString fname = m_win->m_persfold + DIR_SEPARATOR_S MY_SHP_IMPORT DIR_SEPARATOR_S + name;
        QFileInfo finfo(fname);
        if (!finfo.exists()) {
          my_log("err #%s\n", "0x64f6f430");
          continue;
        }
        info_cntrs_c *info = new info_cntrs_c(m_win);
        info->set_name(name);
        QString dbf_plparam = settings.value(CNTRS_DBF_PLS_PARAM_KEY, DEF_DBF_PLS_PARAM).toString();
        info->set_dbf_plparam(dbf_plparam.isEmpty() ? DEF_DBF_PLS_PARAM : dbf_plparam);
        info->set_dbf_plref(info->dbf_plparam_ptr().splitRef(";", QString::SkipEmptyParts));
        float w = settings.value(CNTRS_LINE_WIDTH_KEY, 1).toFloat();
        info->set_line_w(MAX(0.01, MIN(30, w)));
        int dash = settings.value(CNTRS_LINE_DASH_KEY).toInt();
        info->set_dash(MAX(0, MIN(DASH_LINE_NAME_NUM - 1, dash)));
        info->set_font_face(settings.value(CNTRS_FONT_NAME_KEY).toString());
        int sz = settings.value(CNTRS_FONT_SIZE_KEY, DEF_FONT_SIZE).toInt();
        info->set_font_size(MAX(2, MIN(30, sz)));
        info->set_text_c(QColor(settings.value(CNTRS_TEXT_COLOR_KEY).toString()));
        info->set_line_c(QColor(settings.value(CNTRS_LINE_COLOR_KEY).toString()));

        if (!info->make_cntrs()) {
          my_log("err #%s\n", "0x18b00079");
          continue;
        }
      }
      settings.endArray();
    }
    settings.endGroup();
  }
  settings.sync();
  return true;
}
