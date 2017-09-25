#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h> 
#include "HTML.h"
#include "AP_SSID.h"
#include "ledmatrix.h"
#include "FileManager.h"
#include "authentication.h"
#include "WString.h"

ESP8266WebServer server(80); //server init, port 80

String index_html_file;
String config_html_file;

void go_to_config(int status);
void go_to_index(int status, String message);
void handle_post_authentication();
void handleNotFound();
void handle_post_phrase();
void handle_post_matrix();
void handle_post_predefined();
void handle_index();
void handle_admin();

void setup()
{
    Serial.begin(115200);

    if (!SPIFFS.begin())
        Serial.println("No se pudo abrir el file system.");

    //index_html_file = FileManager::read_file("/index.html");
    config_html_file = FileManager::read_file("/config.html");

    Serial.println("Setting up access point...");
    Serial.println(SSID);
    Serial.println(PASSWORD);
    WiFi.softAP(SSID, PASSWORD);

    Serial.println();
    // Usually http://192.168.4.1
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    server.serveStatic("/static/favicon.png", SPIFFS, "/static/favicon.png");
    server.serveStatic("/static/bootstrap.min.css", SPIFFS, "/static/bootstrap.min.css");
    server.serveStatic("/static/jquery-3.2.1.min.js", SPIFFS, "/static/jquery-3.2.1.min.js");
    server.serveStatic("/static/popper.min.js", SPIFFS, "/static/popper.min.js");
    server.serveStatic("/static/bootstrap.min.js", SPIFFS, "/static/bootstrap.min.js");
    server.serveStatic("/static/my-styles.css", SPIFFS, "/static/my-styles.css");
    //server.serveStatic("/", SPIFFS, "/index.html");

    //PATHs
    server.onNotFound(handleNotFound);
    server.on("/authentication", HTTP_POST, handle_post_authentication);
    server.on("/phrase", HTTP_POST, handle_post_phrase);
    server.on("/matrix", HTTP_POST, handle_post_matrix);
    server.on("/predefined", HTTP_POST, handle_post_predefined);
    server.on("/", HTTP_GET, handle_index);
    server.on("/admin", handle_admin);
    
    server.begin(); //Start webserver
}

void loop()
{
    server.handleClient();
}


void handle_index()
{
    go_to_config(200);
}

void handle_admin()
{
    if(!server.authenticate(USER, PASS))
        return server.requestAuthentication();
    server.send(200, "text/html", "HTML admin en construcción");
}

void handleNotFound()
{
    server.sendHeader("Location", String("/"), true);
    server.send(302, "text/plain", "");
}

void handle_post_authentication()
{
    if (server.arg("token").equals(TOKEN))
    {
        Serial.println("Acceso permitido");
        go_to_config(200);
    }
    else
    {
        Serial.println("Acceso denegado");
        go_to_index(200, "La contraseña ingresada es incorrecta");
    }
}

void go_to_config(int status)
{
    server.send(status, "text/html", config_html_file);
}

void go_to_index(int status, String message)
{
    //TODO tiene que haber una manera de pasar mensajes al html. Ver server.sendContent();
    server.send(status, "text/html", index_html_file);
}

void handle_post_phrase()
{
    String phrase = server.arg("phrase");
    Serial.print("Caracteres que se deberian mostrar: ");
    Serial.println(phrase);
    char aux[5];
    phrase.toCharArray(aux, 5);
    ledmatrix_set_str(aux);
    ledmatrix_print_serial(); 
    go_to_config(302);
}

void handle_post_matrix()
{
    for (int row = 0; row < ROWS; row++)
        for (int column = 0; column < COLUMNS; column++)
            matrix[row][column] = server.arg(String(row) + "-" + String(column)) == "1" ? LED_ON : LED_OFF;
    ledmatrix_print_serial();   
    go_to_config(302);
}

void handle_post_predefined()
{
	String value = server.arg("image-predif");
	if (value.equals("smile-face"))
		Serial.println("Smile Face");
	else if (value.equals("pacman"))
		Serial.println("Pacman");
	else if (value.equals("new-life"))
		Serial.println("New life");
	else
		Serial.println("Error al elegir el predefinido");
    go_to_config(302);
}
