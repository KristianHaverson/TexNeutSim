#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.21)
project(TexNeutSim)

#----------------------------------------------------------------------------
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Find ROOT package
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIR})

# Set compiler flags for ROOT
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

# Display ROOT include and library directories
message(STATUS "ROOT Include Directories: ${ROOT_INCLUDE_DIRS}")
message(STATUS "ROOT Library Directory: ${ROOT_LIBRARY_DIR}")

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
include_directories(${PROJECT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIR} ${ROOT_INCLUDE_DIRS})
file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.cc")
file(GLOB headers "${PROJECT_SOURCE_DIR}/include/*.hh")

#----------------------------------------------------------------------------
# Add the executable and link it to the Geant4 and ROOT libraries
add_executable(TexNeutSim TexNeutSim.cc ${sources} ${headers})
target_link_libraries(TexNeutSim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory for runtime execution
set(TexNeutSim_SCRIPTS vis.mac)

foreach(_script ${NeutronSource_SCRIPTS})
    configure_file(
        ${PROJECT_SOURCE_DIR}/${_script}
        ${PROJECT_BINARY_DIR}/${_script}
        COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
install(TARGETS TexNeutSim DESTINATION bin)






