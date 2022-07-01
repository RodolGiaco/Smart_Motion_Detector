#include "wifi.h"
#include "configuracionMovimiento.h"
int extrusorCaliente = 0, calentandoExtrusor = 0;
void setup()
{
  Serial.begin(115200);
  inicializar_Wifi();
  inicializar_Movimiento(); //configura el sensor MPU6050 para la adquisicion de datos para su posterior envio 
}

void loop()
{
  int valor_LeidoExtrusor, movimiento_Leido;
  valor_LeidoExtrusor = voltImpresora(analogRead(A0)); //se manda el valor leido de la entrada analogica 0 y se recibe en funcion de eso un 1 o 0


  
  ///////////------>>>>>ESTABILIZACION TEMPERATURA<<<<<<<<------/////////
  if (valor_LeidoExtrusor == 1 && extrusorCaliente == 0) { // Esto se utiliza para evitar que cuando apenas se prenda la impresora
    Serial.println("Extrusor Calentado");                  // no avise que esta parada hasta que el valor_LeidoExtrusor no sea
    extrusorCaliente = 1;                                  // igual a 1 o sea el extrusor llegue a 195°C
    calentandoExtrusor = 1;                                // Una vez calentado el extrusor no se verifica mas el estado de este 
    //delay(30000);                                        // Tiempo de resguardo para evitar que la impresora diga que este parada 
  }                                                        // mientras se estabiliza la temperatura

  
  if ( valor_LeidoExtrusor == 0 && calentandoExtrusor == 0 ) {  // si el extrusor no esta caliente sigue 
    Serial.println(analogRead(A0));                             // verificando su parametro de referencia (109)
    Serial.println("Calentando Extrusor");                     
    detectando_Movimiento(valor_LeidoExtrusor);
    delay(1000);
  } 
  
  
  //////////------>>>>>>>DETECCION DE MOVIMIENTO<<<<<<-------///////////
  movimiento_Leido = detectando_Movimiento(valor_LeidoExtrusor); // Se llama a la funcion detectar movimiento 
  if (movimiento_Leido == 1) {                                   // y deacuerdo si se movio el eje x recibimos un 1 
    Serial.println("Impresora moviendose");                      // y si no un 0 que es en el caso de parada la impresora
    postear_Datos(1);                                            // Se manda un 1 que significa que esta en movimiento 
  }                          
  if (movimiento_Leido == 0) {
    Serial.println("Impresora Parada");
    postear_Datos(0);
  }
  delay(3);
}
