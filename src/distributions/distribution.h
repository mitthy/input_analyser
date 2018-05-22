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
#include "montecarlo.h"
#include <set>
#include <utility>
#include <string>

enum class DistributionType {
    TRIANGULAR,
    NORMAL,
    UNIFORM,
    EXPONENTIAL,
    LOGNORMAL,
    BETA,
    POISSON
};

//Abstract class that represents a probability distribution.
class Distribution {
public:
    Distribution() = default;
    virtual ~Distribution() = default;
    virtual float generate_value() const = 0;
    virtual float frequency_for(float value) const = 0;
    virtual std::string get_distribution_name() const {
        return "undefined";
    }
    virtual std::string get_parameters_str() const {
        return "undefined";
    }
};

float chi_squared_test(const MonteCarlo& hist, const Distribution& dist);

std::pair<std::unique_ptr<Distribution>, float> create_distribution(const MonteCarlo& hist, std::set<DistributionType>& dsr_types);

#endif
