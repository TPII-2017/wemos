#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "FileManager.h"

ESP8266WebServer server(80);

void setup() {

    Serial.begin(115200);
    WiFi.begin("Agustin", "doctor reformulemelo");

    if (SPIFFS.begin())
        Serial.println("Todo piola");
    else
        Serial.println("Nada piola");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Waiting to connect…");
    }

    Serial.print("IP address: "); Serial.println(WiFi.localIP());

    server.serveStatic("/static/favicon.png", SPIFFS, "/static/favicon.png");
    server.serveStatic("/static/css/bootstrap.min.css", SPIFFS, "/static/bootstrap-4.0.0-beta/css/bootstrap.min.css");
    server.serveStatic("/static/jquery-3.2.1.min.js", SPIFFS, "/static/jquery-3.2.1.min.js");
    server.serveStatic("/static/popper.min.js", SPIFFS, "/static/popper.min.js");
    server.serveStatic("/static/js/bootstrap.min.js", SPIFFS, "/static/bootstrap-4.0.0-beta/js/bootstrap.min.js");
    server.serveStatic("/", SPIFFS, "/index.html");
    server.serveStatic("/config.html", SPIFFS, "/config.html");
    
    server.begin();
    Serial.println("Server listening");
}

void loop() {

    server.handleClient();

}
