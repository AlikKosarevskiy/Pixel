// In this combined code, the NeoPixel control code is integrated into the MQTT callback function. 
//When a message is received on the "Game" topic, it checks the command and sets the LEDs' colors accordingly. 
//Messages "1" set the LEDs to red, "2" to green, "3" to blue, and "0" turns off all LEDs. 
//This way, the LED strip responds to MQTT messages with the desired colors.
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

const char* mqttDevice = "ESP-01";
const int NodeNumber = 1;
const char* ssid = "Redmi";
const char* password = "34253425";
const char* mqttServer = "192.168.43.151";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

const int buttonPin = 14;
int buttonState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;
unsigned long lastMsgArrivalTime = 0;
unsigned long elapsedTime = 0;
boolean timingStarted = false;

#define PIN 2
#define count_led 80
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(count_led, PIN, NEO_GRB + NEO_KHZ800);

// IPAddress ip(192, 168, 1, 100); // Uniq IP
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);

void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, "Game") == 0) {
    char command = (char)payload[0];
    if (command == '1') {
      // Set all LEDs to red
      for (int i = 0; i < count_led; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 0, 0));
        lastMsgArrivalTime = millis();
      timingStarted = true;
      }
      pixels.show();
    } else if (command == '2') {
      // Set all LEDs to green
      for (int i = 0; i < count_led; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 255, 0));
        lastMsgArrivalTime = millis();
      timingStarted = true;
      }
      pixels.show();
    } else if (command == '3') {
      // Set all LEDs to blue
      for (int i = 0; i < count_led; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 255));
        lastMsgArrivalTime = millis();
      timingStarted = true;
      }
      pixels.show();
    } else if (command == '0') {
      // Turn off all LEDs
      for (int i = 0; i < count_led; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
      pixels.show();
    }
  }
}

void setup() {
  pinMode(15, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(15, HIGH);
  delay(1000);
  digitalWrite(15, LOW);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    if (client.connect("mqttDevice")) {
    } else {
      delay(2000);
    }
  }

  client.subscribe("Game");
}
void loop() {
  client.loop();

  buttonState = digitalRead(14);

  if (buttonState == LOW) {
    if (timingStarted) {
 elapsedTime = millis() - lastMsgArrivalTime;
      client.publish("Node1", String(elapsedTime).c_str());
      timingStarted = false;
      for (int i = 0; i < count_led; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
      pixels.show();
    }
  }
}
