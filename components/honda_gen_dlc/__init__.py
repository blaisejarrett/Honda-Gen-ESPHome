import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import uart, sensor, binary_sensor
from esphome.const import (
    CONF_BATTERY_VOLTAGE,
    CONF_ID,
    CONF_ICON,
    CONF_THROTTLE,
    UNIT_VOLT,
    UNIT_MINUTE,
    UNIT_CELSIUS,
    UNIT_DEGREES,
    UNIT_PERCENT,
    UNIT_VOLT_AMPS,
    UNIT_WATT,
    UNIT_AMPERE,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_POWER,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
)

CONF_POWER_SWITCH = 'power_switch'
CONF_ENGINE_RPM = 'engine_rpm'
CONF_RUNTIME = 'runtime'
CONF_CYLINDER_HEAD_TEMPERATURE = 'cylinder_head_temperature'
CONF_ENGINE_TIMING = 'engine_timing'
CONF_OUTPUT_VOLTAGE = 'output_voltage'
CONF_BAROMETRIC_PRESSURE = 'barometric_pressure'
CONF_FUEL_INJECTION_TIME = 'fuel_injection_time'
CONF_OUTPUT_POWER = 'output_power'
CONF_STARTER_COUNT = 'starter_count'
CONF_O2_VOLTS = 'o2_sensor'
CONF_INV_MASTER_CURRENT = 'inv_master_current'
CONF_INV_MASTER_TEMPERATURE = 'inv_master_temperature'
CONF_INV_MASTER_ECO = 'inv_master_eco'
CONF_INV_MASTER_POWER = 'inv_master_power'
CONF_INV_MASTER_VOLTS = 'inv_master_volts'
CONF_INV_SLAVE_CURRENT = 'inv_slave_current'
CONF_INV_SLAVE_TEMPERATURE = 'inv_slave_temperature'
CONF_INV_SLAVE_POWER = 'inv_slave_power'
UNIT_RPM = 'rpm'
UNIT_PRESSURE = 'kPa'
UNIT_MILLISECOND = 'ms'
ICON_RPM = 'mdi:speedometer'
ICON_RUNTIME = 'mdi:clock-outline'
ICON_DEGREES = 'mdi:engine-outline'
ICON_PRESSURE = 'mdi:cloud-outline'
ICON_FUEL_INJECTION_TIME = 'mdi:wrench-clock'
ICON_THROTTLE = 'mdi:speedometer'
ICON_STARTER_COUNT = 'mdi:wrench'
ICON_INVERTER_MODE = 'mdi:water-outline'

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]

voltage_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_VOLT,
    accuracy_decimals=1,
    device_class=DEVICE_CLASS_VOLTAGE,
    state_class=STATE_CLASS_MEASUREMENT,
)

rpm_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_RPM,
    accuracy_decimals=0,
    state_class=STATE_CLASS_MEASUREMENT,
    icon=ICON_RPM,
)

uptime_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_MINUTE,
    accuracy_decimals=0,
    state_class=STATE_CLASS_TOTAL_INCREASING,
    icon=ICON_RUNTIME,
)

cylinder_head_temp_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_CELSIUS,
    accuracy_decimals=0,
    state_class=STATE_CLASS_MEASUREMENT,
    device_class=DEVICE_CLASS_TEMPERATURE,
)

engine_timing_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_DEGREES,
    accuracy_decimals=0,
    state_class=STATE_CLASS_MEASUREMENT,
    icon=ICON_DEGREES,
)

barometric_pressure_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_PRESSURE,
    accuracy_decimals=0,
    state_class=STATE_CLASS_MEASUREMENT,
    icon=ICON_PRESSURE,
)

fuel_injection_time_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_MILLISECOND,
    accuracy_decimals=2,
    state_class=STATE_CLASS_MEASUREMENT,
    icon=ICON_FUEL_INJECTION_TIME,
)

throttle_position_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_PERCENT,
    accuracy_decimals=1,
    state_class=STATE_CLASS_MEASUREMENT,
    icon=ICON_THROTTLE,
)

output_power_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_VOLT_AMPS,
    accuracy_decimals=0,
    device_class=DEVICE_CLASS_POWER,
    state_class=STATE_CLASS_MEASUREMENT,
)

starter_count_schema = sensor.sensor_schema(
    accuracy_decimals=0,
    state_class=STATE_CLASS_TOTAL_INCREASING,
    icon=ICON_STARTER_COUNT,
)

o2_volts_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_VOLT,
    accuracy_decimals=1,
    device_class=DEVICE_CLASS_VOLTAGE,
    state_class=STATE_CLASS_MEASUREMENT,
)

inv_current_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_AMPERE,
    accuracy_decimals=1,
    device_class=DEVICE_CLASS_POWER,
    state_class=STATE_CLASS_MEASUREMENT,
)

inv_temperature_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_CELSIUS,
    accuracy_decimals=0,
    state_class=STATE_CLASS_MEASUREMENT,
    device_class=DEVICE_CLASS_TEMPERATURE,
)

inv_mode_schema = binary_sensor.BINARY_SENSOR_SCHEMA.extend({
    cv.Optional(CONF_ICON, default=ICON_INVERTER_MODE): cv.icon
})

inv_power_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_WATT,
    accuracy_decimals=0,
    device_class=DEVICE_CLASS_POWER,
    state_class=STATE_CLASS_MEASUREMENT,
)

inv_volts_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_VOLT,
    accuracy_decimals=0,
    device_class=DEVICE_CLASS_VOLTAGE,
    state_class=STATE_CLASS_MEASUREMENT,
)

gen_ns = cg.esphome_ns.namespace("honda_gen_dlc")
GENComponent = gen_ns.class_("HondaGenDLCComponent", cg.Component, uart.UARTDevice)


CONFIG_SCHEMA = cv.All(
    cv.Schema({
        cv.GenerateID(): cv.declare_id(GENComponent),
        cv.Optional(CONF_POWER_SWITCH): cv.use_id(binary_sensor.BinarySensor),
        # ECU Info1
        cv.Optional(CONF_BATTERY_VOLTAGE): voltage_schema,
        cv.Optional(CONF_ENGINE_RPM): rpm_schema,
        cv.Optional(CONF_RUNTIME): uptime_schema,
        cv.Optional(CONF_CYLINDER_HEAD_TEMPERATURE): cylinder_head_temp_schema,
        cv.Optional(CONF_ENGINE_TIMING): engine_timing_schema,
        cv.Optional(CONF_OUTPUT_VOLTAGE): voltage_schema,
        # ECU Info2
        cv.Optional(CONF_BAROMETRIC_PRESSURE): barometric_pressure_schema,
        cv.Optional(CONF_FUEL_INJECTION_TIME): fuel_injection_time_schema,
        cv.Optional(CONF_THROTTLE): throttle_position_schema,
        cv.Optional(CONF_OUTPUT_POWER): output_power_schema,
        cv.Optional(CONF_STARTER_COUNT): starter_count_schema,
        cv.Optional(CONF_O2_VOLTS): o2_volts_schema,
        # Inverter Master
        cv.Optional(CONF_INV_MASTER_CURRENT): inv_current_schema,
        cv.Optional(CONF_INV_MASTER_TEMPERATURE): inv_temperature_schema,
        cv.Optional(CONF_INV_MASTER_ECO): inv_mode_schema,
        cv.Optional(CONF_INV_MASTER_POWER): inv_power_schema,
        cv.Optional(CONF_INV_MASTER_VOLTS): inv_volts_schema,
        # Slave Inverter
        cv.Optional(CONF_INV_SLAVE_CURRENT): inv_current_schema,
        cv.Optional(CONF_INV_SLAVE_TEMPERATURE): inv_temperature_schema,
        cv.Optional(CONF_INV_SLAVE_POWER): inv_power_schema,
    })
    .extend(cv.polling_component_schema("2s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)

FINAL_VALIDATE_SCHEMA = uart.final_validate_device_schema("honda_gen_dlc", require_rx=True, require_tx=True,
                                                          baud_rate=9600)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_POWER_SWITCH in config:
        bin = await cg.get_variable(config[CONF_POWER_SWITCH])
        cg.add(var.set_power_switch_sensor(bin))

    #ECU Info1
    if CONF_BATTERY_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_VOLTAGE])
        cg.add(var.set_battery_voltage_sensor(sens))
    if CONF_ENGINE_RPM in config:
        sens = await sensor.new_sensor(config[CONF_ENGINE_RPM])
        cg.add(var.set_engine_rpm_sensor(sens))
    if CONF_RUNTIME in config:
        sens = await sensor.new_sensor(config[CONF_RUNTIME])
        cg.add(var.set_runtime_sensor(sens))
    if CONF_CYLINDER_HEAD_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_CYLINDER_HEAD_TEMPERATURE])
        cg.add(var.set_cylinder_head_temperature_sensor(sens))
    if CONF_ENGINE_TIMING in config:
        sens = await sensor.new_sensor(config[CONF_ENGINE_TIMING])
        cg.add(var.set_engine_timing_sensor(sens))
    if CONF_OUTPUT_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_OUTPUT_VOLTAGE])
        cg.add(var.set_output_voltage_sensor(sens))

    #ECU Info2
    if CONF_BAROMETRIC_PRESSURE in config:
        sens = await sensor.new_sensor(config[CONF_BAROMETRIC_PRESSURE])
        cg.add(var.set_barometric_pressure_sensor(sens))
    if CONF_FUEL_INJECTION_TIME in config:
        sens = await sensor.new_sensor(config[CONF_FUEL_INJECTION_TIME])
        cg.add(var.set_fuel_injection_time_sensor(sens))
    if CONF_THROTTLE in config:
        sens = await sensor.new_sensor(config[CONF_THROTTLE])
        cg.add(var.set_throttle_sensor(sens))
    if CONF_OUTPUT_POWER in config:
        sens = await sensor.new_sensor(config[CONF_OUTPUT_POWER])
        cg.add(var.set_output_power_sensor(sens))
    if CONF_STARTER_COUNT in config:
        sens = await sensor.new_sensor(config[CONF_STARTER_COUNT])
        cg.add(var.set_starter_count_sensor(sens))
    if CONF_O2_VOLTS in config:
        sens = await sensor.new_sensor(config[CONF_O2_VOLTS])
        cg.add(var.set_o2_volts_sensor(sens))

    #Inverter Master
    if CONF_INV_MASTER_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_INV_MASTER_CURRENT])
        cg.add(var.set_inv_master_current(sens))
    if CONF_INV_MASTER_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_INV_MASTER_TEMPERATURE])
        cg.add(var.set_inv_master_temperature(sens))
    if CONF_INV_MASTER_ECO in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_INV_MASTER_ECO])
        cg.add(var.set_inv_master_eco(sens))
    if CONF_INV_MASTER_POWER in config:
        sens = await sensor.new_sensor(config[CONF_INV_MASTER_POWER])
        cg.add(var.set_inv_master_power(sens))
    if CONF_INV_MASTER_VOLTS in config:
        sens = await sensor.new_sensor(config[CONF_INV_MASTER_VOLTS])
        cg.add(var.set_inv_master_volts(sens))

    #Inverter Slave
    if CONF_INV_SLAVE_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_INV_SLAVE_CURRENT])
        cg.add(var.set_inv_slave_current(sens))
    if CONF_INV_SLAVE_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_INV_SLAVE_TEMPERATURE])
        cg.add(var.set_inv_slave_temperature(sens))
    if CONF_INV_SLAVE_POWER in config:
        sens = await sensor.new_sensor(config[CONF_INV_SLAVE_POWER])
        cg.add(var.set_inv_slave_power(sens))
