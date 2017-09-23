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

    server.serveStatic("/stylesheet.css", SPIFFS, "/stylesheet.css");
    server.on("/", handleRootPath);
    
    server.begin();
    Serial.println("Server listening");
}

void loop() {

    server.handleClient();         //Handling of incoming requests

}

void handleRootPath() {            //Handler for the rooth path

    server.send(200, "text/html", FileManager::read_file("/index.html"));
    Serial.println("Se metio al handle");

}
