#include "nvml_wrapper.hpp"

#include <scorep/plugin/plugin.hpp>

#include <nvml.h>

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace scorep::plugin::policy;

using scorep::plugin::logging;

struct nvml_t {
    nvml_t(const std::string& name_, unsigned int device_id_, nvml_metric metric_)
        : name(name_), device_id(device_id_), metric(metric_)
    {
    }
    std::string name;
    unsigned int device_id;
    nvml_metric metric;
};

template <typename T, typename Policies>
using nvml_object_id = scorep::plugin::policy::object_id<nvml_t, T, Policies>;

class nvml_plugin
    : public scorep::plugin::base<nvml_plugin, async, per_host, scorep_clock, nvml_object_id> {
public:
    nvml_plugin()
    {
        nvmlReturn_t nvml = nvmlInit_v2();
        if (NVML_SUCCESS != nvml) {
            throw std::runtime_error("Could not start NVML. Code: " +
                                     std::string(nvmlErrorString(nvml)));
        }
        nvml_devices = get_visible_devices();
    }

    ~nvml_plugin()
    {
        nvmlReturn_t nvml = nvmlShutdown();
        if (NVML_SUCCESS != nvml) {
//            throw std::runtime_error("Could not terminate NVML. Code: " +
//                                     std::string(nvmlErrorString(nvml)));
            logging::warn() << "Could not terminate NVML. Code:" << std::string(nvmlErrorString(nvml));
        }
    }

    // Convert a named metric (may contain wildcards or so) to a vector of
    // actual metrics (may have a different name)
    std::vector<scorep::plugin::metric_property> get_metric_properties(const std::string& metric_name)
    {
        std::vector<scorep::plugin::metric_property> properties;

        logging::info() << "get metric properties called with: " << metric_name;

        nvml_metric metric_type = metricname_2_nvmlfunction(metric_name);

        for (unsigned int i = 0; i < nvml_devices.size(); ++i) {

            /* TODO use device index by nvmlDeviceGetIndex( nvmlDevice_t device, unsigned int* index ) */

            std::string new_name = metric_name + " on CUDA: " + std::to_string(i);
            auto handle = make_handle(new_name, nvml_t{metric_name, i, metric_type});
            properties.push_back(scorep::plugin::metric_property(
                new_name, "", "")
                                     .absolute_point()
                                     .value_uint());
        }
        return properties;
    }

    void add_metric(nvml_t& handle)
    {
        logging::info() << "add metric called with: " << handle.name
                        << " on CUDA " << handle.device_id;
    }

    // start your measurement in this method
    void start()
    {
        begin = scorep::chrono::measurement_clock::now();
    }

    // stop your measurement in this method
    void stop()
    {
        end = scorep::chrono::measurement_clock::now();

//        for (auto& handle : get_handles())
//        {
//            handle.data = get_value(handle.metric, nvml_devices[handle.device_id]);
//        }
    }

    // Will be called post mortem by the measurement environment
    // You return all values measured.
    template <typename C>
    void get_all_values(nvml_t& handle, C& cursor)
    {
        logging::info() << "get_all_values called with: " << handle.name
                        << " CUDA " << handle.device_id;

        std::uint64_t data = get_value(handle.metric, nvml_devices[handle.device_id]);
        cursor.write(end, data);
    }

private:
    std::vector<nvmlDevice_t> nvml_devices;
    scorep::chrono::ticks begin, end;
    int counter = 0;

private:
    std::vector<nvmlDevice_t> get_visible_devices()
    {
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
                logging::info() << "No permission for device: " << i;
            }
            else {
                throw std::runtime_error(nvmlErrorString(ret));
            }
        }
        return devices;
    }
};