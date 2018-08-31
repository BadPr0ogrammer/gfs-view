/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

class win_c;

class graphics_view_c : public QGraphicsView
{
  Q_OBJECT

    double                  m_zoom;
public:
  QPointF                 m_scene_pos;
  QPoint                  m_mouse_pos;
  win_c                   *m_win;

  graphics_view_c(QGraphicsScene *scene, win_c *win, QWidget *parent = 0) :
    QGraphicsView(scene, parent), m_zoom(1.0015), m_win(win), m_scene_pos(0, 0), m_mouse_pos(0, 0) {}
  void zooming(int angle);

private:
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
};

