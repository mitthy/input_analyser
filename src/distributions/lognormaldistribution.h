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

#ifndef LOGNORMALDISTRIBUTION_H
#define LOGNORMALDISTRIBUTION_H
#include "distribution.h"
#include "inputtypes.h"

/**
 * Class that represents a lognormal distribution.
 */
class LogNormalDistribution: public Distribution {
public:
    
    /**
     * Constructs an object
     * @param mean The mean of the lognormal distribution.
     * @param standard_deviation The standard deviation of the lognormal distribution.
     */
    LogNormalDistribution(input_data_t mean, input_data_t standard_deviation): _mean(mean), _standard_deviation(standard_deviation) {}
    
    
    virtual ~LogNormalDistribution() = default;
    
    /**
     * Generates a random value following a lognormal distribution.
     * @return Lognormal distributed random value.
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
        return "log normal";
    }
    
    /**
     * @return A string with the values used as parameters for the distribution.
     */
    virtual std::string get_parameters_str() const {
        return "mean = " + std::to_string(_mean) + "; standard deviation = " + std::to_string(_standard_deviation);
    }
private:
    input_data_t _mean;
    input_data_t _standard_deviation;
};

#endif // LOGNORMALDISTRIBUTION_H
