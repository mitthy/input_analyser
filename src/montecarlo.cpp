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

#include "montecarlo.h"
#include <random>
#include <algorithm>
#include <limits>

using namespace std;

//These variables are made global to this file to preserve state (not sure if necessary).
//Also they're implemented in an unamed namespace so they can't be accessed from outside.
namespace {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<float> dist(0, 1);
}

float MonteCarlo::generate_value() const {
    if(_organized_data.empty()) {
        return numeric_limits<float>::quiet_NaN();
    }
    size_t low = 0, high = _organized_data.size();
    float random_value = dist(mt);
    while(low != high) {
        size_t mid = (low >> 1) + (high >> 1) + (low & high & 1);
        if(random_value < _organized_data[mid].acum_probability) {
            high = mid;
        }
        else {
            low = mid + 1;
        }
    }
    return _organized_data[low].value;
}


float MonteCarlo::histogram_mean() const {
    if(_organized_data.empty()) {
        return numeric_limits<float>::quiet_NaN();
    }
    float sum = 0;
    for(int i = 0; i < _organized_data.size(); ++i) {
        auto& el = _organized_data[i];
        sum += el.class_count * (el.value / (float)_data_count);
    }
    return sum;
}

float MonteCarlo::histogram_variance() const {
    if(_organized_data.empty()) {
        return numeric_limits<float>::quiet_NaN();
    }
    if(_data_count == 1) {
        return 0;
    }
    float mean = histogram_mean();
    float sum = 0;
    for(int i = 0; i < _organized_data.size(); ++i) {
        auto& el = _organized_data[i];
        float dif_from_mean = el.value - mean;
        sum += el.class_count * ((dif_from_mean * dif_from_mean) / max(1.0f, (float)(_data_count - 1)));
    }
    return sum;
}

float MonteCarlo::histogram_standard_deviation() const {
    float variance = histogram_variance();
    return sqrt(variance);
}


float MonteCarlo::histogram_min_value() const {
    if(_organized_data.empty()) {
        return numeric_limits<float>::quiet_NaN();
    }
    return _organized_data.front().value;
}

float MonteCarlo::histogram_max_value() const {
    if(_organized_data.empty()) {
        return numeric_limits<float>::quiet_NaN();
    } 
    return _organized_data.back().value;
}

float MonteCarlo::histogram_mode() const {
    float return_value = numeric_limits<float>::quiet_NaN();
    auto compare_function = [](const monte_carlo_class& left, const monte_carlo_class& right) {
        return left.class_count < right.class_count;
    };
    auto max_count_pos = max_element(_organized_data.begin(), _organized_data.end(), compare_function);
    if(max_count_pos != _organized_data.end()) {
        return_value = max_count_pos->value;
    }
    return return_value;
}

MonteCarlo::monte_carlo_class_printer MonteCarlo::print_classes() {
    MonteCarlo::monte_carlo_class_printer printer;
    printer.classes = &(this->_organized_data);
    return printer;
}

ostream& operator<<(ostream& os, const MonteCarlo::monte_carlo_class_printer& printer) {
    if(printer.classes->empty()) {
        return os;
    }
    int i = 0;
    for(; i < printer.classes->size() - 1; ++i) {
        os << (*printer.classes)[i] << endl;    
    }
    os << (*printer.classes)[i];
    return os;
}


