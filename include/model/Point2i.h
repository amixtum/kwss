#ifndef POINT2I_H_
#define POINT2I_H_

struct Point2i
{
  Point2i(int x, int y);

  Point2i& operator=(Point2i& other);

  int x = 0;
  int y = 0;
};

#endif // Point2i.h included
