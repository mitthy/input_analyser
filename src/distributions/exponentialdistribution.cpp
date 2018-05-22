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

#include "distributions/exponentialdistribution.h"
#include <cmath>
#include <random>
#include <limits>

using namespace std;

//These variables are made global to this file to preserve state (not sure if necessary).
//Also they're implemented in an unamed namespace so they can't be accessed from outside.
namespace {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<input_data_t> dist(nextafter(numeric_limits<input_data_t>::min(), static_cast<input_data_t>(1.0)), 1);
}

input_data_t ExponentialDistribution::generate_value() const {
    //Rand is guaranteed to be greater than 0
    input_data_t rand = dist(mt);
    return -1 * log(rand) / _lambda;
}

input_data_t ExponentialDistribution::frequency_for(input_data_t value) const {
    if(value < 0) {
        return 0;
    }
    return _lambda * pow(M_E, -1 * _lambda * value);
}


