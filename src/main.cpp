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

#include <iostream>
#include <fstream>
#include "dataholder.h"
#include "montecarlo.h"
#include <string>
#include "distributions/distribution.h"
#include <set>
#include "mathutils.h"
#include "inputtypes.h"
#include <iomanip>
#include <limits>

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

using namespace std;

void print_help();

//Just argument parsing in this file and setting up the system.

int main(int argc, char **argv) {
    DataHolder h;
    istream* stream_ptr = &cin;
    ostream* ostream_ptr = &cout;
    ofstream out_file;
    ifstream file;
    set<DistributionType> desired_distributions;
    bool print_var = false;
    bool print_std_deviation = false;
    bool print_mean = false;
    bool print_mode = false;
    bool print_min = false;
    bool print_max = false;
    bool print_classes = false;
    bool print_chi_square_result = false;
    bool print_frequency_difference = false;
    bool print_distribution = true;
    unsigned int generate_output = 0;
    unsigned int class_count = 0;
    for(int i = 1; i < argc; ++i) {
        string cur_arg(argv[i]);
        if(cur_arg == "--help" || cur_arg == "-h") {
            print_help();
            return EXIT_SUCCESS;
        }
        else if(cur_arg == "--input_file" || cur_arg == "-if") {
            if((++i) == argc) {
                cerr << "Error parsing arguments. Argument after " << cur_arg << " should be a file name." << endl;
                cerr << "Found: None." << endl;
                return EXIT_FAILURE;
            }
            file.open(argv[i]);
            if(!file) {
                cerr << "Error parsing arguments. Argument after " << cur_arg << " should be a file name." << endl;
                cerr << "Found: " << argv[i];
                cerr << " which is not a file." << endl;
                return EXIT_FAILURE;
            }
            stream_ptr = &file;
        }
        else if(cur_arg == "--output_file" || cur_arg == "-of") {
            if((++i) == argc) {
                cerr << "Error parsing arguments. Argument after " << cur_arg << " should be a file name." << endl; 
                cerr << "Found: None." << endl;
                return EXIT_FAILURE;
            }
            out_file.open(argv[i]);
            if(!out_file) {
                cerr << "Error opening file " << argv[i] << "for write." << endl;
                return EXIT_FAILURE;
            }
            ostream_ptr = &out_file;
        }
        else if(cur_arg == "--generate_random" || cur_arg == "-gr") {
            if((++i) == argc) {
                cerr << "Error parsing arguments. Argument after " << cur_arg << "should be an unsigned number." << endl;
                cerr << "Found: None." << endl;
                return EXIT_FAILURE;
            }
            size_t next_position = 0;
            string count_str = argv[i];
            try {
                generate_output = stoi(count_str, &next_position);
                if(next_position != count_str.size()) {
                    cerr << "Expected a number after " << cur_arg << ". Found: " << count_str << endl;
                    return EXIT_FAILURE;
                }
            }
            catch(invalid_argument& e) {
                cerr << "Expected a number after " << cur_arg << ". Found: " << count_str << endl;
                return EXIT_FAILURE;
            }
            catch(out_of_range& e) {
                cerr << "Couldn't set number of random numbers. " <<  count_str << " is too big." << endl;
                return EXIT_FAILURE;
            }
        }
        else if(cur_arg == "--class_count" || cur_arg == "-cc") {
            if((++i) == argc) {
                cerr << "Error parsing arguments. Argument after " << cur_arg << "should be an unsigned number." << endl;
                cerr << "Found: None." << endl;
                return EXIT_FAILURE;
            }
            size_t next_position = 0;
            string count_str = argv[i];
            try {
                class_count = stoi(count_str, &next_position);
                if(next_position != count_str.size()) {
                    cerr << "Expected a number after " << cur_arg << ". Found: " << count_str << endl;
                    return EXIT_FAILURE;
                }
            }
            catch(invalid_argument& e) {
                cerr << "Expected a number after " << cur_arg << ". Found: " << count_str << endl;
                return EXIT_FAILURE;
            }
            catch(out_of_range& e) {
                cerr << "Couldn't set class count. " << count_str << " is too big." << endl;
                return EXIT_FAILURE;
            }
        }
        else if(cur_arg == "--print_var" || cur_arg == "-pvr" || cur_arg == "--print_variance") {
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
        else if(cur_arg == "--print_chi_square" || cur_arg == "-pcs") {
            print_chi_square_result = true;
        }
        else if(cur_arg == "--print_frequency_difference" || cur_arg == "-pfd") {
            print_frequency_difference = true;
        }
        else if(cur_arg == "--no_print_dist" || cur_arg == "-npd") {
            print_distribution = false;
        }
        else if(cur_arg == "--normal" || cur_arg == "-nrm") {
            desired_distributions.insert(DistributionType::NORMAL);
        }
        else if(cur_arg == "--triangular" || cur_arg == "-trng") {
            desired_distributions.insert(DistributionType::TRIANGULAR);;
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
    ostream& output = *ostream_ptr;
    istream& input = *stream_ptr;
    output.precision(numeric_limits<input_data_t>::max_digits10);
    while(input >> h);
    if(h.begin() == h.end()) {
        cerr << "Can't process empty data. Please supply floating point values for processing." << endl;
        return EXIT_FAILURE;
    }
    auto amount_of_data = std::distance(h.begin(), h.end());
    while(class_count > amount_of_data) {
        cerr << "Invalid amount of classes. Should be <= amount of data = " << amount_of_data << "." << endl;
        cerr << "Please type in a new class value or quit to quit." << endl;
        string option;
        cin >> option;
        if(option == "quit") {
            return EXIT_FAILURE;
        }
        std::size_t next_position;
        try {
            auto tmp = stoi(option, &next_position);
            if(next_position != option.size()) {
                cerr << option << " is not a number." << endl;
            }
            else {
                class_count = tmp;
            }
        }
        catch(invalid_argument& e) {
            cerr << option << " is not a number." << endl;
        }
        catch(out_of_range& e) {
            cerr << option << " is too big." << endl;
        }
    }
    MonteCarlo monte_carlo(h.begin(), h.end(), class_count);
    unique_ptr<Distribution> distr_ptr;
    input_data_t chi_result;
    tie(distr_ptr, chi_result) = create_distribution(monte_carlo, desired_distributions);
    if(print_classes) {
        output << monte_carlo.print_classes() << endl;
    }
    if(print_mean) {
        output << "Monte Carlo calculated histogram mean: " << monte_carlo.histogram_mean() << "." << endl;
    }
    if(print_var) {
        output << "Monte Carlo calculated histogram variance: " << monte_carlo.histogram_variance() << "." << endl;
    }
    if(print_std_deviation) {
        output << "Monte Carlo calculated histogram standard deviation: " << monte_carlo.histogram_standard_deviation() << "." << endl;
    }
    if(print_mode) {
        output << "Monte Carlo calculated histogram mode: " << monte_carlo.histogram_mode() << "." << endl;
    }
    if(print_min) {
        output << "Monte Carlo calculated histogram min value: " << monte_carlo.histogram_min_value() << "." << endl;
    }
    if(print_max) {
        output << "Monte Carlo calculated histogram max value: " << monte_carlo.histogram_max_value() << "." << endl;
    }
    if(print_chi_square_result) {
        output << "Chi square test result for distribution " << distr_ptr->get_distribution_name() << ": " << chi_result << "." << endl;
    }
    if(print_frequency_difference) {
        output << "Frequency differences between chosen distribution and supplied data:" << endl;
        for(auto it1 = monte_carlo.begin(), it2 = ++monte_carlo.begin(); it1 != monte_carlo.end(); ++it1, ++it2) {
            input_data_t klass_freq = it2 != monte_carlo.end() ? it2->acum_probability - it1->acum_probability : 1 - it1->acum_probability;
            auto fx = [&distr_ptr](input_data_t x) {
                return distr_ptr->frequency_for(x);
            };
            input_data_t chosen_distr_klass_freq = integral(it1->lower_bound, it1->upper_bound, fx);
            output << "[" << it1->lower_bound << " - " << it2->lower_bound << "] - ";
            output << "Data: " << klass_freq << "; ";
            output << "Dist: " << chosen_distr_klass_freq << "." << endl;
        }
    }
    if(print_distribution) {
        output << "Best distribution found: " << distr_ptr->get_distribution_name() << " with parameters:" << endl;
        output << distr_ptr->get_parameters_str() << "." << endl;
    }
    if(generate_output) {
        for(unsigned int i = 0; i < generate_output; ++i) {
            output << distr_ptr->generate_value() << endl;
        }
    }
    return EXIT_SUCCESS;
}

void print_help() {
    cout << "Input analyser for statistical purposes." << endl;
    cout << "================================================================================" << endl;
    cout << "Usage:" << endl;
    cout << "--help or -h to print help." << endl;
    cout << "--input_file or -if filename: opens filename for processing, which should" << endl;
    cout << "contain a list of float values. If not supplied, the user can enter numbers by" << endl;
    cout << "hand when the software starts. Please supply 2 different files for input and" << endl;
    cout << "output." << endl;
    cout << "--output_file or -of filename: opens filename to output results. Please supply" << endl;
    cout << "2 different files for input and output." << endl;
    cout << "--generate_random or -gr number: generates number random values using the" << endl;
    cout << "best distribution found." << endl; 
    cout << "--class_count or -cc number: chooses number as the number of classes for monte" << endl;
    cout << "carlo." << endl;
    cout << "--print_classes or -pclasses if the user wants the classes calculated on the" << endl;
    cout << "histogram to be printed." << endl;
    cout << "--print_mean or -pmn if the user wants the mean calculated on the histogram to" << endl;
    cout << "be printed." << endl;
    cout << "--print_variance or --print_var or -pvr if the user wants the variance calculated"<< endl;
    cout << "on the histogram to be printed." << endl;
    cout << "--print_std_deviation or -pstdev if the user wants the standard deviation" << endl;
    cout << "calculated on the histogram to be printed." << endl;
    cout << "--print_mode or -pmd if the user wants the mode calculated on the histogram to" << endl;
    cout << "be printed." << endl;
    cout << "--print_min or -pmin if the user wants the minimum value on the histogram to" << endl; 
    cout << "be printed." << endl;
    cout << "--print_max or -pmax if the user wants the maximum value on the histogram to be" << endl; 
    cout <<" printed." << endl;
    cout << "--print_chi_square_result or -pcs to print best fit distribution chi squared" << endl;
    cout << "test result." << endl;
    cout << "--print_frequency_difference or -pfd to print the difference between the data" << endl;
    cout << "frequency and the distribution frequency." << endl;
    cout << "--no_print_dist or -npd to disable distribution printing." << endl;
    cout << "================================================================================" << endl;
    cout << "The user can supply a list of distributions that he wants to use. The program" << endl;
    cout << "will then select the one which best fits the data. If no option is supplied, the" << endl; 
    cout << "program will just select the best fit among all available distributions." << endl;
    cout << "================================================================================" << endl;
    cout << "To select the distributions, the user should supply these options:" << endl;
    cout << "--normal or -nrm => Normal distribution." << endl;
    cout << "--triangular or -trng => Triangular distribution." << endl;
    cout << "--uniform or -uni => Uniform distribution." << endl;
    cout << "--exponential or -exp => Exponential distribution." << endl;
    cout << "--log_normal or -ln => Lognormal distribution." << endl;
    cout << "--poisson or -psn => Poisson distribution." << endl;
    cout << "================================================================================" << endl;
}
