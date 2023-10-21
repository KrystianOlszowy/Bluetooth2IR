#include "bt2ir_connection.hpp"

bt2ir::Connection *bt2ir::Connection::connection = nullptr;

bt2ir::Connection *bt2ir::Connection::getInstance()
{
    if (connection == nullptr)
    {
        connection = new bt2ir::Connection();
    }
    return connection;
}

void bt2ir::Connection::initConnectedDevices()
{
    this->connectedDevices = 0;
}

void bt2ir::Connection::addDeviceToCounter()
{
    ++this->connectedDevices;
}

void bt2ir::Connection::removeDeviceFromCounter()
{
    if (this->connectedDevices > 0)
        --this->connectedDevices;
}

int bt2ir::Connection::getConnectedDevices()
{
    return this->connectedDevices;
}

void bt2ir::Connection::setConnectionEvent()
{
    this->connectionEvent = true;
}

void bt2ir::Connection::resetConnnectionEvent()
{
    connectionEvent = false;
}

bool bt2ir::Connection::isConnectionEvent()
{
    return this->connectionEvent;
}

void bt2ir::Connection::setButtonTypeEvent()
{
    this->buttonTypeEvent = true;
}

void bt2ir::Connection::resetButtonTypeEvent()
{
    this->buttonTypeEvent = false;
}

bool bt2ir::Connection::isButtonTypeEvent()
{
    return this->buttonTypeEvent;
}

void bt2ir::Connection::setButtonIrCodeEvent()
{
    this->buttonIrCodeEvent = true;
}

void bt2ir::Connection::resetButtonIrCodeEvent()
{
    this->buttonIrCodeEvent = false;
}

bool bt2ir::Connection::isButtonIrCodeEvent()
{
    return this->buttonIrCodeEvent;
}

void bt2ir::Connection::setupConnection()
{
    this->initConnectedDevices();
    NimBLEDevice::init(this->serverName);

    this->serverBt2ir = NimBLEDevice::createServer();
    this->serverBt2ir->setCallbacks(new bt2ir::ServerCallbacks());

    NimBLEService *serviceB2tir{serverBt2ir->createService(this->serviceBt2ir_UIID)};

    this->buttonTypeCharacteristic = serviceB2tir->createCharacteristic(
        this->buttonTypeCharacteristic_UUID, NIMBLE_PROPERTY::WRITE);
    this->buttonTypeCharacteristic->setCallbacks(new ButtonTypeCharacteristicCallbacks);

    this->buttonIrCodeCharacteristic = serviceB2tir->createCharacteristic(
        this->irCodeCharacteristic_UUID, NIMBLE_PROPERTY::WRITE);
    this->buttonIrCodeCharacteristic->setCallbacks(new IrCodeCharacteristicCallbacks);
    serviceB2tir->start();

    NimBLEAdvertising *advertising{NimBLEDevice::getAdvertising()};
    advertising->addServiceUUID(serviceB2tir->getUUID());
    advertising->setScanResponse(true);
    advertising->start();
}

NimBLECharacteristic* bt2ir::Connection::getButtonTypeCharacteristic()
{
    return this->buttonTypeCharacteristic;
}

NimBLECharacteristic* bt2ir::Connection::getButtonIrCodeCharacteristic()
{
    return this->buttonIrCodeCharacteristic;
}

void bt2ir::Connection::drawServerEvent(bt2ir::Display &display, bool deviceConnected)
{
    if (deviceConnected)
    {
        display.drawBluetoothConnected(this->connectedDevices);
    }
    else
    {
        display.drawBluetoothDisconnected();
    }
}

void bt2ir::ServerCallbacks::onConnect(NimBLEServer *serverBLE)
{
    NimBLEDevice::startAdvertising();
    bt2ir::Connection::getInstance()->addDeviceToCounter();
    bt2ir::Connection::getInstance()->setConnectionEvent();
};

void bt2ir::ServerCallbacks::onDisconnect(NimBLEServer *serverBLE)
{
    NimBLEDevice::startAdvertising();
    bt2ir::Connection::getInstance()->removeDeviceFromCounter();
    bt2ir::Connection::getInstance()->setConnectionEvent();
};

void bt2ir::ButtonTypeCharacteristicCallbacks::onWrite(NimBLECharacteristic *characteristic)
{
    bt2ir::Connection::getInstance()->setButtonTypeEvent();
}

void bt2ir::IrCodeCharacteristicCallbacks::onWrite(NimBLECharacteristic *characteristic)
{
    bt2ir::Connection::getInstance()->setButtonIrCodeEvent();
}