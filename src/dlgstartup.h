/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

#include "winc.h"
#include "gribitem.h"
#include "regui.h"

class info_cntrs_c;

enum {
  CNTRS_FILE_NAME, CNTRS_DBF_PLS_PARAM, CNTRS_TEXT_COLOR, CNTRS_LINE_COLOR, CNTRS_LINE_WIDTH, CNTRS_DASHED, CNTRS_FONT, CNTRS_FONT_SIZE, CNTRS_NUM
};

class cntrs_item_delegate_c : public QStyledItemDelegate
{
  Q_OBJECT
public:
  const list<info_cntrs_c*> *m_vec;

  cntrs_item_delegate_c(const list<info_cntrs_c*> *vec, QObject *parent) : QStyledItemDelegate(parent), m_vec(vec) {}
  QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
  void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;

private slots:
  void commit_and_close();
};

class reg_ui_c;

class dlg_startup_c : public QDialog
{
  Q_OBJECT
public:
  tag_tree_ui_c       m_tag_tree;
  QStandardItemModel  *m_model_cntrs;
  QTableView          *m_view_cntrs;
  win_c               *m_win;

  dlg_startup_c(win_c *win);

  QList<QStandardItem*>     cntrs_add_row(info_cntrs_c *info);

private slots:
  void add_grib_clicked();
  void rem_all_clicked();
  void add_cntrs_clicked();
  void rem_cntrs_clicked();
  void accept();
  void reject();
};

class grib_obj_c
{
public:
  grib_c      *m_grib;

  grib_obj_c(grib_c *grib = nullptr) : m_grib(grib) {}
};
Q_DECLARE_METATYPE(grib_obj_c);
