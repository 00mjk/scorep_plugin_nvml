#ifndef SCOREP_PLUGIN_NVML_NVML_WRAPPER_HPP
#define SCOREP_PLUGIN_NVML_NVML_WRAPPER_HPP

#include <nvml.h>

#include <stdexcept>
#include <string>
#include <vector>

enum metric_measure_type { ABS, REL, ACCU };

enum metric_datatype { INT, UINT, DOUBLE };

using pair_time_sampling_t = std::pair<unsigned long long, std::uint64_t>;

inline static void check_nvml_ret(nvmlReturn_t ret)
{
    if (NVML_SUCCESS != ret) {
        throw std::runtime_error(
            "Could not fetch data from NVML. Error Code: " +
            std::string(nvmlErrorString(ret)));
    }
}

class Nvml_Metric {
public:
    virtual unsigned int get_value(nvmlDevice_t& device) = 0;

    const std::string& getDesc() const
    {
        return desc;
    }

    const std::string& getUnit() const
    {
        return unit;
    }

    const metric_measure_type getMeasureType() const
    {
        return type;
    }

    const metric_datatype getDatatype() const
    {
        return datatype;
    }

protected:
    const std::string desc;
    const std::string unit;
    metric_measure_type type;
    metric_datatype datatype;

protected:
    unsigned int value;
};

class Power : public Nvml_Metric {
public:
    unsigned int get_value(nvmlDevice_t& device)
    {
        nvmlReturn_t ret = nvmlDeviceGetPowerUsage(device, &value);
        check_nvml_ret(ret);

        return value;
    }

protected:
    const std::string desc = "Power Consumption";
    const std::string unit = "mW";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;
};

class Temperature : public Nvml_Metric {
public:
    unsigned int get_value(nvmlDevice_t& device)
    {
        nvmlReturn_t ret = nvmlDeviceGetTemperature(
            device, nvmlTemperatureSensors_t::NVML_TEMPERATURE_GPU, &value);
        check_nvml_ret(ret);

        return value;
    }

protected:
    const std::string desc = "Board Temperature";
    const std::string unit = "°C";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;
};

class Clock_Sm : public Nvml_Metric {
public:
    unsigned int get_value(nvmlDevice_t& device)
    {
        nvmlReturn_t ret =
            nvmlDeviceGetClockInfo(device, nvmlClockType_t::NVML_CLOCK_SM, &value);
        check_nvml_ret(ret);

        return value;
    }

protected:
    const std::string desc = "SM clocks";
    const std::string unit = "MHz";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;
};

class Clock_Mem : public Nvml_Metric {
public:
    unsigned int get_value(nvmlDevice_t& device)
    {
        nvmlReturn_t ret =
            nvmlDeviceGetClockInfo(device, nvmlClockType_t::NVML_CLOCK_MEM, &value);
        check_nvml_ret(ret);

        return value;
    }

protected:
    const std::string desc = "Memory clocks";
    const std::string unit = "MHz";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;
};

class Fan_Speed : public Nvml_Metric {
public:
    unsigned int get_value(nvmlDevice_t& device)
    {
        nvmlReturn_t ret = nvmlDeviceGetFanSpeed(device, &value);
        check_nvml_ret(ret);

        return value;
    }

protected:
    const std::string desc = "Fan speed";
    const std::string unit = "";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;
};

class Mem_Free : public Nvml_Metric {
public:
    unsigned int get_value(nvmlDevice_t& device)
    {
        nvmlMemory_t mem;
        nvmlReturn_t ret = nvmlDeviceGetMemoryInfo(device, &mem);
        check_nvml_ret(ret);
        value = mem.free;

        return value;
    }

protected:
    const std::string desc = "Free memory";
    const std::string unit = "Bytes";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;
};

class Mem_Used : public Nvml_Metric {
public:
    unsigned int get_value(nvmlDevice_t& device)
    {
        nvmlMemory_t mem;
        nvmlReturn_t ret = nvmlDeviceGetMemoryInfo(device, &mem);
        check_nvml_ret(ret);
        value = mem.used;

        return value;
    }

protected:
    const std::string desc = "Used memory";
    const std::string unit = "Bytes";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;
};

class Mem_Total : public Nvml_Metric {
public:
    unsigned int get_value(nvmlDevice_t& device)
    {
        nvmlMemory_t mem;
        nvmlReturn_t ret = nvmlDeviceGetMemoryInfo(device, &mem);
        check_nvml_ret(ret);
        value = mem.total;

        return value;
    }

protected:
    const std::string desc = "Total memory";
    const std::string unit = "Bytes";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;
};

class Pcie_Send : public Nvml_Metric {
public:
    unsigned int get_value(nvmlDevice_t& device)
    {
        nvmlReturn_t ret = nvmlDeviceGetPcieThroughput(
            device, nvmlPcieUtilCounter_t::NVML_PCIE_UTIL_TX_BYTES, &value);
        check_nvml_ret(ret);

        return value;
    }

protected:
    const std::string desc = "PCIe Send";
    const std::string unit = "Bytes";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;
};

class Pcie_Recv : public Nvml_Metric {
public:
    unsigned int get_value(nvmlDevice_t& device)
    {
        nvmlReturn_t ret = nvmlDeviceGetPcieThroughput(
            device, nvmlPcieUtilCounter_t::NVML_PCIE_UTIL_RX_BYTES, &value);
        check_nvml_ret(ret);

        return value;
    }

protected:
    const std::string desc = "PCIe Recv";
    const std::string unit = "Bytes";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;
};

class Utilization_Gpu : public Nvml_Metric {
public:
    unsigned int get_value(nvmlDevice_t& device)
    {
        nvmlUtilization_t util;
        nvmlReturn_t ret = nvmlDeviceGetUtilizationRates(device, &util);
        check_nvml_ret(ret);
        value = util.gpu;

        return value;
    }

protected:
    const std::string desc = "GPU Utilization";
    const std::string unit = "";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;
};

class Utilization_Mem : public Nvml_Metric {
public:
    unsigned int get_value(nvmlDevice_t& device)
    {
        nvmlUtilization_t util;
        nvmlReturn_t ret = nvmlDeviceGetUtilizationRates(device, &util);
        check_nvml_ret(ret);
        value = util.memory;

        return value;
    }

protected:
    const std::string desc = "Memory Utilization";
    const std::string unit = "";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;
};

class Nvml_Sampling_Metric {
public:
    std::vector<pair_time_sampling_t> get_value(nvmlDevice_t device,
                                                unsigned long long last_seen = 0)
    {
        nvmlSamplingType_t sample_type = nvmlSamplingType_t::NVML_GPU_UTILIZATION_SAMPLES;
        nvmlValueType_t val_type;
        unsigned int sample_count;
        nvmlSample_t* samples;

        std::vector<pair_time_sampling_t> vec;
        nvmlReturn_t ret;

        if (device == NULL) {
            throw std::runtime_error(
                "CUDA device for metric sampling not set.");
        }

        // get number of samples to allocate memory
        ret = nvmlDeviceGetSamples(device, sample_type, last_seen, &val_type,
                                   &sample_count, NULL);
        check_nvml_ret(ret);
        vec.reserve(sample_count);

        // get samples
        // TODO reuse buffer
        samples = (nvmlSample_t*)malloc(sample_count * sizeof(nvmlSample_t));
        ret = nvmlDeviceGetSamples(device, sample_type, last_seen, &val_type,
                                   &sample_count, samples);

        if (NVML_SUCCESS != ret) {
            free(samples);
            throw std::runtime_error(
                "Could not fetch data from NVML. Error Code: " +
                std::string(nvmlErrorString(ret)));
        }

        for (unsigned int i = 0; i < sample_count; ++i) {
            vec.emplace_back(samples[i].timeStamp, samples[i].sampleValue.uiVal);
        }

        free(samples);

        return vec;
    }

    const std::string& get_desc() const
    {
        return desc;
    }

    const std::string& get_unit() const
    {
        return unit;
    }

    const metric_measure_type get_measure_type() const
    {
        return type;
    }

    const metric_datatype get_datatype() const
    {
        return datatype;
    }

protected:
    const std::string desc;
    const std::string unit;
    metric_measure_type type;
    metric_datatype datatype;
};

class Power_Sampling : public Nvml_Sampling_Metric {
protected:
    const std::string desc = "Power consumption (samples)";
    const std::string unit = "mW";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;

protected:
    nvmlSamplingType_t sample_type = nvmlSamplingType_t::NVML_TOTAL_POWER_SAMPLES;
};

class Utilization_Gpu_Sampling : public Nvml_Sampling_Metric {
protected:
    const std::string desc = "GPU utilization (samples)";
    const std::string unit = "";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;

protected:
    nvmlSamplingType_t sample_type = nvmlSamplingType_t::NVML_GPU_UTILIZATION_SAMPLES;
};

class Utilization_Mem_Sampling : public Nvml_Sampling_Metric {
protected:
    const std::string desc = "Memory utilization (samples)";
    const std::string unit = "";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;

protected:
    nvmlSamplingType_t sample_type = nvmlSamplingType_t::NVML_MEMORY_CLK_SAMPLES;
};

class Clock_Sm_Sampling : public Nvml_Sampling_Metric {
protected:
    const std::string desc = "SM clocks (sample)";
    const std::string unit = "MHz";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;

protected:
    nvmlSamplingType_t sample_type = nvmlSamplingType_t::NVML_PROCESSOR_CLK_SAMPLES;
};

class Clock_Mem_Sampling : public Nvml_Sampling_Metric {
protected:
    const std::string desc = "Memory clocks (sample)";
    const std::string unit = "MHz";
    const metric_measure_type type = metric_measure_type::ABS;
    const metric_datatype datatype = metric_datatype::UINT;

protected:
    nvmlSamplingType_t sample_type = nvmlSamplingType_t::NVML_MEMORY_CLK_SAMPLES;
};

Nvml_Metric* metricname_2_nvmlfunction(std::string metric_name)
{
    Nvml_Metric* metric;
    if (metric_name.compare("power_usage") == 0) {
        metric = new Power();
    }
    else if (metric_name.compare("temperature") == 0) {
        metric = new Temperature();
    }
    else if (metric_name.compare("clock_sm") == 0) {
        metric = new Clock_Sm();
    }
    else if (metric_name.compare("clock_mem") == 0) {
        metric = new Clock_Mem();
    }
    else if (metric_name.compare("fan_speed") == 0) {
        metric = new Fan_Speed();
    }
    else if (metric_name.compare("mem_free") == 0) {
        metric = new Mem_Free();
    }
    else if (metric_name.compare("mem_used") == 0) {
        metric = new Mem_Used();
    }
    else if (metric_name.compare("pcie_send") == 0) {
        metric = new Pcie_Send();
    }
    else if (metric_name.compare("pcie_recv") == 0) {
        metric = new Pcie_Recv();
    }
    else if (metric_name.compare("utilization_gpu") == 0) {
        metric = new Utilization_Gpu();
    }
    else if (metric_name.compare("utilization_mem") == 0) {
        metric = new Utilization_Mem();
    }
    else {
        std::runtime_error("Unknown metric: " + metric_name);
    }
    return metric;
}

Nvml_Sampling_Metric* metric_name_2_nvml_sampling_function(std::string metric_name)
{
    Nvml_Sampling_Metric* metric;
    if (metric_name.compare("power_usage") == 0) {
        metric = new Power_Sampling();
    }
    else if (metric_name.compare("clock_sm") == 0) {
        metric = new Clock_Sm_Sampling();
    }
    else if (metric_name.compare("clock_mem") == 0) {
        metric = new Clock_Mem_Sampling();
    }
    else if (metric_name.compare("utilization_gpu") == 0) {
        metric = new Utilization_Gpu_Sampling();
    }
    else if (metric_name.compare("utilization_mem") == 0) {
        metric = new Utilization_Mem_Sampling();
    }
    else {
        std::runtime_error("Unknown metric: " + metric_name);
    }
    return metric;
}

#endif // SCOREP_PLUGIN_NVML_NVML_WRAPPER_HPP