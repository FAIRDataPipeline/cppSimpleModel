# Simple Model Example for C++

## Contents
    - [Installation](#installation)
    - [Usage](#usage)

## Installation
The simpleModel executable can be compile using cmake, the only external dependancy not included is [gnuplot](http://www.gnuplot.info/download.html), gnuplot is required to run the executable and should be installed and the gnuplot executable available in the system path.

Compile the executable by running:

```
$ cmake -Bbuild
$ cmake --build build
```

**Important** For multi-config compilers such as Visual Studio xcode or ninja the config type needs to be set to `Release` otherwise the API will not build for example:
```
$ cmake -Bbuild -DFDPAPI_BUILD_TESTS=ON
$ cmake --build build --config=Release
```

## Usage
The executable is located in the build/bin\[/release\] directory

The executable is designed to be run independently using the `--local` flag and taking one parameter:

## on Unix
```
./cppSimpleModel --local "/users/ryan/cppSimpleModel\data\static_params_SEIRS.csv"
```
## or on windows:
```
cppSimpleModel.exe --local "C:\Users\Ryan\Documents\CPP\cppSimpleModel\data\static_params_SEIRS.csv"
```

Or conjuction with the fair cli like so:
(From a fair cli poetry shell within the repo)
## Unix
```
fair init --ci
fair pull data\seirs_config.yaml
fair run data\seirs_config.yaml
```
## Windows
```
fair init --ci
fair pull data\seirs_config_windows.yaml
fair run data\seirs_config_windows.yaml
```
