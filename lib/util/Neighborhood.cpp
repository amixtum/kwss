#include "../../include/util/Neighborhood.h"

std::vector<Point2i>
neighbors(Neighborhood n,
          Point2i center,
          int leftBound,
          int lowBound,
          int rightBound,
          int upBound)
{
  std::vector<Point2i> points;

  if (center.x > leftBound) {
    points.push_back(Point2i(center.x - 1, center.y));

    if (n == Neighborhood::Moore) {
      if (center.y > lowBound) {
        points.push_back(Point2i(center.x - 1, center.y - 1));
      }

      if (center.y < upBound) {
        points.push_back(Point2i(center.x - 1, center.y + 1));
      }
    }
  }

  if (center.x < rightBound) {
    points.push_back(Point2i(center.x + 1, center.y));

    if (n == Neighborhood::Moore) {
      if (center.y > lowBound) {
        points.push_back(Point2i(center.x + 1, center.y - 1));
      }

      if (center.y < upBound) {
        points.push_back(Point2i(center.x + 1, center.y + 1));
      }
    }
  }

  if (center.y > lowBound) {
    points.push_back(Point2i(center.x, center.y - 1));
  }

  if (center.y < upBound) {
    points.push_back(Point2i(center.x, center.y + 1));
  }

  return points;
}
