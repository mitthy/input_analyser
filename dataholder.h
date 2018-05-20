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

#ifndef DATAHOLDER_H
#define DATAHOLDER_H

#include <vector>
#include <iostream>

/**
 * @todo write docs
 */
class DataHolder {
public:
    //Typedefs
    typedef std::vector<float>::iterator iterator;
    
    typedef std::vector<float>::const_iterator const_iterator;
    
    typedef std::vector<float>::value_type value_type;
    
    //Constructors
    DataHolder() = default;
    
    template<typename Iterator>
    DataHolder(Iterator begin, Iterator end);
    
    DataHolder(const DataHolder&) = default;
    
    DataHolder(DataHolder&&) = default;
    
    //Destructor
    ~DataHolder() = default;
    
    //Assignment operators
    DataHolder& operator=(const DataHolder&) = default;
    
    DataHolder& operator=(DataHolder&&) = default;
    
    //Stream operators
    friend std::ostream& operator<<(std::ostream&, const DataHolder&);
    
    friend std::istream& operator>>(std::istream&, DataHolder&);
    
    //Iterators
    iterator begin();
    
    iterator end();
    
    const_iterator begin() const;
    
    const_iterator end() const;
    
private:
    std::vector<float> _data;
};

template<typename Iterator>
DataHolder::DataHolder(Iterator begin, Iterator end) {
    while(begin != end) {
        _data.push_back(*begin++);
    }
}

#endif // DATAHOLDER_H
