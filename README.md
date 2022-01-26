# Honda-Gen-ESPHome
Honda EU7000is Generator [ESPHome](https://esphome.io/) Integration.

It enables Remote Start and extensive sensor monitoring on the
Fuel Injected Honda EU7000is over wifi.

This project provides two Integrations. They can be used separately or together.

![EU7000is](docs/images/EU7000is_16HPE_03.png?raw=true)

## Remote Start

![Start Switch](docs/images/remote_start_switch.PNG?raw=true)

Provides a Home Assistant Switch which implements remote start logic.

You must provide 3 GPIO inputs for this to work:
* (Input) Engine running
* (Output) Start Button
* (Output) Engine Kill Switch

In addition, you can optionally provide
* (Input) Power Switch Sensor

If you provide the power switch sensor it will not allow start attempts
if the engine power switch is off.


**Example GPIO configurations**:
```yaml
binary_sensor:
  - platform: gpio
    pin: ${run_pin}
    name: "${upper_devicename} Running Light"
    id: "run_sensor"
  - platform: gpio
    pin: ${sw_pin}
    name: "${upper_devicename} Power Sw Enabled"
    id: "power_sw_sensor"

output:
  - platform: gpio
    pin: ${start_pin}
    id: "start_button"
  - platform: gpio
    pin: ${stop_pin}
    id: "stop_relay"
```

**Example Remote Start Switch Configuration**

```yaml
switch:
  - platform: honda_gen_remote_start
    name: "${upper_devicename} Remote Start"
    # This one's optional. If you provide it then it won't
    # allow start attempts if the power switch is off.
    power_sw_input: power_sw_sensor
    # The following are required.
    run_input: run_sensor
    start_output: start_button
    stop_output: stop_relay
```

For a full example see [Here](example_honda_gen_remote_start.yaml)

## Data Link Connector (DLC) Sensors
The EU7000is is Fuel Injected and features an engine computer.
You can read the ECU's sensor values via a UART bus.

This integration implements a UART device which provides a number of sensors.

![HA Screenshot](docs/images/ha_complete.png?raw=true)

Some of the more useful ones:
* Power Output
    * Combined and for each phase
* Engine RPM
* Eco mode on/off
* Engine Throttle
* Engine Run Time
* Temperatures

To use this you must provide a UART interface. There are some hardware considerations.
I recommend you checkout this PCB project and schematic (TODO: Insert LINK) here.

**Implement a UART interface:**
```yaml
uart:
  rx_pin: ${dlc_rx_pin}
  tx_pin: ${dlc_tx_pin}
  baud_rate: 9600
```
**Example Configuration**
```yaml
honda_gen_dlc:
  # if the power is off it won't bother attempting to query sensor values.
  power_switch: power_sw_sensor

  battery_voltage:
    name: "${upper_devicename} Battery Voltage"
  engine_rpm:
    name: "${upper_devicename} Engine RPM"
  runtime:
    name: "${upper_devicename} Total Engine Runtime"
  cylinder_head_temperature:
    name: "${upper_devicename} Cylinder Head Temperature"
  engine_timing:
    name: "${upper_devicename} Engine Spark Timing"
  output_voltage:
    name: "${upper_devicename} Output Voltage"

  barometric_pressure:
    name: "${upper_devicename} Barometric Pressure"
  fuel_injection_time:
    name: "${upper_devicename} Fuel Injection Time"
  throttle:
    name: "${upper_devicename} Trottle Position"
  output_power:
    name: "${upper_devicename} Output Power"
  starter_count:
    name: "${upper_devicename} Starter Count"
  o2_sensor:
    name: "${upper_devicename} O2 Sensor"

  inv_master_current:
    name: "${upper_devicename} Master Inv Current"
  inv_master_temperature:
    name: "${upper_devicename} Master Inv Temperature"
  inv_master_eco:
    name: "${upper_devicename} Eco Mode"
  inv_master_power:
    name: "${upper_devicename} Master Inv Power"
  inv_master_volts:
    name: "${upper_devicename} Master Inv Voltage"

  inv_slave_current:
    name: "${upper_devicename} Slave Inv Current"
  inv_slave_temperature:
    name: "${upper_devicename} Slave Inv Temperature"
  inv_slave_power:
    name: "${upper_devicename} Slave Inv Power"
```

You can optionally provide a power switch sensor. If you do it will not
attempt to query the ECU values over UART if the power switch is off.
Otherwise, it will continually send TX packets and time out if you do not
provide one and the Generator's power switch is off.

You can exclude any sensors from the configuration you do not find 
valuable. The complete list is in the configuration example above.

For a full example see [Here](example_honda_gen_dlc.yaml). Here's an example 
which [includes both integrations](example_honda_gen_complete.yaml).

## Installation with ESPHome

You can use install these integrations just by adding the following to your
esphome project's yaml.
```yaml
external_components:
  - source: github://blaisejarrett/Honda-Gen-ESPHome@main
    components: [ honda_gen_dlc, honda_gen_remote_start ]
```

## Disclaimer
This was tested on a Canadian Model EU7000is. I believe it will work
on US model's aswell. However, 2021+ US models have Bluetooth remote start
features built in. I am told that the remote start port functiality of
this model is different.

Hopefully it works for you. If it does or doesn't I would be interested in
updating this readme.
