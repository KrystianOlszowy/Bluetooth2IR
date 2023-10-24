#include "bt2ir_controller.hpp"

#include <IRutils.h>

bt2ir::Controller::Controller()
{
    this->connection = bt2ir::Connection::getInstance();
}

uint8_t bt2ir::Controller::getButtonType()
{
    return buttonType;
}

uint64_t bt2ir::Controller::getButtonIrCode()
{
    return buttonIrCode;
}

void bt2ir::Controller::updateButtonType()
{
    NimBLECharacteristic *characteristic = connection->getButtonTypeCharacteristic();

    if (characteristic->getValue<uint8_t>() >= ButtonType::ZERO && characteristic->getValue<uint8_t>() < ButtonType::LIMIT)
    {
        this->buttonType = characteristic->getValue<uint8_t>();
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

    this->buttonIrCode = characteristic->getValue<uint64_t>();
}