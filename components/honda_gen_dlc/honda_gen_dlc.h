#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/honda_gen_dlc/pgmfi_decoder.h"

namespace esphome {
namespace honda_gen_dlc {

static const size_t RX_BUFF_SIZE = 128;
static const size_t NUM_POSSIBLE_QUERIES = 4;

class HondaGenDLCComponent : public PollingComponent, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
  void update() override;

  float get_setup_priority() const override { return setup_priority::HARDWARE; }

  void set_power_switch_sensor(binary_sensor::BinarySensor *power_switch_sensor) {
    this->power_switch_sensor_ = power_switch_sensor;
  }

  void set_battery_voltage_sensor(sensor::Sensor *battery_voltage_sensor) {
    this->battery_voltage_sensor_ = battery_voltage_sensor;
  }

  void set_engine_rpm_sensor(sensor::Sensor *engine_rpm_sensor) {
    this->engine_rpm_sensor_ = engine_rpm_sensor;
  }

  void set_runtime_sensor(sensor::Sensor *runtime_sensor) {
    this->runtime_sensor_ = runtime_sensor;
  }

  void set_cylinder_head_temperature_sensor(sensor::Sensor *cylinder_head_temperature_sensor) {
    this->cylinder_head_temperature_sensor_ = cylinder_head_temperature_sensor;
  }

  void set_engine_timing_sensor(sensor::Sensor *engine_timing_sensor) {
    this->engine_timing_sensor_ = engine_timing_sensor;
  }

  void set_output_voltage_sensor(sensor::Sensor *output_voltage_sensor) {
    this->output_voltage_sensor_ = output_voltage_sensor;
  }

  void set_barometric_pressure_sensor(sensor::Sensor *barometric_pressure_sensor) {
    this->barometric_pressure_sensor_ = barometric_pressure_sensor;
  }

  void set_fuel_injection_time_sensor(sensor::Sensor *fuel_injection_time_sensor) {
    this->fuel_injection_time_sensor_ = fuel_injection_time_sensor;
  }

  void set_throttle_sensor(sensor::Sensor *throttle_sensor) {
    this->throttle_sensor_ = throttle_sensor;
  }

  void set_output_power_sensor(sensor::Sensor *output_power_sensor) {
    this->output_power_sensor_ = output_power_sensor;
  }

  void set_starter_count_sensor(sensor::Sensor *starter_count_sensor) {
    this->starter_count_sensor_ = starter_count_sensor;
  }
  
  void set_o2_volts_sensor(sensor::Sensor *o2_volts_sensor) {
    this->o2_volts_sensor_ = o2_volts_sensor;
  }

 protected:
  binary_sensor::BinarySensor *power_switch_sensor_{nullptr};
  // ECU Info1
  sensor::Sensor *battery_voltage_sensor_{nullptr};
  sensor::Sensor *engine_rpm_sensor_{nullptr};
  sensor::Sensor *runtime_sensor_{nullptr};
  sensor::Sensor *cylinder_head_temperature_sensor_{nullptr};
  sensor::Sensor *engine_timing_sensor_{nullptr};
  sensor::Sensor *output_voltage_sensor_{nullptr};
  // ECU Info2
  sensor::Sensor *barometric_pressure_sensor_{nullptr};
  sensor::Sensor *fuel_injection_time_sensor_{nullptr};
  sensor::Sensor *throttle_sensor_{nullptr};
  sensor::Sensor *output_power_sensor_{nullptr};
  sensor::Sensor *starter_count_sensor_{nullptr};
  sensor::Sensor *o2_volts_sensor_{nullptr};
  
  uint8_t rx_buffer_[RX_BUFF_SIZE];
  size_t rx_index_;
  ECU_Info1 ecu_info1_;
  ECU_Info2 ecu_info2_;
  Inverter_Master inv_master_;
  Inverter_Slave inv_slave_;
  // to keep track of the next query to do.
  QueryType query_array_[NUM_POSSIBLE_QUERIES];
  size_t query_array_len_;
  size_t query_array_index_;
  // the time of the last query for a timeout.
  uint32_t query_time_;
  bool query_active_;


  bool needs_query_ecu_info1();
  bool needs_query_ecu_info2();
  void update_ecu_info1_sensors();
  void update_ecu_info2_sensors();

  void start_queries();
  void next_query();

  void send_message(uint8_t * msg, size_t len);
  bool recieve_message(uint8_t * msg, size_t len, QueryType &query_type);
  void query(QueryType type);
};
;

}  // namespace mpu6050
}  // namespace esphome
