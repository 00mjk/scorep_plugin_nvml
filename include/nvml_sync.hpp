#include <scorep/plugin/plugin.hpp>

#include <nvml.h>

#include <iostream>
#include <string>
#include <vector>

using namespace scorep::plugin::policy;

using scorep::plugin::logging;
using TVPair = std::pair<scorep::chrono::ticks, unsigned int>;


class nvml_plugin : public scorep::plugin::base<nvml_plugin, sync, per_host, scorep_clock> {
public:
    nvml_plugin()
    {
        logging::debug() << "start constructor";

        nvmlReturn_t nvml = nvmlInit_v2();
        if (NVML_SUCCESS != nvml) {
            throw std::runtime_error("Could not start NVML Code: " +
                                     std::string(nvmlErrorString(nvml)));
        }
        nvml_devices = get_visible_devices();
        logging::debug() << "end constructor";
    }

    // Convert a named metric (may contain wildcards or so) to a vector of
    // actual metrics (may have a different name)
    std::vector<scorep::plugin::metric_property> get_metric_properties(const std::string& metric_name)
    {
        logging::debug() << "start get_metric_properties";
        std::vector<scorep::plugin::metric_property> properties;

        logging::info() << "get metric properties called with: " << metric_name;

        properties.push_back(scorep::plugin::metric_property(
            metric_name, "Power consumption", "mW")
                                 .absolute_point()
                                 .value_uint());

        logging::debug() << "end get_metric_properties";
        return properties;
    }

    int32_t add_metric(const std::string& event)
    {
        logging::info() << "add metric called with: " << event;

        return ++counter;
    }

    // Will be called for every event in by the measurement environment
    // You may or may or may not give it a value here.
    template <typename P>
    bool get_optional_value(std::int32_t id, P& proxy)
    {
        logging::debug() << "start get_optional_values";
        unsigned int power;
        if (nvml_devices.size() == 0)
        {
            logging::warn() << "no cuda devices found";
            return false;
        }
        nvmlDeviceGetPowerUsage(nvml_devices[0], &power);
        logging::debug() << "power usage: " << power;
        proxy.write((double) power);
        logging::debug() << "end get_optional_values";
        return true;
    }

private:
    std::vector<nvmlDevice_t> nvml_devices;
    scorep::chrono::ticks begin, end;
    int counter = 0;

private:
    std::vector<nvmlDevice_t> get_visible_devices()
    {
        logging::debug() << "start get_visible_devices";
        std::vector<nvmlDevice_t> devices;

        nvmlReturn_t ret;
        unsigned int num_devices;

        ret = nvmlDeviceGetCount(&num_devices);
        if (NVML_SUCCESS != ret) {
            throw std::runtime_error(nvmlErrorString(ret));
        }

        /*
         * New nvmlDeviceGetCount_v2 (default in NVML 5.319) returns count of all devices in the system
         * even if nvmlDeviceGetHandleByIndex_v2 returns NVML_ERROR_NO_PERMISSION for such device.
         */
        nvmlDevice_t device;
        for (unsigned i = 0; i < num_devices; ++i) {
            ret = nvmlDeviceGetHandleByIndex(i, &device);

            if (NVML_SUCCESS == ret) {
                devices.push_back(device);
            }
            else if (NVML_ERROR_NO_PERMISSION == ret) {
                logging::debug() << "No permission for device: " << i;
            }
            else {
                throw std::runtime_error(nvmlErrorString(ret));
            }
        }
        return devices;
    }

};