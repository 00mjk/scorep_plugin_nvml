#ifndef SCOREP_PLUGIN_NVML_NVML_TYPES_HPP
#define SCOREP_PLUGIN_NVML_NVML_TYPES_HPP

#include "nvml_wrapper.hpp"

#include <nvml.h>

#include <scorep/chrono/chrono.hpp>
#include <scorep/plugin/plugin.hpp>

// using pair_time_sampling_t = std::pair<unsigned long long, std::uint64_t>;
using pair_chrono_value_t = std::pair<scorep::chrono::ticks, std::uint64_t>;

using scorep::plugin::logging;

class nvml_t {
public:
    nvml_t(const std::string& name_, nvmlDevice_t device_, Nvml_Sampling_Metric* metric_)
        : name(name_), device(device_), metric(metric_)
    {
        nvmlReturn_t ret = nvmlDeviceGetIndex(device, &device_idx);
        check_nvml_ret(ret);
    }
    ~nvml_t()
    {
        logging::info() << "call destructor of " << name;
    }

    //    // delete copy constructor to avoid ... copies,
    //    // needs move and default constructor though!
    //    nvml_t(nvml_t&&) = default;
    //    nvml_t(const nvml_t&) = delete;
    //    /* copy-assign */
    //    nvml_t& operator=(const nvml_t&) = delete;
    //    /* move assignment */
    //    nvml_t& operator=(nvml_t&&) = default;

    bool operator==(const nvml_t& other) const
    {
        return (this->name == other.name) && (this->device_idx == this->device_idx);
    }

    std::string name;
    Nvml_Sampling_Metric* metric;
    unsigned int device_idx;
    nvmlDevice_t device;
};

namespace std {
/** operator to print the metric handle
 */
inline ostream& operator<<(ostream& s, const nvml_t& metric)
{
    s << "(" << metric.name << " on CUDA " << std::to_string(metric.device_idx) << ")";
    return s;
}

/** hashing using the metric name and device id
 */
template <>
struct hash<nvml_t> {
    size_t inline operator()(const nvml_t& metric) const
    {
        return std::hash<std::string>{}(metric.name + std::to_string(metric.device_idx));
    }
};
};     // namespace std
#endif // SCOREP_PLUGIN_NVML_NVML_TYPES_HPP