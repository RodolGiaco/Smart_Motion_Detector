#include "configuracionMovimiento.h"
#include "globals.h"


void inicializar_Movimiento()
{
  //iniciaremos todas las lecturas a la entrada 0
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  Wire.begin(4, 5);               // D2(GPIO4)=SDA (datos) / D1(GPIO5)=SCL (reloj serial)
  Wire.beginTransmission(MPU);    //iniciamos la transmicion en MPU ->(0x68)
  Wire.write(0x6B);               //escribimos en el registro 0x68 -> SIGNAL_PATH_RESET
  Wire.write(0);
  Wire.endTransmission(true);
}

int detectando_Movimiento(int auxiliar) {

  Wire.beginTransmission(MPU);    //Inicio la transmisión con el MPU (unidad de medicion inercial)
  Wire.write(0x3B);               //Pedir el registro 0x3B que es el alto para aceleracion en x
  Wire.endTransmission(false);    //Finalizo de la transmisión
  Wire.requestFrom(MPU, 2, true); //A partir del 0x3B, se piden solo los registro que vamos a usar del eje X, que es el alto y el bajo

  accelerometerBitsX = Wire.read() << 8 | Wire.read();     //Desplazamos el registro alto de 8 bits y lo unimos al registro bajo para luego leer una variable de 16bits
  accelerometerBitsX = (-1) * (accelerometerBitsX / 1000); //Dividimos entre mil para hacer menos sensible el sensor


  //////////-----------------FILTRO EN X-----------------------------//////////

  total = total - readings[vector];           //Restamos la ultima lectura
  readings[vector] = accelerometerBitsX;      //Leemos el rensor
  total = total + readings[vector];           //Sumamos al total el nuevo valor
  vector = vector + 1;                        //avanzamos a la proxima posicion del array

  if (vector >= numReadings) {                //Si estamos en el final del array 
    vector = 0;                               //entonces volvemos al inicio
    averageX = total / numReadings;           //Calculamos el promedio
  }
  if (averageX >= 5)  contadorMovimiento = 1; //Nos define si vamos para adelante en el eje X
  if (averageX <= -1) contadorMovimiento = 1; //Nos define si vamos para atras en el eje X




  ///////////------CONDICION DE EQUILIBRIO-----//////////
  if (averageX >= 0 && averageX <= 0  && envioUnico == 0) 
  {
    if (contadorMovimiento == 1)
    {
      contadorParada = 0;                //Si hubo movimiento reinicia el contadorParada
      contadorMovimiento = 0;            //Para detectar movimiento de nuevo
      return 1;                          //Retorno un 1 para simular el movimiento
    }
    contadorParada++;

    ////////////------PARADA DE LA IMPRESORA------/////////////
    if (contadorParada == 5000)          //Se usa este contador como delay por 
    {                                    //si hay movimientos largos en el eje Y
      if (auxiliar == 1)                 //Si el valor del argumento pasado a esta funcion 
      {                                  //es 1, es decir el extrusor esta caliente retorna un 0
        envioUnico = 1;                  //Esto es para que solo envie una sola vez la solicitud con el mensaje 0 
        return 0;                        //para indicar que paro y no lo haga mas una vez que este parada
      }
      contadorParada = 0;
    }
  }
  return 2;
}
int voltImpresora(double adc_value)      //Toma un parametro de referencia 109 -> 0.7Volt -> 195°C 
{                                        //para saber que el extrusor llego a la temperatura de impresion
  if (adc_value <= 109.00)               //Se envia un 1 si el valor analogico  ya calento a la temperatura de 195°C
  {                                      //es <109 para indicar que el extrusor
    return 1;
  } else
  {
    return 0;
    contadorParada = 0;                  //Cada vez que el extrusor este calentando esto 
  }                                      //evita que envie un 1 para indicar que esta parada

}
