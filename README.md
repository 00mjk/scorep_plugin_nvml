# Score-P NVML Plugin
This code provides a Score-P plugin to access [NVIDIA Management Library (NVML)](https://developer.nvidia.com/nvidia-management-library-nvml)

## Installation
If not present on your system get NVML from [NVIDIA Management Library (NVML)](https://developer.nvidia.com/nvidia-management-library-nvml), install it with
`./gdk_linux_*_release.run --installdir=<PATH> --silent`
and setup paths

```
git clone --recurse-submodules git@github.com:NanoNabla/scorep_plugin_nvml.git
cd scorep_plugin_nvml
mkdir build && cd build
cmake ../
make

# copy libnvml*_plugin.so into your LD_LIBRARY_PATH
```


## Usage
Sampling plugin seems to be more efficient than async (which also uses a way of sampling) or sync plugin but supports less metrics

> The advantage of using this method for samples in contrast to polling via existing methods is to get get higher frequency data at lower polling cost. 
> -- <cite>[NVML Docs](https://docs.nvidia.com/deploy/nvml-api/group__nvmlDeviceQueries.html#group__nvmlDeviceQueries_1gb7d2a6d2a9b4584cd985765d1ff46c94) </cite>

### Sampling
- `SCOREP_METRIC_PLUGINS=nvml_sampling_plugin`
- `SCOREP_METRIC_NVML_SAMPLING_PLUGIN="power_usage,temperature"`
    
Optional :
- `SCOREP_METRIC_NVML_SAMPLING_PLUGIN_INTERVAL="5000"` (interval to poll the gpu in milliseconds, default 5000ms, note that this does not effect the interval between two sampling point, which are device specific and differ for different metrics. The maximum stored values on the gpus seems to differ also. Setting `SCOREP_METRIC_NVML_SAMPLING__PLUGIN_INTERVAL` to high will make you loose datapoints.)

#### Available metrics
- `clock_sm`
- `clock_mem`
- `power_usage`
- `utilization_gpu`
- `utilization_mem`

### Async
- `SCOREP_METRIC_PLUGINS=nvml_plugin`
- `SCOREP_METRIC_NVML_SYNC_PLUGIN="power_usage,temperature"`
    
Optional :
- `SCOREP_METRIC_NVML_PLUGIN_INTERVAL="50"` (measurement interval in milliseconds, default 50ms)
    
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
