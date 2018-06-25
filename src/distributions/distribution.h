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

#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H
#include <memory>
#include "datahistogram.h"
#include <set>
#include <utility>
#include <string>
#include "inputtypes.h"

/**
 * Enum listing the distribution types.
 * We don't list beta here since it is not yet implemented.
 */
enum class DistributionType {
    TRIANGULAR,
    NORMAL,
    UNIFORM,
    EXPONENTIAL,
    LOGNORMAL,
    POISSON
};

/**
 * Abstract class that represents a probability distribution.
 */
class Distribution {
public:
    Distribution() = default;
    virtual ~Distribution() = default;
    
    /**
     * Calculates a random value following a certain distribution. Subclasses should implement this method.
     * @return A random value.
     */
    virtual input_data_t generate_value() const = 0;
    
    /**
     * Calculates the probability distribution. Subclasses should implement this method.
     * @param value The value to calculate the probability.
     * @return The probability of value.
     */
    virtual input_data_t frequency_for(input_data_t value) const = 0;
    
    /**
     * Method that returns the name of the distribution. Subclasses can choose to not implement this in which case it is simply undefined.
     * @return The name of the distribution.
     */
    virtual std::string get_distribution_name() const {
        return "undefined";
    }
    
    /**
     * Method that returns the parameters of the distribution. Subclasses can choose to not implement this in which case it is simply undefined.
     * @return A string with the values used as parameters for the distribution.
     */
    virtual std::string get_parameters_str() const {
        return "undefined";
    }
};

/**
 * Chi Squared test for a certain distribution given a monte carlo histogram.
 */
input_data_t chi_squared_test(const DataHistogram& hist, const Distribution& dist);

/**
 * Creates distribution with the best Chi Squared score for the histogram among the desired types. If no types are supplied, it picks the best among all types
 * @param hist The Monte Carlo histogram.
 * @param dsr_types The types of distributions the user desires. If empty, it assumes the user wants to check all types.
 * @return The distribution with the best Chi Squared score.
 */
std::pair<std::unique_ptr<Distribution>, input_data_t> create_distribution(const DataHistogram& hist, std::set<DistributionType>& dsr_types);

#endif
