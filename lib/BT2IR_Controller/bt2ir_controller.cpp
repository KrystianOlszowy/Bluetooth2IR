#include "bt2ir_controller.hpp"

bt2ir::Controller::Controller()
{
    this->connection = bt2ir::Connection::getInstance();
}

u_int8_t bt2ir::Controller::getButtonType()
{
    return buttonType;
}

void bt2ir::Controller::updateButtonType()
{
    NimBLECharacteristic *characteristic = connection->getButtonTypeCharacteristic();
    Serial.print(characteristic->getUUID().toString().c_str());
    Serial.print(": onWrite(), value: ");
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