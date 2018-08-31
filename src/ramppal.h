/**
Copyright Andrej Barkhatov, RPA Typhoon (Obninsk) (c) 2009 - 2018
*/
#pragma once

using namespace std;

class sets_c;

class ramp_c
{
  double          m_dmin;
  double          m_dmax;
  int             m_num;
  double          m_val0;
  double          m_hpa0;
  double          m_val1;
  double          m_hpa1;
  double          m_step;
  double          m_hpas;
  vector<string>  m_labs;

public:
  ramp_c() : m_num(0), m_step(0), m_hpas(0), m_dmin(FLT_MAX), m_val0(FLT_MAX), m_hpa0(FLT_MAX), m_dmax(-FLT_MAX), m_val1(-FLT_MAX), m_hpa1(-FLT_MAX) {};

  bool            ramp_step(sets_c *sets, double dmin, double dmax, const string& role, const string& tag);
  QPixmap*        make_pal(sets_c *sets, const string& role, const string& tag);

  static double   Pressure(double z);
  static int      tick_step(double dmin, double dmax, int col_tab_sz, double &step, double &dmin0, double &dmax0);
  static int      hpa2gpt_bsearch(float val, bool press);

  const vector<string>& labs() const { return m_labs; }
  vector<string>&       labs_ptr() { return m_labs; }
  void                  set_labs(const vector<string>& labs) { m_labs = labs; }
  double                dmin() const { return m_dmin; }
  double                dmax() const { return m_dmax; }
  int                   num() const { return m_num; }
  double                val0() const { return m_val0; }
  double                hpa0() const { return m_hpa0; }
  double                val1() const { return m_val1; }
  double                hpa1() const { return m_hpa1; }
  double                step() const { return m_step; }
  double                hpas() const { return m_hpas; }
  string                get_lab(int i) { return i >= 0 && i < m_labs.size() ? m_labs[i] : string(); }
};
