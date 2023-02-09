# Find and add the .cxx files to SRC_FILES
FILE( GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx )

# add the executable
add_executable(cppSimpleModel ${SRC_FILES})

IF(WIN32)
    add_definitions(-DNOMINMAX)    
    add_definitions(-DNOGDI)
ENDIF()

find_package(ghc_filesystem)
if(NOT ghc_filesystem_FOUND)

    set(GHC_URL "https://github.com/gulrak/filesystem/archive/refs/tags/v1.5.10.zip")

    message( STATUS "[GHC]" )
    message( STATUS "\tGHC Will be installed." )
    message( STATUS "\tURL: ${GHC_URL}" )

    include(FetchContent)
    set(GHC_FILESYSTEM_WITH_INSTALL ON CACHE INTERNAL "Create import targets for ghc-filesystem")
    FetchContent_Declare(
        GHC
        URL ${GHC_URL}
    )
    FetchContent_MakeAvailable(GHC)

endif()

if(FDPAPI_NO_FETCHCONTENT)
    find_package(fdpapi REQUIRED)
    message(STATUS "fdpapi found")
else()
    find_package(fdpapi)
    if(NOT fdpapi_FOUND)

        SET( FDPAPI_URL "https://github.com/FAIRDataPipeline/cppDataPipeline/archive/refs/${CPPDATAPIPELINEREF}.zip" )

        MESSAGE( STATUS "[FDPAPI]" )
        MESSAGE( STATUS "\tFDPAPI Will be installed." )
        MESSAGE( STATUS "\tURL: ${FDPAPI_URL}" )

        include(FetchContent)
        FetchContent_Declare(
            FDPApi
            URL ${FDPAPI_URL}
        )
        FetchContent_MakeAvailable(FDPApi)
    endif()
endif()

SET( SCIPLOT_URL "https://github.com/RyanJField/sciplot/archive/refs/heads/master.zip" )

MESSAGE( STATUS "[SCIPLOT]" )
MESSAGE( STATUS "\tSCIPLOT Will be installed." )
MESSAGE( STATUS "\tURL: ${SCIPLOT_URL}" )

set(SCIPLOT_BUILD_TESTS OFF CACHE INTERNAL "Turn off sciplot tests")
set(SCIPLOT_BUILD_EXAMPLES OFF CACHE INTERNAL "Turn off sciplot examples")
set(SCIPLOT_BUILD_DOCS OFF CACHE INTERNAL "Turn off sciplot docs")

include(FetchContent)
FetchContent_Declare(
    SCIPLOT
    URL ${SCIPLOT_URL}
)

FetchContent_MakeAvailable(SCIPLOT)

TARGET_INCLUDE_DIRECTORIES( ${CPPSIMPLEMODEL} PRIVATE ${CPPSIMPLEMODEL_INCLUDE_DIRS} )

# Ensure Compatibility with older version of fdpapi
if(NOT fdpapi_FOUND)
    target_include_directories(${CPPSIMPLEMODEL} PRIVATE ${fdpapi_SOURCE_DIR}/include)
    TARGET_LINK_LIBRARIES( ${CPPSIMPLEMODEL} PRIVATE fdpapi )
else()
    TARGET_LINK_LIBRARIES( ${CPPSIMPLEMODEL} PRIVATE fdpapi::fdpapi )
endif()

TARGET_LINK_LIBRARIES( ${CPPSIMPLEMODEL} PRIVATE sciplot ) 
TARGET_LINK_LIBRARIES( ${CPPSIMPLEMODEL} PRIVATE ghcFilesystem::ghc_filesystem)

# Install the libraries
INSTALL( TARGETS ${CPPSIMPLEMODEL} 
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
