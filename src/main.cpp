#include <SPI.h>
#include <Wire.h>

// biblioteki sygnałów podczerwonych
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>

// własne bliblioteki
#include <bt2ir_graphics.hpp>
#include <bt2ir_connection.hpp>
#include <bt2ir_controller.hpp>

// globalne definicje dotyczące ekranu
#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// inicjalizacja obiiektów zarządzających urządzeniem
bt2ir::Display display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bt2ir::Connection *connection = bt2ir::Connection::getInstance();
bt2ir::Controller controller{};

// globalne definicje dotyczące transmitera IR
const uint16_t receiverPin{5};
IRrecv IRReceiver(receiverPin);
decode_results results;

const uint16_t senderPin{4};
IRsend IRSender(senderPin);

void setup()
{
  Serial.begin(115200);

  // odczekanie czasu do zapalenia się diod ekranu
  delay(250);

  if (!display.begin(i2c_Address, true))
  {
    Serial.print("Couldn't connect to the OLED display at address: ");
    Serial.print(i2c_Address, HEX);
  }
  else
  {
    // Naprawić
    display.clearDisplay();
    display.drawBluetoothInitializing();
    display.display();
  }

  connection->setupConnection();
  Serial.println("Advertising Started");

  // IR, do zmiany
  IRReceiver.enableIRIn();
  IRSender.begin();
  Serial.println();
  Serial.println("IRReceiver and IRSender started. Waiting for signals..");
}

unsigned long buttonInfoStartTime{0};

void loop()
{
  if (connection->isConnectionEvent())
  {
    connection->drawServerEvent(display, connection->getConnectedDevices());
    connection->resetConnnectionEvent();
  }

  if (connection->isButtonTypeEvent())
  {
    controller.updateButtonType();
    connection->resetButtonTypeEvent();
    int buttonType = controller.getButtonType();

    switch (buttonType)
    {
    case bt2ir::ButtonType::ZERO:
    case bt2ir::ButtonType::ONE:
    case bt2ir::ButtonType::TWO:
    case bt2ir::ButtonType::THREE:
    case bt2ir::ButtonType::FOUR:
    case bt2ir::ButtonType::FIVE:
    case bt2ir::ButtonType::SIX:
    case bt2ir::ButtonType::SEVEN:
    case bt2ir::ButtonType::EIGHT:
    case bt2ir::ButtonType::NINE:
      display.clearDisplay();
      display.drawDigit(buttonType);
      display.display();
      break;
    case bt2ir::ButtonType::POWER:
      display.clearDisplay();
      display.drawPower();
      display.display();
      IRSender.sendNEC(0XFEA857);
      break;
    case bt2ir::ButtonType::MUTE:
      display.clearDisplay();
      display.drawMute();
      IRSender.sendNEC(0XFE6897);
      display.display();
      break;
    case bt2ir::ButtonType::CHANNEL_UP:
      display.clearDisplay();
      display.drawChannelUp();
      display.display();
      break;
    case bt2ir::ButtonType::CHANNEL_DOWN:
      display.clearDisplay();
      display.drawChannelDown();
      display.display();
      break;
    case bt2ir::ButtonType::VOLUME_UP:
      display.clearDisplay();
      display.drawVolumeUp();
      display.display();
      break;
    case bt2ir::ButtonType::VOLUME_DOWN:
      display.clearDisplay();
      display.drawVolumeDown();
      display.display();
      break;
    case bt2ir::ButtonType::MENU:
      display.clearDisplay();
      display.drawMenu();
      display.display();
      break;
    case bt2ir::ButtonType::OKAY:
      display.clearDisplay();
      display.drawOK();
      display.display();
      break;
    case bt2ir::ButtonType::MOVE_UP:
      display.clearDisplay();
      display.drawMoveUp();
      display.display();
      break;
    case bt2ir::ButtonType::MOVE_DOWN:
      display.clearDisplay();
      display.drawMoveDown();
      display.display();
      break;
    case bt2ir::ButtonType::MOVE_LEFT:
      display.clearDisplay();
      display.drawMoveLeft();
      display.display();
      break;
    case bt2ir::ButtonType::MOVE_RIGHT:
      display.clearDisplay();
      display.drawMoveRight();
      display.display();
      break;
    default:
      Serial.println("Button code error!");
      break;
    }
    buttonInfoStartTime = millis();
  }

  if (millis() - buttonInfoStartTime >= 500UL)
  {
    connection->drawServerEvent(display, connection->getConnectedDevices());
  }

  // IR
  if (IRReceiver.decode(&results))
  {
    serialPrintUint64(results.value, HEX);
    Serial.println();
    IRReceiver.resume();
  }
}