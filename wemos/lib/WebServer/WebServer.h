#pragma once


#define WEB_NAME		"www.ledcartel.com"
#define WEB_SSID		"Remera LED"
#define WEB_PASSWORD	"12345678"

#define TOKEN			"tres tristes tigres"



// Esta clase se encarga de establecer una red Wifi, con las credenciales
// definidas en el encabezado de este archivo. Además inicia un servidor DNS
// para asociar el nombre del dominio con la dirección IP que el Wemos D1
// posee. De esta forma se puede acceder al servidor WEB sin tener que recordar
// la dirección IP del microcontrolador.
class WebServer {

public:

	// Inicializa la red Wifi, con las credenciales definidas en este archivo.
	// Además inicia el servidor DNS y el servidor WEB seteando los callbacks, 
	// para cada petición que realice el cliente,
	static void init();

	// Atiende los pedidos que realice el cliente ya sea para el servidor DNS o
	// para el servidor WEB.
	static void tick();

private:

	// Maneja el POST del proceso de autenticación.
	static void handlePostAuthentication();
	// Maneja el GET del proceso de autenticación.
	static void handleGetAuthentication();
	// Maneja el evento, cuando no se encuentra la dirección solicitada.
	static void handleNotFound();
	// Maneja el POST del evento de postear una frase.
	static void handlePostPhrase();
	// Maneja el POST del proceso de postear una matriz.
	static void handlePostMatrix();
	// Maneja el POST del proceso de postear un sprite predefinido.
	static void handlePostPredefined();
	// Maneja el POST del proceso de partyOn.
	static void handlePostPartyOn();
	// Maneja el GET del proceso de index.
	static void handleGetIndex();
	// Maneja el POST del proceso de autenticación.
	static void handleAdmin();
	// Devuelve los archivos css.
	static void handleCss();
	// Devuelve los archivos js.
	static void handleJs();
	// Devuelve el ícono.
	static void handleGetFavicon();


};
