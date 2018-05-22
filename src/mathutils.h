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

/**
 * Applies Box Muller Transform algorithm to calculate a normal distribution.
 * @param mean The mean of the desired normal distribution random value.
 * @param standard_deviation The standard deviation of the desired normal distribution random value.
 * @return A random value from the normal distribution with mean = 0, standard deviation = 1 transformed to mean, standard_deviation
 */
float box_muller_transform(float mean, float standard_deviation);

namespace {
    
    //Since this function is templated, we got to implement it in the header.
    //We hide it in an anonymous namespace because we don't want to export it.
    
    /**
    * Function that calculates the integral in the interval [first, last] given a function fx.
    * @param first First number of the interval.
    * @param last Last number of the interval.
    * @param is_floating used for static dispatch.
    * @return The intergral of the function fx.
    */
    template<typename Function, typename NumberType>
    float integral(NumberType first, NumberType last, Function fx, const std::true_type&) {
        NumberType max = std::max(first, last);
        NumberType min = std::min(first, last);
        NumberType step = static_cast<NumberType>(0.05);
        NumberType _first = first, _second = first + step;
        float sum = 0;
        int iterations = (max - _second) / step;
        //Uses the sum of areas of trapezia to calculate the integral of fx.
        //(y0 + y1)/2 * (x1 - x0) + (y1 + y2)/2 * (x2 - x1)...
        //Since (x(n) - x(n-1)) delta x is always equal to step, we don't need to calculate it.
        #pragma omp parallel for reduction (+:sum)
        for(int i = 0; i < iterations; ++i) {
            //This is necessary for OpenMP support.
            NumberType x_first = _first + (i * step);
            NumberType x_second = _second + (i * step);
            NumberType y_first = fx(x_first), y_second = fx(x_second);
            sum += static_cast<float>(y_first + y_second) / 2.0f * step;
        }
        return sum;
    }
    
    //Since this function is templated, we got to implement it in the header.
    
    /**
    * Function that calculates the integral in the interval [first, last] given a function fx.
    * @param first First number of the interval.
    * @param last Last number of the interval.
    * @param is_floating used for static dispatch.
    * @return The intergral of the function fx in discrete steps.
    */
    template<typename Function, typename NumberType>
    float integral(NumberType first, NumberType last, Function fx, const std::false_type&) {
        NumberType max = std::max(first, last);
        NumberType min = std::min(first, last);
        NumberType step = static_cast<NumberType>(1);
        NumberType _first = first, _second = _first + step;
        int iterations = (max - _second);
        float sum = 0;
        //Uses the sum of areas of trapezia to calculate the integral of fx.
        //(y0 + y1)/2 * (x1 - x0) + (y1 + y2)/2 * (x2 - x1)...
        //Since (x(n) - x(n-1)) delta x is always equal to step, we don't need to calculate it.
        #pragma omp parallel for reduction (+:sum)
        for(int i = 0; i < iterations; ++i) {
            //This is necessary for OpenMP support.
            NumberType x_first = _first + i;
            NumberType x_second = _second + i;
            NumberType y_first = fx(_first), y_second = fx(_second);
            sum += static_cast<float>(y_first + y_second) / 2.0f;
            _first += step;
            _second += step;
        }
        return sum;
    }
}

//Since this function is templated, we got to implement it in the header.

/**
 * Function that calculates the integral in the interval [first, last] given a function fx.
 * @param first First number of the interval.
 * @param last Last number of the interval.
 * @return The integral of the function fx. If NumberType is a floating point number, it calculates in small steps. Else, it calculates on discrete steps.
 */
template<typename Function, typename NumberType>
float integral(NumberType first, NumberType last, Function fx) {
    //We just static dispatch to the above function.
    return integral(first, last, fx, typename std::is_floating_point<NumberType>::type());
}

#endif
