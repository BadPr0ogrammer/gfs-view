/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <opencv2/core/core.hpp>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "defs.h"
#include "local_str.h"
#include "staticstr.h"
#include "regui.h"
#include "datefcst.h"
#include "griblatlon.h"
#include "gribitem.h"
#include "gribini.h"
#include "infodata.h"
#include "fcstui.h"
#include "sets.h"
#include "winc.h"
#include "mgrb2.h"
#include "tvdelegate.h"

#include "grib_strings.h"

tag_tree_ui_c::tag_tree_ui_c(win_c *pwin, bool sig) : m_win(pwin)
{
  m_model = new QStandardItemModel;
  const QStringList list = { IDSTR_GRIB_LEVEL_PLANES };
  m_model->setHorizontalHeaderLabels(list);

  m_view = new QTreeView;
  m_view->setAlternatingRowColors(true);
  m_view->header()->setSectionsMovable(false);
  m_view->setSelectionMode(QAbstractItemView::SingleSelection);
  m_view->setModel(m_model);
  m_view->setItemDelegate(new treeview_delegate_c(m_win, m_view));

  if (sig)
    connect(m_view->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(tree_sel_changed(const QModelIndex &, const QModelIndex &)));
}

QModelIndex tag_tree_ui_c::tree_get_sel(const rtlv_rf_c& params)
{
  if (!m_model)
    return QModelIndex();
  for (int n = 0; n < m_model->rowCount(); n++) {
    QModelIndex idx = m_model->index(n, 0, QModelIndex());
    if (!idx.isValid() || !idx.data(Qt::UserRole + 1).isValid())
      continue;

    const rtlv_rf_c obj = idx.data(Qt::UserRole + 1).value<rtlv_rf_c>();
    if (obj.m_role == params.m_role) {
      if (params.m_tag.empty())
        return idx;

      for (int i = 0; i < m_model->rowCount(idx); i++) {
        QModelIndex idx0 = m_model->index(i, 0, idx);
        if (!idx0.isValid() || !idx0.data(Qt::UserRole + 1).isValid())
          continue;

        const rtlv_rf_c obj0 = idx0.data(Qt::UserRole + 1).value<rtlv_rf_c>();
        if (obj0.m_tag == params.m_tag) {
          if (params.m_lev.empty())
            return idx0;

          for (int j = 0; j < m_model->rowCount(idx0); j++) {
            QModelIndex idx1 = m_model->index(j, 0, idx0);
            if (!idx1.isValid() || !idx1.data(Qt::UserRole + 1).isValid())
              continue;

            const rtlv_rf_c obj1 = idx1.data(Qt::UserRole + 1).value<rtlv_rf_c>();
            if (obj1.m_lev == params.m_lev) {
              if (params.m_lval.empty())
                return idx1;

              for (int k = 0; k < m_model->rowCount(idx1); k++) {
                QModelIndex idx2 = m_model->index(k, 0, idx1);
                if (!idx2.isValid() || !idx2.data(Qt::UserRole + 1).isValid())
                  continue;
                const rtlv_rf_c obj2 = idx2.data(Qt::UserRole + 1).value<rtlv_rf_c>();
                if (obj2.m_lval == params.m_lval) {
                  if (params.m_run.empty())
                    return idx2;

                  for (int m = 0; m < m_model->rowCount(idx2); m++) {
                    QModelIndex idx3 = m_model->index(m, 0, idx2);
                    if (!idx3.isValid() || !idx3.data(Qt::UserRole + 1).isValid())
                      continue;
                    const rtlv_rf_c obj3 = idx3.data(Qt::UserRole + 1).value<rtlv_rf_c>();
                    if (obj3.m_run == params.m_run) {
                      if (params.m_fc.empty())
                        return idx3;

                      for (int n = 0; n < m_model->rowCount(idx3); n++) {
                        QModelIndex idx4 = m_model->index(n, 0, idx3);
                        if (!idx4.isValid() || !idx4.data(Qt::UserRole + 1).isValid())
                          continue;
                        const rtlv_rf_c obj4 = idx4.data(Qt::UserRole + 1).value<rtlv_rf_c>();
                        if (obj4.m_fc == params.m_fc)
                          return idx4;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return QModelIndex();
}

void tag_tree_ui_c::tree_sel_changed(const QModelIndex &current, const QModelIndex &)
{
  if (!m_model || !current.isValid() || !current.data(Qt::UserRole + 1).isValid())
    return;
  const rtlv_rf_c& obj = current.data(Qt::UserRole + 1).value<rtlv_rf_c>();
  if (obj.is_empty()) {
#ifdef DEBUG
    my_log("err #%s\n", "0x1eb54b5e");
#endif
    return;
  }
  grib_c *cgrib = m_win->m_data->cur_grib_ptr();
  const datefc_c& cdatefc = cgrib->datefc();
  const rtlv_c& cpar = m_win->m_data->cur_param();
  rtlv_rf_c cobj(cpar, cdatefc.run_str(), cdatefc.fc_str());
  if (obj == cobj)
    return;

  grib_c *grib = m_win->m_data->cur_grib_ptr();
  if (!obj.m_run.empty() && !obj.m_fc.empty() && (obj.m_run != cdatefc.run_str() || obj.m_fc != cdatefc.fc_str())) {
    if (!(grib = m_win->m_data->get_grib(obj.m_run, obj.m_fc)) || !grib->grib_data(obj)) {
      m_win->set_msg(IDSTR_NO_DATA_B, true, true);
      my_log("err #%s\n", "0x418a02bf");
      return;
    }
  }

  if (!m_win->update_canv(tag_grib_c(obj, grib))) {
    m_win->set_msg(IDSTR_NO_DATA_B, true, true);
#ifdef DEBUG
    my_log("err #%s\n", "0xe12d493");
#endif
  }
  m_win->set_cur(true, true);
}

void tag_tree_ui_c::expand_all(int level)
{
  for (int n = 0; n < m_model->rowCount(); n++) {
    QModelIndex idx = m_model->index(n, 0, QModelIndex());
    if (!m_view->isExpanded(idx))
      m_view->expand(idx);

    if (level > 0) {
      for (int i = 0; i < m_model->rowCount(idx); i++) {
        QModelIndex idx0 = m_model->index(i, 0, idx);
        if (!m_view->isExpanded(idx0))
          m_view->expand(idx0);

        if (level > 1) {
          for (int j = 0; j < m_model->rowCount(idx0); j++) {
            QModelIndex idx1 = m_model->index(j, 0, idx0);
            if (!m_view->isExpanded(idx1))
              m_view->expand(idx1);
          }
        }
      }
    }
  }
}

bool tag_tree_ui_c::append(QStandardItem **p_item, QStandardItem *parent, const rtlv_rf_c& obj, const char *name, bool sel)
{
  bool f = false;
  QStandardItem *item;
  QModelIndex test = tree_get_sel(obj);
  if (test.isValid())
    item = m_model->itemFromIndex(test);
  else {
    item = new QStandardItem(name);
    item->setEditable(false);
    item->setSelectable(sel);
    item->setData(QVariant::fromValue<rtlv_rf_c>(obj));
    parent->appendRow(item);
    f = true;
  }
  *p_item = item;
  return f;
}

void tag_tree_ui_c::tree_from_gribs(const list<grib_c*>* grib_list)
{
  tree_done();
  QStandardItem *root = m_model->invisibleRootItem();

  for (auto slot = grib_list->begin(); slot != grib_list->end(); slot++) {
    const data_map_t& planes = (*slot)->data_map();
    const datefc_c&  datefc = (*slot)->datefc();
    string run_str = datefc.run_str();
    string fc_str = datefc.fc_str();

    for (auto it = planes.begin(); it != planes.end(); it++) {
      const string& role = (*it).first;

      QStandardItem *item = nullptr;
      {
        rtlv_rf_c obj(role, string(), string(), string(), string(), string());
        append(&item, root, obj, role.c_str(), false);
      }
      for (auto it0 = (*it).second.begin(); it0 != (*it).second.end(); it0++) {
        const string& tag = (*it0).first;
        int idx = tag2level_idx(role, tag);
        if (idx == -1) {
#ifdef DEBUG
          my_log("err #%s\n", "0x5f19d1f6");
#endif
          continue;
        }

        const char *short_name = role == string(GFS_025) ? grib2_all[gfs_tag2level[idx].idx].short_name : "";
        QStandardItem *item0 = nullptr;
        {
          rtlv_rf_c obj0(role, tag, string(), string(), string(), string());
          append(&item0, item, obj0, short_name, false);
        }

        for (auto it1 = (*it0).second.begin(); it1 != (*it0).second.end(); it1++) {
          const string& level = (*it1).first;
          if (tag2level_idx(role, tag, level) == -1)
            continue;

          QStandardItem *item1 = nullptr;
          {
            rtlv_rf_c obj1(role, tag, level, string(), string(), string());
            append(&item1, item0, obj1, grib_level_list[grib_level_idx(level)].name, false);
          }

          for (auto it2 = (*it1).second.begin(); it2 != (*it1).second.end(); it2++) {
            const string& lval = (*it2).first;
            if (tag2level_idx(role, tag, level, lval) == -1)
              continue;

            QStandardItem *item2 = nullptr;
            {
              rtlv_rf_c obj2(role, tag, level, lval, string(), string());
              append(&item2, item1, obj2, lval.c_str(), true);
            }
            QStandardItem *item3 = nullptr;
            {
              rtlv_rf_c obj3(role, tag, level, lval, run_str, string());
              append(&item3, item2, obj3, run_str.c_str(), false);
            }
            QStandardItem *item4 = nullptr;
            {
              rtlv_rf_c obj4(role, tag, level, lval, run_str, fc_str);
              append(&item4, item3, obj4, fc_str.c_str(), true);
            }
          }
        }
      }
    }
  }
}

bool tag_tree_ui_c::tree_add(bool check, bool abbr, bool clear, const string& role, const data_tmpl_t* tmpl, bool force_check)
{
  if (clear)
    tree_done();
  QStandardItem *item0;
  {
    QStandardItem *root = m_model->invisibleRootItem();
    item0 = new QStandardItem(role.c_str());
    item0->setEditable(false);
    item0->setSelectable(false);
    root->appendRow(item0);

    rtlv_c obj(role.c_str(), string(), string(), string());
    item0->setData(QVariant::fromValue<rtlv_c>(obj));
  }
  if (role == string(GFS_025)) {
    string tag;
    QStandardItem *item = nullptr;
    for (int i = 0; i < TAG_TO_LEVEL_NUM; i++) {
      if (tag != gfs_tag2level[i].tag) {
        tag = gfs_tag2level[i].tag;
        int idx = gfs_tag2level_idx(tag);
        const char *descr = grib2_all[gfs_tag2level[idx].idx].short_name;
        item = new QStandardItem(abbr ? gfs_tag2level[i].tag : descr);
        item->setEditable(false);
        item->setSelectable(false);
        if (abbr)
          item->setToolTip(descr);

        rtlv_c obj(role, tag, string(), string());
        item->setData(QVariant::fromValue<rtlv_c>(obj));

        item0->appendRow(item);
      }
      int k = grib_level_idx(gfs_tag2level[i].lev);
      QStandardItem *item1 = new QStandardItem(abbr ? gfs_tag2level[i].lev : grib_level_list[k].name);

      item1->setEditable(false);
      item1->setSelectable(false);
      if (abbr)
        item1->setToolTip(grib_level_list[k].name);
      {
        rtlv_c obj(role, tag, gfs_tag2level[i].lev, string());
        item1->setData(QVariant::fromValue<rtlv_c>(obj));
      }
      item->appendRow(item1);

      for (int j = 0; j < TAG_TO_LEVEL_SIZE; j++) {
        if (!gfs_tag2level[i].lval[j])
          break;

        QStandardItem *item2 = new QStandardItem(gfs_tag2level[i].lval[j]);
        item2->setEditable(false);
        item2->setSelectable(true);
        item2->setCheckable(check);

        rtlv_c obj1(role, tag, gfs_tag2level[i].lev, gfs_tag2level[i].lval[j]);
        item2->setData(QVariant::fromValue<rtlv_c>(obj1));

        if (tmpl && data_tmpl_exists(*tmpl, obj1) || force_check)
          item2->setCheckState(Qt::Checked);

        item1->appendRow(item2);
      }
    }
  }
  return true;
}

void tag_tree_ui_c::check_all(bool f)
{
  for (int n = 0; n < m_model->rowCount(); n++) {
    QModelIndex idx = m_model->index(n, 0, QModelIndex());
    for (int i = 0; i < m_model->rowCount(idx); i++) {
      QModelIndex idx0 = m_model->index(i, 0, idx);
      for (int j = 0; j < m_model->rowCount(idx0); j++) {
        QModelIndex idx1 = m_model->index(j, 0, idx0);
        for (int k = 0; k < m_model->rowCount(idx1); k++) {
          QModelIndex idx2 = m_model->index(k, 0, idx1);
          m_model->setData(idx2, f ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
        }
      }
    }
  }
}

bool tag_tree_ui_c::tree_map(slot_tmpl_t &slot_tmpl)
{
  slot_tmpl.clear();

  for (int n = 0; n < m_model->rowCount(); n++) {
    QModelIndex idx = m_model->index(n, 0, QModelIndex());
    if (!idx.isValid()) {
      my_log("err #%s\n", "0xe4013ebd");
      return false;
    }
    for (int i = 0; i < m_model->rowCount(idx); i++) {
      QModelIndex idx0 = m_model->index(i, 0, idx);
      if (!idx0.isValid()) {
        my_log("err #%s\n", "0xb5454f4e");
        return false;
      }
      for (int j = 0; j < m_model->rowCount(idx0); j++) {
        QModelIndex idx1 = m_model->index(j, 0, idx0);
        if (!idx1.isValid()) {
          my_log("err #%s\n", "0xe8bd3f6b");
          return false;
        }
        {
          vector<string> vlval;
          string role, tag, lev;
          for (int k = 0; k < m_model->rowCount(idx1); k++) {
            QModelIndex idx2 = m_model->index(k, 0, idx1);
            if (!idx2.isValid()) {
              my_log("err #%s\n", "0x8f0a7b51");
              return false;
            }
            if (idx2.data(Qt::CheckStateRole).toInt() == Qt::Checked) {
              if (!idx2.data(Qt::UserRole + 1).isValid()) {
                my_log("err #%s\n", "0xc10a6099");
                return false;
              }
              rtlv_c obj = idx2.data(Qt::UserRole + 1).value<rtlv_c>();
              if (obj.is_empty()) {
                my_log("err #%s\n", "0x237663ad");
                return false;
              }
              vlval.push_back(obj.m_lval);
              role = obj.m_role;
              tag = obj.m_tag;
              lev = obj.m_lev;
            }
          }
          if (vlval.size()) {
            slot_tmpl[role][tag][lev] = vlval;
          }
        }
      }
    }
  }
  return slot_tmpl.size() > 0;
}
