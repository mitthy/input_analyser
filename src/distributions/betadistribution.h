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

#ifndef BETADISTRIBUTION_H
#define BETADISTRIBUTION_H
#include "distribution.h"

/**
 * Class that represents a beta distribution. It is not yet implemented.
 */
class BetaDistribution: public Distribution {
public:
    /**
     * Constructs an object.
     * @param alpha The alpha value of a beta distribution.
     * @param beta The beta value of a beta distribution.
     */
    BetaDistribution(float alpha, float beta);
    virtual ~BetaDistribution() = default;
    
    /**
     * Generates a random value following a beta distribution.
     * @return Beta distributed random value.
     */
    virtual float generate_value() const;
    
    /**
     * Calculates the probability distribution.
     * @param value The value to calculate the probability.
     * @return The probability of value.
     */
    virtual float frequency_for(float value) const;
};

#endif // BETADISTRIBUTION_H
