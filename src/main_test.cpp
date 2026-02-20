//
// updated by ...: Loreto Notarantonio
// Date .........: 20-02-2026 17.54.06
//



#include <Arduino.h>
#include "WiFiManager.h"
// #include "lnLogger_Class.h"

WiFiManagerNB wifiManager;

void setup() {
    Serial.begin(115200);

    wifiManager.addSSID("Casetta", "PASSWORD1");
    wifiManager.addSSID("SSID2", "PASSWORD2");
    wifiManager.addSSID("SSID3", "PASSWORD3");

    wifiManager.init(
        30000,   // scan ogni 30s se connesso
        10000,   // scan ogni 10s se non connesso
        60000,  // timeout max 5 minuti (5*60*1000)
        8        // rssi gap
    );
}

void loop() {
    wifiManager.update();

    // if (wifiManager.isConnected()) {
    //     Serial.println("Connected to: " + wifiManager.getConnectedSSID());
    // }
    delay(100);
}