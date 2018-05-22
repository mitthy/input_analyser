/*
 * Input analyser for statistical data processing
 * Copyright (C) 2018  Lucas Finger Roman lfrfinger@gmail.com
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

#include "distributions/triangulardistribution.h"
#include <random>
#include <cmath>
using namespace std;

//These variables are made global to this file to preserve state (not sure if necessary).
//Also they're implemented in an unamed namespace so they can't be accessed from outside.
namespace {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<float> dist(0, 1);
}

float TriangularDistribution::generate_value() const {
    float temp = (_mode - _min) / (_max - _min);
    float rand = dist(mt);
    if(rand < temp) {
        return _min + sqrt(rand * (_max - _min) * (_mode - _min));
    }
    return _max - sqrt((1 - rand) * (_max - _min) * (_max - _mode));
}

float TriangularDistribution::frequency_for(float value) const {
    if(value > _max || value < _min) {
        return 0;
    }
    if(value < _mode) {
        return 2 * (value - _min) / ((_max - _min) * (_mode - _min)); 
    }
    return 2 * (_max - value) / ((_max - _min) * (_max - _mode));
}


