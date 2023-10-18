#include "bt2ir_graphics.hpp"

#include <array>
#include <Fonts/FreeSans24pt7b.h>

// rysowanie ikony przycisku przejścia w górę i jego opisu
void bt2ir::Display::drawMoveUp()
{
    this->clearDisplay();
    const std::string description_text{"Move Up"};
    drawIconWithDescription(moveUp_icon, description_text);
    this->display();
}

// rysowanie ikony przycisku przejścia w dół i jego opisu
void bt2ir::Display::drawMoveDown()
{
    this->clearDisplay();
    const std::string description_text{"Move Down"};
    drawIconWithDescription(moveDown_icon, description_text);
    this->display();
}

// rysowanie ikony przycisku przejścia w prawo i jego opisu
void bt2ir::Display::drawMoveRight()
{
    this->clearDisplay();
    const std::string description_text{"Move Right"};
    drawIconWithDescription(moveRight_icon, description_text);
    this->display();
}

// rysowanie ikony przycisku przejścia w lewo i jego opisu
void bt2ir::Display::drawMoveLeft()
{
    this->clearDisplay();
    const std::string description_text{"Move Left"};
    drawIconWithDescription(moveLeft_icon, description_text);
    this->display();
}

// rysowanie ikony przycisku włączenia/wyłączenia i jego opisu
void bt2ir::Display::drawPower()
{
    this->clearDisplay();
    const std::string description_text{"Power"};
    drawIconWithDescription(power_icon, description_text);
    this->display();
}

// rysowanie ikony przycisku potwierdzenia i jego opisu
void bt2ir::Display::drawOK()
{
    this->clearDisplay();
    const std::string description_text{"OK"};
    drawIconWithDescription(OK_icon, description_text);
    this->display();
}

// rysowanie ikony przycisku menu i jego opisu
void bt2ir::Display::drawMenu()
{
    this->clearDisplay();
    const std::string description_text{"Menu"};
    drawIconWithDescription(menu_icon, description_text);
    this->display();
}

// rysowanie ikony przycisku wyciszenia i jego opisu
void bt2ir::Display::drawMute()
{
    this->clearDisplay();
    const std::string description_text{"Mute"};
    drawIconWithDescription(mute_icon, description_text);
    this->display();
}

// rysowanie ikony przycisku podgłoszenia i jego opisu
void bt2ir::Display::drawVolumeDown()
{
    this->clearDisplay();
    const std::string description_text{"Volume Down"};
    drawIconWithDescription(volumeDown_icon, description_text);
    this->display();
}

// rysowanie ikony przycisku przyciszenia i jego opisu
void bt2ir::Display::drawVolumeUp()
{
    this->clearDisplay();
    const std::string description_text{"Volume Up"};
    drawIconWithDescription(volumeUp_icon, description_text);
    this->display();
}

// rysowanie ikony przycisku kanału niżej i jego opisu
void bt2ir::Display::drawChannelDown()
{
    this->clearDisplay();
    const std::string description_text{"Channel Down"};
    drawIconWithDescription(channelDown_icon, description_text);
    this->display();
}

// rysowanie ikony przycisku kanału wyżej i jego opisu
void bt2ir::Display::drawChannelUp()
{
    this->clearDisplay();
    const std::string description_text{"Channel Up"};
    drawIconWithDescription(channelUp_icon, description_text);
    this->display();
}

// rysowanie stanu rozłączenia wszystkich urządzeń bluetooth
void bt2ir::Display::drawBluetoothDisconnected()
{
    this->clearDisplay();
    const std::string description_text{"No devices connected!"};
    drawIconWithDescription(bluetoothDisconnected_icon, description_text);
    this->display();
}

// rysowanie stanu połączenia określonej liczby urządzeń bluetooth
void bt2ir::Display::drawBluetoothConnected(const int connectedDevices)
{
    this->clearDisplay();
    const std::string description_text{"Connected devices:" + std::to_string(connectedDevices)};
    drawIconWithDescription(bluetoothConnected_icon, description_text);
    this->display();
}

// rysowanie stanu rozłączenia z urządzeniem bluetooth
void bt2ir::Display::drawBluetoothInitializing()
{
    this->clearDisplay();
    const std::string description_text{"Initializing..."};
    drawIconWithDescription(bluetoothInitializing_icon, description_text);
    this->display();
}

// rysowanie ikony przycisku cyfry 1 i jego opisu
void bt2ir::Display::drawDigit(int digitToPrint)
{
    this->clearDisplay();
    std::string description_text{"Error"};
    std::array<const std::string, 10> description_texts = {"Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};

    if (digitToPrint < 0 || digitToPrint > 9)
    {
        digitToPrint = -1;
    }
    else
    {
        description_text = description_texts[digitToPrint];
    }

    drawBigDigitWithDescription(digitToPrint, description_text);

    this->display();
}

// rysowanie wyrównanego tekstu na wyświetlaczu
void bt2ir::Display::drawCenteredString(const std::string &text, const int field_width, const int start_x, const int field_height, const int start_y)
{
    int16_t x1, y1;                   // nieużywane zmienne, tylko aby zadowolić wywołanie funkcji
    uint16_t text_width, text_height; // długość i wysokość tesktu

    this->getTextBounds(text.c_str(), 0, 0, &x1, &y1, &text_width, &text_height);
    this->setCursor(start_x + (field_width - text_width) / 2, start_y + (field_height - text_height) / 2);
    this->println(text.c_str());
}

// rysowanie dużej cyfry
void bt2ir::Display::drawBigDigit(const int digit, const int field_width, const int start_x, const int start_y)
{
    int16_t x1, y1;                   // nieużywane zmienne, tylko aby zadowolić wywołanie funkcji
    uint16_t text_width, text_height; // długość i wysokość tesktu

    // display.setTextSize(4);
    this->setFont(&FreeSans24pt7b);
    this->getTextBounds(std::to_string(digit).c_str(), 0, 0, &x1, &y1, &text_width, &text_height);

    // wyrównanie w osi x oraz ustawienie y na zdaną wysokość doodatkowo zwiększoną o wysokość textu przez niepodstawową czcionkę
    // ta poprawka jest także inna dla cyfry "1"
    int adjust = (digit == 1) ? -8 : -2;
    this->setCursor(start_x + adjust + (field_width - text_width) / 2, start_y + text_height);
    this->println(std::to_string(digit).c_str());
}

// rysowanie dużej cyfry wraz z opisem
void bt2ir::Display::drawBigDigitWithDescription(const int digit, const std::string &description_text)
{
    constexpr int digit_y_offset = 8;
    constexpr int digit_height = 32;

    this->setTextColor(SH110X_WHITE);
    drawBigDigit(digit, this->WIDTH, 0, digit_y_offset);

    this->setFont();
    this->setTextSize(1);
    this->setTextColor(SH110X_WHITE);
    drawCenteredString(description_text, this->WIDTH, 0, this->HEIGHT - (digit_y_offset + digit_height), (digit_y_offset + digit_height + 3));
}

// rysowanie ikony wraz z opisem
void bt2ir::Display::drawIconWithDescription(const unsigned char icon[], const std::string &description_text)
{
    constexpr int icon_width = 32;
    constexpr int icon_height = 32;
    constexpr int icon_height_offset = 10;

    this->drawBitmap((this->WIDTH - icon_width) / 2, icon_height_offset, icon, icon_width, icon_height, SH110X_WHITE);

    this->setTextSize(1);
    this->setTextColor(SH110X_WHITE);
    drawCenteredString(description_text, this->WIDTH, 0, this->HEIGHT - (icon_height_offset + icon_height), (icon_height_offset + icon_height + 1));
}
