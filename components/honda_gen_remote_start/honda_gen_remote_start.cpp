#include "honda_gen_remote_start.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace honda_gen_remote_start {

static const char *const TAG = "honda_gen_remote_start";
// How long the start command is held down in MS.
static const uint32_t START_PRESS_TIME = 300;
// How long we must wait to check if the generator was able to start in MS.
// The gen will attempt to start for 5s. Give it another second
static const uint32_t START_WAIT_TIME = 6000;
// How long we will wait for the gen to stop in MS.
static const uint32_t STOP_WAIT_TIME = 6000;

void HondaGenRemoteStartComponent::setup() {
  this->starting_ = false;
  this->stopping_ = false;
  this->start_button_down_ = false;
  // default state.
  this->stop_output_->turn_off();
  this->start_output_->turn_off();
}

void HondaGenRemoteStartComponent::loop() {
  if (this->starting_) {
    // should we depress the start button?
    if (this->start_button_down_ && (this->start_t_ + START_PRESS_TIME < millis())) {
      ESP_LOGD(TAG, "Depress Starter.");
      this->start_output_->turn_off();
      this->start_button_down_ = false;
    }

    // did it start?
    if (this->running()) {
      ESP_LOGD(TAG, "Succesfully Started.");
      this->start_output_->turn_off();
      this->starting_ = false;
      this->publish_state(true);
      return;

    } else if (millis() > this->start_t_ + START_WAIT_TIME) {
      // we have exceeded the start wait. Give up.
      ESP_LOGD(TAG, "Failed to Start.");
      this->starting_ = false;
      this->start_output_->turn_off();
      return;
    }

  } else if (this->stopping_) {
    // did it stop?
    if (!this->running()) {
      // Great.
      ESP_LOGD(TAG, "Succesfully Stopped.");
      this->stop_output_->turn_off();
      this->stopping_ = false;
      this->publish_state(false);

    } else if (millis() > this->stopping_t_ + STOP_WAIT_TIME) {
      // we have exceeded the stop wait time. Give up.
      ESP_LOGD(TAG, "Failed to Stop.");
      this->stopping_ = false;
      this->stop_output_->turn_off();
    }
  }
}

void HondaGenRemoteStartComponent::update() {
  if (this->starting_ || this->stopping_) {
    // leave the state handling to the loop routine.
    return;
  }
  
  this->publish_state(this->running());
}

bool HondaGenRemoteStartComponent::can_start() {
  bool power_sw_on = true;
  if (this->power_switch_sensor_ != nullptr) {
    power_sw_on = this->power_switch_sensor_->has_state() && this->power_switch_sensor_->state;
  }
  return power_sw_on && !this->running();
}

bool HondaGenRemoteStartComponent::running() {
    return (this->run_sensor_->has_state() && this->run_sensor_->state);
}

void HondaGenRemoteStartComponent::start_gen() {
    if (!this->can_start()) {
        ESP_LOGD(TAG, "Cannot Start. Run sensor says its running or Power Sw says its off.");
        return;
    }

    // must make sure the gen power is on.
    this->stop_output_->turn_off();
    
    // ask it to start. Depress the start button.
    ESP_LOGD(TAG, "Starter Pressed.");
    this->start_output_->turn_on();
    this->start_button_down_ = true;

    this->starting_ = true;
    this->start_t_ = millis();
}

void HondaGenRemoteStartComponent::stop_gen() {
  if (!this->running()) {
    ESP_LOGD(TAG, "Cannot Stop. Sensors say gen is not running.");
    return;
  }
  
  // ask it to stop.
  this->start_output_->turn_off();
  this->stop_output_->turn_on();

  this->stopping_ = true;
  this->stopping_t_ = millis();
}

void HondaGenRemoteStartComponent::write_state(bool state) {
  if (this->starting_ || this->stopping_) {
    return;
  }

  if (state) {
    this->start_gen();
  } else {
    this->stop_gen();
  }
}


}}