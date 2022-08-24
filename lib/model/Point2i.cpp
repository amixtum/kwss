#include "../../include/model/Point2i.h"

Point2i::Point2i(int x, int y)
{
  this->x = x;
  this->y = y;
}

Point2i&
Point2i::operator=(Point2i& other)
{
  x = other.x;
  y = other.y;
  return *this;
}
