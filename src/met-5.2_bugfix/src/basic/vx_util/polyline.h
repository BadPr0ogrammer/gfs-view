// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

///////////////////////////////////////////////////////////////////////////////
//
//   Filename:   polyline.h
//
//   Description:
//
//
//   Mod#   Date       Name           Description
//   ----   ----       ----           -----------
//   000    11-03-06   Halley Gotway
//
///////////////////////////////////////////////////////////////////////////////

#ifndef  __DATA2D_UTIL_POLYLINE_H__
#define  __DATA2D_UTIL_POLYLINE_H__

///////////////////////////////////////////////////////////////////////////////

#include "vx_util.h"
#include "vx_math.h"

///////////////////////////////////////////////////////////////////////////////

static const int polyline_alloc_jump = 100;

///////////////////////////////////////////////////////////////////////////////

class Polyline {

   private:

      void assign(const Polyline &);

   public:

      Polyline();
     ~Polyline();
      Polyline(const Polyline &);
      Polyline & operator=(const Polyline &);

      void clear();

      /////////////////////////////////////////////////////////////////////////

      char *name;
      
      double *u;
      double *v;

      int n_points;
      int n_alloc;

      void set_name(const char *);
      
      void add_point(double, double);

      void extend_points(int);

      /////////////////////////////////////////////////////////////////////////
      //
      // Single polyline functions
      //
      /////////////////////////////////////////////////////////////////////////

      int is_closed() const;

      void centroid(double &ubar, double &vbar) const;

      void translate(double du, double dv);

      double angle() const;

      void rotate(double deg);

      void rotate(double deg, double ubar, double vbar);

      double uv_signed_area() const;

      int is_inside(double u_test, double v_test) const;

      int is_polyline_point(double u_test, double v_test) const;

      void bounding_box(Box &) const;

      void sum_first_moments(double &, double &) const;

      void sum_second_moments(double, double, double &, double &, double &) const;
};

///////////////////////////////////////////////////////////////////////////////

extern double point_dist(double x1, double y1, double x2, double y2);

/* _bp_
extern double polyline_dist(const Polyline &p1, const Polyline &p2);

extern double polyline_pw_ls_mean_dist(const Polyline &p1, const Polyline &p2);

extern double polyline_pw_mean_sq_dist(const Polyline &p1, const Polyline &p2);
*/
extern int intersect_linesegment(double x1, double y1, double x2, double y2,
                                 double x3, double y3, double x4, double y4);

extern double min_dist_linesegment(double px, double py,
                                   double qx, double qy,
                                   double x_test, double y_test);

/* _bp_
extern void parse_latlon_poly_str(const char *, Polyline &);
extern void parse_latlon_poly_file(const char *, Polyline &);
extern void parse_xy_poly_str(const char *, Polyline &);
extern void parse_xy_poly_file(const char *, Polyline &);
*/
///////////////////////////////////////////////////////////////////////////////

#endif   //  __DATA2D_UTIL_POLYLINE_H__

///////////////////////////////////////////////////////////////////////////////
