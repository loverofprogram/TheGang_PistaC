#include <Wire.h>
#include <Adafruit_TCS34725.h>

// Inicializa el sensor TCS34725
Adafruit_TCS34725 tcs;

// Pines de los LEDs para indicar los colores
const int ledAzul = 3;
const int ledAmarillo = 4;
const int ledRosa = 5;
const int ledNegro = 6;

// Pines para los motores
const int motorIzquierdo = 9;
const int motorDerecho = 10;

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
  pinMode(ledAzul, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledRosa, OUTPUT);
  pinMode(ledNegro, OUTPUT);

  // Configura los pines de los motores como salida
  pinMode(motorIzquierdo, OUTPUT);
  pinMode(motorDerecho, OUTPUT);

  // Inicializa los LEDs apagados
  apagarLeds();
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
    encenderLed(ledAzul);
    moverAdelante();
  }
  else if (esColorAmarillo(rNorm, gNorm, bNorm)) {
    encenderLed(ledAmarillo);
    moverAdelante();
  }
  else if (esColorRosa(rNorm, gNorm, bNorm)) {
    encenderLed(ledRosa);
    moverAdelante();
  }
  else if (esColorNegro(rNorm, gNorm, bNorm)) {
    encenderLed(ledNegro);
    evitarObstaculo();
  }

  delay(500); // Pausa para evitar lecturas muy rápidas
}

// Función para apagar todos los LEDs
void apagarLeds() {
  digitalWrite(ledAzul, LOW);
  digitalWrite(ledAmarillo, LOW);
  digitalWrite(ledRosa, LOW);
  digitalWrite(ledNegro, LOW);
}

// Función para encender un LED específico
void encenderLed(int led) {
  apagarLeds();
  digitalWrite(led, HIGH);
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
  analogWrite(motorIzquierdo, 255);  // Motor izquierdo a máxima velocidad
  analogWrite(motorDerecho, 255);    // Motor derecho a máxima velocidad
}

// Función para evitar el obstáculo (casilla negra)
void evitarObstaculo() {
  analogWrite(motorIzquierdo, 0);   // Detener motor izquierdo
  analogWrite(motorDerecho, 0);     // Detener motor derecho
  delay(1000);                      // Pausa para evitar choque
  // Aquí podrías agregar lógica para cambiar de dirección si es necesario
  analogWrite(motorIzquierdo,



