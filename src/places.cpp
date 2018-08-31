/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <QtGui>

#include "defs.h"
#include "places.h"
#include "ne10m_places_sim.h"

QStandardItemModel *places_create(int idx, QModelIndex &sel_idx)
{
  QStandardItemModel *model = new QStandardItemModel;
  name_ll_t *places = ne10m_places;

  QStandardItem *root = model->invisibleRootItem();
  const char *adm0 = nullptr;
  QStandardItem *item = nullptr;
  QList<QStandardItem *> list;

  int j = -1, k = -1;
  for (int i = 0; i < NE10M_PLACES_NUM; i++, places++) {
    if (!adm0 || strcmp(adm0, places->adm0)) {
      if (item) {
        item->appendRows(list);
        root->appendRow(item);
        list.clear();
      }
      adm0 = places->adm0;
      item = new QStandardItem(adm0);
      item->setSelectable(false);
      item->setEditable(false);
      if (i <= idx) {
        j++;
        k = -1;
      }
    }
    QStandardItem *item1 = new QStandardItem(QString::fromWCharArray(places->city));
    item1->setData(QVariant(i));
    list.append(item1);
    if (i <= idx)
      k++;
  }
  item->appendRows(list);
  root->appendRow(item);
  if (idx == NE10M_PLACES_NUM - 1)
    k++;

  sel_idx = model->index(k, 0, model->index(j, 0));
  if (!sel_idx.isValid()) {
#ifdef DEBUG
    my_log("err #%s\n", "0xba7c111f");
#endif
  }
  return model;
}
