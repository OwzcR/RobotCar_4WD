# Robot Car 4WD - Multifuncional

Este proyecto es un robot móvil de cuatro ruedas capaz de navegar de forma autónoma evadiendo obstáculos, ser controlado vía Bluetooth mediante una App y responder a comandos de voz con seguridad asistida.

## Características
- Evasión de Obstáculos: Algoritmo inteligente que escanea el entorno (izquierda/derecha) antes de decidir la ruta.
- Control Bluetooth: Manejo total de dirección, claxon y velocidad.
- Modo Turbo: Función 'Boost' para máxima potencia de motores.
- Control por Voz: Comandos hablados con freno de emergencia automático si se detecta un objeto.

## Requisitos e Instalación

### Hardware
- Arduino Uno R3
- Motor Shield L293D (v1)
- 4x Motores DC con caja reductora y llantas
- Sensor Ultrasónico HC-SR04
- Módulo Bluetooth HC-06
- Servomotor SG90
- Buzzer activo/pasivo

### Conexiones (Pinout)
| Componente | Pin Arduino | Notas |
|------------|-------------|-------|
| Servo | 10 | Timer1 compartido |
| Trig (Sensor) | A1 | |
| Echo (Sensor) | A0 | |
| Buzzer (+) | A3 | Pin Analógico usado como Digital |
| Bluetooth TX | 0 (RX) | **Desconectar para subir código** |
| Bluetooth RX | 1 (TX) | **Desconectar para subir código** |

### Instalación de Software
1. Instalar Arduino IDE.
2. Instalar la librería **AFMotor** (compatible con v1).
3. Abrir el archivo `RobotCar_4WD.ino`.
4. Seleccionar la placa "Arduino Uno".
5. Subir el código (recordando desconectar el Bluetooth temporalmente).

## Cómo Usarlo (Comandos Bluetooth)

El robot espera caracteres vía Serial para operar. Puedes usar cualquier App de "Bluetooth Terminal" o "Arduino RC Car".

| Carácter | Acción |
|:---:|---|
| **F** | Avanzar (Forward) |
| **B** | Retroceder (Backward) |
| **L** | Girar Izquierda (Left) |
| **R** | Girar Derecha (Right) |
| **S** | Detenerse (Stop) |
| **Z** | Activar Modo Turbo (Velocidad 255) |
| **Y** | Tocar Claxon (Horn) |
| **^** | Voz: Avanzar con Auto-Stop |

## Arquitectura y Flujo

Modo Obstáculos
1. Leer distancia frontal.
2. ¿Distancia < 20cm?
   - SÍ: Frenar -> Sonar Alerta -> Retroceder -> Escanear Izq/Der -> Girar al lado libre.
   - NO: Seguir avanzando.

Modo Bluetooth
    A[Esperar Dato Serial] --> B{¿Dato Recibido?};
    B -- No --> A;
    B -- Si --> C[Leer Caracter];
    C --> D{Evaluar Comando};
    D -- 'F', 'B', 'L', 'R' --> E[Mover Motores];
    D -- 'S' --> F[Frenar & Resetear Velocidad];
    D -- 'Z' --> G[Modo Turbo PWM 255];
    D -- 'Y' --> H[Sonar Claxon A3];

Modo Voz
    A[Recibir Comando Voz] --> B{¿Comando es AVANZAR?};
    B -- No --> C[Ejecutar Acción Simple];
    B -- Si --> D[Entrar Bucle de Seguridad];
    D --> E{¿Hay Obstáculo < 20cm?};
    E -- Si --> F[Freno de Emergencia & Alerta];
    E -- No --> G[Avanzar];
    F --> H[Cancelar Comando 'Avanzar'];
    G --> D;

## FAQ (Preguntas Frecuentes)

P: ¿Por qué el robot pita constantemente y no se mueve?
R: Probablemente las baterías tienen poca carga. El buzzer puede sonar erráticamente si el voltaje cae por debajo de 5V.

P: ¿Por qué no puedo subir el código?
R: Asegúrate de desconectar los cables de los pines 0 y 1. Estos pines se comparten con el USB del ordenador.

P: ¿Por qué en modo obstáculo se detiene un momento antes de girar?
R: Es intencional. El robot conecta el servo, mira a los lados, y luego desconecta el servo para liberar los recursos del sistema y permitir que las 4 ruedas giren con fuerza.

---
**Autores**
[OSCAR VINIEGRA AGUAYO] - Estudiante en Facultad de Ingeniería Mecánica y Eléctrica
[OMAR SANTIAGO DE LEÓN SALINAS] - Estudiante en Facultad de Ingeniería Mecánica y Eléctrica
[ALFONSO SAINZ CORONADO] - Estudiante en Facultad de Ingeniería Mecánica y Eléctrica 
