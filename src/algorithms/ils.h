#ifndef ILS_H
#define ILS_H

#include <vector>
#include "../heart/airport.h"

class Airport;

std::vector<Voo> ILS(Airport* airport, int maxIter);

#endif
