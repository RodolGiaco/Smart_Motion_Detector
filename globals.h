const int numReadings = 15;         //numero de muestras para promedio
int readings[numReadings];          //lecturas de la entrada analogica
int vector = 0;                     //el indice de la lectura actual
int total = 0;                      //Acumulador para el total de muestras sumadas
float averageX = 0;                 //Promedio de muestras en el eje X
int16_t  accelerometerBitsX;        //Valores crudos del MPU6050
int  contadorParada = 0;            //Contador para determinar cuando la impresora esta parada
int contadorMovimiento = 0;         //Se incrementa cada vez que se mueve
int envioUnico=0;                   //Esta variable se pone en 1 si el extrusor esta caliente
#define MPU 0x68                    //Direccion I2C de la IMU
