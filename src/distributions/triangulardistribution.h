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

#ifndef TRIANGULARDISTRIBUTION_H
#define TRIANGULARDISTRIBUTION_H
#include "distribution.h"
#include "inputtypes.h"

/**
 * Class that represents a triangular distribution.
 */
class TriangularDistribution: public Distribution {
public:
    
    /**
     * Constructs an object.
     * @param min The minimum (a) value of a triangular distribution.
     * @param mode The mode(c) of a triangular distribution.
     * @param max the maximum (b) value of a triangular distribution.
     * @pre <strong class="paramname">min</strong> <= <strong class="paramname">mode</strong> <= <strong class="paramname">max</strong>.
     * @pre <strong class="paramname">min</strong> < <strong class="paramname">max</strong>.
     */
    TriangularDistribution(input_data_t min, input_data_t max, input_data_t mode): _min(min), _max(max), _mode(mode){}
    virtual ~TriangularDistribution() = default;
    
    /**
     * Generates a random value following a triangular distribution.
     * @return Triangular distributed random value.
     */
    virtual input_data_t generate_value() const;
    
    /**
     * Calculates the probability distribution.
     * @param value The value to calculate the probability.
     * @return The probability of value.
     */
    virtual input_data_t frequency_for(input_data_t value) const;
    
    /**
     * @return The name of the distribution.
     */
    virtual std::string get_distribution_name() const {
        return "triangular";
    }
    
    /**
     * @return A string with the values used as parameters for the distribution.
     */
    virtual std::string get_parameters_str() const {
        return "a = " + std::to_string(_min) + "; b = " + std::to_string(_max) + "; c = " + std::to_string(_mode);
    }
private:
    input_data_t _min;
    input_data_t _max;
    input_data_t _mode;
};

#endif // TRIANGULARDISTRIBUTION_H
