#ifndef POINT2I_H_
#define POINT2I_H_

#include <string>

struct Point2i
{
  Point2i(int x, int y);

  Point2i(const Point2i& other);

  float length() const;

  std::string to_string() const;

  Point2i& operator=(const Point2i& other);

  Point2i operator+(const Point2i& rhs) const;

  Point2i operator-(const Point2i& rhs) const;

  Point2i operator*(const double& d) const;

  Point2i& operator+=(const Point2i& rhs);

  Point2i& operator-=(const Point2i& rhs);

  Point2i& operator*=(const double& d);

  bool operator==(const Point2i& rhs) const;

  bool operator!=(const Point2i& rhs) const;

  bool operator<(const Point2i& rhs) const;

  int x = 0;
  int y = 0;
};

#endif // Point2i.h included
