cmake_minimum_required(VERSION 3.10)

# set the project name
project(scorep_plugin_nvml)

add_subdirectory(lib/scorep_plugin_cxx_wrapper)

add_library(nvml_plugin MODULE src/plugin.cpp)

target_include_directories(nvml_plugin PUBLIC include)

target_compile_features(nvml_plugin PUBLIC cxx_std_17)

target_link_libraries(nvml_plugin PUBLIC Scorep::scorep-plugin-cxx)

install(
    TARGETS nvml_plugin
    LIBRARY DESTINATION lib
)