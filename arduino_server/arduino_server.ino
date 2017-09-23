#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "FileManager.h"

ESP8266WebServer server(80);

void setup() {

    Serial.begin(115200);
    WiFi.begin("Agustin", "doctor reformulemelo");

    if (!SPIFFS.begin())
        Serial.println("No se pudo abrir el file system.");

	Serial.println();
    while (WiFi.status() != WL_CONNECTED) {
        delay(100); Serial.print(".");
    }
    Serial.println();
    

    Serial.print("IP address: "); Serial.println(WiFi.localIP());

    server.serveStatic("/static/favicon.png", SPIFFS, "/static/favicon.png");
    server.serveStatic("/static/bootstrap.min.css", SPIFFS, "/static/bootstrap.min.css");
    server.serveStatic("/static/jquery-3.2.1.min.js", SPIFFS, "/static/jquery-3.2.1.min.js");
    server.serveStatic("/static/popper.min.js", SPIFFS, "/static/popper.min.js");
    server.serveStatic("/static/bootstrap.min.js", SPIFFS, "/static/bootstrap.min.js");
    server.serveStatic("/static/my-styles.css", SPIFFS, "/static/my-styles.css");
    server.on("/config.html", HTTP_GET, handle_config_get);
    server.on("/config.html", HTTP_POST, handle_config_post);
    server.on("/", HTTP_GET, handle_index_get);
    server.on("/", HTTP_POST, handle_index_post);

    server.begin();
}

void handle_index_get() {
	server.send(200, "text/html", FileManager::read_file("/index.html"));
}

void handle_index_post() {
	for (int i = 0; i < server.args(); i++ ) {
        Serial.println(server.argName(i));
    }
	server.send(200, "text/html", FileManager::read_file("/index.html"));
}

void handle_config_get() {
    server.send(200, "text/html", FileManager::read_file("/config.html"));
}

void handle_config_post() {
    for (int i = 0; i < server.args(); i++ ) {
        Serial.println(server.argName(i));
    }
    server.send(200, "text/html", FileManager::read_file("/config.html"));
}

void loop() {
    server.handleClient();
}
