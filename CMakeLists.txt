
cmake_minimum_required(VERSION 2.8) # Check CMake version

# Set source files
set(SOURCE src/main.cpp src/Size.cpp src/Config.cpp src/ImageResizer.cpp src/ImageResizerMagick.cpp)

# Set executable output path
set(EXECUTABLE_OUTPUT_PATH bin)

# Set additional modules path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

# Find packages
##########################################################
find_package(Boost COMPONENTS program_options filesystem system REQUIRED)
if(NOT Boost_FOUND)
    message(SEND_ERROR "Failed to find required boost libraries.")
    return()
else()
    include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(GraphicsMagick REQUIRED)
if(NOT GraphicsMagick_FOUND)
	message(SEND_ERROR "Failed to find GraphicsMagick libraries.")
	return()
else()
	include_directories(${GraphicsMagick_INCLUDE_DIRS})
endif()

##########################################################


# Setup include directories
include_directories(src inc)

# Create executable from sources
add_executable(phresizer ${SOURCE})

# Link libraries
target_link_libraries(phresizer ${Boost_LIBRARIES} ${GraphicsMagick_LIBRARIES})

# Install command 
INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/bin/phresizer DESTINATION /usr/local/bin)