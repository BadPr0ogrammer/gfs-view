/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

typedef struct name_ll_t {
  const char     *adm0;
  const wchar_t  *city;
  double    lon;
  double    lat;
  int       i;
} name_ll_t;

#define NE10M_PLACES_NUM 7322
extern name_ll_t ne10m_places[NE10M_PLACES_NUM];

QStandardItemModel *places_create(int idx, QModelIndex &sel_idx);
