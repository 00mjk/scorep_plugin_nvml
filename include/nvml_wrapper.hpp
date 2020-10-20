#include <nvml.h>

#include "nvml.h"
#include <stdexcept>
#include <string>

enum nvml_metric {
    CLOCK_MEM,
    CLOCK_SM,
    FAN_SPEED,
    MEMORY_FREE,
    MEMORY_USED,
    MEMORY_TOTAL,
    PCIE_SEND,
    PCIE_RECV,
    POWER_USAGE,
    TEMPERATURE,
    UTILIZATION_GPU,
    UTILIZATION_MEM,
    TOTAL_ENERGY_CONSUMPTION
};

nvml_metric metricname_2_nvmlfunction(std::string metric_name)
{
    nvml_metric metric;
    if (metric_name.compare("clock_mem") == 0) {
        metric = CLOCK_MEM;
    }
    else if (metric_name.compare("clock_sm") == 0) {
        metric = CLOCK_SM;
    }
    else if (metric_name.compare("fanspeed") == 0) {
        metric = FAN_SPEED;
    }
    else if (metric_name.compare("memory_free") == 0) {
        metric = MEMORY_FREE;
    }
    else if (metric_name.compare("memory_used") == 0) {
        metric = MEMORY_USED;
    }
    else if (metric_name.compare("memory_total") == 0) {
        MEMORY_TOTAL;
    }
    else if (metric_name.compare("pcie_send") == 0) {
        metric = PCIE_SEND;
    }
    else if (metric_name.compare("pcie_recv") == 0) {
        metric = PCIE_RECV;
    }
    else if (metric_name.compare("power_usage") == 0) {
        metric = POWER_USAGE;
    }
    else if (metric_name.compare("temperature") == 0) {
        metric = TEMPERATURE;
    }
    else if (metric_name.compare("utilization_gpu") == 0) {
        metric = UTILIZATION_GPU;
    }
    else if (metric_name.compare("utilization_memory") == 0) {
        metric = UTILIZATION_MEM;
    }
    else if (metric_name.compare("total_energy_consumption") == 0) {
        metric = TOTAL_ENERGY_CONSUMPTION;
        std::runtime_error("nvmlDeviceGetTotalEnergyConsumption is not supported yet");
    }
    else {
        std::runtime_error("Unknown metric: " + metric_name);
    }

    return metric;
}

unsigned int get_value(nvml_metric metric, nvmlDevice_t& device)
{
    unsigned int value;
    nvmlReturn_t ret;
    if (metric == CLOCK_MEM) {
        ret = nvmlDeviceGetClockInfo(device, nvmlClockType_t::NVML_CLOCK_MEM, &value);
    }
    else if (metric == CLOCK_SM) {
        ret = nvmlDeviceGetClockInfo(device, nvmlClockType_t::NVML_CLOCK_SM, &value);
    }
    else if (metric == FAN_SPEED) {
        ret = nvmlDeviceGetFanSpeed(device, &value);
    }
    else if (metric == MEMORY_FREE) {
        nvmlMemory_t mem;
        ret = nvmlDeviceGetMemoryInfo(device, &mem);
        value = mem.free;
    }
    else if (metric == MEMORY_USED) {
        nvmlMemory_t mem;
        ret = nvmlDeviceGetMemoryInfo(device, &mem);
        value = mem.used;
    }
    else if (metric == MEMORY_TOTAL) {
        nvmlMemory_t mem;
        ret = nvmlDeviceGetMemoryInfo(device, &mem);
        value = mem.total;
    }
    else if (metric == PCIE_SEND) {
        ret = nvmlDeviceGetPcieThroughput(
            device, nvmlPcieUtilCounter_t::NVML_PCIE_UTIL_TX_BYTES, &value);
    }
    else if (metric == PCIE_RECV) {
        ret = nvmlDeviceGetPcieThroughput(
            device, nvmlPcieUtilCounter_t::NVML_PCIE_UTIL_RX_BYTES, &value);
    }
    else if (metric == POWER_USAGE) {
        ret = nvmlDeviceGetPowerUsage(device, &value);
    }
    else if (metric == TEMPERATURE) {
        ret = nvmlDeviceGetTemperature(
            device, nvmlTemperatureSensors_t::NVML_TEMPERATURE_GPU, &value);
    }
    else if (metric == UTILIZATION_GPU) {
        nvmlUtilization_t util;
        ret = nvmlDeviceGetUtilizationRates(device, &util);
        value = util.gpu;
    }
    else if (metric == UTILIZATION_MEM) {
        nvmlUtilization_t util;
        ret = nvmlDeviceGetUtilizationRates(device, &util);
        value = util.memory;
    }
    else if (metric == TOTAL_ENERGY_CONSUMPTION) {
        std::runtime_error("nvmlDeviceGetTotalEnergyConsumption is not supported yet");
        //ret = nvmlDeviceGetTotalEnergyConsumption(device, &value);
    }

    if (NVML_SUCCESS != ret) {
        throw std::runtime_error("Could not fetch data from NVML. Error Code: " +
                                 std::string(nvmlErrorString(ret)));
    }

    return value;
}