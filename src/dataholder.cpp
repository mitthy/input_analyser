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
    
//TODO Could be nice if we could read values into it even if there were some strings mixed into the stream.
istream& operator>>(istream& is, DataHolder& dh) {
    input_data_t value = 0;
    if(is >> value) {
        dh._data.push_back(value);
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

DataHolder& operator<<(input_data_t data, DataHolder& ob) {
    //TODO
    return ob;
}

input_data_t DataHolder::mean() const {
    
}
    
    
input_data_t DataHolder::variance() const {
    
}
    
    
input_data_t DataHolder::standard_deviation() const {
    
}

    
input_data_t DataHolder::max() const {
    
}
    
    
input_data_t DataHolder::min() const {
    
}
    
    
DataHistogram DataHolder::generate_histogram(std::size_t number_classes) const {
    
}
