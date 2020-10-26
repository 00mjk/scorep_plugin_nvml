# Score-P NVML Plugin
This code provides a Score-P plugin to access [NVIDIA Management Library (NVML)](https://developer.nvidia.com/nvidia-management-library-nvml)

## Installation
Get NVML from [NVIDIA Management Library (NVML)](https://developer.nvidia.com/nvidia-management-library-nvml) and istall it with
`./gdk_linux_*_release.run --installdir=<PATH> --silent`
Set Paths
```
    git clone --recurse-submodules git@github.com:NanoNabla/scorep_plugin_nvml.git
    cd scorep_plugin_nvml
    mkdir build && cd build
    cmake ../
    make
```


## Usage

## Environment Variables
INTERVAL = Interval in milliseconds
## Developer note 
Current `nvml.h` can be found under 
https://github.com/NVIDIA/nvidia-settings/blob/master/src/nvml.h
