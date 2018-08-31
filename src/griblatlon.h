/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

class dlatlon_c
{
public:
  double  m_delta;
  double  m_lat_ll;
  double  m_lon_ll;

  dlatlon_c() : m_delta(0), m_lat_ll(0), m_lon_ll(0)
  {}

  double east(double nx) const { return m_lon_ll + nx * m_delta; };
  double north(double ny) const { return m_lat_ll + ny * m_delta; };
  double width(double nx) const { return nx * m_delta; };
  double height(double ny) const { return ny * m_delta; };
};
