#include <stdio.h>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <istream>
#include <regex>
#include <sciplot/sciplot.hpp>
#include <ghc/filesystem.hpp>
#include <fdp/fdp.hxx>

class seirsModel {
    public:
        seirsModel(std::string inital_parameters_path,
        double timesteps,
        double years,
        double S,
        double E,
        double I,
        double R);
        void run_seirs_model();
        void write_to_csv(std::string path);
        void plot_model(std::string path, bool show = false);

    private:
        double S_;
        double E_;
        double I_;
        double R_;

        double timesteps_;
        double years_;
        double alpha_;
        double beta_;
        double inv_gamma_;
        double inv_omega_;
        double inv_mu_;
        double inv_sigma_;

        std::vector<double> time_points_, S_data_, E_data_, I_data_, R_data_;

        void validate_initial_parameters(std::map<std::string, double> &initial_state);
        void read_initial_parameters(std::string path);
};

double round_3dp(double var);
std::vector<double> to_percent(std::vector<double> &input);
void run_local(std::string inital_parameters_path,
    std::string csv_output_path,
    std::string figure_output_path);

void run_ci();

std::string getEnvVar( std::string const & key );