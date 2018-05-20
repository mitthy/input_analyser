add_definitions(-std=c++11)

add_library(distributions betadistribution.cpp betadistribution.h distribution.cpp distribution.h exponentialdistribution.cpp exponentialdistribution.h lognormaldistribution.cpp lognormaldistribution.h normaldistribution.cpp normaldistribution.h poissondistribution.cpp poissondistribution.h triangulardistribution.cpp triangulardistribution.h uniformdistribution.cpp uniformdistribution.h)
