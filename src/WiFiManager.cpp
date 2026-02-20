//
// updated by ...: Loreto Notarantonio
// Date .........: 20-02-2026 17.16.03
//


// #include "lnLogger_Class.h"

#include "WiFiManager.h"

WiFiManagerNB::WiFiManagerNB() {}
WiFiManagerNB* WiFiManagerNB::s_instance = nullptr;

void WiFiManagerNB::init(uint32_t scanIntervalWhenConnected, uint32_t scanIntervalWhenNotConnected, uint32_t maxWifiTimeout, int rssiGap) {
    m_scanIntervalWhenConnected = scanIntervalWhenConnected;
    m_scanIntervalWhenNotConnected = scanIntervalWhenNotConnected;
    m_maxWifiTimeout = maxWifiTimeout;
    m_rssiGap = rssiGap;

    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);

    s_instance = this;
    WiFi.onEvent(WiFiEventHandler);

    startScan();
}


void WiFiManagerNB::WiFiEventHandler(WiFiEvent_t event, WiFiEventInfo_t info) {
    if (s_instance) {
        s_instance->onWiFiEvent(event);
    }
}


void WiFiManagerNB::addSSID(const char* ssid, const char* password) {
    WifiCredential cred;
    cred.ssid = ssid;
    cred.password = password;
    m_credentials.push_back(cred);
}

void WiFiManagerNB::update() {
    uint32_t now = millis();

    bool connected = (WiFi.status() == WL_CONNECTED);

    if (connected) {
        m_lastConnectedTime = now;
    }
    else {
        if (now - m_lastConnectedTime > m_maxWifiTimeout) {
            Serial.println("Max WiFi timeout reached. Restarting scan.");
            startScan();
            m_lastConnectedTime = now;
        }
    }

    uint32_t interval = connected ? m_scanIntervalWhenConnected : m_scanIntervalWhenNotConnected;
    if (now - m_lastScanTime >= interval) {
        m_lastScanTime = now;
        startScan();
    }

    int scanStatus = WiFi.scanComplete();
    if (scanStatus >= 0) {
        handleScanResult();
        WiFi.scanDelete();
    }
}

void WiFiManagerNB::startScan() {
    if (WiFi.scanComplete() == -2) {// no scan running
        Serial.println("Starting scan....");
        WiFi.scanNetworks(true); // async
    }
}

void WiFiManagerNB::handleScanResult() {
    int n = WiFi.scanComplete();
    if (n <= 0)
        return;

    int bestRSSI = -1000;
    String bestSSID = "";
    String bestPassword = "";

    for (int i = 0; i < n; ++i) {
        String ssid = WiFi.SSID(i);
        int rssi = WiFi.RSSI(i);
        Serial.print("SSID: ");Serial.print(ssid);Serial.print(" - RSSI: ");Serial.println(rssi);

        for (auto &cred : m_credentials) {
            if (ssid == cred.ssid) {
                if (rssi > bestRSSI) {
                    bestRSSI = rssi;
                    bestSSID = cred.ssid;
                    bestPassword = cred.password;
                }
            }
        }
    }

    if (bestSSID == "") {
        Serial.print("no better SSID found. standing on: ");Serial.println(WiFi.SSID());
        return;
    }

    if (WiFi.status() == WL_CONNECTED) {
        int currentRSSI = WiFi.RSSI();
        if (bestSSID == m_currentSSID) {
            Serial.print("best ssid is already active: ");Serial.println(WiFi.SSID());
            return;
        }

        if ((bestRSSI - currentRSSI) < m_rssiGap) {
            Serial.print("gap between current ssid and best ssid is:");Serial.println(bestRSSI - currentRSSI);
            return;
        }

        Serial.println("Switching to stronger SSID: " + bestSSID);
        WiFi.disconnect();
    }

    m_currentSSID = bestSSID;
    WiFi.begin(bestSSID.c_str(), bestPassword.c_str());
}

void WiFiManagerNB::onWiFiEvent(WiFiEvent_t event) {
    switch (event) {
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            Serial.println("WiFi Connected");
            break;

        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.println("Got IP: " + WiFi.localIP().toString());
            break;

        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("WiFi Disconnected");
            break;

        default:
            break;
    }
}

bool WiFiManagerNB::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManagerNB::getConnectedSSID() {
    return m_currentSSID;
}