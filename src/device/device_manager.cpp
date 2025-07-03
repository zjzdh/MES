#include "device/device_manager.h"

void DeviceManager::triggerSensor(int sensorId) {
    modbusClient.writeSingleRegister(
        MODBUS_SENSOR_TRIGGER_ADDR + sensorId,
        1);
}