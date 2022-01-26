#include "honda_gen_dlc.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include <string.h>

namespace esphome {
namespace honda_gen_dlc {

static const char *const TAG = "honda_gen_dlc";

static const uint8_t QUERY_SIZE = 4;
static const uint8_t VT_MSG_START = 0x01;
static const uint8_t VT_MSG_END = 0x04;
static const uint32_t QUERY_TIMEOUT = 5000;

bool HondaGenDLCComponent::needs_query_ecu_info1() {
  if (this->battery_voltage_sensor_ != nullptr)
    return true;
  if (this->engine_rpm_sensor_ != nullptr)
    return true;
  if (this->runtime_sensor_ != nullptr)
    return true;
  if (this->cylinder_head_temperature_sensor_ != nullptr)
    return true;
  if (this->engine_timing_sensor_ != nullptr)
    return true;
  if (this->output_voltage_sensor_ != nullptr)
    return true;
  return false; 
}

bool HondaGenDLCComponent::needs_query_ecu_info2() {
  if (this->barometric_pressure_sensor_ != nullptr)
    return true;
  if (this->fuel_injection_time_sensor_ != nullptr)
    return true;
  if (this->throttle_sensor_ != nullptr)
    return true;
  if (this->output_power_sensor_ != nullptr)
    return true;
  if (this->starter_count_sensor_ != nullptr)
    return true;
  if (this->o2_volts_sensor_ != nullptr)
    return true;
  return false;
}

bool HondaGenDLCComponent::needs_query_inverter_master() {
  if (this->inv_master_current_ != nullptr)
    return true;
  if (this->inv_master_temperature_ != nullptr)
    return true;
  if (this->inv_master_eco_ != nullptr)
    return true;
  if (this->inv_master_power_ != nullptr)
    return true;
  if (this->inv_master_volts_ != nullptr)
    return true;
  return false;
}

bool HondaGenDLCComponent::needs_query_inverter_slave() {
  if (this->inv_slave_current_ != nullptr) {
    return true;
  }
  if (this->inv_slave_temperature_ != nullptr) {
    return true;
  }
  if (this->inv_slave_power_ != nullptr) {
    return true;
  }
  return false;
}

void HondaGenDLCComponent::setup() {
  this->query_active_ = false;

  size_t i = 0;
  
  if (this->needs_query_ecu_info1()) {
    this->query_array_[i++] = QueryType::T_ECU_Info1;
  }
  if (this->needs_query_ecu_info2()) {
    this->query_array_[i++] = QueryType::T_ECU_Info2;
  }
  if (this->needs_query_inverter_master()) {
    this->query_array_[i++] = QueryType::T_INV_Master;
  }
  if (this->needs_query_inverter_slave()) {
    this->query_array_[i++] = QueryType::T_INV_Slave;
  }

  this->query_array_len_ = i;
  this->query_array_index_ = 0;
}

void HondaGenDLCComponent::update() {
  // this is the poll method. Here we will query for new messages.
  if (this->query_active_ && (millis() < this->query_time_ + QUERY_TIMEOUT)) {
    // there is an ongoing query. just wait for it or the timeout.
    return;
  }

  if (this->power_switch_sensor_ != nullptr) {
    // if you've provided a power switch sensor. Don't bother querying if the generator power is off.
    if (this->power_switch_sensor_->has_state() && !this->power_switch_sensor_->state) {
      return;
    }
  }

  this->start_queries();
}

void HondaGenDLCComponent::start_queries() {
  this->query_array_index_ = 0;
  this->query_active_ = true;
  this->next_query();
}

void HondaGenDLCComponent::next_query() {
  if (this->query_array_len_ > this->query_array_index_) {
    this->query(this->query_array_[this->query_array_index_++]);
  } else {
    this->query_active_ = false;
  }
}

void HondaGenDLCComponent::update_ecu_info1_sensors() {
  if (this->battery_voltage_sensor_ != nullptr) {
    this->battery_voltage_sensor_->publish_state(this->ecu_info1_.battery_voltage * 0.1f);
  }
  if (this->engine_rpm_sensor_ != nullptr) {
    this->engine_rpm_sensor_->publish_state(this->ecu_info1_.rpm);
  }
  if (this->runtime_sensor_ != nullptr) {
    uint32_t minutes = 0;
    minutes += this->ecu_info1_.runtime_m;
    minutes += this->ecu_info1_.runtime_h * 60;
    minutes += this->ecu_info1_.runtime_d * 60 * 24;
    this->runtime_sensor_->publish_state(minutes);
  }
  if (this->cylinder_head_temperature_sensor_ != nullptr) {
    this->cylinder_head_temperature_sensor_->publish_state(this->ecu_info1_.cylinder_head_temp);
  }
  if (this->engine_timing_sensor_ != nullptr) {
    this->engine_timing_sensor_->publish_state(this->ecu_info1_.ignition_degrees);
  }
  if (this->output_voltage_sensor_ != nullptr) {
    this->output_voltage_sensor_->publish_state(this->ecu_info1_.output_voltage);
  }
}

void HondaGenDLCComponent::update_ecu_info2_sensors() {
  if (this->barometric_pressure_sensor_ != nullptr) {
    this->barometric_pressure_sensor_->publish_state(this->ecu_info2_.barometric_pressure);
  }
  if (this->fuel_injection_time_sensor_ != nullptr) {
    this->fuel_injection_time_sensor_->publish_state(this->ecu_info2_.fuel_injection_time * 0.01f);
  }
  if (this->throttle_sensor_ != nullptr) {
    this->throttle_sensor_->publish_state(this->ecu_info2_.throttle_position);
  }
  if (this->output_power_sensor_ != nullptr) {
    this->output_power_sensor_->publish_state(this->ecu_info2_.output_power);
  }
  if (this->starter_count_sensor_ != nullptr) {
    this->starter_count_sensor_->publish_state(this->ecu_info2_.starter_count);
  }
  if (this->o2_volts_sensor_ != nullptr) {
    this->o2_volts_sensor_->publish_state(this->ecu_info2_.o2 * 0.1f);
  }
}

void HondaGenDLCComponent::update_inverter_master_sensors() {
  if (this->inv_master_current_ != nullptr) {
    this->inv_master_current_->publish_state(this->inv_master_.amperage * 0.1);
  }
  if (this->inv_master_temperature_ != nullptr) {
    this->inv_master_temperature_->publish_state(this->inv_master_.temperature);
  }
  if (this->inv_master_eco_ != nullptr) {
    bool eco = this->inv_master_.mode == 2;
    this->inv_master_eco_->publish_state(eco);
  }
  if (this->inv_master_power_ != nullptr) {
    this->inv_master_power_->publish_state(this->inv_master_.watts);
  }
  if (this->inv_master_volts_ != nullptr) {
    this->inv_master_volts_->publish_state(this->inv_master_.voltage);
  }
}

void HondaGenDLCComponent::update_inverter_slave_sensors() {
  if (this->inv_slave_current_ != nullptr) {
    this->inv_slave_current_->publish_state(this->inv_slave_.amperage * 0.1);
  }
  if (this->inv_slave_temperature_ != nullptr) {
    this->inv_slave_temperature_->publish_state(this->inv_slave_.temperature);
  }
  if (this->inv_slave_power_ != nullptr) {
    this->inv_slave_power_->publish_state(this->inv_slave_.watts);
  }
}

void HondaGenDLCComponent::loop() {
  QueryType query_type;

  while (this->available()) {
    int current_byte = this->read();
    if (current_byte == -1) {
        return;
    }
    
    uint8_t byte = (uint8_t)current_byte;

    if (byte == VT_MSG_START) {
        this->rx_index_ = 0;
        return;
    } else if(byte == VT_MSG_END) {
        if (this->recieve_message(this->rx_buffer_, this->rx_index_, query_type)) {
          switch (query_type) {
            case QueryType::T_ECU_Info1:
              this->update_ecu_info1_sensors();
              break;
            case QueryType::T_ECU_Info2:
              this->update_ecu_info2_sensors();
              break;
            case QueryType::T_INV_Master:
              this->update_inverter_master_sensors();
              break;
            case QueryType::T_INV_Slave:
              this->update_inverter_slave_sensors();
              break;
            default:
              break;
          }
          this->next_query();
        }
    } else {
        this->rx_buffer_[this->rx_index_] = byte;
        this->rx_index_ += 1;
    }
  }
}

bool HondaGenDLCComponent::recieve_message(uint8_t * msg, size_t len, QueryType &query_type) {
  ESP_LOGD(TAG, "Recieve: %.*s", len, msg);
  
  if (len % 2 != 0)
    // It must be dividible by 2 to be a valid hex message.
    return false;
  // This should be the message in Hex excluding the terinal begin/end.
  size_t binary_len = len / 2;
  uint8_t * binary_msg = new uint8_t[binary_len];
  char * msg_ptr = (char*)msg;
  // convert it from hex to binary
  for (size_t i = 0; i < binary_len; i++) {
      sscanf(msg_ptr, "%2hhX", &binary_msg[i]);
      msg_ptr += 2;
  }

  // great lets decode it.
  if (!PGMFI_Decoder::decode_msg_type(binary_msg, binary_len, query_type))
      return false;
  
  bool success = false;
  
  switch (query_type) {
      case QueryType::T_ECU_Info1:
          success = PGMFI_Decoder::decode(binary_msg, binary_len, this->ecu_info1_);
          break;
      case QueryType::T_ECU_Info2:
          success = PGMFI_Decoder::decode(binary_msg, binary_len, this->ecu_info2_);
          break;
      case QueryType::T_INV_Master:
          success = PGMFI_Decoder::decode(binary_msg, binary_len, this->inv_master_);
          break;
      case QueryType::T_INV_Slave:
          success = PGMFI_Decoder::decode(binary_msg, binary_len, this->inv_slave_);
          break;
      default:
          break;
  }

  delete[] binary_msg;

  if (!success)
  {
    ESP_LOGD(TAG, "Failed to decode msg");
    return success;
  }
  ESP_LOGD(TAG, "Decoded Msg Type: %u", query_type);
  return success;
}

void HondaGenDLCComponent::send_message(uint8_t * msg, size_t len) {
    // Takes a binary string and converts it to Hex and sents it as a terminal message.
    // bin to ascii = *2. + a header, end, and null terminator.
    size_t buff_len = (len*2)+3;
    uint8_t * tx_buff = new uint8_t[buff_len];

    tx_buff[0] = VT_MSG_START;
    size_t index = 1;

    for(size_t i = 0; i < len; i++) {
        // convert it to hex.
        sprintf((char*)tx_buff + index, "%02X", msg[i]);
        index += 2;
    }
    tx_buff[index++] = VT_MSG_END;
    tx_buff[index] = 0x00;

    ESP_LOGD(TAG, "Send: %s", tx_buff);
    this->write_array(tx_buff, buff_len);
    delete[] tx_buff;
}

void HondaGenDLCComponent::query(QueryType type) {
    uint8_t msg[QUERY_SIZE];
    msg[0] = QUERY_START;
    msg[1] = type;
    msg[2] = 0x00;

    // The last byte i think is a checksum but i dunno. Would be nice to figure that out.
    switch (type) {
        case QueryType::T_ECU_Info1:
            msg[3] = 0x01;
            break;
        case QueryType::T_ECU_Info2:
            msg[3] = 0x02;
            break;
        case QueryType::T_INV_Master:
            msg[3] = 0x00;
            break;
        case QueryType::T_INV_Slave:
            msg[3] = 0x03;
            break;
        default:
            break;
    }

    this->query_active_ = true;
    this->query_time_ = millis();
    this->send_message(msg, QUERY_SIZE);
}


}  // namespace
}  // namespace esphome
