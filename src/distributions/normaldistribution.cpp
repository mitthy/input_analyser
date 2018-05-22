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

#include "distributions/normaldistribution.h"
#include <cmath>
#include <random>
#include <limits>

using namespace std;

//These variables are made global to this file to preserve state (not sure if necessary).
//Also they're implemented in an unamed namespace so they can't be accessed from outside.
namespace {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<float> dist(nextafter(numeric_limits<float>::min(), 1.0f), 1);
    
    //We hide this function here since we don't want to export it outside. 
    //Also, we can't make it private to NormalDistribution since we can't change attributes inside it.
    //It would be good if we could reuse the same z0 and z1 to all normal distributions, which leaving this function outside the class also helps.
    float box_muller_transform(float mean, float standard_deviation) {
        //Implementation of a normal random number generator using the Box-Muller transform. 
        //Link: https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
        static bool generate = true;
        static float z0, z1;
        //If generate is false, it means we calculated both z0 and z1 on a previous iteration, so we just use z1.
        if(!generate) {
            generate = true;
            return z1 * standard_deviation + mean;
        }
        //It is guaranteed that dist(mt) will never be 0, since its limit is set to the minimum positive float point + 1.
        float random1 = dist(mt);
        float random2 = dist(mt);
        z0 = sqrt(-2.0f * log(random1)) * cos(2 * M_PI * random2);
        z1 = sqrt(-2.0f * log(random1)) * sin(2 * M_PI * random2);
        generate = false;
        return z0 * standard_deviation + mean;
    }
}

float NormalDistribution::generate_value() const {
    return box_muller_transform(_mean, _standard_deviation);
}

float NormalDistribution::frequency_for(float value) const {
    float expo = -1 * pow((value - _mean), 2) /  (2 * pow(_standard_deviation, 2));
    float pot = pow(M_E, expo);
    const float sqr_2pi = 2.50662827463;
    return 1 / (_standard_deviation * sqr_2pi) * pot;
}



