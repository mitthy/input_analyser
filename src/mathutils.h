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


#ifndef MATH_UTILS_H
#define MATH_UTILS_H
#include <type_traits>
#include <algorithm>
#include <iostream>
#include "inputtypes.h"
#include <cmath>

/**
 * Applies Box Muller Transform algorithm to calculate a normal distribution.
 * @param mean The mean of the desired normal distribution random value.
 * @param standard_deviation The standard deviation of the desired normal distribution random value.
 * @return A random value from the normal distribution with mean = 0, standard deviation = 1 transformed to mean, standard_deviation
 */
input_data_t box_muller_transform(input_data_t mean, input_data_t standard_deviation);


//Since this function is templated, we got to implement it in the header.

/**
 * Function that calculates the integral in the interval [first, last] given a function fx.
 * @param first First number of the interval.
 * @param last Last number of the interval.
 * @param fx The function that we'd like to integrate.
 * @return The integral of the function fx. If NumberType is a floating point number, it calculates in small steps. Else, it calculates on discrete steps.
 */
template<typename Function>
input_data_t integral(input_data_t first, input_data_t last, Function fx) {
    //If the interval is really small, we just compute and return the first.
    if(std::abs(first - last) <= std::nextafter(std::numeric_limits<input_data_t>::min() * 3, (input_data_t)1.0)) {
        return fx(first);
    }
    input_data_t max = std::max(first, last);
    input_data_t step = std::min(std::abs(first - last) / static_cast<input_data_t>(100), 0.05);
    const input_data_t EPSLON = std::nextafter(std::numeric_limits<input_data_t>::min() * 10, (input_data_t)1.0);
    step = step < EPSLON ? EPSLON : step;
    input_data_t _first = first, _second = first + step;
    input_data_t sum = 0;
    int iterations = (max - _second) / step;
    //Uses the sum of areas of trapezia to calculate the integral of fx.
    //(y0 + y1)/2 * (x1 - x0) + (y1 + y2)/2 * (x2 - x1)...
    //Since (x(n) - x(n-1)) delta x is always equal to step, we don't need to calculate it.
    #pragma omp parallel for reduction (+:sum)
    for(int i = 0; i < iterations; ++i) {
        //This is necessary for OpenMP support.
        input_data_t x_first = _first + (i * step);
        input_data_t x_second = _second + (i * step);
        input_data_t y_first = fx(x_first), y_second = fx(x_second);
        sum += static_cast<input_data_t>(y_first + y_second) / 2.0f * step;
    }
    return sum;
}

#endif
