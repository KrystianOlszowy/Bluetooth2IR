#include <SPI.h>
#include <Wire.h>
// biblioteki wyświetlacza
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// biblioteki bluetooth
#include <nimBLEDevice.h>
#include <nimBLEUtils.h>
#include <nimBLEServer.h>

// biblioteki sygnałów podczerwonych

// własne bliblioteki
#include <bt2ir_graphics.hpp>

// globalne definicje dotyczące sterowania
int stan{1};
int connnectedDevices{0};
bool connectionEvent{true};

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
bool buttonEvent{false};

// globalne definicje dotyczące ekranu
#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// globalne definicje dotyczące serwera BLE
#define SERVICE_UUID "0f761ee5-3da9-40ef-9eb9-702db7e13037"
#define CHARACTERISTIC_UUID "c7e55ae3-855b-4d3b-be0e-f5153a8830c4"
#define DESCRIPTOR_UIID "6055f164-06b7-11ee-be56-0242ac120002"

static NimBLEServer *serverBLE{};

class ServerCallbacks : public NimBLEServerCallbacks
{
  void onConnect(NimBLEServer *serverBLE)
  {
    NimBLEDevice::startAdvertising();
    ++connnectedDevices;
    connectionEvent = true;
  };

  void onDisconnect(NimBLEServer *pServer)
  {
    NimBLEDevice::startAdvertising();
    --connnectedDevices;
    connectionEvent = true;
  };
};

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

class DescriptorCallbacks : public NimBLEDescriptorCallbacks
{
  void onRead(NimBLEDescriptor *descriptor)
  {
    Serial.print(descriptor->getUUID().toString().c_str());
    Serial.print(" Someone is reading this descriptor which has value: ");
    Serial.println(descriptor->getStringValue().c_str());
  };
};

static CharacteristicCallbacks characteristicsCallbacks{};
static DescriptorCallbacks descriptorsCallbacks{};

void drawConnectionEvent(Adafruit_SH1106G &, const int screen_width, const int screen_height, bool deviceConnected)
{
  if (deviceConnected)
  {
    display.clearDisplay();
    drawBluetoothConnected(display, SCREEN_WIDTH, SCREEN_HEIGHT, connnectedDevices);
    display.display();
  }
  else
  {
    display.clearDisplay();
    drawBluetoothDisconnected(display, SCREEN_WIDTH, SCREEN_HEIGHT);
    display.display();
  }
}

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
    drawBluetoothInitializing(display, SCREEN_WIDTH, SCREEN_HEIGHT);
    display.display();
  }

  NimBLEDevice::init("Bluetooth2IR for TV");

  serverBLE = NimBLEDevice::createServer();
  serverBLE->setCallbacks(new ServerCallbacks());

  NimBLEService *serviceBLE{serverBLE->createService(SERVICE_UUID)};

  NimBLECharacteristic *buttonPressedCharacteristic{serviceBLE->createCharacteristic(
      CHARACTERISTIC_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE)};

  buttonPressedCharacteristic->setValue(uint(50));
  buttonPressedCharacteristic->setCallbacks(&characteristicsCallbacks);

  NimBLEDescriptor *descriptor{buttonPressedCharacteristic->createDescriptor(DESCRIPTOR_UIID, NIMBLE_PROPERTY::READ)};

  descriptor->setValue(std::string("ID of last presssed button on remote controller aplication."));
  descriptor->setCallbacks(&descriptorsCallbacks);

  serviceBLE->start();

  NimBLEAdvertising *advertising{NimBLEDevice::getAdvertising()};
  advertising->addServiceUUID(serviceBLE->getUUID());
  advertising->setScanResponse(true);
  advertising->start();

  Serial.println("Advertising Started");
}

bool deviceConnected{false};
unsigned long buttonInfoStartTime{0};

void loop()
{
  deviceConnected = connnectedDevices > 0;

  if (connectionEvent)
  {
    drawConnectionEvent(display, SCREEN_WIDTH, SCREEN_HEIGHT, deviceConnected);
    connectionEvent = false;
  }

  switch (stan)
  {
  case 1:
  {
    if (buttonEvent)
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
      drawDigit(display, SCREEN_WIDTH, SCREEN_HEIGHT, button);
      display.display();
      buttonEvent = false;
      break;
    case Button::POWER:
      display.clearDisplay();
      drawPower(display, SCREEN_WIDTH, SCREEN_HEIGHT);
      display.display();
      buttonEvent = false;
      break;
    case Button::MUTE:
      display.clearDisplay();
      drawMute(display, SCREEN_WIDTH, SCREEN_HEIGHT);
      display.display();
      buttonEvent = false;
      break;
    case Button::CHANNEL_UP:
      display.clearDisplay();
      drawChannelUp(display, SCREEN_WIDTH, SCREEN_HEIGHT);
      display.display();
      buttonEvent = false;
      break;
    case Button::CHANNEL_DOWN:
      display.clearDisplay();
      drawChannelDown(display, SCREEN_WIDTH, SCREEN_HEIGHT);
      display.display();
      buttonEvent = false;
      break;
    case Button::VOLUME_UP:
      display.clearDisplay();
      drawVolumeUp(display, SCREEN_WIDTH, SCREEN_HEIGHT);
      display.display();
      buttonEvent = false;
      break;
    case Button::VOLUME_DOWN:
      display.clearDisplay();
      drawVolumeDown(display, SCREEN_WIDTH, SCREEN_HEIGHT);
      display.display();
      buttonEvent = false;
      break;
    case Button::MENU:
      display.clearDisplay();
      drawMenu(display, SCREEN_WIDTH, SCREEN_HEIGHT);
      display.display();
      buttonEvent = false;
      break;
    case Button::OKAY:
      display.clearDisplay();
      drawOK(display, SCREEN_WIDTH, SCREEN_HEIGHT);
      display.display();
      buttonEvent = false;
      break;
    case Button::MOVE_UP:
      display.clearDisplay();
      drawMoveUp(display, SCREEN_WIDTH, SCREEN_HEIGHT);
      display.display();
      buttonEvent = false;
      break;
    case Button::MOVE_DOWN:
      display.clearDisplay();
      drawMoveDown(display, SCREEN_WIDTH, SCREEN_HEIGHT);
      display.display();
      buttonEvent = false;
      break;
    case Button::MOVE_LEFT:
      display.clearDisplay();
      drawMoveLeft(display, SCREEN_WIDTH, SCREEN_HEIGHT);
      display.display();
      buttonEvent = false;
      break;
    case Button::MOVE_RIGHT:
      display.clearDisplay();
      drawMoveRight(display, SCREEN_WIDTH, SCREEN_HEIGHT);
      display.display();
      buttonEvent = false;
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
      drawConnectionEvent(display, SCREEN_WIDTH, SCREEN_HEIGHT, deviceConnected);
      connectionEvent = false;
      stan = 1;
    }
    break;
  }
  }
}