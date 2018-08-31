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
#include "color_tables.h"
#include "datefcst.h"
#include "shaperead.h"
#include "griblatlon.h"
#include "cntrs.h"
#include "mgrb2.h"
#include "graphicsvw.h"
#include "winc.h"
#include "sets.h"
#include "infodata.h"
#include "tvdelegate.h"
#include "gribitem.h"

void treeview_delegate_c::paint(QPainter *painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
  QStyleOptionViewItem option(opt);
  if (m_win->m_tag_tree.m_model && index.isValid() && index.data().isValid() && index.data(Qt::UserRole + 1).isValid()) {
    const rtlv_rf_c obj = index.data(Qt::UserRole + 1).value<rtlv_rf_c>();
    if (m_win->m_data && !((rtlv_c) obj).is_empty() && !m_win->m_data->gdata_empty()) {
      const grib_c *grib = m_win->m_data->cur_grib();
      if (grib && grib->grib_data(obj) && grib->grib_data(obj)->plane()) {
        painter->fillRect(option.rect, qRgba(190, 220, 240, 255));
        QItemDelegate::paint(painter, option, index);
        return;
      }
    }
    QItemDelegate::paint(painter, option, index);
  }
}
