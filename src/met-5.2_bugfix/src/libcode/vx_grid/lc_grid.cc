// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*




////////////////////////////////////////////////////////////////////////


using namespace std;


#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

#ifdef _WINDOWS
#include <float.h>
#include <math.h>
#define isnan _isnan
#define strcasecmp strcmpi
#endif
void my_log(const char *frmt, const char *s = NULL);

#include "vx_math.h"
#include "vx_util.h"
#include "vx_log.h"
#include "lc_grid.h"


////////////////////////////////////////////////////////////////////////


static double     lc_func(double lat, double Cone);
static double lc_der_func(double lat, double Cone);

static double lc_inv_func(double   r, double Cone);

static void reduce(double &);

static double lambert_segment_area(double u0, double v0, double u1, double v1, double c);

static double lambert_beta(double u0, double delta_u, double v0, double delta_v, double c, double t);


////////////////////////////////////////////////////////////////////////


   //
   //  Code for class LambertGrid
   //


////////////////////////////////////////////////////////////////////////


LambertGrid::LambertGrid()

{

clear();

}


////////////////////////////////////////////////////////////////////////


LambertGrid::~LambertGrid()

{

clear();

}


////////////////////////////////////////////////////////////////////////


void LambertGrid::clear()

{

Lat_LL = 0.0;
Lon_LL = 0.0;

Lon_orient = 0.0;

Alpha = 0.0;

Cone = 0.0;

Bx = 0.0;
By = 0.0;

Nx = 0;
Ny = 0;

Name.clear();

memset(&Data, 0, sizeof(Data));

return;

}


////////////////////////////////////////////////////////////////////////


LambertGrid::LambertGrid(const LambertData & data)

{

clear();



Lat_LL = data.lat_pin;   //  temporarily
Lon_LL = data.lon_pin;   //  temporarily

reduce(Lon_LL);

Lon_orient = data.lon_orient;

reduce(Lon_orient);

Bx = 0.0;
By = 0.0;

Nx = data.nx;
Ny = data.ny;

Name = data.name;

   //
   //  calculate Cone constant
   //

if ( fabs(data.scale_lat_1 - data.scale_lat_2) < 1.0e-5 )  Cone = sind(data.scale_lat_1);
else {

   double t, b;

   t = cosd(data.scale_lat_1)/cosd(data.scale_lat_2);

   b = tand(45.0 - 0.5*(data.scale_lat_1))/tand(45.0 - 0.5*(data.scale_lat_2));

   Cone = log(t)/log(b);

}

   //
   //  calculate Alpha
   //

Alpha = (-1.0/lc_der_func(data.scale_lat_1, Cone))*((data.r_km)/(data.d_km));

   //
   //  Calculate Bx, By
   //

double r0, theta0;

r0 = lc_func(data.lat_pin, Cone);

theta0 = Cone*(Lon_orient - Lon_LL);

Bx = data.x_pin - Alpha*r0*sind(theta0);
By = data.y_pin + Alpha*r0*cosd(theta0);

xy_to_latlon(0.0, 0.0, Lat_LL, Lon_LL);

reduce(Lon_LL);

Data = data;

   //
   //  Done
   //

}


////////////////////////////////////////////////////////////////////////


double LambertGrid::f(double lat) const

{

return ( lc_func(lat, Cone) );

}


////////////////////////////////////////////////////////////////////////


double LambertGrid::df(double lat) const

{

return ( lc_der_func(lat, Cone) );

}


////////////////////////////////////////////////////////////////////////


void LambertGrid::latlon_to_xy(double lat, double lon, double & x, double & y) const

{

double r, theta;


reduce(lon);

r = lc_func(lat, Cone);

theta = Cone*(Lon_orient - lon);

x = Bx + Alpha*r*sind(theta);

y = By - Alpha*r*cosd(theta);

return;

}


////////////////////////////////////////////////////////////////////////


void LambertGrid::xy_to_latlon(double x, double y, double & lat, double & lon) const

{

double r, theta;

x = (x - Bx)/Alpha;
y = (y - By)/Alpha;

r = sqrt( x*x + y*y );

lat = lc_inv_func(r, Cone);

if ( fabs(r) < 1.0e-5 )  theta = 0.0;
else                     theta = atan2d(x, -y);   //  NOT atan2d(y, x);

lon = Lon_orient - theta/Cone;

reduce(lon);

return;

}


////////////////////////////////////////////////////////////////////////


double LambertGrid::calc_area(int x, int y) const

{

double u[4], v[4];
double sum;


// xy_to_uv(x - 0.5, y - 0.5, u[0], v[0]);  //  lower left
// xy_to_uv(x + 0.5, y - 0.5, u[1], v[1]);  //  lower right
// xy_to_uv(x + 0.5, y + 0.5, u[2], v[2]);  //  upper right
// xy_to_uv(x - 0.5, y + 0.5, u[3], v[3]);  //  upper left


xy_to_uv(x      , y      , u[0], v[0]);  //  lower left
xy_to_uv(x + 1.0, y      , u[1], v[1]);  //  lower right
xy_to_uv(x + 1.0, y + 1.0, u[2], v[2]);  //  upper right
xy_to_uv(x      , y + 1.0, u[3], v[3]);  //  upper left


sum = uv_closedpolyline_area(u, v, 4);

sum *= earth_radius_km*earth_radius_km;

return ( sum );

}


////////////////////////////////////////////////////////////////////////


int LambertGrid::nx() const

{

return ( Nx );

}


////////////////////////////////////////////////////////////////////////


int LambertGrid::ny() const

{

return ( Ny );

}


////////////////////////////////////////////////////////////////////////


ConcatString LambertGrid::name() const

{

return ( Name );

}


////////////////////////////////////////////////////////////////////////


double LambertGrid::uv_closedpolyline_area(const double * u, const double * v, int n) const

{

int j, k;
double sum;


sum = 0.0;

for (j=0; j<n; ++j)  {

   k = (j + 1)%n;

   sum += lambert_segment_area(u[j], v[j], u[k], v[k], Cone);

}

sum = fabs(sum);

return ( sum );

}


////////////////////////////////////////////////////////////////////////


double LambertGrid::xy_closedpolyline_area(const double * x, const double *y , int n) const

{

int j;
double sum;
double *u = (double *) 0;
double *v = (double *) 0;

u = new double [n];
v = new double [n];

if ( !u || !v )  {
  my_log("err #%s\n", "0xd5671a05");

  return NAN;
}

for (j=0; j<n; ++j)  {

   xy_to_uv(x[j], y[j], u[j], v[j]);

}

sum = uv_closedpolyline_area(u, v, n);

sum *= earth_radius_km*earth_radius_km;

delete [] u;  u = (double *) 0;
delete [] v;  v = (double *) 0;

return ( sum );

}


////////////////////////////////////////////////////////////////////////


void LambertGrid::uv_to_xy(double u, double v, double & x, double & y) const

{

x = Alpha*v + Bx;

y = -Alpha*u + By;

return;

}


////////////////////////////////////////////////////////////////////////


void LambertGrid::xy_to_uv(double x, double y, double & u, double & v) const

{

u = (x - Bx)/Alpha;

v = (y - By)/(-Alpha);

return;

}


////////////////////////////////////////////////////////////////////////

/* _bp_
void LambertGrid::dump(ostream & out, int depth) const

{

Indent prefix(depth);



out << prefix << "Name       = ";

if ( Name.length() > 0 )  out << '\"' << Name << '\"';
else                      out << "(nul)\n";

out << '\n';

out << prefix << "Projection = Lambert Conformal\n";

out << prefix << "\n";

out << prefix << "Lat_LL     = " << Lat_LL << "\n";
out << prefix << "Lon_LL     = " << Lon_LL << "\n";

out << prefix << "\n";

out << prefix << "Alpha      = " << Alpha << "\n";
out << prefix << "Cone       = " << Cone  << "\n";

out << prefix << "\n";

out << prefix << "Bx         = " << Bx << "\n";
out << prefix << "By         = " << By << "\n";

out << prefix << "\n";

out << prefix << "Nx         = " << Nx << "\n";
out << prefix << "Ny         = " << Ny << "\n";


   //
   //  done
   //

out.flush();

return;

}


////////////////////////////////////////////////////////////////////////


ConcatString LambertGrid::serialize() const

{

ConcatString a;
char junk[256];

a << "Projection: Lambert Conformal";

a << " Nx: " << Nx;
a << " Ny: " << Ny;

sprintf(junk, " Lat_LL: %.3f", Lat_LL);   a << junk;
sprintf(junk, " Lon_LL: %.3f", Lon_LL);   a << junk;

sprintf(junk, " Lon_orient: %.3f", Lon_orient);   a << junk;

sprintf(junk, " Alpha: %.3f", Alpha);   a << junk;

sprintf(junk, " Cone: %.3f", Cone);   a << junk;

sprintf(junk, " Bx: %.4f", Bx);   a << junk;
sprintf(junk, " By: %.4f", By);   a << junk;

   //
   //  done
   //

return ( a );

}
*/

////////////////////////////////////////////////////////////////////////


GridInfo LambertGrid::info() const

{

GridInfo i;

i.set(Data);

return ( i );

}


////////////////////////////////////////////////////////////////////////


double LambertGrid::rot_grid_to_earth(int x, int y) const

{

double lat, lon, angle;
double diff, hemi;


xy_to_latlon((double) x, (double) y, lat, lon);

diff = Lon_orient - lon;

// Figure out if the grid is in the northern or southern hemisphere
// by checking whether the first latitude (p1_deg -> Phi1_radians)
// is greater than zero
// NH -> hemi = 1, SH -> hemi = -1
// if(Phi1_radians < 0.0) hemi = -1.0;
// else                   hemi = 1.0;

   //
   //  assume northern hemisphere
   //

hemi = 1.0;

angle = diff*Cone*hemi;

return ( angle );

}


////////////////////////////////////////////////////////////////////////


bool LambertGrid::is_global() const

{

return ( false );

}


////////////////////////////////////////////////////////////////////////


void LambertGrid::shift_right(int N)

{

if ( N == 0 )  return;

my_log("err #%s\n", "0x2102d1e2");

return;

}


////////////////////////////////////////////////////////////////////////


GridRep * LambertGrid::copy() const

{

LambertGrid * p = new LambertGrid (Data);

return ( p );

}


////////////////////////////////////////////////////////////////////////


   //
   //  Code for misc functions
   //


////////////////////////////////////////////////////////////////////////


double lc_func(double lat, double Cone)

{

double r;

r = tand(45.0 - 0.5*lat);

r = pow(r, Cone);

return ( r );

}


////////////////////////////////////////////////////////////////////////


double lc_inv_func(double r, double Cone)

{

double lat;

lat = 90.0 - 2.0*atand(pow(r, 1.0/Cone));

return ( lat );

}


////////////////////////////////////////////////////////////////////////


double lc_der_func(double lat, double Cone)

{

double a;

a = -(Cone/cosd(lat))*lc_func(lat, Cone);

return ( a );

}


////////////////////////////////////////////////////////////////////////


void reduce(double & angle)

{

angle -= 360.0*floor( (angle/360.0) + 0.5 );

return;

}


////////////////////////////////////////////////////////////////////////


double lambert_segment_area(double u0, double v0, double u1, double v1, double c)

{

int i, j, k, n;
double rom, denom, h, delta_u, delta_v;
double trap, t[15], left, right, test, sum;
const double a = 0.0, b = 1.0;
const double tol = 1.0e-6;

delta_u = u1 - u0;
delta_v = v1 - v0;

i = 0;
n = 2;

h = (b - a)/n;

sum = lambert_beta(u0, delta_u, v0, delta_v, c, a) + lambert_beta(u0, delta_u, v0, delta_v, c, b);

t[0] = trap = (h/2.0)*sum + h*lambert_beta(u0, delta_u, v0, delta_v, c, a + h);

do {

   ++i;

   n *= 2;

   h = (b - a)/n;

   sum = 0.0;

   for (j=1; j<n; j+=2)   sum += lambert_beta(u0, delta_u, v0, delta_v, c, a + j*h);

   trap = 0.5*trap + h*sum;

   left = trap;

   for (k=1; k<=i; ++k)  {

      denom = pow(4.0, (double) k) - 1.0;

      right = left + (left - t[k-1])/denom;

      test = 2.0*(left - t[k-1]);

      t[k-1] = left;

      left = right;

   }

   t[i] = left;

}  while ( (fabs(test) >= tol) && (i <= 14) );

if ( i >= 14 )  {
  my_log("err #%s\n", "0xde6a8b1d");

  return NAN;
}

rom = t[i];

rom *= (2.0/c)*(u0*v1 - u1*v0);

return ( rom );

}


////////////////////////////////////////////////////////////////////////


double lambert_beta(double u0, double delta_u, double v0, double delta_v, double c, double t)

{

double answer;
double u, v, r2, e_top, e_bot;

u = u0 + t*delta_u;
v = v0 + t*delta_v;

r2 = u*u + v*v;

e_bot = 1.0/c;

e_top = e_bot - 1.0;

answer = pow(r2, e_top)/(1.0 + pow(r2, e_bot));

return ( answer );

}


////////////////////////////////////////////////////////////////////////


Grid::Grid(const LambertData & data)

{

init_from_scratch();

set(data);


}


////////////////////////////////////////////////////////////////////////


void Grid::set(const LambertData & data)

{

clear();

rep = new LambertGrid (data);

if ( !rep )  {
  my_log("err #%s\n", "0x7eb22718");
}

}


////////////////////////////////////////////////////////////////////////
