#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "FileManager.h"
#include "LedMatrix.h"
#include "Authentication.h"
#include "LedMatrix.h"

ESP8266WebServer server(80);

void handle_index() {
	server.send(200, "text/html", FileManager::read_file("/index.html"));
}

void handle_authentication() {
	if(server.arg("token") == TOKEN){
        handle_config();
    }else{
        handle_index();
    }
}

void handle_config() {
    server.send(200, "text/html", FileManager::read_file("/config.html"));
}

void handleNotFound() {
    //redirect to root "/"
    server.sendHeader("Location", String("/"), true);
    server.send ( 302, "text/plain", "");
}

void redirect_config(){
    //redirect to "/config.html"
    server.sendHeader("Location", String("/config.html"), true);
    server.send ( 302, "text/plain", ""); // redirect URI response
}

void handle_matrix(){
    for(int row = 0; row < ROWS; row++){
        for(int column = 0; column < COLUMNS; column++){
            matrix[row][column] = server.arg(String(row) + "-" + String(column))=="1"?1:0;
        }
    }

    //for debug
    for(int row = 0; row < ROWS; row++){
        for(int column = 0; column < COLUMNS; column++){
            if(matrix[row][column] == 1){
                Serial.print("X");                
            }else{
                Serial.print("0");
            }
        }
        Serial.println("");
    }
    redirect_config();   
}

void handle_phrase(){
    String phrase = server.arg("phrase");
    Serial.print("Caracteres que se deberian mostrar: ");
    Serial.println(phrase);
    redirect_config();
}

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
    server.on("/config.html", handle_config);
    server.on("/phrase", handle_phrase);
    server.on("/matrix", handle_matrix);
    server.onNotFound(handleNotFound);
    server.on("/", handle_index);
    server.on("/authentication", handle_authentication);

    server.begin();
}

void loop() {
    server.handleClient();
}