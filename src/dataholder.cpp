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

#include "dataholder.h"

using namespace std;

ostream& operator<<(ostream& os, const DataHolder& dh) {
    for(float value: dh._data) {
        os << value << " ";
    }
    return os;
}
    
istream& operator>>(istream& is, DataHolder& dh) {
    input_data_t value = 0;
    
    std::string parsed;
    size_t next_position;
    is >> parsed;
    try {
        value = stod(parsed, &next_position);
        if(next_position != parsed.size()) {
            is.setstate(ios::failbit);
            return is;
        }
        std::size_t n = dh._data.size();
        input_data_t new_size = static_cast<input_data_t>(n + 1);
        dh._current_mean = dh._current_mean * (n / new_size) + value / new_size;
        dh._current_max = std::max(dh._current_max, value);
        dh._current_min = std::min(dh._current_min, value);
        dh._data.push_back(value);
        dh._compute_variance_and_standard_deviation();
    }
            
    catch(exception& e) {
        is.setstate(ios::failbit);
        return is;
    }
    return is;
}

typename DataHolder::iterator DataHolder::begin() {
    return _data.begin();
}
    
typename DataHolder::iterator DataHolder::end() {
    return _data.end();
}

typename DataHolder::const_iterator DataHolder::begin() const {
    return _data.begin();
}

typename DataHolder::const_iterator DataHolder::end() const {
    return _data.end();
}

DataHolder& operator<<(DataHolder& ob, input_data_t dat) {
    std::size_t n = ob._data.size();
    input_data_t new_size = static_cast<input_data_t>(n + 1);
    ob._current_mean = ob._current_mean * (n / new_size) + dat / new_size;
    ob._data.push_back(dat);
    ob._current_max = std::max(ob._current_max, dat);
    ob._current_min = std::min(ob._current_min, dat);
    ob._compute_variance_and_standard_deviation();
    return ob;
}

input_data_t DataHolder::mean() const {
    return _current_mean;
}
    
    
input_data_t DataHolder::variance() const {
    return _current_var;
}
    
    
input_data_t DataHolder::standard_deviation() const {
    return _current_standard_deviation;
}

    
input_data_t DataHolder::max() const {
    return _current_max;
}
    
    
input_data_t DataHolder::min() const {
    return _current_min;
}
    
    
DataHistogram DataHolder::generate_histogram(std::size_t number_classes) const {
    return DataHistogram(_data.begin(), _data.end(), number_classes);
}

void DataHolder::_compute_variance_and_standard_deviation() {
    if(!_data.size()) {
        _current_standard_deviation = std::numeric_limits<input_data_t>::quiet_NaN();
        _current_var = std::numeric_limits<input_data_t>::quiet_NaN();
        return;
    }
    _current_var = 0;
    if(_data.size() > 1) {
        for(auto dat: _data) {
            _current_var -= 2 / static_cast<input_data_t>(_data.size() - 1) * _current_mean * dat;
            _current_var += (dat / static_cast<input_data_t>(_data.size() - 1) * dat);
            _current_var += _current_mean / static_cast<input_data_t>(_data.size() - 1) * _current_mean;
        }
        _current_standard_deviation = std::sqrt(_current_var);
    }
    
}
