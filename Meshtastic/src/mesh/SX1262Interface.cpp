#include "SX1262Interface.h"
#include "configuration.h"
#include "error.h"

SX1262Interface::SX1262Interface(LockingArduinoHal *hal, RADIOLIB_PIN_TYPE cs, RADIOLIB_PIN_TYPE irq, RADIOLIB_PIN_TYPE rst,
                                 RADIOLIB_PIN_TYPE busy)
    : SX126xInterface(hal, cs, irq, rst, busy)
{
    LOG_INFO("SX126xInterface(cs=%d, irq=%d, rst=%d, busy=%d)\n", cs, irq, rst, busy);
}