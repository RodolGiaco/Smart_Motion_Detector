#include "wifi.h"


const String DIRECCIONPOST = "http://147.182.194.244:7896/iot/d?k=2tggokgpepnvsb2uv4s40d59oc&i=GiacoAcelerometro001";
const String DIRRECIONGET =  "http://147.182.194.244:1026/v2/entities/urn:ngsd-ld:GiacoAcelerometro:001?options=values&attrs=measure";
const char * headerKeys[] = {"fiware-service", "openiot"} ;
const size_t numberOfHeaders = 2;

#define WLAN_SSID  "Neutrino"
#define WLAN_PASS  "Ragnar20"


void inicializar_Wifi() {
  pinMode(3,OUTPUT);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)  //Siemrpe que el estado sea distinto a 
  {                                      //WL_CONNECTED no conectara a internet 
    delay(500);
    Serial.print(".");
  }
  digitalWrite(3, HIGH);
  delay(500);
  digitalWrite(3, LOW);
  delay(500);
  digitalWrite(3, HIGH);
  delay(500);
  digitalWrite(3, LOW);

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
}
void postear_Datos(int valor){
  
   String data;
   HTTPClient http;                               //Declara el objeto de clase HTTPClient
   http.begin(DIRECCIONPOST);                     //Espesifica el destino solicitado
   http.addHeader("Content-Type", "text/plain");  //Espesifica el content-type header

   data = String("m|")+ String(valor);            //Une el string m con el valor de 
   Serial.print("Datos a enviar: ");              //argumento de la funcion que es 1 o 0
   Serial.println(data);
   int httpCode = http.POST(data);                //Envia la solicitud
   String payload = http.getString();             //Get the response payload

   Serial.print("Respuesta del servidor: ");
   Serial.println(httpCode);                      //Print HTTP return code
   Serial.println(payload);                       //Print request response payload
 
   http.end();                                    //Close connection
}
