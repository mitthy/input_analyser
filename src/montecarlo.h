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
#include "inputtypes.h"

/**
 * Class representing a Monte Carlo method and its generated histogram.
 */
class MonteCarlo {
public:
    //Constructors
    
    /**
     * Constructor for Monte Carlo histogram.
     * @param begin iterator to first element of data.
     * @param end iterator to element one past the end of data.
     * @pre <strong class="paramname">begin</strong> < <strong class="paramname">end</strong>.
     */
    template<typename Iterator>
    MonteCarlo(Iterator begin, Iterator end);
    
    /**
     * Function that uses the Monte Carlo histogram as a random number generator.
     * @return A random number based on the frequency distribution of the supplied data.
     */
    input_data_t generate_value() const;
    
    /**
     * Function that calculates the mean of the Monte Carlo histogram.
     * @return The mean of the aggruped data or NaN if there is no data.
     */
    input_data_t histogram_mean() const;
    
    /**
     * Function that calculates the variance of the Monte Carlo histogram.
     * @return The variance of the aggruped data, 0 if there is only 1 class or NaN if there is no data.
     */
    input_data_t histogram_variance() const;
    
    /**
     * Function that calculates the standard deviation of the Monte Carlo histogram.
     * @return The standard deviation of the aggruped data, 0 if there is only 1 class or NaN if there is no data.
     */
    input_data_t histogram_standard_deviation() const;
    
    /**
     * Function that calculates the minimum value of the Monte Carlo histogram.
     * @return The minimum class value of the aggruped data or NaN if there is no data.
     */
    input_data_t histogram_min_value() const;
    
    /**
     * Function that calculates the maximum value of the Monte Carlo histogram.
     * @return The maximum class value of the aggruped data or NaN if there is no data.
     */
    input_data_t histogram_max_value() const;
    
    /**
     * Function that calculates the mode of the Monte Carlo histogram.
     * @return The class with the most data of the aggruped data or NaN if there is no data.
     */
    input_data_t histogram_mode() const;
    
    /**
    * Internal struct that contains data for each Monte Carlo histogram class
    */
    struct monte_carlo_class {
        /**
        * Value that represents the class. This is usually the mid point.
        */
        input_data_t value;
        
        /**
        * Value that represents the accumulated probability. This value is equal to acum_probability[cur_class - 1] + prob[cur_class].
        */
        input_data_t acum_probability;
        
        /**
        * The number of elements in the class.
        */
        std::size_t class_count;
        
        /**
        * The lower bound of the class.
        */
        input_data_t lower_bound;
        
        /**
        * The upper bound of the class.
        */
        input_data_t upper_bound;
        
        /**
        * C++ stream operator for printing the class.
        * @param os The output stream.
        * @param monte_class The class to be printed.
        * @return The output stream <strong class="paramname">os</strong>
        */
        friend std::ostream& operator<<(std::ostream& os, const monte_carlo_class& monte_class) {
            os << "Class value " << monte_class.value << "; Initial probability " << monte_class.acum_probability << "; Number of data ";
            os << monte_class.class_count;
            return os;
        }
    };
    
    /**
    * Typedef to iterator.
    */
    typedef std::vector<monte_carlo_class>::iterator iterator;
    
    /**
    * Typedef to const_iterator.
    */
    typedef std::vector<monte_carlo_class>::const_iterator const_iterator;
    
    /**
    * Gets iterator to Monte Carlo class.
    * @return iterator to first Monte Carlo class.
    */
    iterator begin() {
        return _organized_data.begin();
    }
    
    /**
    * Gets iterator to Monte Carlo class.
    * @return iterator to one past the end Monte Carlo class.
    */
    iterator end() {
        return _organized_data.end();
    }
    
    /**
    * Gets const_iterator to Monte Carlo class.
    * @return const_iterator to first Monte Carlo class.
    */
    const_iterator begin() const {
        return _organized_data.begin();
    }
    
    /**
    * Gets const_iterator to Monte Carlo class.
    * @return const_iterator to one past the end Monte Carlo class.
    */
    const_iterator end() const {
        return _organized_data.end();
    }
    
    /**
    * Size of all the data in the histogram.
    * @return amount of data supplied to construct the histogram.
    */
    std::size_t data_size() const {
        return _data_count;
    }
    
private:
    
    std::vector<monte_carlo_class> _organized_data;
    
    std::size_t _data_count;
public:
    
    /**
    * Internal struct that can be used to print all the Monte Carlo classes.
    */
    struct monte_carlo_class_printer {
        friend class MonteCarlo;
        /**
        * C++ stream operator for printing the class.
        * @param os The output stream.
        * @param printer The printer itself.
        * @return The output stream <strong class="paramname">os</strong>
        */
        friend std::ostream& operator<<(std::ostream& os, const monte_carlo_class_printer& printer); 
    private:
        monte_carlo_class_printer() = default;
        std::vector<monte_carlo_class>* classes;
    };
    
    /**
    * Returns a printer to print the classes on the screen.
    * @return An object of type monte_carlo_class_printer.
    */
    monte_carlo_class_printer print_classes();
};


//Since it is a templated method, we implement it in the header.
template<typename Iterator>
MonteCarlo::MonteCarlo(Iterator begin, Iterator end): _data_count(std::distance(begin, end)) {
    //If the difference between the max element and the minimum element is lesser than this value, we don't split the data into classes.
    const input_data_t EPSLON = static_cast<input_data_t>(1e-4);
    auto min_max = std::minmax_element(begin, end);
    input_data_t min = *min_max.first, max = *min_max.second;
    //We split the input data into classes. For now, it just gets the minimum between the amount of data / 10 + 1 and 15.
    int number_of_classes = max - min < EPSLON ? 1 : std::min((_data_count / 10) + 1, (size_t)15);
    input_data_t step = 0;
    //The step that we take between each classes.
    if(number_of_classes > 1) {
        step = (max - min) / (number_of_classes - 1);
    }
    //Calculates the bound for each class.
    input_data_t lower_bound = step ? min - step / 2 : min;
    input_data_t upper_bound = step ? min + step / 2 : max;
    //Insert all the classes into a vector.
    for(int i = 0; i < number_of_classes; ++i) {
        input_data_t avg = lower_bound + (upper_bound - lower_bound) / 2;
        monte_carlo_class new_class = {avg, 0, 0, lower_bound, upper_bound};
        lower_bound += step;
        upper_bound += step;
        _organized_data.push_back(new_class);
    }
    //For each element, we do a binary search to find its position in the class vector and then we increment the amount of data of that class.
    while(begin != end) {
        std::size_t low = 0, high = _organized_data.size();
        input_data_t val = *begin++;
        //Binary search to find first element less than val.
        while(low != high) {
            //We do this to avoid overflow.
            std::size_t mid = (low >> 1) + (high >> 1) + ((low & high & 1));
            if(val < _organized_data[mid].lower_bound) {
                high = mid;
            }
            else {
                low = mid + 1;
            }
        }
        ++_organized_data[--low].class_count;
    }
    input_data_t acum = 0;
    for(monte_carlo_class& klass : _organized_data) {
        klass.acum_probability = acum;
        acum += klass.class_count * static_cast<input_data_t>(1.0) / _data_count;
    }
}

#endif // MONTECARLO_H
