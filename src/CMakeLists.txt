# Find and add the .cxx files to SRC_FILES
FILE( GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx )

# add the executable
add_executable(cppSimpleModel simpleModel.cxx)

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

TARGET_INCLUDE_DIRECTORIES( ${CPPSIMPLEMODEL} PRIVATE ${CPPSIMPLEMODEL_INCLUDE_DIRS} )
TARGET_INCLUDE_DIRECTORIES( ${CPPSIMPLEMODEL} PUBLIC ${fdpapi_SOURCE_DIR}/include )

TARGET_LINK_LIBRARIES( ${CPPSIMPLEMODEL} PUBLIC fdpapi ) 

# Install the libraries
INSTALL( TARGETS ${CPPSIMPLEMODEL} 
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)