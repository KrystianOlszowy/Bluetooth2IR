#pragma once

// biblioteki bluetooth
#include <nimBLEDevice.h>
#include <nimBLEUtils.h>
#include <nimBLEServer.h>

#include <bt2ir_graphics.hpp>

namespace bt2ir
{
    class Connection
    {
    public:
        static Connection *getInstance();

        void setupConnection();

        void initConnectedDevices();
        void addDeviceToCounter();
        void removeDeviceFromCounter();
        int getConnectedDevices();

        void setConnectionEvent();
        void resetConnnectionEvent();
        bool isConnectionEvent();

        void setButtonTypeEvent();
        void resetButtonTypeEvent();
        bool isButtonTypeEvent();

        void setButtonIrCodeEvent();
        void resetButtonIrCodeEvent();
        bool isButtonIrCodeEvent();

        NimBLECharacteristic *getButtonTypeCharacteristic();
        NimBLECharacteristic *getButtonIrCodeCharacteristic();

        void drawServerEvent(Display &display, bool deviceConnected);

    private:
        static Connection *connection;
        int connectedDevices{};
        bool connectionEvent{true};
        bool buttonTypeEvent{};
        bool buttonIrCodeEvent{};

        const std::string serverName{"Bluetooth2IR for TV"};

        NimBLEServer *serverBt2ir{};
        NimBLECharacteristic *buttonTypeCharacteristic{};
        NimBLECharacteristic *buttonIrCodeCharacteristic{};

        const std::string serviceBt2ir_UIID{"0f761ee5-3da9-40ef-9eb9-702db7e13037"};
        const std::string buttonTypeCharacteristic_UUID{"c7e55ae3-855b-4d3b-be0e-f5153a8830c4"};
        const std::string irCodeCharacteristic_UUID{"84cbc1dd-b84d-4c8f-9df2-602152b3c2e2"};
    };

    class ServerCallbacks : public NimBLEServerCallbacks
    {
        void onConnect(NimBLEServer *serverBLE);
        void onDisconnect(NimBLEServer *serverBLE);
    };

    class ButtonTypeCharacteristicCallbacks : public NimBLECharacteristicCallbacks
    {
        void onWrite(NimBLECharacteristic *characteristic);
    };

    class IrCodeCharacteristicCallbacks : public NimBLECharacteristicCallbacks
    {
        void onWrite(NimBLECharacteristic *characteristic);
    };
}