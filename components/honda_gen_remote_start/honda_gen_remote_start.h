#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/output/binary_output.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace honda_gen_remote_start {


class HondaGenRemoteStartComponent : public PollingComponent, public switch_::Switch {
  public:
    void setup() override;
    void loop() override;
    void update() override;

    void write_state(bool state) override;

    void set_power_sw_sensor(binary_sensor::BinarySensor *power_switch_sensor) {
      this->power_switch_sensor_ = power_switch_sensor;
    }

    void set_run_sensor(binary_sensor::BinarySensor *run_sensor) {
      this->run_sensor_ = run_sensor;
    }

    void set_start_output(output::BinaryOutput *start_output) {
      this->start_output_ = start_output;
    }

    void set_stop_output(output::BinaryOutput *stop_output) {
      this->stop_output_ = stop_output;
    }
  private:
    binary_sensor::BinarySensor *power_switch_sensor_{nullptr};
    binary_sensor::BinarySensor *run_sensor_{nullptr};
    output::BinaryOutput *start_output_{nullptr};
    output::BinaryOutput *stop_output_{nullptr};
    uint32_t start_t_;
    bool starting_;
    bool start_button_down_;
    uint32_t stopping_t_;
    bool stopping_;

    void start_gen();
    void stop_gen();
    bool running();
    bool can_start();
};


}}