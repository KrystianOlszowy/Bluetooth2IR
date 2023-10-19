#include "bt2ir_controller.hpp"

#include <IRutils.h>

bt2ir::Controller::Controller()
{
    this->connection = bt2ir::Connection::getInstance();
}

u_int8_t bt2ir::Controller::getButtonType()
{
    return buttonType;
}

u_int64_t bt2ir::Controller::getButtonIrCode()
{
    return buttonIrCode;
}

void bt2ir::Controller::updateButtonType()
{
    NimBLECharacteristic *characteristic = connection->getButtonTypeCharacteristic();
    Serial.print("Button type: onWrite(), value: ");
    Serial.println(*characteristic->getValue());

    if (*characteristic->getValue() >= ButtonType::ZERO && *characteristic->getValue() < ButtonType::LIMIT)
    {
        this->buttonType = *characteristic->getValue();
        connection->setButtonTypeEvent();
    }
    else
    {
        this->buttonType = ButtonType::LIMIT;
    }
}

void bt2ir::Controller::updateButtonIrCode()
{
    NimBLECharacteristic *characteristic = connection->getButtonIrCodeCharacteristic();
    Serial.print("Button IR code: onWrite(), value: ");

    this->buttonIrCode = characteristic->getValue<u_int32_t>();
    serialPrintUint64(buttonIrCode, DEC);
    Serial.print(", ");
    serialPrintUint64(buttonIrCode, HEX);
}