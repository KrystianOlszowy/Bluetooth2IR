#include <SPI.h>
#include <Wire.h>

// biblioteki wyświetlacza
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// biblioteki sygnałów podczerwonych
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>

// własne bliblioteki
#include <bt2ir_graphics.hpp>
#include <bt2ir_connection.hpp>

// globalne definicje dotyczące sterowania
int stan{1};

enum Button
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

int button{-1};

// globalne definicje dotyczące ekranu
#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
bt2ir::Display display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// globalne definicje dotyczące transmitera IR
const uint16_t receiverPin{5};
IRrecv IRReceiver(receiverPin);
decode_results results;

const uint16_t senderPin{4};
IRsend IRSender(senderPin);

static NimBLEServer *serverBLE{};

/*
class CharacteristicCallbacks : public NimBLECharacteristicCallbacks
{
  void onWrite(NimBLECharacteristic *characteristic)
  {
    Serial.print(characteristic->getUUID().toString().c_str());
    Serial.print(": onWrite(), value: ");
    Serial.println(*characteristic->getValue());

    if (*characteristic->getValue() >= Button::ZERO && *characteristic->getValue() < Button::LIMIT)
    {
      button = *characteristic->getValue();
      buttonEvent = true;
    }
    else
    {
      button = -1;
    }
  }
};
*/

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
    display.clearDisplay();
    display.drawBluetoothInitializing();
    display.display();
  }

  bt2ir::Connection *connection = bt2ir::Connection::getInstance();
  connection->setupConnection();
  Serial.println("Advertising Started");

  // IR
  IRReceiver.enableIRIn();
  IRSender.begin();
  Serial.println();
  Serial.println("IRReceiver and IRSender started. Waiting for signals..");
}

unsigned long buttonInfoStartTime{0};

void loop()
{
  bt2ir::Connection *connection = bt2ir::Connection::getInstance();

  if (connection->isConnectionEvent())
  {
    Serial.println("przed rysowaniem");
    connection->drawServerEvent(display, connection->getConnectedDevices());
    Serial.println("narysowane");
    connection->resetConnnectionEvent();
    Serial.println("zresetowany event");
  }

  switch (stan)
  {
  case 1:
  {
    if (connection->isButtonTypeEvent())
      stan = 2;
    break;
  }
  case 2:
  {
    switch (button)
    {
    case Button::ZERO:
    case Button::ONE:
    case Button::TWO:
    case Button::THREE:
    case Button::FOUR:
    case Button::FIVE:
    case Button::SIX:
    case Button::SEVEN:
    case Button::EIGHT:
    case Button::NINE:
      display.clearDisplay();
      display.drawDigit(button);
      display.display();
      connection->resetButtonTypeEvent();
      break;
    case Button::POWER:
      display.clearDisplay();
      display.drawPower();
      display.display();
      IRSender.sendNEC(0XFEA857);
      connection->resetButtonTypeEvent();
      break;
    case Button::MUTE:
      display.clearDisplay();
      display.drawMute();
      IRSender.sendNEC(0XFE6897);
      display.display();
      connection->resetButtonTypeEvent();
      break;
    case Button::CHANNEL_UP:
      display.clearDisplay();
      display.drawChannelUp();
      display.display();
      connection->resetButtonTypeEvent();
      break;
    case Button::CHANNEL_DOWN:
      display.clearDisplay();
      display.drawChannelDown();
      display.display();
      connection->resetButtonTypeEvent();
      break;
    case Button::VOLUME_UP:
      display.clearDisplay();
      display.drawVolumeUp();
      display.display();
      connection->resetButtonTypeEvent();
      break;
    case Button::VOLUME_DOWN:
      display.clearDisplay();
      display.drawVolumeDown();
      display.display();
      connection->resetButtonTypeEvent();
      break;
    case Button::MENU:
      display.clearDisplay();
      display.drawMenu();
      display.display();
      connection->resetButtonTypeEvent();
      break;
    case Button::OKAY:
      display.clearDisplay();
      display.drawOK();
      display.display();
      connection->resetButtonTypeEvent();
      break;
    case Button::MOVE_UP:
      display.clearDisplay();
      display.drawMoveUp();
      display.display();
      connection->resetButtonTypeEvent();
      break;
    case Button::MOVE_DOWN:
      display.clearDisplay();
      display.drawMoveDown();
      display.display();
      connection->resetButtonTypeEvent();
      break;
    case Button::MOVE_LEFT:
      display.clearDisplay();
      display.drawMoveLeft();
      display.display();
      connection->resetButtonTypeEvent();
      break;
    case Button::MOVE_RIGHT:
      display.clearDisplay();
      display.drawMoveRight();
      display.display();
      connection->resetButtonTypeEvent();
      break;
    default:
      Serial.println("Button code error!");
      break;
    }
    stan = 3;
    buttonInfoStartTime = millis();
    break;
  }
  case 3:
  {
    unsigned long presentTime = millis();

    if (presentTime - buttonInfoStartTime >= 500UL)
    {
      connection->drawServerEvent(display, connection->getConnectedDevices());
      connection->resetButtonTypeEvent();
      stan = 1;
    }
    break;
  }
  }
  // IR
  if (IRReceiver.decode(&results))
  {
    serialPrintUint64(results.value, HEX);
    Serial.println();
    IRReceiver.resume();
  }
}