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

#include "distribution.h"
#include "normaldistribution.h"
#include "exponentialdistribution.h"
#include "triangulardistribution.h"
#include "betadistribution.h"
#include "lognormaldistribution.h"
#include "poissondistribution.h"
#include "uniformdistribution.h"
#include <limits>
#include <cmath>
#include "mathutils.h"

using namespace std;

template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

pair<unique_ptr<Distribution>, input_data_t> create_distribution(const DataHolder& dat, set<DistributionType>& desired_type, std::size_t num_cl) {
    //If no type was supplied, we assume all.
    if(desired_type.empty()) {
        desired_type.insert(DistributionType::TRIANGULAR);
        desired_type.insert(DistributionType::NORMAL);
        desired_type.insert(DistributionType::UNIFORM);
        desired_type.insert(DistributionType::EXPONENTIAL);
        desired_type.insert(DistributionType::LOGNORMAL);
        desired_type.insert(DistributionType::POISSON);
    }
    unique_ptr<Distribution> best_distribution = nullptr;
    auto monte_carlo_histogram = dat.generate_histogram(num_cl);
    input_data_t best_fit = numeric_limits<input_data_t>::quiet_NaN();
    //We initialize all those values as NaN so we don't have to recalculate them for each type.
    input_data_t mean = numeric_limits<input_data_t>::quiet_NaN();
    input_data_t variance = numeric_limits<input_data_t>::quiet_NaN();
    input_data_t standard_deviation = numeric_limits<input_data_t>::quiet_NaN();
    input_data_t min = numeric_limits<input_data_t>::quiet_NaN();
    input_data_t max = numeric_limits<input_data_t>::quiet_NaN();
    for(auto& type: desired_type) {
        unique_ptr<Distribution> dist_to_test(nullptr);
        //Based on the current distribution, we estimate its parameters and perform the Chi Squared test.
        //If it is better than the current best, we just swap the current best with the current distribution and update the best score.
        //Also, each type we check on the monte carlo calculated values we check if it is NaN. If it is, we just initialize it.
        //This prevents multiple unecessary calculations.
        switch(type) {
            case(DistributionType::TRIANGULAR): {
                if(isnan(min)) {
                    min = dat.min();
                }
                if(isnan(max)) {
                    max = dat.max();
                }
                if(isnan(mean)) {
                    mean = dat.mean();
                }
                input_data_t mode = mean - min - max + mean + mean;
                dist_to_test = make_unique<TriangularDistribution>(min, max, mode);
                break;
            }
            case(DistributionType::NORMAL): {
                if(isnan(mean)) {
                    mean = dat.mean();
                }
                if(isnan(standard_deviation)) {
                    if(isnan(variance)) {
                        standard_deviation = dat.standard_deviation();
                    }
                    else {
                        standard_deviation = sqrt(variance);
                    }
                }
                dist_to_test = make_unique<NormalDistribution>(mean, standard_deviation);
                break;
            }
            case(DistributionType::UNIFORM): {
                if(isnan(min)) {
                    min = dat.min();
                }
                if(isnan(max)) {
                    max = dat.max();
                }
                dist_to_test = make_unique<UniformDistribution>(min, max);
                break;
            }
            case(DistributionType::EXPONENTIAL): {
                if(isnan(mean)) {
                    mean = dat.mean();
                }
                dist_to_test = make_unique<ExponentialDistribution>(1 / mean);
                break;
            }
            case(DistributionType::LOGNORMAL): {
                input_data_t log_mean = 0;
                input_data_t log_standard_dev = 0;
                auto sz = dat.data_size();
                for(auto value: dat) {
                    if(value > 0) {
                        log_mean += log(value) / static_cast<input_data_t>(sz);
                    }
                }
                for(auto value : dat) {
                    if(value > 0) {
                        input_data_t tmp = pow(log(value) - log_mean, 2);
                        log_standard_dev += tmp / std::max((input_data_t)1.0, (input_data_t)(sz - 1));
                    }
                }
                dist_to_test = make_unique<LogNormalDistribution>(log_mean, log_standard_dev);
                break;
            }
            case(DistributionType::POISSON): {
                if(isnan(mean)) {
                    mean = dat.mean();
                }
                dist_to_test = make_unique<PoissonDistribution>(mean);
                break;
            }
            default: {
                break;
            }
        }
        input_data_t test_result = chi_squared_test(monte_carlo_histogram, *dist_to_test);
        if(isnan(best_fit) || test_result < best_fit) {
            best_fit = test_result;
            best_distribution.swap(dist_to_test);
        }
    }
    return make_pair(move(best_distribution), best_fit);
}


input_data_t chi_squared_test(const DataHistogram& hist, const Distribution& dist) {
    auto sz = hist.data_size();
    input_data_t sum = 0;
    //We check the data count for the current distribution and compare it with the data count for the histogram.
    //We know that the data count for the distribution is equal to the integral of the probability function from the lower limit to the upper limit * the total amount of data, so we just compute it for every class and apply the Chi Squared test formula.
    for(auto& klass : hist) {
        auto fx = [&dist](input_data_t x) {
            return dist.frequency_for(x);
        };
        input_data_t integral_value = integral(klass.lower_bound, klass.upper_bound, fx);
        auto expected = integral_value * sz;
        if(expected) {
            sum += pow(expected - klass.class_count, 2) / static_cast<input_data_t>(expected);
        }
        else {
            return numeric_limits<input_data_t>::infinity();
        }
    }
    return sum;
}
