#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "FileManager.h"
#include "LedMatrix.h"
#include "Authentication.h"
#include "LedMatrix.h"
#include "WString.h"

ESP8266WebServer server(80);

String index_html_file;
String config_html_file;

void go_to_config(int status);
void go_to_index(int status, String message);
void handle_post_authentication();
void handleNotFound();
void handle_post_phrase();
void handle_post_matrix();
void handle_post_predefined();

void setup()
{
    Serial.begin(115200);
    WiFi.begin("Agustin", "doctor reformulemelo");

    if (!SPIFFS.begin())
        Serial.println("No se pudo abrir el file system.");

    index_html_file = FileManager::read_file("/index.html");
    config_html_file = FileManager::read_file("/config.html");

    Serial.println();
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.serveStatic("/static/favicon.png", SPIFFS, "/static/favicon.png");
    server.serveStatic("/static/bootstrap.min.css", SPIFFS, "/static/bootstrap.min.css");
    server.serveStatic("/static/jquery-3.2.1.min.js", SPIFFS, "/static/jquery-3.2.1.min.js");
    server.serveStatic("/static/popper.min.js", SPIFFS, "/static/popper.min.js");
    server.serveStatic("/static/bootstrap.min.js", SPIFFS, "/static/bootstrap.min.js");
    server.serveStatic("/static/my-styles.css", SPIFFS, "/static/my-styles.css");
    server.serveStatic("/", SPIFFS, "/index.html");

    server.onNotFound(handleNotFound);
    server.on("/authentication", HTTP_POST, handle_post_authentication);
    server.on("/phrase", HTTP_POST, handle_post_phrase);
    server.on("/matrix", HTTP_POST, handle_post_matrix);
    server.on("/predefined", HTTP_POST, handle_post_predefined);

    server.begin();
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
    go_to_config(302);
}

void handle_post_matrix()
{
    for (int row = 0; row < ROWS; row++)
        for (int column = 0; column < COLUMNS; column++)
            matrix[row][column] = server.arg(String(row) + "-" + String(column)) == "1" ? 1 : 0;

    //for debug
    for (int row = 0; row < ROWS; row++)
    {
        for (int column = 0; column < COLUMNS; column++)
        {
            if (matrix[row][column] == 1)
                Serial.print("X");
            else
                Serial.print("0");
        }
        Serial.println("");
    }
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

void loop()
{
    server.handleClient();
}
