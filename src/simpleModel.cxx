#include <simpleModel.hxx>

namespace FDP = FairDataPipeline;

int main(int argc, char** argv) {
    try{
        if(argc == 1){
            run_cli();  
        }
        else if(argc > 1)
        {
            if (std::string(argv[1]).substr(0,2) == "--") {
                if (std::string(argv[1]).find("--h") != std::string::npos) {
                    std::cout << "Usage:\
                    \n cppSimpleModel : Run the model using the CLI\
                    \n cppSimpleModel initial_parameters_path : Run the model with the given initial parameters";
                    return 1;
                }
                else{
                    std::cout << "Unknown switch supplied see cppSimpleModel --h";
                    return 1;
                }
            }            
            else if(argc != 2){
                std::cout << "Incorrect Number of parameters supplied see --h";
                return 1;
            }
            else {
                ghc::filesystem::create_directories(ghc::filesystem::path("data_store"));
                run_local(std::string(argv[1]), "data_store/cpp_model_output.csv", "data_store/cpp_figure_output.png");
            }
        }
        else {
            std::cout << "Incorrect usage see --h";
            return 1;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    catch (const char* msg) {
        std::cerr << msg << std::endl;
        return 1;
    }
    catch(...)
    {
        std::cerr << "somthing went wrong";
        return 1;
    }

    return 0;
}

void run_local(std::string initial_parameters_path,
    std::string csv_output_path,
    std::string figure_output_path){
    FDP::logger::get_logger()->info() << "Running SIERS model";

    seirsModel sm = seirsModel(initial_parameters_path,
    1000, 5, 0.999, 0.001, 0, 0);
    sm.run_seirs_model();
    sm.write_to_csv(csv_output_path);
    sm.plot_model(figure_output_path, true);
    }

void run_cli(){
    FDP::logger::get_logger()->info() <<"Reading token";
    std::string token = getEnvVar("FDP_LOCAL_TOKEN");
    ghc::filesystem::path fdp_path = getEnvVar("FDP_CONFIG_DIR");
    ghc::filesystem::path config_path = fdp_path / "config.yaml";
    #ifdef _WIN32
        ghc::filesystem::path script_path = fdp_path / "script.bat";
    #else
        ghc::filesystem::path script_path = fdp_path / "script.sh";
    #endif

    FDP::logger::get_logger()->info() << "FDP Path: {0}" << fdp_path.string();
    FDP::logger::get_logger()->info() << "Config Path: {0}" << config_path.string();
    FDP::logger::get_logger()->info() << "Script Path: {0}" << script_path.string();

    FDP::DataPipeline::sptr datapipeline = FDP::DataPipeline::construct(config_path.string(), script_path.string(), token);

    std::string model_parameters = "SEIRS_model/parameters";
    std::string csv_output = "SEIRS_model/results/model_output/cpp";
    std::string figure_output = "SEIRS_model/results/figure/cpp";

    ghc::filesystem::path initial_parameters_path = datapipeline->link_read(model_parameters);
    ghc::filesystem::path csv_output_path = datapipeline->link_write(csv_output);
    ghc::filesystem::path figure_output_path = datapipeline->link_write(figure_output);

    ghc::filesystem::create_directories(ghc::filesystem::path("data_store"));
    std::string figure_output_path_ = "data_store/fig.png";

    seirsModel sm = seirsModel(initial_parameters_path.string(),
    1000, 5, 0.999, 0.001, 0, 0);
    sm.run_seirs_model();
    sm.write_to_csv(csv_output_path.string());
    sm.plot_model(figure_output_path_, false);

    ghc::filesystem::copy_file(ghc::filesystem::path(figure_output_path_), figure_output_path);

    datapipeline->finalise();

}

seirsModel::seirsModel(std::string inital_parameters_path,
        double timesteps,
        double years,
        double S,
        double E,
        double I,
        double R):
        timesteps_(timesteps), years_(years), S_(S), E_(E), I_(I), R_(R)
        {
            read_initial_parameters(inital_parameters_path);
        }

void seirsModel::validate_initial_parameters(std::map<std::string, double> &initial_state){
    std::string keys;
    std::string required_keys = "alpha,beta,inv_gamma,inv_omega,inv_mu,inv_sigma";
    for( std::map<std::string, double>::iterator it = initial_state.begin(); it!= initial_state.end(); it++){
        keys += it->first;
    }
    std::istringstream iss(required_keys);
    std::string current_parameter;
    while (getline(iss, current_parameter, ',')){
        if (keys.find(current_parameter) == std::string::npos) {
            FDP::logger::get_logger()->error() << "Required parameter not found: {0}" << current_parameter ;
            throw "Required parameter not found: " + current_parameter ;
        }
    }
}

void seirsModel::read_initial_parameters(std::string path)
{
    std::cout << "\nReading parameters from: " + path;
    std::ifstream csvFile;
    csvFile.open(path.c_str());

    if (!csvFile.is_open())
    {
        FDP::logger::get_logger()->error() << "Could not open initial parameters from {0}" << path;
        throw "Could not open initial parameters";
    }

    std::string line;
    std::vector <std::string> vec;
    getline(csvFile, line); // skip the 1st line

    std::map<std::string, double> values;

    while (getline(csvFile,line))
    {
        std::istringstream iss(line);
        std::string lineStream;
        std::string::size_type sz;

        std::vector<std::string> kv;
        while (getline(iss, lineStream, ',')){
            kv.push_back(lineStream);
            //std::cout << "\nCurrent LN" + lineStream;
        }
        try {
            std::string key = std::regex_replace(kv[0], std::regex(std::string("\\\"")), "");
            values[key] = std::stod(kv[1]);
            //std::cout << "\nadded value: " + kv[1] + " to: " + kv[0];
        }
        catch (...) {
            FDP::logger::get_logger()->warn() << "Warning Could not read parameter on {0}" << lineStream;
        }      
    }

    validate_initial_parameters(values);
    
    alpha_ = values["alpha"];
    beta_ = values["beta"];
    inv_gamma_ = values["inv_gamma"];
    inv_omega_ = values["inv_omega"];
    inv_mu_ = values["inv_mu"];
    inv_sigma_ = values["inv_sigma"];

    std::cout << "\nParameters read successfully";

}

void seirsModel::run_seirs_model(){

    // Prepare Time Units
    double time_unit_years = years_ / timesteps_;
    double time_unit_days = time_unit_years * 365.25;

    // Convert Parameters to days
    double alpha = alpha_ * time_unit_days;
    double beta = beta_ * time_unit_days;
    double gamma = time_unit_days / inv_gamma_;
    double omega = time_unit_days / (inv_omega_ * 365.25);
    double mu = time_unit_days / (inv_mu_ * 365.25);
    double sigma = time_unit_days / inv_sigma_;

    time_points_.push_back(0);
    S_data_.push_back(S_);
    E_data_.push_back(E_);
    I_data_.push_back(I_);
    R_data_.push_back(R_);

    for(int i = 0; i < timesteps_; i++){
        double N = S_data_[i] +
            E_data_[i] +
            I_data_[i] +
            R_data_[i];

        double birth = mu * N;
        double infection = (beta * I_data_[i] * S_data_[i]) / N;
        double lost_immunity = omega * R_data_[i];
        double death_S = mu * S_data_[i];
        double death_E = mu * E_data_[i];
        double death_I = (mu + alpha) * I_data_[i];
        double death_R = mu * R_data_[i];
        double latency = sigma * E_data_[i];
        double recovery = gamma * I_data_[i];

        double S_rate = birth - infection + lost_immunity - death_S;
        double E_rate = infection - latency - death_E;
        double I_rate = latency - recovery - death_I;
        double R_rate = recovery - lost_immunity - death_R;

        int row = i + 1;
        double time_point_ = round_3dp(row * time_unit_years);

        time_points_.push_back(time_point_);
        S_data_.push_back(S_data_[i] + S_rate);
        E_data_.push_back(E_data_[i] + E_rate);
        I_data_.push_back(I_data_[i] + I_rate);
        R_data_.push_back(R_data_[i] + R_rate);

    }

}

void seirsModel::write_to_csv(std::string path){
    FDP::logger::get_logger()->info() << "Writing CSV to: {0}" << path;
    std::ofstream writeCSV;
    writeCSV.open(path);

    writeCSV << "time,S,E,I,R\n";

    //std::cout << "\nTime Points: " + std::to_string(time_points_.size());

    for (int row = 0; row < time_points_.size(); row++){
        writeCSV << time_points_[row];
        writeCSV << ",";
        writeCSV << S_data_[row];
        writeCSV << ",";
        writeCSV << E_data_[row];
        writeCSV << ",";
        writeCSV << I_data_[row];
        writeCSV << ",";
        writeCSV << R_data_[row];
        writeCSV << "\n";
    }
    writeCSV.close();

    FDP::logger::get_logger()->info() << "CSV Written Succesfuly to: {0}" << path;

}

void seirsModel::plot_model(std::string path, bool show){
    FDP::logger::get_logger()->info() << "Generating plot";
    sciplot::Plot plot;

    // Set the x and y labels
    plot.xlabel("Time (Years)");
    plot.ylabel("Relative Group Size (%)");

    // plot.legend()
    //     .atTop()
    //     .fontSize(10)
    //     .displayHorizontal()
    //     .displayExpandWidthBy(2);

    plot.drawCurve(time_points_, to_percent(S_data_)).label("S");
    plot.drawCurve(time_points_, to_percent(E_data_)).label("E");
    plot.drawCurve(time_points_, to_percent(I_data_)).label("I");
    plot.drawCurve(time_points_, to_percent(R_data_)).label("R");

    sciplot::Figure fig = {{plot}};
    fig.title("SEIRS Model Trajectories\
     R₀=3, 1/γ=14 Days, 1/σ=7 Days, 1/ω=1 Year");
    fig.size(1024, 768);

    if(show){
        fig.show();
        //plot.show();
    }
    FDP::logger::get_logger()->info() << "Saving plot to path {0}" << path;
    fig.save(path);
    //plot.save(path);

}

double round_3dp(double var)
{
    double value = (int)(var * 1000 + .5);
    return (double)value / 1000;
}

std::vector<double> to_percent(std::vector<double> &input){
    std::vector<double> output;
    for(double i: input){
        output.push_back(i * 100);
    }
    return output;
}

std::string getEnvVar( std::string const & key )
{
    char * val = getenv( key.c_str() );
    return val == NULL ? std::string("") : std::string(val);
}