// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

////////////////////////////////////////////////////////////////////////

using namespace std;

#include <cstdlib>
#include <iostream>
#include <math.h>
#include <string.h>
#include <unistd.h>

#ifdef _WINDOWS
#include <float.h>
#include <math.h>
#define isnan _isnan
#endif
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
void my_log(const char *frmt, const char *s = NULL);

#include "interp_util.h"
#include "interp_mthd.h"

#include "vx_math.h"
#include "vx_log.h"

////////////////////////////////////////////////////////////////////////
//
// Utility functions for horizontal interpolation on a DataPlane
//
////////////////////////////////////////////////////////////////////////

double interp_min(const DataPlane &dp, int x_ll, int y_ll, int wdth, double t) {
   int x, y, count;
   double v, min_v;

   // Search the neighborhood for the minimum value
   count = 0;
   min_v = 1.0e30;
   for(x=x_ll; x<x_ll+wdth; x++) {
      if(x < 0 || x >= dp.nx()) continue;

      for(y=y_ll; y<y_ll+wdth; y++) {
         if(y < 0 || y >= dp.ny())     continue;
         if(is_bad_data(dp.get(x, y))) continue;

         v = dp.get(x, y);
         if(v < min_v) min_v = v;
         count++;
      } // end for y
   } // end for x

   // Check whether enough valid grid points were found to trust
   // the minimum value computed
   if( (double) count/(wdth*wdth) < t || count == 0) {
      min_v = bad_data_double;
   }

   return(min_v);
}

////////////////////////////////////////////////////////////////////////

double interp_max(const DataPlane &dp, int x_ll, int y_ll, int wdth, double t) {
   int x, y, count;
   double v, max_v;

   // Search the neighborhood for the maximum value
   count = 0;
   max_v = -1.0e30;
   for(x=x_ll; x<x_ll+wdth; x++) {
      if(x < 0 || x >= dp.nx()) continue;

      for(y=y_ll; y<y_ll+wdth; y++) {
         if(y < 0 || y >= dp.ny())     continue;
         if(is_bad_data(dp.get(x, y))) continue;

         v = dp.get(x, y);
         if(v > max_v) max_v = v;
         count++;
      } // end for y
   } // end for x

   // Check whether enough valid grid points were found to trust
   // the maximum value computed
   if( (double) count/(wdth*wdth) < t || count == 0) {
      max_v = bad_data_double;
   }

   return(max_v);
}

////////////////////////////////////////////////////////////////////////

double interp_median(const DataPlane &dp, int x_ll, int y_ll, int wdth, double t) {
   double *data = (double *) 0;
   int x, y, count;
   double v, median_v;

   // Allocate space to store the data points
   data = new double [wdth*wdth];

   // Search the neighborhood for valid data points
   count = 0;
   for(x=x_ll; x<x_ll+wdth; x++) {
      if(x < 0 || x >= dp.nx()) continue;

      for(y=y_ll; y<y_ll+wdth; y++) {
         if(y < 0 || y >= dp.ny())     continue;
         if(is_bad_data(dp.get(x, y))) continue;

         v = dp.get(x, y);
         data[count] = v;
         count++;
      } // end for y
   } // end for x

   // Check whether enough valid grid points were found to compute
   // a median value
   if( (double) count/(wdth*wdth) < t || count == 0) {
      median_v = bad_data_double;
   }
   else {
      sort(data, count);
      median_v = percentile(data, count, 0.50);
   }

   if(data) { delete [] data; data = (double *) 0; }

   return(median_v);
}

////////////////////////////////////////////////////////////////////////

double interp_uw_mean(const DataPlane &dp, int x_ll, int y_ll, int wdth, double t) {
   int x, y, count;
   double v, sum, uw_mean_v;

   // Sum up the valid data in the neighborhood
   count = 0;
   sum = 0.0;
   for(x=x_ll; x<x_ll+wdth; x++) {
      if(x < 0 || x >= dp.nx()) continue;

      for(y=y_ll; y<y_ll+wdth; y++) {
         if(y < 0 || y >= dp.ny())     continue;
         if(is_bad_data(dp.get(x, y))) continue;

         v = dp.get(x, y);
         sum += v;
         count++;
      } // end for y
   } // end for x

   // Check whether enough valid grid points were found to compute
   // a mean value
   if( (double) count/(wdth*wdth) < t || count == 0) {
      uw_mean_v = bad_data_double;
   }
   else {
      uw_mean_v = sum/count;
   }

   return(uw_mean_v);
}

////////////////////////////////////////////////////////////////////////
//
// Compute the distance-weighted mean using Shepards Method
//
////////////////////////////////////////////////////////////////////////

double interp_dw_mean(const DataPlane &dp, int x_ll, int y_ll, int wdth,
                      double obs_x, double obs_y, int i_pow, double t) {
   double *data = (double *) 0;
   double *dist = (double *) 0;
   int i, x, y, count;
   double v, dw_mean_v, w, wght_sum;

   // Allocate space to store the data points
   data = new double [wdth*wdth];
   dist = new double [wdth*wdth];

   // Search the neighborhood for valid data points
   count = 0;
   for(x=x_ll; x<x_ll+wdth; x++) {
      if(x < 0 || x >= dp.nx()) continue;

      for(y=y_ll; y<y_ll+wdth; y++) {
         if(y < 0 || y >= dp.ny())     continue;
         if(is_bad_data(dp.get(x, y))) continue;

         v = dp.get(x, y);
         data[count] = v;
         dist[count] = sqrt(pow((obs_x-x), 2) + pow((obs_y-y), 2));
         count++;
      } // end for y
   } // end for x

   // Check whether enough valid grid points were found to compute
   // a distance weighted mean value
   if( (double) count/(wdth*wdth) < t || count == 0) {
      dw_mean_v = bad_data_double;
   }
   else {

      // Compute the sum of the weights
      for(i=0, wght_sum=0; i<count; i++) {

         // If the distance is very close to zero,
         // break out of the loop
         if(dist[i] <= 0.001) break;

         // Otherwise, compute the sum of the weights
         wght_sum += pow(dist[i], -1*i_pow);
      }

      for(i=0, dw_mean_v=0; i<count; i++) {

         // If the distance is very close to zero, set the dw_mean_v
         // value to the current data value and break out of the loop
         if(dist[i] <= 0.001) {
            dw_mean_v = data[i];
            break;
         }

         w = pow(dist[i], -1*i_pow)/wght_sum;
         dw_mean_v += w*data[i];
      }
   }

   if(data) { delete [] data; data = (double *) 0; }
   if(dist) { delete [] dist; dist = (double *) 0; }

   return(dw_mean_v);
}

////////////////////////////////////////////////////////////////////////
//
// Compute a least-squares fit interpolation
//
////////////////////////////////////////////////////////////////////////

double interp_ls_fit(const DataPlane &dp, int x_ll, int y_ll, int wdth,
                     double obs_x, double obs_y, double t) {
   int i, j, x, y, count;
   const int N  = wdth;
   const int N2 = N*N;
   const double alpha     = (N2*(N2 - 1.0))/12.0;
   const double beta      = 0.5*(N - 1.0);
   const double x_center  = x_ll + beta;
   const double y_center  = y_ll + beta;
   const double u_test    = obs_x - x_center;
   const double v_test    = obs_y - y_center;
   double A, B, C;
   double suz, svz, sz;
   double u, v, z;

   if(N < 2) {
     my_log("err #%s\n", "0x84bbf469");

     return NAN;
   }

   suz = svz = sz = 0.0;

   // Search the neighborhood
   count = 0;
   for(i=0; i<N; i++) {

      u = i - beta;
      x = x_ll + i;

      if(x < 0 || x >= dp.nx()) continue;

      for(j=0; j<N; j++) {

         v = j - beta;
         y = y_ll + j;

         if(y < 0 || y >= dp.ny())     continue;
         if(is_bad_data(dp.get(x, y))) continue;

         z = dp.get(x, y);
         count++;

         suz += u*z;
         svz += v*z;
         sz  += z;
      }
   }

   A = suz/alpha;
   B = svz/alpha;
   C = sz/N2;

   z = A*u_test + B*v_test + C;

   // Check for not enough valid data
   if( (double) count/N2 < t || count == 0) {
      z = bad_data_double;
   }

   return(z);
}

////////////////////////////////////////////////////////////////////////
//
// Compute bilinear interpolation.
//
////////////////////////////////////////////////////////////////////////

double interp_bilin(const DataPlane &dp, double obs_x, double obs_y) {
   int x, y;
   double bilin_v, dx, dy;
   double wtsw, wtse, wtnw, wtne;

   x = nint(floor(obs_x));
   y = nint(floor(obs_y));

   // Compute dx and dy
   dx = obs_x - x;
   dy = obs_y - y;

   // Compute weights for 4 corner points
   wtsw = (1.0-dx) * (1.0-dy);
   wtse = dx * (1.0-dy);
   wtnw = (1.0-dx) * dy;
   wtne = dx * dy;

   // Compute interpolated value
   if(x < 0 || x+1 >= dp.nx() ||
      y < 0 || y+1 >= dp.ny()) {
      bilin_v = bad_data_double;
   }
   else if(is_bad_data(dp.get(x,   y  )) ||
           is_bad_data(dp.get(x+1, y  )) ||
           is_bad_data(dp.get(x,   y+1)) ||
           is_bad_data(dp.get(x+1, y+1))) {
      bilin_v = bad_data_double;
   }
   else {
      bilin_v = wtsw * dp.get(x,   y)   +
                wtse * dp.get(x+1, y)   +
                wtnw * dp.get(x,   y+1) +
                wtne * dp.get(x+1, y+1);
   }

   return(bilin_v);
}

////////////////////////////////////////////////////////////////////////
//
// Determine the lower-left (x, y) based on height and width parity.
//
////////////////////////////////////////////////////////////////////////

void get_xy_ll(double x, double y, int w, int h, int &x_ll, int &y_ll) {

   // Range check
   if(w <= 0 || h <= 0) {
     my_log("err #%s\n", "0xc11787fc");

     return;
   }

   // Define left-most x based on the parity of width
   if(w%2 == 1) x_ll = nint(x) - (w - 1)/2;
   else         x_ll = nint(floor(x) - (w/2 - 1));

   // Define lower-most y based on the parity of height
   if(h%2 == 1) y_ll = nint(y) - (h - 1)/2;
   else         y_ll = nint(floor(y) - (h/2 - 1));

   return;
}

////////////////////////////////////////////////////////////////////////
//
// Utility functions for horizontal and vertical interpolation
//
////////////////////////////////////////////////////////////////////////

double compute_horz_interp(const DataPlane &dp,
                           double obs_x, double obs_y,
                           int mthd, int wdth, double interp_thresh) {
   double v;
   int x_ll, y_ll;

   // Get the lower-left x, y
   get_xy_ll(obs_x, obs_y, wdth, wdth, x_ll, y_ll);

   // Compute the interpolated value for the fields above and below
   switch(mthd) {

      case(InterpMthd_Min):     // Minimum
         v = interp_min(dp, x_ll, y_ll, wdth, interp_thresh);
         break;

      case(InterpMthd_Max):     // Maximum
         v = interp_max(dp, x_ll, y_ll, wdth, interp_thresh);
         break;

      case(InterpMthd_Median):  // Median
         v = interp_median(dp, x_ll, y_ll, wdth, interp_thresh);
         break;

      case(InterpMthd_UW_Mean): // Unweighted Mean
         v = interp_uw_mean(dp, x_ll, y_ll, wdth, interp_thresh);
         break;

      case(InterpMthd_DW_Mean): // Distance-Weighted Mean
         v = interp_dw_mean(dp, x_ll, y_ll, wdth, obs_x, obs_y,
                            dw_mean_pow, interp_thresh);
         break;

      case(InterpMthd_LS_Fit):  // Least-squares fit
         v = interp_ls_fit(dp, x_ll, y_ll, wdth, obs_x, obs_y,
                           interp_thresh);
         break;

      case(InterpMthd_Bilin):   // Bilinear interpolation
         v = interp_bilin(dp, obs_x, obs_y);
         break;

      case(InterpMthd_Nearest): // Nearest Neighbor
         v = interp_uw_mean(dp, x_ll, y_ll, 1, interp_thresh);
         break;

      default:
        my_log("err #%s\n", "0x2be31af5");

        return NAN;
   }

   return(v);
}

////////////////////////////////////////////////////////////////////////
//
// Interpolate lineary in the log of pressure between values "v1" and
// "v2" at pressure levels "prs1" and "prs2" to pressure level "to_prs".
//
////////////////////////////////////////////////////////////////////////

double compute_vert_pinterp(double v1, double prs1,
                            double v2, double prs2,
                            double to_prs) {
   double v_interp;

   if(prs1 <= 0.0 || prs2 <= 0.0 || to_prs <= 0.0) {
     my_log("err #%s\n", "0xea95aaa2");

     return NAN;
   }

   // Check that the to_prs falls between the limits
   if( !(to_prs >= prs1 && to_prs <= prs2) &&
       !(to_prs <= prs1 && to_prs >= prs2) ) {

     my_log("warn #%s\n", "0x10cf6389");
   }

   v_interp = v1 + ((v2-v1)*log(prs1/to_prs)/log(prs1/prs2));

   return(v_interp);
}

////////////////////////////////////////////////////////////////////////
//
// Linearly interpolate between values "v1" and "v2" at height levels
// "lvl1" and "lvl2" to height level "to_lvl".
//
////////////////////////////////////////////////////////////////////////

double compute_vert_zinterp(double v1, double lvl1,
                            double v2, double lvl2,
                            double to_lvl) {
   double d1, d2, v_interp;

   if(lvl1 <= 0.0 || lvl2 <= 0.0 || to_lvl <= 0.0) {
     my_log("err #%s\n", "0x2439749e");

     return NAN;
   }

   // Check that the to_lvl falls between the limits
   if( !(to_lvl >= lvl1 && to_lvl <= lvl2) &&
       !(to_lvl <= lvl1 && to_lvl >= lvl2) ) {

     my_log("warn #%s\n", "0xb14d436d");
   }

   d1 = fabs(lvl1 - to_lvl);
   d2 = fabs(lvl2 - to_lvl);

   // Linearly interpolate betwen lvl_1 and lvl_2
   v_interp = v1*(1.0 - d1/(d1+d2)) + v2*(1.0 - d2/(d1+d2));

   return(v_interp);
}

////////////////////////////////////////////////////////////////////////

DataPlane valid_time_interp(const DataPlane &in1, const DataPlane &in2,
                            const unixtime to_ut, InterpMthd mthd) {
   DataPlane dp, dp1, dp2;
   int x, y;
   bool use_min;
   double v, v1, v2, w1, w2;

   // Store min and max valid times.
   dp1 = (in1.valid() <= in2.valid() ? in1 : in2);
   dp2 = (in1.valid() >  in2.valid() ? in1 : in2);

   // Range check the times
   if(dp1.valid() > to_ut || dp2.valid() < to_ut ||
      dp1.valid() == dp2.valid()) {

     my_log("err #%s\n", "0xbeb0f8ff");

      return DataPlane();
   }

   // Check grid dimensions
   if(dp1.nx() != dp2.nx() || dp1.ny() != dp2.ny()) {

     my_log("err #%s\n", "0x3896438f");

      return DataPlane();
   }

   // Compute interpolation weights
   switch(mthd) {
      case(InterpMthd_Min):     // Minimum
      case(InterpMthd_Max):     // Maximum
         w1 = w2 = bad_data_double;
         break;

      case(InterpMthd_UW_Mean): // Unweighted Mean
         w1 = w2 = 0.5;
         break;

      case(InterpMthd_DW_Mean): // Distance-Weighted Mean
         w1 = (double) (dp2.valid() - to_ut) /
                       (dp2.valid() - dp1.valid());
         w2 = (double) (to_ut - dp1.valid()) /
                       (dp2.valid() - dp1.valid());
         break;

      case(InterpMthd_Nearest): // Nearest Neighbor
         use_min = ((to_ut - dp1.valid()) <=
                    (dp2.valid() - to_ut));
         w1 = (use_min ? 1.0 : 0.0);
         w2 = (use_min ? 0.0 : 1.0);
         break;

      case(InterpMthd_Median):  // Median
      case(InterpMthd_LS_Fit):  // Least-squares fit
      case(InterpMthd_Bilin):   // Bilinear interpolation
      default:

        my_log("err #%s\n", "0x101fe249");

        return DataPlane();
   }

   // Initialize
   dp.clear();
   dp.set_size(dp1.nx(), dp1.ny());
   dp.set_valid(to_ut);

   // Compute interpolated value for each point
   for(x=0; x<dp.nx(); x++) {
      for(y=0; y<dp.ny(); y++) {

         // Get current values
         v1 = dp1.get(x, y);
         v2 = dp2.get(x, y);
         v  = bad_data_double;

         // Check for bad data
         if(!is_bad_data(v1) && !is_bad_data(v2)) {

            // Minimum
                 if(mthd == InterpMthd_Min) v = MIN(v1, v2);
            // Maximum
            else if(mthd == InterpMthd_Max) v = MAX(v1, v2);
            // Apply weights
            else                            v = w1*v1 + w2*v2;
         }

         // Store interpolated value
         dp.set(v, x, y);
      } // end for y
   } // end for x

   return(dp);
}

////////////////////////////////////////////////////////////////////////


void dp_interp_min (const DataPlane & fat, DataPlane & out, int width, double fraction)

{

   //
   //  we expect that the size of the "out" dataplane has already been set
   //

int x, y, x_fat_ll, y_fat_ll;
double v;


for (x=0; x<(out.nx()); ++x)  {

   x_fat_ll = x*width;

   for (y=0; y<(out.ny()); ++y)  {

      y_fat_ll = y*width;

      v = interp_min(fat, x_fat_ll, y_fat_ll, width, fraction);

      out.put(v, x, y);

   }   //  for x

}   //  for x



   //
   //  done
   //

return;

}


////////////////////////////////////////////////////////////////////////


void dp_interp_max (const DataPlane & fat, DataPlane & out, int width, double fraction)

{

   //
   //  we expect that the size of the "out" dataplane has already been set
   //

int x, y, x_fat_ll, y_fat_ll;
double v;


for (x=0; x<(out.nx()); ++x)  {

   x_fat_ll = x*width;

   for (y=0; y<(out.ny()); ++y)  {

      y_fat_ll = y*width;

      v = interp_max(fat, x_fat_ll, y_fat_ll, width, fraction);

      out.put(v, x, y);

   }   //  for x

}   //  for x



   //
   //  done
   //

return;

}


////////////////////////////////////////////////////////////////////////


void dp_interp_uw_mean (const DataPlane & fat, DataPlane & out, int width, double fraction)

{

   //
   //  we expect that the size of the "out" dataplane has already been set
   //

int x, y, x_fat_ll, y_fat_ll;
double v;


for (x=0; x<(out.nx()); ++x)  {

   x_fat_ll = x*width;

   for (y=0; y<(out.ny()); ++y)  {

      y_fat_ll = y*width;

      v = interp_uw_mean(fat, x_fat_ll, y_fat_ll, width, fraction);

      out.put(v, x, y);

   }   //  for x

}   //  for x



   //
   //  done
   //

return;

}


////////////////////////////////////////////////////////////////////////
