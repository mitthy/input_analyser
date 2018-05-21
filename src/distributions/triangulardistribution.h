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

#ifndef TRIANGULARDISTRIBUTION_H
#define TRIANGULARDISTRIBUTION_H
#include "distribution.h"

/**
 * @todo write docs
 */
class TriangularDistribution: public Distribution {
public:
    TriangularDistribution(float min, float max, float mode);
    virtual ~TriangularDistribution() = default;
    virtual float generate_value() const;
    virtual float frequency_for(float value) const;
};

#endif // TRIANGULARDISTRIBUTION_H
