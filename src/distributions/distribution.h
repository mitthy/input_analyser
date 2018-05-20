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

#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H
#include <memory>
#include "montecarlo.h"
#include <set>

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
    virtual float generate_value() const = 0;
    virtual ~Distribution() = default;
};

std::unique_ptr<Distribution> create_distribution(const MonteCarlo& monte_carlo_histogram, const std::set<DistributionType>& desired_type);

#endif
