cmake_minimum_required(VERSION 3.10)

#  Disable Building in Debug as HD5 
get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
IF(isMultiConfig)
    set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)
    MESSAGE("Multi-Config Generator found, please ensure you are building with a realease configuration")
ELSE()
    if(NOT CMAKE_BUILD_TYPE)
        message("Defaulting to release build.")
        set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
    ENDIF()
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY HELPSTRING "Choose the type of build")
    # set the valid options for cmake-gui drop-down list
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release")
ENDIF()

if(NOT DEFINED CPPDATAPIPELINEREF)
    SET(CPPDATAPIPELINEREF "tags/v0.2.7")
endif()


# set the project name
project(cppSimpleModel
    VERSION 0.1.1
    DESCRIPTION "C++ Simple Model for the FAIR Data Pipeline"
    HOMEPAGE_URL "https://github.com/FAIRDataPipeline/cppSimpleModel"
    LANGUAGES CXX C
)

set( CPPSIMPLEMODEL cppSimpleModel )

# Set Output Directories to avoid issues on multi release compilers
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include External Project and GNUInstallDirs - For Installing Libries
INCLUDE( ExternalProject )
INCLUDE( GNUInstallDirs )

# Set C++ Standard to 17
SET( CMAKE_CXX_STANDARD 17 )
SET( CMAKE_CXX_STANDARD_REQUIRED ON )

# Set Include and Header Variables
SET( CPPSIMPLEMODEL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include )
SET( CPPSIMPLEMODEL_HEADER ${FDPAPI_INCLUDE_DIRS}/simpleModel.hxx )

# Add the src dirctory compiling it's CMakeLists.txt
ADD_SUBDIRECTORY( src )

