cmake_minimum_required(VERSION 3.10)

# set the project name
project(scorep_plugin_nvml)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(NVML REQUIRED)

add_subdirectory(lib/scorep_plugin_cxx_wrapper)

add_library(nvml_plugin MODULE src/nvml_plugin.cpp)
# should be removed?
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories(nvml_plugin PUBLIC include)

target_compile_features(nvml_plugin PUBLIC cxx_std_14)

target_link_libraries(nvml_plugin PUBLIC Scorep::scorep-plugin-cxx nvidia-ml)

install(
    TARGETS nvml_plugin
    LIBRARY DESTINATION lib
)