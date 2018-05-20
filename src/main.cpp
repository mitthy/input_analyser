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

#include <iostream>
#include <fstream>
#include "dataholder.h"
#include "montecarlo.h"
#include <string>
#include "distributions/distribution.h"
#include <set>

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

using namespace std;

void print_help();

int main(int argc, char **argv) {
    DataHolder h;
    istream* stream_ptr = &cin;
    ifstream file;
    set<DistributionType> desired_distributions;
    bool print_var = false;
    bool print_std_deviation = false;
    bool print_mean = false;
    bool print_mode = false;
    bool print_min = false;
    bool print_max = false;
    bool print_classes = false;
    for(int i = 1; i < argc; ++i) {
        string cur_arg(argv[i]);
        if(cur_arg == "--help" || cur_arg == "-h") {
            print_help();
        }
        else if(cur_arg == "--file" || cur_arg == "-f") {
            if((++i) == argc) {
                cerr << "Error parsing arguments. Argument after --file should be a file name. Found: None." << endl;
                return EXIT_FAILURE;
            }
            file.open(argv[i]);
            if(!file) {
                cerr << "Error parsing arguments. Argument after --file should be a file name. Found: " << argv[i];
                cerr << " which is not a file." << endl;
                return EXIT_FAILURE;
            }
            stream_ptr = &file;
        }
        else if(cur_arg == "--print_var" || cur_arg == "-pvr") {
            print_var = true;
        }
        else if(cur_arg == "--print_std_deviation" || cur_arg == "-pstdev") {
            print_std_deviation = true;
        }
        else if(cur_arg == "--print_mean" || cur_arg == "-pmn") {
            print_mean = true;
        }
        else if(cur_arg == "--print_mode" || cur_arg == "-pmd") {
            print_mode = true;
        }
        else if(cur_arg == "--print_min" || cur_arg == "-pmin") {
            print_min = true; 
        }
        else if(cur_arg == "--print_max" || cur_arg == "-pmax") {
            print_max = true;
        }
        else if(cur_arg == "--print_classes" || cur_arg == "-pclasses") {
            print_classes = true;
        }
        else if(cur_arg == "--normal" || cur_arg == "-nrm") {
            desired_distributions.insert(DistributionType::NORMAL);
        }
        else if(cur_arg == "--triangular" || cur_arg == "-trng") {
            desired_distributions.insert(DistributionType::TRIANGULAR);;
        }
        else if(cur_arg == "--beta" || cur_arg == "-bet") {
            desired_distributions.insert(DistributionType::BETA);
        }
        else if(cur_arg == "--uniform" || cur_arg == "-uni") {
            desired_distributions.insert(DistributionType::UNIFORM);
        }
        else if(cur_arg == "--exponential" || cur_arg == "-exp") {
            desired_distributions.insert(DistributionType::EXPONENTIAL);
        }
        else if(cur_arg == "--log_normal" || cur_arg == "-ln") {
            desired_distributions.insert(DistributionType::LOGNORMAL);
        }
        else if(cur_arg == "--poisson" || cur_arg == "-psn") {
            desired_distributions.insert(DistributionType::POISSON);
        }
        else {
            cerr << "Invalid argument " << cur_arg << "." << endl;
            cerr << "Printing help:" << endl;
            print_help();
            return EXIT_FAILURE;
        }
    }
    istream& input = *stream_ptr;
    while(input >> h);
    if(h.begin() == h.end()) {
        cerr << "Can't process empty data. Please supply floating point values for processing." << endl;
        return EXIT_FAILURE;
    }
    MonteCarlo monte_carlo(h.begin(), h.end());
    if(print_classes) {
        cout << monte_carlo.print_classes() << endl;
    }
    if(print_mean) {
        cout << "Monte Carlo calculated histogram mean: " << monte_carlo.histogram_mean() << "." << endl;
    }
    if(print_var) {
        cout << "Monte Carlo calculated histogram variance: " << monte_carlo.histogram_variance() << "." << endl;
    }
    if(print_std_deviation) {
        cout << "Monte Carlo calculated histogram standard deviation: " << monte_carlo.histogram_standard_deviation() << "." << endl;
    }
    if(print_mode) {
        cout << "Monte Carlo calculated histogram mode: " << monte_carlo.histogram_mode() << "." << endl;
    }
    if(print_min) {
        cout << "Monte Carlo calculated histogram min value: " << monte_carlo.histogram_min_value() << "." << endl;
    }
    if(print_max) {
        cout << "Monte Carlo calculated histogram max value: " << monte_carlo.histogram_max_value() << "." << endl;
    }
    return EXIT_SUCCESS;
}

void print_help() {
    cout << "Input analyser for statistical purposes." << endl;
    cout << "================================================================================" << endl;
    cout << "Usage:" << endl;
    cout << "--help or -h to print help." << endl;
    cout << "--file or -f filename: opens filename for processing, which should contain a" << endl;
    cout << "list of float values. If not supplied, the user can enter numbers by hand" << endl;
    cout << "when the software starts." << endl;
    cout << "--print_classes or -pclasses if the user wants the classes calculated on the" << endl;
    cout << "histogram to be printed." << endl;
    cout << "--print_mean or -pmn if the user wants the mean calculated on the histogram to" << endl;
    cout << "be printed." << endl;
    cout << "--print_var or -pvr if the user wants the variance calculated on the histogram"<< endl;
    cout << "to be printed." << endl;
    cout << "--print_std_deviation or -pstdev if the user wants the standard deviation" << endl;
    cout << "calculated on the histogram to be printed." << endl;
    cout << "--print_mode or -pmd if the user wants the mode calculated on the histogram to" << endl;
    cout << "be printed." << endl;
    cout << "--print_min or -pmin if the user wants the minimum value on the histogram to" << endl; 
    cout << "be printed." << endl;
    cout << "--print_max or -pmax if the ser wants the maximum value on the histogram to be" << endl; 
    cout <<" printed." << endl;
    cout << "================================================================================" << endl;
    cout << "The user can supply a list of distributions that he wants to use. The program" << endl;
    cout << "will then select the one which best fits the data. If no option is supplied, the" << endl; 
    cout << "program will just select the best fit among all available distributions." << endl;
    cout << "================================================================================" << endl;
    cout << "To select the distributions, the user should supply these options:" << endl;
    cout << "--normal or -nrm => Normal distribution." << endl;
    cout << "--triangular or -trng => Triangular distribution." << endl;
    cout << "--beta or -bet => Beta distribution." << endl;
    cout << "--uniform or -uni => Uniform distribution." << endl;
    cout << "--exponential or -exp => Exponential distribution." << endl;
    cout << "--log_normal or -ln => Lognormal distribution." << endl;
    cout << "--poisson or -psn => Poisson distribution." << endl;
    cout << "================================================================================" << endl;
}
