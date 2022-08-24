#include "../../include/model/Point2i.h"

#include <cmath>

Point2i::Point2i(int x, int y)
{
  this->x = x;
  this->y = y;
}

Point2i::Point2i(const Point2i& other)
{
  this->x = other.x;
  this->y = other.y;
}

float
Point2i::length() const
{
  return std::sqrt((x * x) + (y * y));
}

Point2i&
Point2i::operator=(const Point2i& other)
{
  x = other.x;
  y = other.y;
  return *this;
}

Point2i
Point2i::operator+(const Point2i& rhs) const
{
  return Point2i(x + rhs.x, y + rhs.y);
}

Point2i
Point2i::operator-(const Point2i& rhs) const
{
  return Point2i(x - rhs.x, y - rhs.y);
}

Point2i
Point2i::operator*(const double& d) const
{
  return Point2i(x * d, y * d);
}

Point2i&
Point2i::operator+=(const Point2i& rhs)
{
  x += rhs.x;
  y += rhs.y;
  return *this;
}

Point2i&
Point2i::operator-=(const Point2i& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}

Point2i&
Point2i::operator*=(const double& d)
{
  x *= d;
  y *= d;
  return *this;
}

bool
Point2i::operator==(const Point2i& rhs) const
{
  return x == rhs.x && y == rhs.y;
}

bool
Point2i::operator<(const Point2i& rhs) const
{
  return (x * y) + x < (rhs.x * rhs.y) + rhs.x;
}
