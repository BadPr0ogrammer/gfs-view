

// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*




////////////////////////////////////////////////////////////////////////


#ifndef  __STEREOGRAPHIC_GRID_H__
#define  __STEREOGRAPHIC_GRID_H__


////////////////////////////////////////////////////////////////////////


#include "st_grid_defs.h"
#include "grid_base.h"


////////////////////////////////////////////////////////////////////////


class StereographicGrid : public GridRep {

      friend class Grid;

   private:

      StereographicGrid();
     ~StereographicGrid();
      StereographicGrid(const StereographicData &);

         //
         //
         //

      void clear();

      void xy_to_uv(double x, double y, double &u, double &v) const;
      void uv_to_xy(double u, double v, double &x, double &y) const;

      double uv_closedpolyline_area(const double * u, const double * v, int n) const;

      double xy_closedpolyline_area(const double * x, const double * y, int n) const;

      double f(double) const;

      double df(double) const;

      bool IsNorthHemisphere;

      double Lon_orient;

      double Bx;
      double By;

      double Alpha;

      int Nx;
      int Ny;

      ConcatString Name;

      StereographicData Data;

         //
         //  grid interface
         //

      void latlon_to_xy(double lat, double lon, double & x, double & y) const;

      void xy_to_latlon(double x, double y, double & lat, double & lon) const;

      double calc_area(int x, int y) const;

      int nx() const;
      int ny() const;

      ConcatString name() const;

      /* //???? void dump(ostream &, int = 0) const;

      ConcatString serialize() const;*/

      GridInfo info() const;

      double rot_grid_to_earth(int x, int y) const;

      bool is_global() const;

      void shift_right(int);

      GridRep * copy() const;

};


////////////////////////////////////////////////////////////////////////


#endif   //  __STEREOGRAPHIC_GRID_H__


////////////////////////////////////////////////////////////////////////



