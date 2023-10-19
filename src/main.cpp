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

// inicjalizacja obiektów zarządzających urządzeniem
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
  delay(250);

  if (!display.begin(i2c_Address, true))
  {
    Serial.print("Couldn't connect to the OLED display at address: ");
    Serial.print(i2c_Address, HEX);
  }
  else
  {
    display.drawBluetoothInitializing();
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
      display.drawDigit(buttonType);
      break;
    case bt2ir::ButtonType::POWER:
      display.drawPower();
      IRSender.sendNEC(0XFEA857);
      break;
    case bt2ir::ButtonType::MUTE:
      display.drawMute();
      IRSender.sendNEC(0XFE6897);
      break;
    case bt2ir::ButtonType::CHANNEL_UP:
      display.drawChannelUp();
      break;
    case bt2ir::ButtonType::CHANNEL_DOWN:
      display.drawChannelDown();
      break;
    case bt2ir::ButtonType::VOLUME_UP:
      display.drawVolumeUp();
      break;
    case bt2ir::ButtonType::VOLUME_DOWN:
      display.drawVolumeDown();
      break;
    case bt2ir::ButtonType::MENU:
      display.drawMenu();
      break;
    case bt2ir::ButtonType::OKAY:
      display.drawOK();
      break;
    case bt2ir::ButtonType::MOVE_UP:
      display.drawMoveUp();
      break;
    case bt2ir::ButtonType::MOVE_DOWN:
      display.drawMoveDown();
      break;
    case bt2ir::ButtonType::MOVE_LEFT:
      display.drawMoveLeft();
      break;
    case bt2ir::ButtonType::MOVE_RIGHT:
      display.drawMoveRight();
      break;
    default:
      Serial.println("Wrong ButtonType value!");
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
    Serial.println("\n Odebrany kod IR: ");
    serialPrintUint64(results.value, HEX);
    Serial.println();
    IRReceiver.resume();
  }

  if (connection->isButtonIrCodeEvent())
  {
    controller.updateButtonIrCode();
    connection->resetButtonIrCodeEvent();
    IRSender.sendNEC(controller.getButtonIrCode());
  }
}