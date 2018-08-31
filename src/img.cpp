/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "img.h"

void my_log(const char *frmt, const char *s = nullptr);

Mat* img_c::pal2rgb(uchar(*pal)[3], short sz, int scale)
{
  uchar *p0 = data, *p1;
  Mat *rgb = new Mat(rows, cols, CV_8UC4, Scalar(0)), *ret = nullptr;
  if (!rgb) {
    my_log("err #%s\n", "0x10421ff9");
    return nullptr;
  }
  int i;
  p1 = rgb->data;

  for (i = 0; i < rows; i++, p0 += step, p1 += rgb->step) {
    uchar *p2 = p0, *p3 = p1;
    int j;
    for (j = 0; j < cols; j++, p2++, p3 += 4) {
      int k = MAX(0, MIN(sz - 1, *p2));
      p3[2] = pal[k][0];//2-0 ???
      p3[1] = pal[k][1];
      p3[0] = pal[k][2];
      p3[3] = 255;
    }
  }
  if (scale != 1) {
    ret = new Mat(scale * rows, scale * cols, CV_8UC4);
    if (!ret) {
      my_log("err #%s\n", "0x8580b926");
      delete rgb;
      return nullptr;
    }
    cv::resize(*rgb, *ret, Size(), 0, 0, CV_INTER_AREA);
    delete rgb;
  }
  else
    ret = rgb;
  return ret;
}

Mat* img_c::gray2rgb(double alpha)
{
  Mat *tmp = new Mat(rows, cols, CV_8UC1);
  if (!tmp) {
    my_log("err #%s\n", "0xb42a5c3d");
    return nullptr;
  }
  if (alpha > FLT_EPSILON)
    convertTo(*tmp, CV_8UC1, alpha);
  else {
    double min0, max0;
    minMaxLoc(*this, &min0, &max0);
    convertTo(*tmp, CV_8UC1, 255. / (max0 - min0), -255. / (max0 - min0) * min0);
  }
  Mat *rgb = new Mat(rows, cols, CV_8UC4);
  if (!rgb) {
    my_log("err #%s\n", "0x63fe1c1");
    delete tmp;
    return nullptr;
  }
  cvtColor(*tmp, *rgb, CV_GRAY2RGBA);
  return rgb;
}

void img_c::check_roi(CvRect *roi)
{
  if (roi->height && roi->width && (roi->height != rows || roi->width != cols)) {
    roi->width = MIN(roi->width, cols - roi->x);
    roi->height = MIN(roi->height, rows - roi->y);
  }
}
