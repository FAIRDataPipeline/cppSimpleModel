# Simple Model Example for C++
[![cppSimpleModel](https://github.com/FAIRDataPipeline/cppSimpleModel/actions/workflows/cpp_simple_model.yaml/badge.svg)](https://github.com/FAIRDataPipeline/cppSimpleModel/actions/workflows/cpp_simple_model.yaml)
[![License: GPL-3.0](https://img.shields.io/badge/licence-GPL--3-3891A6)](https://opensource.org/licenses/GPL-3.0)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.5879292.svg)](https://doi.org/10.5281/zenodo.5879292)

## Contents
  - [Installation](#installation)
  - [Compiling](#compiling)
  - [Usage](#usage)

This simple model application runs a simple [SEIRS epidemiology model](https://www.nature.com/articles/s41592-020-0856-2) and registers the metadata of the model run on the FAIR Data Pipeline,
using the [cppDataPipeline](https://github.com/FAIRDataPipeline/cppDataPipeline) API.

## Installation
The simpleModel executable can be compiled using cmake.

The two external dependancies not included are:
[gnuplot](http://www.gnuplot.info/download.html), gnuplot is required to run the executable and should be installed and the gnuplot executable available in the system path.

And [HDF5](https://support.hdfgroup.org/ftp/HDF5/current/src/), this should be installed and the HDF5_ROOT environment variable defined.
### HDF5
#### Windows
HDF5 can be installed for windows 10 64bit windows using a precompiled installer available [here](https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.12/hdf5-1.12.1/bin/windows/hdf5-1.12.1-Std-win10_64-vs14.zip)

Or by compiling it using the [source code](https://www.hdfgroup.org/downloads/hdf5/source-code/)

The HDF5_ROOT should then be set to the installation directory eg.:`C:\Program Files (x86)\HDF_Group\HDF5\1.12.1`

** Note: The architecture of your compiler should match the installation of HDF5.
#### Linux
HDF5 can be install on linux through APT:
```
sudo apt-get install libhdf5-dev
```
#### Mac OS
HDF5 can be installed on mac using homebrew:
```
brew install hdf5
```
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
fair init --ci
fair pull data/seirs_config.yaml
fair run data/seirs_config.yaml
```
### Windows
```
fair init --ci
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
build/bin/release/cppSimpleModel.exe data\static_params_SEIRS.csv"
```
