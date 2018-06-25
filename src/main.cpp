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
#include "datahistogram.h"
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

void print_help(std::ostream&);

//Just argument parsing in this file and setting up the system.

int main(int argc, char **argv) {
    DataHolder h;
    istream* stream_ptr = &cin;
    ostream* ostream_ptr = &cout;
    std::string out_file_name;
    ifstream file;
    set<DistributionType> desired_distributions;
    bool print_var = false;
    bool print_std_deviation = false;
    bool print_mean = false;
    bool print_mode = false;
    bool print_min = false;
    bool print_max = false;
    bool print_chi_square_result = false;
    bool print_frequency_difference = false;
    bool print_distribution = true;
    bool print_histogram = false;
    unsigned int generate_output = 0;
    unsigned int class_count = 0;
    for(int i = 1; i < argc; ++i) {
        string cur_arg(argv[i]);
        if(cur_arg == "--help" || cur_arg == "-h") {
            print_help(std::cout);
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
            out_file_name = argv[i];
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
        else if(cur_arg == "--print_histogram" || cur_arg == "-ph") {
            print_histogram = true;
        }
        else if(cur_arg == "--print_all" || cur_arg == "-pa") {
            print_chi_square_result = true;
            print_frequency_difference = true;
            print_histogram = true;
            print_max = true;
            print_min = true;
            print_mode = true;
            print_std_deviation = true;
            print_var = true;
        }
        else {
            cerr << "Invalid argument " << cur_arg << "." << endl;
            cerr << "Printing help:" << endl;
            print_help(std::cerr);
            return EXIT_FAILURE;
        }
    }
    istream& input = *stream_ptr;
    while(input.peek() != EOF) {
        input >> h;
        if(input.fail()) {
            input.clear();
            input.ignore();
        }
    }
    if(h.begin() == h.end()) {
        cerr << "Can't process empty data. Please supply floating point values for processing." << endl;
        return EXIT_FAILURE;
    }
    auto amount_of_data = distance(h.begin(), h.end());
    if(class_count > amount_of_data) {
        cerr << "Too many classes for amount of data. Classes: " << class_count << " Data: " << amount_of_data << endl;
        cerr << "Falling back to default." << endl;
        class_count = 0;
    }
    unique_ptr<Distribution> distr_ptr;
    input_data_t chi_result;
    tie(distr_ptr, chi_result) = create_distribution(h, desired_distributions, class_count);
    DataHistogram monte_carlo = h.generate_histogram(class_count);
    if(file.is_open()) {
        file.close();
    }
    ofstream out_file;
    if(!out_file_name.empty()) {
        out_file.open(out_file_name);
        if(!out_file) {
            cerr << "Error opening " << out_file_name << "." << endl;
            return EXIT_FAILURE;
        }
        ostream_ptr = &out_file;
    }
    ostream& output = *ostream_ptr;
    output.precision(numeric_limits<input_data_t>::max_digits10);
    if(print_histogram) {
        output << monte_carlo.print_classes() << endl;
        output << "Histogram mean: " << monte_carlo.histogram_mean() << "." << endl;
        output << "Histogram variance: " << monte_carlo.histogram_variance() << "." << endl;
        output << "Histogram standard deviation: " << monte_carlo.histogram_standard_deviation() << "." << endl;
        output << "Histogram min value: " << monte_carlo.histogram_min_value() << "." << endl;
        output << "Histogram max value: " << monte_carlo.histogram_max_value() << "." << endl;
    }
    if(print_mean) {
        output << "Data mean " << h.mean() << "." << endl;
    }
    if(print_var) {
        output << "Data variance " << h.variance() << "." << endl;
    }
    if(print_std_deviation) {
        output << "Data standard deviation " << h.standard_deviation() << "." << endl;
    }
    if(print_mode) {
        output << "Data mode: " << monte_carlo.histogram_mode() << "." << endl;
    }
    if(print_min) {
        output << "Data min value " << h.min() << "." << endl;
    }
    if(print_max) {
        output << "Data max value " << h.max() << "." << endl;
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

void print_help(std::ostream& os) {
    os << "Input analyser for statistical purposes." << endl;
    os << "================================================================================" << endl;
    os << "Usage:" << endl;
    os << "--help or -h to print help." << endl;
    os << "--input_file or -if filename: opens filename for processing, which should" << endl;
    os << "contain a list of float values. If not supplied, the user can enter numbers by" << endl;
    os << "hand when the software starts. When done typing numbers, just press ^D twice." << endl;
    os << "--output_file or -of filename: opens filename to output results." << endl;
    os << "--generate_random or -gr number: generates number random values using the" << endl;
    os << "best distribution found." << endl; 
    os << "--class_count or -cc number: chooses number as the number of classes for monte" << endl;
    os << "carlo." << endl;
    os << "--print_histogram or -ph if the user wants the classes calculated on the" << endl;
    os << "histogram to be printed." << endl;
    os << "--print_mean or -pmn if the user wants the mean calculated on the histogram to" << endl;
    os << "be printed." << endl;
    os << "--print_variance or --print_var or -pvr if the user wants to print the variance." << endl;
    os << "--print_std_deviation or -pstdev if the the user wants to print the standard" << endl; 
    os << "deviation." << endl;
    os << "--print_mode or -pmd if the user wants to print the mode." << endl;
    os << "--print_min or -pmin if the user wants to print the minimum value." << endl;
    os << "--print_max or -pmax if the user wants to print the maximum value." << endl; 
    os << "--print_chi_square_result or -pcs to print best fit distribution chi squared" << endl;
    os << "test result." << endl;
    os << "--print_frequency_difference or -pfd to print the difference between the data" << endl;
    os << "frequency and the distribution frequency." << endl;
    os << "--print_all or -pa to print everything. Even if this option is set, it is still" << endl; 
    os << "possible to override the print distribution value." << endl;
    os << "--no_print_dist or -npd to disable distribution printing." << endl;
    os << "================================================================================" << endl;
    os << "The user can supply a list of distributions that he wants to use. The program" << endl;
    os << "will then select the one which best fits the data. If no option is supplied, the" << endl; 
    os << "program will just select the best fit among all available distributions." << endl;
    os << "================================================================================" << endl;
    os << "To select the distributions, the user should supply these options:" << endl;
    os << "--normal or -nrm => Normal distribution." << endl;
    os << "--triangular or -trng => Triangular distribution." << endl;
    os << "--uniform or -uni => Uniform distribution." << endl;
    os << "--exponential or -exp => Exponential distribution." << endl;
    os << "--log_normal or -ln => Lognormal distribution." << endl;
    os << "--poisson or -psn => Poisson distribution." << endl;
    os << "================================================================================" << endl;
}
