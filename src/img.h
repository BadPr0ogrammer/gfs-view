/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

#include <opencv2/core/types_c.h>

using namespace std;
using namespace cv;

class img_c : public Mat
{
  using Mat::Mat;

public:
  Mat*  pal2rgb(uchar(*pal)[3], short sz, int scale);
  Mat*  gray2rgb(double alpha);
  void  check_roi(CvRect *roi);
};
