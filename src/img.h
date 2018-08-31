/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

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
