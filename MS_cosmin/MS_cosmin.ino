#define PWM_IZQUIERDA   5  // Pin PWM para el motor izquierdo
#define MOTOR_IZQ_1     9  // Pin control del motor izquierdo (IN1)
#define MOTOR_IZQ_2     4  // Pin control del motor izquierdo (IN2)
#define PWM_DERECHA     6  // Pin PWM para el motor derecho
#define MOTOR_DER_1     8  // Pin control del motor derecho (IN1)
#define MOTOR_DER_2     7  // Pin control del motor derecho (IN2)

int velocidad = 255;
int velocidad_baja = 200;

#include <SoftwareSerial.h>
SoftwareSerial bluetoothSerial(0, 1); // RX, TX
char comando;

void setup() {
  bluetoothSerial.begin(9600);

  pinMode(MOTOR_IZQ_1, OUTPUT);
  pinMode(MOTOR_IZQ_2, OUTPUT);
  pinMode(MOTOR_DER_1, OUTPUT);
  pinMode(MOTOR_DER_2, OUTPUT);
  pinMode(PWM_IZQUIERDA, OUTPUT);
  pinMode(PWM_DERECHA, OUTPUT);

  detener();
}

void loop() {
  String comandos = "";

  while (bluetoothSerial.available() > 0) {
    comandos += (char)bluetoothSerial.read();
  }

  if (comandos.indexOf('F') != -1 && comandos.indexOf('L') != -1) {
    adelanteIzquierda();  
  } else if (comandos.indexOf('F') != -1 && comandos.indexOf('R') != -1) {
    adelanteDerecha(); 
  } else if (comandos.indexOf('B') != -1 && comandos.indexOf('L') != -1) {
    atrasIzquierda();  
  } else if (comandos.indexOf('B') != -1 && comandos.indexOf('R') != -1) {
    atrasDerecha(); 
  } else {
    for (int i = 0; i < comandos.length(); i++) {
      switch (comandos[i]) {
        case 'F':
          moverAdelante();
          break;
        case 'B':
          moverAtras();
          break;
        case 'L':
          girarIzquierda();
          break;
        case 'R':
          girarDerecha();
          break;
        default:
          detener();
          break;
      }
    }
  }
}

void moverAdelante() {
  analogWrite(PWM_IZQUIERDA, velocidad);
  analogWrite(PWM_DERECHA, velocidad);
  digitalWrite(MOTOR_IZQ_1, LOW);
  digitalWrite(MOTOR_IZQ_2, HIGH);
  digitalWrite(MOTOR_DER_1, HIGH);
  digitalWrite(MOTOR_DER_2, LOW);
}

void moverAtras() {
  analogWrite(PWM_IZQUIERDA, velocidad);
  analogWrite(PWM_DERECHA, velocidad);
  digitalWrite(MOTOR_IZQ_1, HIGH);
  digitalWrite(MOTOR_IZQ_2, LOW);
  digitalWrite(MOTOR_DER_1, LOW);
  digitalWrite(MOTOR_DER_2, HIGH);
}

void girarIzquierda() {
  analogWrite(PWM_IZQUIERDA, velocidad_baja);
  analogWrite(PWM_DERECHA, velocidad);
  digitalWrite(MOTOR_IZQ_1, HIGH);
  digitalWrite(MOTOR_IZQ_2, LOW);
  digitalWrite(MOTOR_DER_1, HIGH);
  digitalWrite(MOTOR_DER_2, LOW);
}

void girarDerecha() {
  analogWrite(PWM_IZQUIERDA, velocidad);
  analogWrite(PWM_DERECHA, velocidad_baja);
  digitalWrite(MOTOR_IZQ_1, LOW);
  digitalWrite(MOTOR_IZQ_2, HIGH);
  digitalWrite(MOTOR_DER_1, LOW);
  digitalWrite(MOTOR_DER_2, HIGH);
}

void adelanteIzquierda() {
  analogWrite(PWM_IZQUIERDA, velocidad_baja);
  analogWrite(PWM_DERECHA, velocidad);
  digitalWrite(MOTOR_IZQ_1, LOW);
  digitalWrite(MOTOR_IZQ_2, HIGH);
  digitalWrite(MOTOR_DER_1, HIGH);
  digitalWrite(MOTOR_DER_2, LOW);
}

void adelanteDerecha() {
  analogWrite(PWM_IZQUIERDA, velocidad);
  analogWrite(PWM_DERECHA, velocidad_baja);
  digitalWrite(MOTOR_IZQ_1, LOW);
  digitalWrite(MOTOR_IZQ_2, HIGH);
  digitalWrite(MOTOR_DER_1, HIGH);
  digitalWrite(MOTOR_DER_2, LOW);
}

void atrasIzquierda() {
  analogWrite(PWM_IZQUIERDA, velocidad_baja);
  analogWrite(PWM_DERECHA, velocidad);
  digitalWrite(MOTOR_IZQ_1, HIGH);
  digitalWrite(MOTOR_IZQ_2, LOW);
  digitalWrite(MOTOR_DER_1, LOW);
  digitalWrite(MOTOR_DER_2, HIGH);
}

void atrasDerecha() {
  analogWrite(PWM_IZQUIERDA, velocidad);
  analogWrite(PWM_DERECHA, velocidad_baja);
  digitalWrite(MOTOR_IZQ_1, HIGH);
  digitalWrite(MOTOR_IZQ_2, LOW);
  digitalWrite(MOTOR_DER_1, LOW);
  digitalWrite(MOTOR_DER_2, HIGH);
}

void detener() {
  digitalWrite(MOTOR_IZQ_1, LOW);
  digitalWrite(MOTOR_IZQ_2, LOW);
  digitalWrite(MOTOR_DER_1, LOW);
  digitalWrite(MOTOR_DER_2, LOW);
}
