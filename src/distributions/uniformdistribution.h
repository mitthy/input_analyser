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

#ifndef UNIFORMDISTRIBUTION_H
#define UNIFORMDISTRIBUTION_H
#include "distribution.h"

/**
 * @todo write docs
 */
class UniformDistribution: public Distribution {
public:
    UniformDistribution(float min, float max): _min(min), _max(max) {}
    virtual ~UniformDistribution() = default;
    virtual float generate_value() const;
    virtual float frequency_for(float value) const;
    virtual std::string get_distribution_name() const {
        return "uniform";
    }
    virtual std::string get_parameters_str() const {
        return "a = " + std::to_string(_min) + "; b = " + std::to_string(_max);
    }
private:
    float _min;
    float _max;
};

#endif // UNIFORMDISTRIBUTION_H
