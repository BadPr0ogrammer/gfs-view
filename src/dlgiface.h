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

class win_c;

class role_tag_c
{
public:
  string  m_role;
  string  m_tag;

  role_tag_c(const role_tag_c& par) : m_role(par.m_role), m_tag(par.m_tag) {}
  role_tag_c(const string& role, const string& tag) : m_role(role), m_tag(tag) {}
  role_tag_c() {}
};
Q_DECLARE_METATYPE(role_tag_c);

class dlg_iface_c : public QDialog
{
  Q_OBJECT
public:
  win_c     *m_win;

  dlg_iface_c(win_c *win);

private slots:
  void accept();
};

class palette_item_delegate_c : public QStyledItemDelegate
{
  Q_OBJECT
public:
  win_c     *m_win;

  palette_item_delegate_c(win_c *win, QObject *parent) : QStyledItemDelegate(parent), m_win(win) {}
  QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
  void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;

private slots:
  void commit_and_close();
};
