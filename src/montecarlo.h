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

#ifndef MONTECARLO_H
#define MONTECARLO_H
#include "dataholder.h"
#include <vector>
#include <algorithm>
#include <iostream>

/**
 * @todo write docs
 */
class MonteCarlo {
public:
    //Constructors
    template<typename Iterator>
    MonteCarlo(Iterator begin, Iterator end);
    
    float generate_value() const;
    
    float histogram_mean() const;
    
    float histogram_variance() const;
    
    float histogram_standard_deviation() const;
    
    float histogram_min_value() const;
    
    float histogram_max_value() const;
    
    float histogram_mode() const;
    
    struct monte_carlo_class {
        float value;
        float acum_probability;
        std::size_t class_count;
        friend std::ostream& operator<<(std::ostream& os, const monte_carlo_class& monte_class) {
            os << "Class value " << monte_class.value << "; Initial probability " << monte_class.acum_probability << "; Number of data ";
            os << monte_class.class_count;
            return os;
        }
    };
    
    typedef std::vector<monte_carlo_class>::iterator iterator;
    
    typedef std::vector<monte_carlo_class>::const_iterator const_iterator;
    
    iterator begin() {
        return _organized_data.begin();
    }
    
    iterator end() {
        return _organized_data.end();
    }
    
    const_iterator begin() const {
        return _organized_data.begin();
    }
    
    const_iterator end() const {
        return _organized_data.end();
    }
    
    std::size_t data_size() const {
        return _data_count;
    }
    
private:
    
    std::vector<monte_carlo_class> _organized_data;
    
    std::size_t _data_count;
public:
    struct monte_carlo_class_printer {
        friend class MonteCarlo;
        friend std::ostream& operator<<(std::ostream& os, const monte_carlo_class_printer& printer); 
    private:
        monte_carlo_class_printer() = default;
        std::vector<monte_carlo_class>* classes;
    };
    
    monte_carlo_class_printer print_classes();
};

template<typename Iterator>
MonteCarlo::MonteCarlo(Iterator begin, Iterator end): _data_count(std::distance(begin, end)) {
    const float EPSLON = 1e-4;
    auto min_max = std::minmax_element(begin, end);
    float min = *min_max.first, max = *min_max.second;
    int number_of_classes = max - min < EPSLON ? 1 : std::min((_data_count / 10) + 1, (size_t)15);
    float step = 0;
    if(number_of_classes > 1) {
        step = (max - min) / (number_of_classes - 1);
    }
    float lower_bound = step ? min - step / 2 : min;
    float upper_bound = step ? min + step / 2 : max;
    struct monte_carlo_frenquecy {
        std::size_t count = 0;
        float lower_bound;
        float upper_bound;
    };
    std::vector<monte_carlo_frenquecy> frequency_count;
    for(int i = 0; i < number_of_classes; ++i) {
        monte_carlo_frenquecy freq;
        freq.lower_bound = lower_bound;
        freq.upper_bound = upper_bound;
        lower_bound += step;
        upper_bound += step;
        frequency_count.push_back(freq);
    }
    while(begin != end) {
        std::size_t low = 0, high = frequency_count.size();
        float val = *begin++;
        while(low != high) {
            std::size_t mid = (low >> 1) + (high >> 1) + ((low & high & 1));
            if(val < frequency_count[mid].lower_bound) {
                high = mid;
            }
            else {
                low = mid + 1;
            }
        }
        ++frequency_count[--low].count;
    }
    float acum = 0;
    for(const monte_carlo_frenquecy& analysed : frequency_count) {
        float avg = analysed.lower_bound + (analysed.upper_bound - analysed.lower_bound) / 2;
        float prob = acum;
        monte_carlo_class new_class = {avg, prob, analysed.count};
        _organized_data.push_back(new_class);
        acum += analysed.count * 1.0 / _data_count;
    }
}

#endif // MONTECARLO_H
