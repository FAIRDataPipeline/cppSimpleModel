# Find and add the .cxx files to SRC_FILES
FILE( GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx )

# add the executable
add_executable(cppSimpleModel ${SRC_FILES})

IF(WIN32)
    add_definitions(-DNOMINMAX)
ENDIF()

# Use HDF5 Static Libraries on Windows
if ( WIN32 )
    SET (HDF5_USE_STATIC_LIBRARIES TRUE)
ENDIF()

# Find HDF5 Package
find_package(HDF5 COMPONENTS C CXX)

# If the HDF5 Package is not found stop
if (NOT HDF5_FOUND)
    message( FATAL_ERROR "HDF5 Library Not Found, 
        Please install it and ensure the Environment Variable HDF5_ROOT is set")
ENDIF()

SET( FDPAPI_URL "https://github.com/FAIRDataPipeline/cppDataPipeline/archive/refs/heads/master.zip" )

MESSAGE( STATUS "[FDPAPI]" )
MESSAGE( STATUS "\tFDPAPI Will be installed." )
MESSAGE( STATUS "\tURL: ${FDPAPI_URL}" )

include(FetchContent)
FetchContent_Declare(
    FDPApi
    URL ${FDPAPI_URL}
)
FetchContent_MakeAvailable(FDPApi)

SET( SCIPLOT_URL "https://github.com/RyanJField/sciplot/archive/refs/heads/master.zip" )

MESSAGE( STATUS "[SCIPLOT]" )
MESSAGE( STATUS "\tSCIPLOT Will be installed." )
MESSAGE( STATUS "\tURL: ${SCIPLOT_URL}" )

include(FetchContent)
FetchContent_Declare(
    SCIPLOT
    URL ${SCIPLOT_URL}
)

FetchContent_MakeAvailable(SCIPLOT)

TARGET_INCLUDE_DIRECTORIES( ${CPPSIMPLEMODEL} PUBLIC ${sciplot_SOURCE_DIR}/sciplot )
TARGET_INCLUDE_DIRECTORIES( ${CPPSIMPLEMODEL} PRIVATE ${CPPSIMPLEMODEL_INCLUDE_DIRS} )
TARGET_INCLUDE_DIRECTORIES( ${CPPSIMPLEMODEL} PUBLIC ${fdpapi_SOURCE_DIR}/include )

TARGET_LINK_LIBRARIES( ${CPPSIMPLEMODEL} PUBLIC fdpapi ) 
TARGET_LINK_LIBRARIES( ${CPPSIMPLEMODEL} PUBLIC sciplot ) 

# Install the libraries
INSTALL( TARGETS ${CPPSIMPLEMODEL} 
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)