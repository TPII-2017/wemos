#pragma once

#define WEB_NAME		"www.ledcartel.com"
#define WEB_SSID		"Remera LED"
#define WEB_PASSWORD	"12345678"

#define TOKEN			"tres tristes tigres"

class WebServer {

public:

	static void init();
	static void tick();

private:


	static void handlePostAuthentication();
	static void handleGetAuthentication();
	static void handleNotFound();
	static void handlePostPhrase();
	static void handlePostMatrix();
	static void handlePostPredefined();
	static void handleGetIndex();
	static void handleAdmin();
	static void handleCss();
	static void handleJs();
	static void handleGetFavicon();


};
