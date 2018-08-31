/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <math.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "defs.h"
#include "color_tables.h"
#include "cntrs.h"
#include "griblatlon.h"
#include "img.h"
#include "datefcst.h"
#include "ramppal.h"
#include "infodata.h"
#include "winc.h"
#include "mgrb2.h"
#include "staticstr.h"
#include "sets.h"
#include "hpa2gpt.h"

bool ramp_c::ramp_step(sets_c *sets, double dmin, double dmax, const string& role, const string& tag)
{
  int ipal = MIN(NCL_PAL_NUM - 1, MAX(0, sets->m_pal_idx[role][tag]));
  int col_tab_sz = ncl_pal_ctbl_sz[ipal];

  bool f_hpa = GEOPOTENTIAL_CODES(tag) && sets->m_units_geopt;
  double us = f_hpa ? (sets->m_units_geopt == 2 ? 0.750062 : sets->m_units_geopt == 3 ? 0.0295386 : 1) : 1;

  m_dmax = MAX(m_dmax, dmax);
  m_dmin = MIN(m_dmin, dmin);

  if (!f_hpa)
    m_num = tick_step(m_dmin, m_dmax, col_tab_sz, m_step, m_val0, m_val1);
  else {
    int i0 = hpa2gpt_bsearch(0.01 * Pressure(m_dmin), true);
    int i1 = hpa2gpt_bsearch(0.01 * Pressure(m_dmax), true);

    m_num = tick_step(us * gpt2hpa[i1].hPa, us * gpt2hpa[i0].hPa, col_tab_sz, m_hpas, m_hpa0, m_hpa1);
    i0 = hpa2gpt_bsearch(m_hpa1 / us, true);
    i1 = hpa2gpt_bsearch(m_hpa1 / us - m_hpas / us, true);
    int i2 = hpa2gpt_bsearch(m_hpa0 / us, true);
    if (i0 == i1) {
      my_log("err #%s\n", "0xb30bef0");
      return false;
    }
    m_val0 = gpt2hpa[i0].gpt;
    m_step = gpt2hpa[i1].gpt - m_val0;
    m_val1 = gpt2hpa[i2].gpt;
  }
  return m_num != 0;
}

#define PAL_FACET_NUM 8

QPixmap* ramp_c::make_pal(sets_c *sets, const string& role, const string& tag)
{
  int ipal = MIN(NCL_PAL_NUM - 1, MAX(0, sets->m_pal_idx[role][tag]));
  float(*col_tab)[4] = ncl_pal_ctbl[ipal];
  const char *unit = "";

  int idx = tag2level_idx(role, tag);
  if (idx == -1) {
    my_log("err #%s\n", "0x328da693");
    return nullptr;
  }

  const char *units = TEMPERATURE_CODES(tag) ? TEMPERATURE_LABEL(sets->m_units_temp)
    : GEOPOTENTIAL_CODES(tag) ? GEOPOTENTIAL_LABEL(sets->m_units_geopt)
    : SPEED_CODES(tag) ? SPEED_LABEL(sets->m_units_speed)
    : PRECIPITATION_CODES(tag) ? PRECIPITATION_LABEL(sets->m_units_precip)
    : PRECIP_RATE_CODES(tag) ? PRECIP_RATE_LABEL(sets->m_units_prerate)
    : PRESSURE_CODES(tag) ? PRESSURE_LABEL(sets->m_units_press)
    : unit;

  if (!m_num || !col_tab) {
    my_log("err #%s\n", "0x217322ad");
    return nullptr;
  }

  QPixmap* ret = new QPixmap(PAL_FACET_NUM * PAL_FACET_SZ, PAL_FACET_SZ * m_num);
  QPainter painter(ret);
  const QRect rc(0, 0, PAL_FACET_NUM * PAL_FACET_SZ, PAL_FACET_SZ * m_num);
  painter.fillRect(rc, Qt::black);
  painter.setPen(Qt::white);
  painter.drawRect(rc);

  for (int i = 0, j = m_num - 1, k = m_labs.size() - 1; i < m_num; i++, j--) {
    QColor c(255 * col_tab[j][1], 255 * col_tab[j][2], 255 * col_tab[j][3]);
    painter.setPen(c);
    painter.setBrush(c);
    painter.drawRect(0, i * PAL_FACET_SZ, PAL_FACET_SZ, PAL_FACET_SZ);

    if (!(i % sets->m_iso_step)) {
      if (k < 0) {
        my_log("err #%s\n", "0xff837e64");
        k = 0;
      }
      QString s(m_labs[k--].c_str());
      if (!i) {
        s += " ";
        s += units;
      }
      painter.setPen(Qt::white);
      const QRect rc1(PAL_FACET_SZ, i * PAL_FACET_SZ, (PAL_FACET_NUM - 1) * PAL_FACET_SZ, PAL_FACET_SZ);
      painter.drawText(rc1, Qt::AlignCenter, s);
    }
  }

  painter.setPen(Qt::black);
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(0, 0, PAL_FACET_NUM * PAL_FACET_SZ, m_num * PAL_FACET_SZ);
  return ret;
}

int ramp_c::tick_step(double dmin, double dmax, int col_tab_sz, double &step, double &dmin0, double &dmax0)
{
  int num = 0;
  if (dmax <= dmin) {
    my_log("err #%s\n", "0xb5197ba5");
    return 0;
  }
  double rstep = (dmax - dmin) / col_tab_sz;
  double istep = pow(10., (int)log10(rstep));
  step = ceil(rstep / istep) * istep;

  dmin0 = floor(dmin / step) * step;
  dmax0 = ceil(dmax / step) * step;
  num = MIN((int)((dmax0 - dmin0) / step), col_tab_sz);
  dmax0 = dmin0 + step * num;

  if ((dmax0 - dmin0) / (0.2 * step) <= col_tab_sz)
    step *= 0.2;
  else if ((dmax0 - dmin0) / (0.5 * step) <= col_tab_sz)
    step *= 0.5;

  dmin0 = floor(dmin / step) * step;
  dmax0 = ceil(dmax / step) * step;
  num = MIN((int)((dmax0 - dmin0) / step), col_tab_sz);
  dmax0 = dmin0 + step * num;
  return num;
}

int ramp_c::hpa2gpt_bsearch(float val, bool press)
{
  int first = 0;
  int last = array_countof(gpt2hpa) - 1;
  int i = last / 2;
  if (press) {
    while (first < last - 1) {
      if (val >= gpt2hpa[i].hPa) {
        if (val < gpt2hpa[i + 1].hPa)
          break;
        first = i;
      }
      else
        last = i;
      i = (first + last) / 2;
    }
  }
  else {
    while (first < last - 1) {
      if (val <= gpt2hpa[i].gpt) {
        if (val > gpt2hpa[i + 1].gpt)
          break;
        first = i;
      }
      else
        last = i;
      i = (first + last) / 2;
    }
  }
  return gpt2hpa[i].idx;
}

const double TABLE4[8][4] =
{
  00000 , -0.0065 , 288.150 , 1.01325000000000E+5 ,
  11000 , 0.0000 , 216.650 , 2.26320639734629E+4 ,
  20000 , 0.0010 , 216.650 , 5.47488866967777E+3 ,
  32000 , 0.0028 , 228.650 , 8.68018684755228E+2 ,
  47000 , 0.0000 , 270.650 , 1.10906305554966E+2 ,
  51000 , -0.0028 , 270.650 , 6.69388731186873E+1 ,
  71000 , -0.0020 , 214.650 , 3.95642042804073E+0 ,
  84852 , 0.0000 , 186.946 , 3.73383589976215E-1
};

double ramp_c::Pressure(double z)
{
  double H = z * 6356766 / (z + 6356766);
  int b;
  for (b = 0; b < 7; ++b)
    if (H < TABLE4[b + 1][0])
      break;
  double C = -.0341631947363104;
  double Hb = TABLE4[b][0], Lb = TABLE4[b][1], Tb = TABLE4[b][2], Pb = TABLE4[b][3];
  return Pb * (fabs(Lb) > 1E-12 ? pow(1 + Lb / Tb * (H - Hb), C / Lb) : exp(C*(H - Hb) / Tb));
}
