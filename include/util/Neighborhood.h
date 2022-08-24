#ifndef NEIGHBORHOOD_H_
#define NEIGHBORHOOD_H_

#include <vector>

#include "../model/Point2i.h"

enum Neighborhood
{
  Moore,
  Neumann
};

std::vector<Point2i>
neighbors(Neighborhood n,
          Point2i center,
          int leftBound,
          int lowBound,
          int rightBound,
          int upBound);

#endif // Neighborhood.h included
