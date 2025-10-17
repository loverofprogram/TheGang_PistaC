// PISTA C

#include <Wire.h>
#include <Adafruit_TCS34725.h>

// Inicializa el sensor TCS34725
Adafruit_TCS34725 tcs Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

// Pines para el LED RGB 
const int ledRojo = 3;
const int ledVerde = 4;
const int ledAzul = 5;

// Pines para los motores
const int izqln1 = 4;
const int izqln2 = 5;
const int inA = 9;
const int derln3 = 6;
const int derln4 = 7;
const int enB = 10;

// Variables para los valores RGB
uint16_t clear, red, green, blue;

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
  pinMode(Izqln2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(derln3, OUTPUT);
  pinMode(derln4, OUTPUT);
  pinMode(enB, OUTPUT);

  analogWrite(enA, 255);
  analogWrite(enB, 255);

  // Inicializa los LEDs apagados
  apagarLedRGB();
}

void loop() {
  // Lee los valores RGB y el valor de "clear"
  tcs.getRawData(&red, &green, &blue, &clear);

  // Imprime los valores en el monitor serial
  Serial.print("R: "); Serial.print(red);
  Serial.print(" G: "); Serial.print(green);
  Serial.print(" B: "); Serial.print(blue);
  Serial.print(" Clear: "); Serial.println(clear);

  // Normaliza los valores de los colores
  float rNorm = (float)red / clear * 256;
  float gNorm = (float)green / clear * 256;
  float bNorm = (float)blue / clear * 256;

  // Muestra los valores RGB normalizados para facilitar la comparación
  Serial.print("Normalizado - R: "); Serial.print(rNorm);
  Serial.print(" G: "); Serial.print(gNorm);
  Serial.print(" B: "); Serial.println(bNorm);

  // Determina el color detectado
  if (esColorAzul(rNorm, gNorm, bNorm)) {
    encenderLedRGB(0,0,255);
    moverAdelante();
  }
  else if (esColorAmarillo(rNorm, gNorm, bNorm)) {
    encenderLedRGB(255,255,0);
    moverAdelante();
  }
  else if (esColorRosa(rNorm, gNorm, bNorm)) {
    encenderLedRGB(255,0,255);
    moverAdelante();
  }
  else if (esColorNegro(rNorm, gNorm, bNorm)) {
    encenderLed(0,0,0);
    evitarObstaculo();
  }

  delay(500); // Pausa para evitar lecturas muy rápidas
}

// Función para apagar todos los LEDs
void apagarLeds() {
  digitalWrite(ledRojo, LOW);
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAzul, LOW);
}

// Función para encender un LED específico
void encenderLed(int rojo, int verde, int azul) {
  analogWrite(ledRojo, rojo);
  analogWrite(ledVerde, Verde);
  analogWrite(ledAzul, azul);
}

// Funciones para determinar el color basándose en los valores normalizados
bool esColorAzul(float r, float g, float b) {
  return (b > umbralAzul && r < umbralAzul && g < umbralAzul);
}

bool esColorAmarillo(float r, float g, float b) {
  return (r > umbralAmarillo && g > umbralAmarillo && b < umbralAmarillo);
}

bool esColorRosa(float r, float g, float b) {
  return (r > umbralRosa && g < umbralRosa && b > umbralRosa);
}

bool esColorNegro(float r, float g, float b) {
  return (r < umbralNegro && g < umbralNegro && b < umbralNegro);
}

// Función para mover el robot hacia adelante
void moverAdelante() {
  digitalWrite(Izqln1, HIGH);
  digitalWrite(Izqln2, LOW);
  digitalWrite(Derln3, HIGH);
  digitalWrite(Derln4, LOW);
}

// Función para evitar el obstáculo (casilla negra)
void evitarObstaculo() {
  analogWrite(motorIzquierdo, 0);   // Detener motor izquierdo
  analogWrite(motorDerecho, 0);     // Detener motor derecho
  delay(1000);                      // Pausa para evitar choque
  
  //analogWrite(motorIzquierdo,



