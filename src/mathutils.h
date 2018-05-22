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

float box_muller_transform(float mean, float standard_deviation);

namespace {
    template<typename Function, typename NumberType>
    float integral(NumberType first, NumberType last, Function fx, std::true_type is_floating) {
        NumberType max = std::max(first, last);
        NumberType min = std::min(first, last);
        NumberType step = static_cast<NumberType>(0.05);
        NumberType _first = first, _second = first + step;
        float sum = 0;
        int iterations = (max - _second) / step;
        for(int i = 0; i < iterations; ++i) {
            float y_first = fx(_first), y_second = fx(_second);
            sum += (y_first + y_second) / 2 * step;
            _first += step;
            _second += step;
        }
        return sum;
    }
    
    template<typename Function, typename NumberType>
    float integral(NumberType first, NumberType last, Function fx, std::false_type is_floating) {
        NumberType max = std::max(first, last);
        NumberType min = std::min(first, last);
        NumberType step = static_cast<NumberType>(1);
        NumberType _first = first, _second = first + step;
        int iterations = (max - _second) / step;
        float sum = 0;
        for(int i = 0; i < iterations; ++i) {
            float y_first = fx(_first), y_second = fx(_second);
            sum += (y_first + y_second) / 2;
            _first += step;
            _second += step;
        }
        return sum;
    }
}


template<typename Function, typename NumberType>
float integral(NumberType first, NumberType last, Function fx) {
    return integral(first, last, fx, typename std::is_floating_point<NumberType>::type());
}

#endif
