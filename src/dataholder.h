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

#ifndef DATAHOLDER_H
#define DATAHOLDER_H

#include <vector>
#include <iostream>

/**
 * Class that we use to grab data from istream. It can also be used to output data to an ostream.
 */
class DataHolder {
public:
    //Typedefs
    
    /**
     * Typedef to iterator
     */
    typedef std::vector<float>::iterator iterator;
    
    /**
     * Typedef to const_iterator
     */
    typedef std::vector<float>::const_iterator const_iterator;
    
    /**
     * Typedef to value_type
     */
    typedef std::vector<float>::value_type value_type;
    
    //Constructors
    
    /**
     * Construct an object with no data.
     */
    DataHolder() = default;
    
    
    /**
     * Constructs an object with data from begin to end.
     * @param begin Iterator to first element of range.
     * @param end Iterator to one past the last element of the range.
     * @pre <strong class="paramname">begin</strong> <= <strong class="paramname">end</strong>.
     */
    template<typename Iterator>
    DataHolder(Iterator begin, Iterator end);
    
    /**
     * Copy construct an object
     * @param rhs other object
     */
    DataHolder(const DataHolder& rhs) = default;
    
    /**
     * Move construct an object
     * @param rhs other object
     */
    DataHolder(DataHolder&& rhs) = default;
    
    //Destructor
    
    ~DataHolder() = default;
    
    //Assignment operators
    
    /**
     * Copy assigns an object
     * @param rhs other object
     */
    DataHolder& operator=(const DataHolder& rhs) = default;
    
    /**
     * Move assigns an object
     * @param rhs other object
     */
    DataHolder& operator=(DataHolder&& rhs) = default;
    
    //Stream operators
    
    /**
     * Outputs data to stream.
     * @param os Output stream.
     * @param obj DataHolder to output.
     * @return The output stream <strong class="paramname">os</strong>
     */
    friend std::ostream& operator<<(std::ostream& os, const DataHolder& obj);
    
    /**
     * Inputs data from stream.
     * @param is Input stream.
     * @param obj DataHolder object that will read value from stream.
     * @return The input stream <strong class="paramname">is</strong>
     */
    friend std::istream& operator>>(std::istream& is, DataHolder& obj);
    
    //Iterators
    
    /**
     * Gets an iterator.
     * @return iterator to first element.
     */
    iterator begin();
    
    /**
     * Gets an iterator.
     * @return iterator to one past the end element.
     */
    iterator end();
    
    /**
     * Gets a const_iterator.
     * @return const_iterator to first element.
     */
    const_iterator begin() const;
    
    /**
     * Gets a const_iterator.
     * @return const_iterator to one past the end element.
     */
    const_iterator end() const;
    
private:
    std::vector<float> _data;
};

template<typename Iterator>
DataHolder::DataHolder(Iterator begin, Iterator end) {
    _data.insert(_data.begin(), begin, end);
}

#endif // DATAHOLDER_H
