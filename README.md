# Simple Model Example for C++
[![cppSimpleModel](https://github.com/FAIRDataPipeline/cppSimpleModel/actions/workflows/cpp_simple_model.yaml/badge.svg)](https://github.com/FAIRDataPipeline/cppSimpleModel/actions/workflows/cpp_simple_model.yaml)
[![License: GPL-3.0](https://img.shields.io/badge/licence-GPL--3-3891A6)](https://opensource.org/licenses/GPL-3.0)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.5878251.svg)](https://doi.org/10.5281/zenodo.5878251)

## Contents
  - [Installation](#installation)
  - [Compiling](#compiling)
  - [Usage](#usage)

This simple model application runs a simple [SEIRS epidemiology model](https://www.nature.com/articles/s41592-020-0856-2) and registers the metadata of the model run on the FAIR Data Pipeline,
using the [cppDataPipeline](https://github.com/FAIRDataPipeline/cppDataPipeline) API.

## Installation
The simpleModel executable can be compiled using cmake.

The only external dependancies not included is:
[gnuplot](http://www.gnuplot.info/download.html), gnuplot is required to run the executable and should be installed and the gnuplot executable available in the system path.

### gnuplot
gnuplot can be installed using the following commands:

#### Windows
gnuplot can be installed on windows using chocolatey:
```
choco install gnuplot
```
#### Linux
gnuplot can be installed on linux using apt:
```
sudo apt-get install gnuplot
```
#### Mac OS
gnuplot can be installed on mac using homebrew:
```
brew install gnuplot
```
## Compiling
Compile the executable by running:
```
$ cmake -Bbuild
$ cmake --build build
```
**Important** For multi-config compilers such as Visual Studio xcode or ninja the config type needs to be set to `Release` otherwise the API will not build for example:
```
$ cmake -Bbuild
$ cmake --build build --config=Release
```

## Usage
The executable is located in the `build/bin[/release]` directory
The executable is designed to be run in conjuction with the [fair cli](#Fair-CLI):

(From a virtual environment within the repo directory)
#### Fair CLI
The [FAIR commandline interface]("https://github.com/FAIRDataPipeline/FAIR-CLI") is available to install using pip:
```
pip install fair-cli
```
Once installed the model can be run using the following:
### Unix
```
fair init
fair pull data/seirs_config.yaml
fair run data/seirs_config.yaml
```
### Windows
```
fair init
fair pull data/seirs_config_windows.yaml
fair run data/seirs_config_windows.yaml
```

The executable can run standalone and run the SEIRS model taking in a csv with initial parameters: 
### on Unix
```
build/bin/cppSimpleModel data/static_params_SEIRS.csv
```
### or on windows:
```
build/bin/release/cppSimpleModel.exe data\static_params_SEIRS.csv
```
