#include <Arduino.h>
#include <LOTODA.h>
#include <LOTODA-Config.h>
#include <WiFi.h>

/**
 * Relay parameters: NUMBER_OF_RELAYS is from 1 to 3.
 */
#define NUMBER_OF_RELAYS 1
#define buttonpin 0
DEVICE _Device;
RELAY _Relay[NUMBER_OF_RELAYS];

int buttonstate = 0;
int laststateRelay = 0;
String relaystate;
const int debounceDelay = 100; // Thời gian chống dội (milliseconds)


/**
 * WiFi parameters.
 */
const char* ssid      = "LOTODA";
const char* password  = "0388187172";
WiFiClient client;

/**
 * Initialize the connection with the cloud.
 */
String useridkey    = "Y9L4MytvWM";
String passidkey  = "xY1SI02Y9Oi2USoD25Fb";
String deviceid      = "LotodaEsp32-1234";
LOTODA lotoda = LOTODA(useridkey, passidkey, deviceid);

void setup() {
    Serial.begin(9600);
    delay(10);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(50);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connected to LOTODA");


    /**
     * Set the relay pin (is from 1 to 3).
     */
    _Relay[0].gpio = 2;
    pinMode(_Relay[0].gpio, OUTPUT);
    pinMode(buttonpin, INPUT_PULLUP);
    setControlRelay(NUMBER_OF_RELAYS);

    /**
     * Set the debug printer (optional).
     */
    lotoda.setDebugPrinter(&Serial);
    /**
     * Set the connect client.
     */
    lotoda.setConnectClient(&client);
}

void loop() {
    int reading = digitalRead(buttonpin);
    if(reading == 0){
        delay(debounceDelay);
        reading = digitalRead(buttonpin);
        if(reading == 0) {
            digitalWrite(_Relay[0].gpio, !digitalRead(_Relay[0].gpio));
            laststateRelay = digitalRead(_Relay[0].gpio);
        }
    }
    if (laststateRelay == 0){
        relaystate = "0";
    } else {
        relaystate = "1";
    }
    String relaytopic = "Y9L4MytvWM/stat/LotodaEsp32-1234/POWER1";
    lotoda.stat(relaytopic, relaystate);
    _Relay[0].state.powerOnStr = relaystate;

    lotoda.getControlRelay();
    lotoda.loop();
    /**
     * State of ControlRelay.
     */
    lotoda.stateControlRelay(NUMBER_OF_RELAYS);
}