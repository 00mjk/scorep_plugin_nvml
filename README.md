# Score-P NVML Plugin
This code provides a Score-P plugin to access [NVIDIA Management Library (NVML)](https://developer.nvidia.com/nvidia-management-library-nvml)

## Installation
If not present on your system get NVML from [NVIDIA Management Library (NVML)](https://developer.nvidia.com/nvidia-management-library-nvml) and istall it with
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

### Async
- `SCOREP_METRIC_PLUGINS=nvml_plugin`
- `SCOREP_METRIC_NVML_SYNC_PLUGIN="power_usage,temperature"`
    
Optional :
- `SCOREP_METRIC_NVML_PLUGIN_INTERVAL="3"` (Interval in milliseconds)
    
### Sync Plugin

- `SCOREP_METRIC_PLUGINS=nvml_plugin` 
- `SCOREP_METRIC_NVML_SYNC_PLUGIN="power_usage,temperature"`

#### Available metrics
- `clock_sm`
- `clock_mem`
- `fan_speed`
- `mem_free`
- `mem_used`
- `pcie_send`
- `pcie_recv`
- `power_usage`
- `temperature`
- `utilization_gpu`
- `utilization_mem`


## Developer note 
Current `nvml.h` can be found under 
https://github.com/NVIDIA/nvidia-settings/blob/master/src/nvml.h
