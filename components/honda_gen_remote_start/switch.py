import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import switch, binary_sensor, output
from esphome.const import CONF_ID

CONF_POWER_SW_INPUT = 'power_sw_input'
CONF_RUN_INPUT = 'run_input'
CONF_START_OUTPUT = 'start_output'
CONF_STOP_OUTPUT = 'stop_output'

gen_ns = cg.esphome_ns.namespace("honda_gen_remote_start")
GENComponent = gen_ns.class_("HondaGenRemoteStartComponent", cg.PollingComponent, switch.Switch)

CONFIG_SCHEMA = cv.All(
    switch.SWITCH_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(GENComponent),
            cv.Optional(CONF_POWER_SW_INPUT): cv.use_id(binary_sensor.BinarySensor),
            cv.Required(CONF_RUN_INPUT): cv.use_id(binary_sensor.BinarySensor),
            cv.Required(CONF_START_OUTPUT): cv.use_id(output.BinaryOutput),
            cv.Required(CONF_STOP_OUTPUT): cv.use_id(output.BinaryOutput),
        }
    )
    .extend(cv.polling_component_schema("2s"))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    if CONF_POWER_SW_INPUT in config:
        sw = await cg.get_variable(config[CONF_POWER_SW_INPUT])
        cg.add(var.set_power_sw_sensor(sw))

    sw = await cg.get_variable(config[CONF_RUN_INPUT])
    cg.add(var.set_run_sensor(sw))

    bin_out = await cg.get_variable(config[CONF_START_OUTPUT])
    cg.add(var.set_start_output(bin_out))

    bin_out = await cg.get_variable(config[CONF_STOP_OUTPUT])
    cg.add(var.set_stop_output(bin_out))

    await cg.register_component(var, config)
    await switch.register_switch(var, config)

