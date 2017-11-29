#include <WebServer.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <FS.h>
#include <Letter.h>

static IPAddress apIP(192, 168, 0, 128);
static DNSServer dnsServer;
static ESP8266WebServer server(80);

void WebServer::init()
{
	SPIFFS.begin();

	WiFi.mode(WIFI_AP);
  	WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  	WiFi.softAP(WEB_SSID, WEB_PASSWORD);
	dnsServer.setTTL(30);
	dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  	dnsServer.start(53, WEB_NAME, apIP);
	
	server.onNotFound(handleNotFound);
	server.on("/authentication", HTTP_POST, handlePostAuthentication);
	server.on("/authentication", HTTP_GET, handleGetAuthentication);
	server.on("/phrase", HTTP_POST, handlePostPhrase);
	server.on("/matrix", HTTP_POST, handlePostMatrix);
	server.on("/predefined", HTTP_POST, handlePostPredefined);
	server.on("/", HTTP_GET, handleGetIndex);
	server.on("/admin", handleAdmin);
	server.on("/static/favicon.png", HTTP_GET, handleGetFavicon);

	server.begin();
}

void WebServer::tick()
{
	dnsServer.processNextRequest();
	server.handleClient();
}

void WebServer::handleGetFavicon()
{
	File f = SPIFFS.open("/static/favicon.png.gz", "r");
	server.streamFile(f, "image/png");
	f.close();
}

void WebServer::handleGetIndex()
{
	File f = SPIFFS.open("/config.html.gz", "r");
	server.streamFile(f, "text/html");
	f.close();
}

void WebServer::handleAdmin()
{
	File f = SPIFFS.open("/admin.html.gz", "r");
	server.streamFile(f, "text/html");
	f.close();
}

void WebServer::handleNotFound()
{
	server.sendHeader("Location", String("/"), true);
	server.send(302, "text/plain", "");
}

void WebServer::handlePostAuthentication()
{
	if (server.arg("token").equals(TOKEN)) {
		server.sendHeader("Location", String("/admin"), true);
		server.send(302, "text/plain", "");
	} else {
		handleGetAuthentication();
	}
}

void WebServer::handleGetAuthentication()
{
	File f = SPIFFS.open("/authentication.html.gz", "r");
	server.streamFile(f, "text/html");
	f.close();
}

void WebServer::handlePostPhrase()
{
	String value = server.arg("phrase");
	uint8_t size = value.length() + 1;
	char message[size];
	value.toCharArray(message, size);
	
	Letter::setMessage(message, size - 1, server.arg("sliderate").toInt());

	server.sendHeader("Location", String("/"), true);
	server.send(302, "text/plain", "");
}

void WebServer::handlePostMatrix()
{
	uint8_t columns[2 * MAX_COLUMNS];

	for (uint8_t x = 0; x < 2 * MAX_COLUMNS; x++) {
		columns[x] = 0;
		for (int8_t y = MAX_COLUMNS - 1; y >= 0 ; y--) {
			String args = String(y) + "-" + String(x);
			columns[x] |= (server.arg(args) == "1" ? 1 : 0) << y;
		}
	}

	Letter::setMap(columns, 2 * MAX_COLUMNS, server.arg("sliderate").toInt());
	
	server.sendHeader("Location", String("/"), true);
	server.send(302, "text/plain", "");
}

void WebServer::handlePostPredefined()
{
	String value = server.arg("image-predif");
	uint16_t srate = server.arg("sliderate").toInt();

	if (value.equals("smile-face"))
		Letter::setPredefined(Letter::predefined_t::smile, srate);
	else if (value.equals("pacman"))
		Letter::setPredefined(Letter::predefined_t::pacman, srate);
	else if (value.equals("new-life"))
		Letter::setPredefined(Letter::predefined_t::newLife, srate);
	else
		Letter::setPredefined(Letter::predefined_t::noPredefined, 0);

	server.sendHeader("Location", String("/"), true);
	server.send(302, "text/plain", "");
}
