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

#include "distributions/lognormaldistribution.h"
#include <cmath>
#include "mathutils.h"

using namespace std;

input_data_t LogNormalDistribution::generate_value() const {
    return pow(M_E, box_muller_transform(_mean, _standard_deviation));
}

input_data_t LogNormalDistribution::frequency_for(input_data_t value) const {
    if(value <= 0) {
        return 0;
    }
    value = log(value);
    input_data_t expo = -1 * pow((value - _mean), 2) /  (2 * pow(_standard_deviation, 2));
    input_data_t pot = pow(M_E, expo);
    const input_data_t sqr_2pi = 2.50662827463;
    return 1 / (_standard_deviation * sqr_2pi) * pot;
}




