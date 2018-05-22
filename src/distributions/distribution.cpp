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

using namespace std;

template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

pair<unique_ptr<Distribution>, float> create_distribution(const MonteCarlo& monte_carlo_histogram, set<DistributionType>& desired_type) {
    if(desired_type.empty()) {
        desired_type.insert(DistributionType::TRIANGULAR);
        desired_type.insert(DistributionType::NORMAL);
        desired_type.insert(DistributionType::UNIFORM);
        desired_type.insert(DistributionType::EXPONENTIAL);
        desired_type.insert(DistributionType::LOGNORMAL);
        desired_type.insert(DistributionType::BETA);
        desired_type.insert(DistributionType::POISSON);
    }
    unique_ptr<Distribution> best_distribution = nullptr;
    float best_fit = numeric_limits<float>::quiet_NaN();
    float mean = numeric_limits<float>::quiet_NaN();
    float variance = numeric_limits<float>::quiet_NaN();
    float standard_deviation = numeric_limits<float>::quiet_NaN();
    float min = numeric_limits<float>::quiet_NaN();
    float max = numeric_limits<float>::quiet_NaN();
    for(auto& type: desired_type) {
        unique_ptr<Distribution> dist_to_test(nullptr);
        switch(type) {
            case(DistributionType::TRIANGULAR): {
                if(isnan(min)) {
                    min = monte_carlo_histogram.histogram_min_value();
                }
                if(isnan(max)) {
                    max = monte_carlo_histogram.histogram_max_value();
                }
                if(isnan(mean)) {
                    mean = monte_carlo_histogram.histogram_mean();
                }
                float mode = mean - min - max + mean + mean;
                dist_to_test = make_unique<TriangularDistribution>(min, max, mode);
                break;
            }
            case(DistributionType::NORMAL): {
                if(isnan(mean)) {
                    mean = monte_carlo_histogram.histogram_mean();
                }
                if(isnan(standard_deviation)) {
                    if(isnan(variance)) {
                        standard_deviation = monte_carlo_histogram.histogram_standard_deviation();
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
                    min = monte_carlo_histogram.histogram_min_value();
                }
                if(isnan(max)) {
                    max = monte_carlo_histogram.histogram_max_value();
                }
                dist_to_test = make_unique<UniformDistribution>(min, max);
                break;
            }
            case(DistributionType::EXPONENTIAL): {
                if(isnan(mean)) {
                    mean = monte_carlo_histogram.histogram_mean();
                }
                dist_to_test = make_unique<ExponentialDistribution>(1 / mean);
                break;
            }
            case(DistributionType::LOGNORMAL): {
                if(isnan(mean)) {
                    mean = monte_carlo_histogram.histogram_mean();
                }
                if(isnan(standard_deviation)) {
                    if(isnan(variance)) {
                        standard_deviation = monte_carlo_histogram.histogram_standard_deviation();
                    }
                    else {
                        standard_deviation = sqrt(variance);
                    }
                }
                dist_to_test = make_unique<LogNormalDistribution>(mean, standard_deviation);
                break;
            }
            case(DistributionType::BETA): {
                if(isnan(mean)) {
                    mean = monte_carlo_histogram.histogram_mean();
                }
                if(isnan(standard_deviation)) {
                    if(isnan(variance)) {
                        variance = monte_carlo_histogram.histogram_variance();
                    }
                    standard_deviation = sqrt(variance);
                }
                if(isnan(variance)) {
                    variance = monte_carlo_histogram.histogram_variance();
                }
                float alpha = standard_deviation == 0 ? 0 : pow((mean / standard_deviation), 2);
                float beta = mean == 0 ? 0 : variance / mean;
                dist_to_test = make_unique<BetaDistribution>(alpha, beta);
                break;
            }
            case(DistributionType::POISSON): {
                if(isnan(mean)) {
                    mean = monte_carlo_histogram.histogram_mean();
                }
                dist_to_test = make_unique<PoissonDistribution>(mean);
                break;
            }
            default: {
                break;
            }
        }
        float test_result = chi_squared_test(monte_carlo_histogram, *dist_to_test);
        if(isnan(best_fit) || test_result < best_fit) {
            best_fit = test_result;
            best_distribution.swap(dist_to_test);
        }
    }
    return pair<unique_ptr<Distribution>, float>(move(best_distribution), best_fit);
}

float chi_squared_test(const MonteCarlo& hist, const Distribution& dist) {
    //TODO
    return 0;
}
