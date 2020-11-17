#ifndef SCOREP_PLUGIN_NVML_NVML_MEASUREMENT_THREAD_HPP
#define SCOREP_PLUGIN_NVML_NVML_MEASUREMENT_THREAD_HPP

#include <chrono>
#include <thread>
#include <unordered_map>
#include <vector>

#include <nvml.h>

#include <scorep/plugin/plugin.hpp>

#include "nvml_types.hpp"
#include "nvml_wrapper.hpp"

class nvml_measurement_thread {
public:
    nvml_measurement_thread(std::chrono::milliseconds interval_)
        : interval(interval_)
    {
        last = scorep::chrono::measurement_clock::now();
    }

    void add_handles(const std::vector<nvml_t>& handles)
    {
        for (auto& handle : handles) {
            measurements.insert(std::make_pair(std::ref(const_cast<nvml_t&>(handle)),
                                               std::vector<pair_chrono_value_t>()));
        }
    }

    std::vector<pair_chrono_value_t> get_readings(nvml_t& handle)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return measurements[handle];
    }

    void measurement_sampling()
    {
        stop = false;

        while (!stop) {
            try {
                std::lock_guard<std::mutex> lock(m_mutex);
                for (auto& metric_it : measurements) {
                    std::vector<pair_time_sampling_t> sampling_values =
                        metric_it.first.get().metric->get_value(
                            metric_it.first.get().device, last.count());

                    for (auto& pair_it : sampling_values) {
                        auto tick = scorep::chrono::ticks(pair_it.first);
                        metric_it.second.push_back(
                            std::make_pair(tick, (std::uint64_t)pair_it.second));
                    }
                }
            }
            catch (scorep::exception::null_pointer& e) {
                logging::warn() << "Score-P Clock not set.";
            }
            last = scorep::chrono::measurement_clock::now();
            std::this_thread::sleep_for(interval);
        }
    }

    void stop_measurement()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        stop = true;
    }

private:
    std::chrono::milliseconds interval;

    std::mutex m_mutex;

    bool stop = true;

    scorep::chrono::ticks last;

    std::unordered_map<std::reference_wrapper<nvml_t>, std::vector<pair_chrono_value_t>, std::hash<nvml_t>, std::equal_to<nvml_t>> measurements;
};

#endif // SCOREP_PLUGIN_NVML_NVML_MEASUREMENT_THREAD_HPP
