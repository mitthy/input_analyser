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

#include "mathutils.h"
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

input_data_t box_muller_transform(input_data_t mean, input_data_t standard_deviation) {
    //Implementation of a normal random number generator using the Box-Muller transform. 
    //Link: https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
    static bool generate = true;
    static input_data_t z0, z1;
    //If generate is false, it means we calculated both z0 and z1 on a previous iteration, so we just use z1.
    if(!generate) {
        generate = true;
        return z1 * standard_deviation + mean;
    }
    //It is guaranteed that dist(mt) will never be 0, since its limit is set to the minimum positive float point + 1.
    input_data_t random1 = dist(mt);
    input_data_t random2 = dist(mt);
    z0 = sqrt(static_cast<input_data_t>(-2.0) * log(random1)) * cos(2 * M_PI * random2);
    z1 = sqrt(static_cast<input_data_t>(-2.0) * log(random1)) * sin(2 * M_PI * random2);
    generate = false;
    return z0 * standard_deviation + mean;
}
