#pragma once

#include <bt2ir_connection.hpp>

namespace bt2ir
{
    enum ButtonType
    {
        ZERO,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        POWER,
        MUTE,
        CHANNEL_UP,
        CHANNEL_DOWN,
        VOLUME_UP,
        VOLUME_DOWN,
        MENU,
        OKAY,
        MOVE_UP,
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT,
        LIMIT
    };

    class Controller
    {
    public:
        Controller();
        void updateButtonType();
        void updateButtonIrCode();
        uint8_t getButtonType();
        uint64_t getButtonIrCode();

    private:
        uint8_t buttonType = bt2ir::ButtonType::LIMIT;
        uint64_t buttonIrCode{};
        Connection *connection{};
    };    
}