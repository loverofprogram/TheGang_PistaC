// PISTA C

#include <Wire.h>
#include <Adafruit_TCS34725.h>

// Inicializa el sensor TCS34725
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// Pines para el LED RGB 
const int ledRojo = 3;
const int ledVerde = 4;
const int ledAzul = 5;

// Pines para los motores
const int izqln1 = 4;
const int izqln2 = 5;
const int enA = 9;
const int derln3 = 6;
const int derln4 = 7;
const int enB = 10;

// Pines para ultrasónicos
const int trigIzq = 44;
const int echoIzq = 46;
const int trigDer = 9;
const int echoDer = 8;
const int trigCentro = 38;
const int echoCentro = 40;

// Variables para los valores RGB
uint16_t clear, red, green, blue;

// Calibracion de color
int minColor = 10;
int maxColor = 400;

// Umbrales para la detección de colores
int umbralAzul = 150;
int umbralAmarillo = 120;
int umbralRosa = 140;
int umbralNegro = 50;

void setup() {
  Serial.begin(9600);

  // Inicia el sensor TCS34725
  if (tcs.begin()) {
    Serial.println("Sensor TCS34725 inicializado correctamente.");
  } else {
    Serial.println("No se pudo inicializar el sensor TCS34725.");
    while (1);
  }

  // Configura los pines de los LEDs como salida
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);

  // Configura los pines de los motores como salida
  pinMode(izqln1, OUTPUT);
  pinMode(izqln2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(derln3, OUTPUT);
  pinMode(derln4, OUTPUT);
  pinMode(enB, OUTPUT);

  analogWrite(enA, 255);
  analogWrite(enB, 255);

  // Configurar Ultrasonicos
  pinMode(trigCentro, OUTPUT);
  pinMode(echoCentro, INPUT);

  pinMode(trigDer, OUTPUT);
  pinMode(echoDer, INPUT);

  pinMode(trigIzq, OUTPUT);
  pinMode(echoIzq, INPUT);

  // Inicializa los LEDs apagados
  apagarLeds();
}

void loop() {
  // Lee los valores RGB y el valor de "clear"
  tcs.getRawData(&red, &green, &blue, &clear);

  // Normaliza los valores de los colores
  if (clear > 0){
    float rNorm = (float)red / clear * 256;
    float gNorm = (float)green / clear * 256;
    float bNorm = (float)blue / clear * 256;
  }

  // Ver que tal funciona con esto
  int rMap = constrain(map((int)rNorm, minColor, maxColor,0,255),0,255); //red
  int gMap = constrain(map((int)gNorm, minColor, maxColor,0,255),0,255); //green
  int bMap = constrain(map((int)bNorm, minColor, maxColor,0,255),0,255); //blue

  // Muestra los valores RGB normalizados para facilitar la comparación
  Serial.print(" R: "); Serial.print(rMap);
  Serial.print(" G: "); Serial.print(gMap);
  Serial.print(" B: "); Serial.println(bMap);
  Serial.print(" Clear: "); Serial.println(clear);

  // Medir distancias
  long distIzq = medirDistancia(trigIzq, echoIzq);
  long distCen = medirDistancia(trigCentro, echoCentro);
  long distDer = medirDistancia(trigDer, echoDer);

  Serial.print("Izq: "); Serial.print(distIzq);
  Serial.print("  Centro: "); Serial.print(distCen);
  Serial.print("  Der: "); Serial.println(distDer);
 


  if (esColorNegro(rMap, gMap, bMap)) {
    Serial.println("Negro/Obstaculo");
    encenderLed(0,0,0);
    evitarObstaculo();
    return;
  }

  if (distCen < 5){
    detenerMotores();
    delay(200);
    if (distDer > distIzq){
      Serial.println("Gira derecha");
      girarDerecha();
    } else {
      Serial.println("Gira izquierda");
      girarIzquierda();
    }
    delay(600);
    return;
  }



  // Determina el color detectado
  if (esColorAzul(rMap, gMap, bMap)) {
    Serial.println("Azul");
    encenderLedRGB(0,0,255);
  }
  else if (esColorAmarillo(rMap, gMap, bMap)) {
    Serial.println("Amarillo");
    encenderLedRGB(255,255,0);
  }
  else if (esColorRosa(rMap, rMap, rMap)) {
    Serial.println("Rosa");
    encenderLedRGB(255,0,255);
  }

  moverAdelante();
  delay(200);

// Función para apagar todos los LEDs
void apagarLeds() {
  digitalWrite(ledRojo, 0);
  digitalWrite(ledVerde, 0);
  digitalWrite(ledAzul, 0);
}

// Función para encender un LED específico
void encenderLed(int rojo, int verde, int azul) {
  analogWrite(ledRojo, rojo);
  analogWrite(ledVerde, verde);
  analogWrite(ledAzul, azul);
}

 

// Funciones para determinar el color basándose en los valores normalizados
bool esColorAzul(float r, float g, float b) {
  //return (b > umbralAzul && r < umbralAzul && g < umbralAzul);
  return (b > umbralAzul && r < b * 0.5 && g < b *0.7);
}

bool esColorAmarillo(float r, float g, float b) {
  //return (r > umbralAmarillo && g > umbralAmarillo && b < umbralAmarillo);
  return (r > umbralAmarillo && g > umbralAmarillo && b < 100);
}

bool esColorRosa(float r, float g, float b) {
  //return (r > umbralRosa && g < umbralRosa && b > umbralRosa);
  return (r > umbralRosa && b > umbralRosa && g < 100);
}

bool esColorNegro(float r, float g, float b) {
  //return (r < umbralNegro && g < umbralNegro && b < umbralNegro);
  return (r < umbralNegro && g < umbralNegro && b < umbralNegro);
}

// Ultrasonicos
  long medirDistancia(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duracion = pulseIn(echoPin, HIGH, 25000);
    return duracion * 0.034 / 2; // distancia en cm
}

// Función para mover el robot hacia adelante
void moverAdelante() {
  digitalWrite(izqln1, HIGH);
  digitalWrite(izqln2, LOW);
  digitalWrite(derln3, HIGH);
  digitalWrite(derln4, LOW);
}

void detenerMotores(){
  digitalWrite(izqln1, LOW);
  digitalWrite(izqln2, LOW);
  digitalWrite(derln3, LOW);
  digitalWrite(derln4, LOW);
}

void girarDerecha(){
  digitalWrite(izqln1, HIGH);
  digitalWrite(izqln2, LOW);
  digitalWrite(derln3, LOW);
  digitalWrite(derln4, HIGH);
}

void girarIzquierda(){
  digitalWrite(izqln1, LOW);
  digitalWrite(izqln2, HIGH);
  digitalWrite(derln3, HIGH);
  digitalWrite(derln4, LOW);
}


// Función para evitar el obstáculo (casilla negra)
void evitarObstaculo() {
  detenerMotores();
  delay(500);

  //Retroceder un poquito
  digitalWrite(izqln1, LOW);
  digitalWrite(izqln2,HIGH);
  digitalWrite(derln3,LOW);
  digitalWrite(derln4,HIGH);
  delay(600);

  detenerMotores();
  delay(400);

  long distIzq = medirDistancia(trigIzq, echoIzq);
  long distDer = medirDistancia(trigDer, echoDer);

  if (distDer > distIzq){
    girarDerecha();
  } else{
    girarIzquierda();
  }

  delay(600);
  detenerMotores();
  delay(300);
}



