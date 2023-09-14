#include "bt2ir_graphics.hpp"

#include <array>
#include <Fonts/FreeSans24pt7b.h>

// rysowanie wyrównanego napisu na wyświetlaczu
void drawCenteredString(Adafruit_SH110X &display, const std::string &text, const int field_width, const int start_x, const int field_height, const int start_y)
{
    int16_t x1, y1;                   // nieużywane zmienne, tylko aby zadowolić wywołanie funkcji
    uint16_t text_width, text_height; // długość i wysokość tesktu

    display.getTextBounds(text.c_str(), 0, 0, &x1, &y1, &text_width, &text_height);
    display.setCursor(start_x + (field_width - text_width) / 2, start_y + (field_height - text_height) / 2);
    display.println(text.c_str());
}

// rysowanie dużej cyfry
void drawBigDigit(Adafruit_SH110X &display, const int digit, const int field_width, const int start_x, const int start_y)
{
    int16_t x1, y1;                   // nieużywane zmienne, tylko aby zadowolić wywołanie funkcji
    uint16_t text_width, text_height; // długość i wysokość tesktu

    // display.setTextSize(4);
    display.setFont(&FreeSans24pt7b);
    display.getTextBounds(std::to_string(digit).c_str(), 0, 0, &x1, &y1, &text_width, &text_height);

    // wyrównanie w osi x i ustawienie y na zdaną wysokość, zwiększoną o wysokość textu przez niepodstawową czcionkę
    int adjust = (digit == 1) ? -8 : -2;
    display.setCursor(start_x + adjust + (field_width - text_width) / 2, start_y + text_height);
    display.println(std::to_string(digit).c_str());
}

// rysowanie dużej cyfry wraz z opisem
void drawBigDigitWithDescription(Adafruit_SH110X &display, const int screen_width, const int screen_height, const int digit, const std::string &description_text)
{
    constexpr int digit_y_offset = 8;
    constexpr int digit_height = 32;

    display.setTextColor(SH110X_WHITE);
    drawBigDigit(display, digit, screen_width, 0, digit_y_offset);

    display.setFont();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    drawCenteredString(display, description_text, screen_width, 0, screen_height - (digit_y_offset + digit_height), (digit_y_offset + digit_height + 3));
}

// rysowanie ikony wraz z opisem
void drawIconWithDescription(Adafruit_SH110X &display, const int screen_width, const int screen_height, const unsigned char icon[], const std::string &description_text)
{
    constexpr int icon_width = 32;
    constexpr int icon_height = 32;
    constexpr int icon_height_offset = 10;

    display.drawBitmap((screen_width - icon_width) / 2, icon_height_offset, icon, icon_width, icon_height, SH110X_WHITE);

    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    drawCenteredString(display, description_text, screen_width, 0, screen_height - (icon_height_offset + icon_height), (icon_height_offset + icon_height + 1));
}

// rysowanie ikony przycisku przejścia w górę i jego opisu
void drawMoveUp(Adafruit_SH110X &display, const int screen_width, const int screen_height)
{
    const std::string description_text{"Move Up"};
    drawIconWithDescription(display, screen_width, screen_height, moveUp_icon, description_text);
}

// rysowanie ikony przycisku przejścia w dół i jego opisu
void drawMoveDown(Adafruit_SH110X &display, const int screen_width, const int screen_height)
{
    const std::string description_text{"Move Down"};
    drawIconWithDescription(display, screen_width, screen_height, moveDown_icon, description_text);
}

// rysowanie ikony przycisku przejścia w prawo i jego opisu
void drawMoveRight(Adafruit_SH110X &display, const int screen_width, const int screen_height)
{
    const std::string description_text{"Move Right"};
    drawIconWithDescription(display, screen_width, screen_height, moveRight_icon, description_text);
}

// rysowanie ikony przycisku przejścia w lewo i jego opisu
void drawMoveLeft(Adafruit_SH110X &display, const int screen_width, const int screen_height)
{
    const std::string description_text{"Move Left"};
    drawIconWithDescription(display, screen_width, screen_height, moveLeft_icon, description_text);
}

// rysowanie ikony przycisku włączenia/wyłączenia i jego opisu
void drawPower(Adafruit_SH110X &display, const int screen_width, const int screen_height)
{
    const std::string description_text{"Power"};
    drawIconWithDescription(display, screen_width, screen_height, power_icon, description_text);
}

// rysowanie ikony przycisku potwierdzenia i jego opisu
void drawOK(Adafruit_SH110X &display, const int screen_width, const int screen_height)
{
    const std::string description_text{"OK"};
    drawIconWithDescription(display, screen_width, screen_height, OK_icon, description_text);
}

// rysowanie ikony przycisku menu i jego opisu
void drawMenu(Adafruit_SH110X &display, const int screen_width, const int screen_height)
{
    const std::string description_text{"Menu"};
    drawIconWithDescription(display, screen_width, screen_height, menu_icon, description_text);
}

// rysowanie ikony przycisku wyciszenia i jego opisu
void drawMute(Adafruit_SH110X &display, const int screen_width, const int screen_height)
{
    const std::string description_text{"Mute"};
    drawIconWithDescription(display, screen_width, screen_height, mute_icon, description_text);
}

// rysowanie ikony przycisku podgłoszenia i jego opisu
void drawVolumeDown(Adafruit_SH110X &display, const int screen_width, const int screen_height)
{
    const std::string description_text{"Volume Down"};
    drawIconWithDescription(display, screen_width, screen_height, volumeDown_icon, description_text);
}

// rysowanie ikony przycisku przyciszenia i jego opisu
void drawVolumeUp(Adafruit_SH110X &display, const int screen_width, const int screen_height)
{
    const std::string description_text{"Volume Up"};
    drawIconWithDescription(display, screen_width, screen_height, volumeUp_icon, description_text);
}

// rysowanie ikony przycisku kanału niżej i jego opisu
void drawChannelDown(Adafruit_SH110X &display, const int screen_width, const int screen_height)
{
    const std::string description_text{"Channel Down"};
    drawIconWithDescription(display, screen_width, screen_height, channelDown_icon, description_text);
}

// rysowanie ikony przycisku kanału wyżej i jego opisu
void drawChannelUp(Adafruit_SH110X &display, const int screen_width, const int screen_height)
{
    const std::string description_text{"Channel Up"};
    drawIconWithDescription(display, screen_width, screen_height, channelUp_icon, description_text);
}

// rysowanie stanu rozłączenia wszytskich urządzeń bluetooth
void drawBluetoothDisconnected(Adafruit_SH110X &display, const int screen_width, const int screen_height)
{
    const std::string description_text{"No devices connected!"};
    drawIconWithDescription(display, screen_width, screen_height, bluetoothDisconnected_icon, description_text);
}

// rysowanie stanu połączenia określonej liczby urządzeń bluetooth
void drawBluetoothConnected(Adafruit_SH110X &display, const int screen_width, const int screen_height, const int connectedDevices)
{
    const std::string description_text{"Connected devices:" + std::to_string(connectedDevices)};
    drawIconWithDescription(display, screen_width, screen_height, bluetoothConnected_icon, description_text);
}

// rysowanie stanu rozłącznia z urządzeniem bluetooth
void drawBluetoothInitializing(Adafruit_SH110X &display, const int screen_width, const int screen_height)
{
    const std::string description_text{"Initializing..."};
    drawIconWithDescription(display, screen_width, screen_height, bluetoothInitializing_icon, description_text);
}

// rysowanie ikony przycisku cyfry 1 i jego opisu
void drawDigit(Adafruit_SH110X &display, const int screen_width, const int screen_height, int digitToPrint)
{
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

    drawBigDigitWithDescription(display, screen_width, screen_height, digitToPrint, description_text);
}
