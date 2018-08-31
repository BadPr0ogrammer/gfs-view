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

class treeview_delegate_c : public QItemDelegate
{
  Q_OBJECT
    win_c *m_win;

public:
  treeview_delegate_c(win_c *win, QObject *parent = 0) : QItemDelegate(parent), m_win(win) {}
  virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return QItemDelegate::sizeHint(option, index);
  }
};
