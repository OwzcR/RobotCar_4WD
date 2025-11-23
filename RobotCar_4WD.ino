/*
   ===============================================================
   ===      PROYECTO ROBOT CAR 4WD - CÓDIGO FINAL              ===
   ===============================================================
   Autores:
   Oscar vinegra Aguayo - 2055328
   Omar Santiago De León Salinas - 2057902
   Alfonso Sainz Coronado - 2049875

   Funciones:
   - Control por Bluetooth (Modo `Bluetoothcontrol`)
   - Evasión de Obstáculos (Modo `Obstacle`)
   - Control por Voz (Modo `voicecontrol`)
   *** ADVERTENCIA: Desconectar pines 0 y 1 para subir código ***
*/

#include <Servo.h>
#include <AFMotor_R4.h> 

// --- Pines ---
#define Echo A0
#define Trig A1
#define motor 10       
#define BUZZER_PIN 13   

// --- Configuración ---
#define Speed 200       // Velocidad NORMAL (0-255)
#define spoint 103      // Posición central del servo
#define OBSTACLE_DISTANCE 20 // Distancia para detenerse (en cm)

// --- Variables Globales ---
char value;     
int distance;   
int L = 0;    
int R = 0;    

// --- Objetos ---
Servo servo;
AF_DCMotor M1(1); 
AF_DCMotor M2(2); 
AF_DCMotor M3(3); 
AF_DCMotor M4(4); 

void setup() {
  Serial.begin(9600); 
  
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(BUZZER_PIN, OUTPUT); // Inicializar el Buzzer
  
  
  // Iniciar motores a velocidad NORMAL
  M1.setSpeed(Speed);
  M2.setSpeed(Speed);
  M3.setSpeed(Speed);
  M4.setSpeed(Speed);
}

void loop() {
  // --- SELECCIONA TU MODO ---
  // Descomenta solo UNA de las siguientes líneas.
  
  // Obstacle();
  Bluetoothcontrol();
  // voicecontrol();
}

// --- MODO 1: Evasión de Obstáculos ---
void Obstacle() {
  distance = ultrasonic();
  
  if (distance <= OBSTACLE_DISTANCE) { 
    Stop();
    tone(BUZZER_PIN, 300, 150); 
    backward();
    delay(100);
    Stop();
    
    L = leftsee(); 
    delay(100);    
    R = rightsee(); 
    delay(100);

    servo.attach(motor);
    servo.write(spoint);
    delay(500);
    servo.detach(); /

    if (L > R) { 
      left();
    } else { 
      right();
    }
    delay(500); 
    Stop();
    
  } else { 
    forward();
  }
}

// --- MODO 2: Control por Bluetooth (CON TURBO Y CLAXON) ---
void Bluetoothcontrol() {
  if (Serial.available() > 0) {
    value = Serial.read();
  }
  
  if (value == 'F') {
    forward();
  } else if (value == 'B') {
    backward();
  } else if (value == 'L') {
    left();
  } else if (value == 'R') {
    right();
  } else if (value == 'S') {
    Stop();
    // Al parar, reseteamos la velocidad a la normal 
    M1.setSpeed(Speed);
    M2.setSpeed(Speed);
    M3.setSpeed(Speed);
    M4.setSpeed(Speed);
    
  } else if (value == 'Y') { // --- CLAXON ---
    tone(BUZZER_PIN, 1000, 200); 
    
  } else if (value == 'Z') { // --- MODO TURBO ---
    // Ponemos todos los motores a máxima velocidad
    M1.setSpeed(255);
    M2.setSpeed(255);
    M3.setSpeed(255);
    M4.setSpeed(255);
  }
}

// --- MODO 3: Control por Voz ---
void voicecontrol() {
  // Lee desde 'Serial' (Bluetooth)
  if (Serial.available() > 0) {
    value = Serial.read();
    
    if (value == '^') {
      forward();
    } else if (value == '-') {
      backward();
    } else if (value == '<') { 
      L = leftsee(); 
      servo.attach(motor); servo.write(spoint); delay(500); servo.detach(); 
      if (L >= 10 ) { 
        left(); delay(500); Stop();
      } else { Stop(); }
    } else if (value == '>') { 
      R = rightsee(); 
      servo.attach(motor); servo.write(spoint); delay(500); servo.detach(); 
      if (R >= 10 ) { 
        right(); delay(500); Stop();
      } else { Stop(); }
    } else if (value == '*') {
      Stop();
    } else if (value == 'Y'){ 
       tone(BUZZER_PIN, 1000, 200);
    }
  }
}

// --- Función del Sensor Ultrasónico ---
int ultrasonic() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(4);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  
  long t = pulseIn(Echo, HIGH, 30000); 
  if (t == 0) {
    return 999; 
  }
  long cm = t / 29 / 2; 
  return cm;
}


// ######################################################################
// --- Funciones de Movimiento  ---
// ######################################################################

void forward() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}

void backward() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}

void right() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}

void left() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}

void Stop() {
  M1.run(RELEASE);
  M2.run(RELEASE);
  M3.run(RELEASE);
  M4.run(RELEASE);
}



// --- Funciones de Medición ---
int rightsee() {
  servo.attach(motor); 
  servo.write(20);     
  delay(800);          
  int dist = ultrasonic(); 
  servo.detach();      
  return dist; 
}

int leftsee() {
  servo.attach(motor); 
  servo.write(180);    
  delay(800);          
  int dist = ultrasonic(); 
  servo.detach();      
  return dist; 
}