#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h> 
#include "ap_ssid.h"
#include "ledmatrix.h"
#include "FileManager.h"
#include "authentication.h"
#include "WString.h"

ESP8266WebServer server(80); //server init, port 80

void handle_post_authentication();
void handle_get_authentication();
void handleNotFound();
void handle_post_phrase();
void handle_post_matrix();
void handle_post_predefined();
void handle_index();
void handle_admin();
void handle_css();
void handle_js();
void handle_favicon();

void setup()
{
	Serial.begin(115200);

	Serial.println("");
	if (!SPIFFS.begin())
		Serial.println("No se pudo abrir el file system.");

	Serial.println("Setting up access point...");
	Serial.println(SSID);
	Serial.println(PASSWORD);
	WiFi.softAP(SSID, PASSWORD);

	Serial.println();
	// Usually http://192.168.4.1
	Serial.print("IP address: ");
	Serial.println(WiFi.softAPIP());

	//PATHs
	server.onNotFound(handleNotFound);
	server.on("/authentication", HTTP_POST, handle_post_authentication);
	server.on("/authentication", HTTP_GET, handle_get_authentication);
	server.on("/phrase", HTTP_POST, handle_post_phrase);
	server.on("/matrix", HTTP_POST, handle_post_matrix);
	server.on("/predefined", HTTP_POST, handle_post_predefined);
	server.on("/", HTTP_GET, handle_index);
	server.on("/admin", handle_admin);
	server.on("/static/favicon.png", HTTP_GET, handle_favicon);

	server.begin(); //Start webserver
}

void loop()
{
	server.handleClient();
}

void handle_favicon()
{
	File f = SPIFFS.open("/static/favicon.png.gz", "r");
	server.streamFile(f, "image/png");
	f.close();
}

void handle_index()
{
	File f = SPIFFS.open("/config.html.gz", "r");
	server.streamFile(f, "text/html");
	f.close();
}

void handle_admin()
{
	File f = SPIFFS.open("/admin.html.gz", "r");
	server.streamFile(f, "text/html");
	f.close();
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
		server.sendHeader("Location", String("/admin"), true);
		server.send(302, "text/plain", "");
	}
	else
	{
		Serial.println("Acceso denegado");
		handle_get_authentication();
	}
}

void handle_get_authentication()
{
	File f = SPIFFS.open("/authentication.html.gz", "r");
	server.streamFile(f, "text/html");
	f.close();
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
	server.sendHeader("Location", String("/"), true);
	server.send(302, "text/plain", "");
}

void handle_post_matrix()
{
	for (int row = 0; row < ROWS; row++)
		for (int column = 0; column < COLUMNS; column++)
			matrix[row][column] = server.arg(String(row) + "-" + String(column)) == "1" ? LED_ON : LED_OFF;
	ledmatrix_print_serial();   
	server.sendHeader("Location", String("/"), true);
	server.send(302, "text/plain", "");
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
	server.sendHeader("Location", String("/"), true);
	server.send(302, "text/plain", "");
}
