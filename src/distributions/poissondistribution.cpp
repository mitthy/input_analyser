/*
 * Input analyser for statistical data processing
 * Copyright (C) 2018  Lucas Finger Roman <lfrfinger@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "distributions/poissondistribution.h"
#include <cmath>
#include <iostream>
#include <random>
#include <limits>

using namespace std;

//These variables are made global to this file to preserve state (not sure if necessary).
//Also they're implemented in an unamed namespace so they can't be accessed from outside.
namespace {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<float> dist(nextafter(numeric_limits<float>::min(), 1.0f), 1);
}

float PoissonDistribution::generate_value() const {
    float temp = pow(M_E, -_lambda);
    int k = 0;
    float p = 1;
    do {
        ++k;
        float random = dist(mt);
        p *= random;
    } while(p > temp);
    return k - 1;
}

float PoissonDistribution::frequency_for(float value) const {
    int val = static_cast<long>(value);
    if(val < 0) {
        return 0;
    }
    float temp = pow(M_E, -_lambda) * (pow(_lambda, val));
    while(val) {
        temp /= 1.0f * val--;
    }
    return temp;
}




