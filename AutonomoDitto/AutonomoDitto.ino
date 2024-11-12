// Pines de control de motores
const int PWM_LEFT = 3;
const int PWM_RIGHT = 11;
const int IN1 = 5;
const int IN2 = 4;
const int IN3 = 6;
const int IN4 = 7;

// Pines de sensores y módulos
const int SENSOR_PROXIMIDAD_IZQUIERDO = 13;
const int SENSOR_PROXIMIDAD_DERECHO = 12;
const int SENSOR_LINEA = A0;
const int BOTON_ESTRATEGIA = 8;
const int MODULO_ARRANQUE = 10;

// Constantes de modos de combate
enum Estrategias { ATAQUE, DEFENSA, BUSQUEDA, ROTACION, EMBESTIDA, ESQUIVA };
const int NUM_ESTRATEGIAS = 6;

// Variables globales
int estrategiaSeleccionada = 0;
int estadoArranque = LOW;

void setup() {
  // Configuración de pines de motores como salidas
  pinMode(PWM_LEFT, OUTPUT);
  pinMode(PWM_RIGHT, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Configuración de pines de sensores como entradas
  pinMode(SENSOR_PROXIMIDAD_IZQUIERDO, INPUT);
  pinMode(SENSOR_PROXIMIDAD_DERECHO, INPUT);
  pinMode(SENSOR_LINEA, INPUT);
  pinMode(BOTON_ESTRATEGIA, INPUT);
  pinMode(MODULO_ARRANQUE, INPUT);

  // Inicialización de variables
  estrategiaSeleccionada = ATAQUE; // Estrategia predeterminada
}

void seleccionarEstrategia() {
  while (digitalRead(MODULO_ARRANQUE) == LOW) {
    if (digitalRead(BOTON_ESTRATEGIA) == HIGH) {
      estrategiaSeleccionada = (estrategiaSeleccionada + 1) % NUM_ESTRATEGIAS;
      delay(300); // Evitar rebotes del botón
    }
  }
}

struct EstadoSensores {
  int proximidadIzquierda;
  int proximidadDerecha;
  int linea;
};

EstadoSensores leerSensores() {
  EstadoSensores estado;
  estado.proximidadIzquierda = digitalRead(SENSOR_PROXIMIDAD_IZQUIERDO);
  estado.proximidadDerecha = digitalRead(SENSOR_PROXIMIDAD_DERECHO);
  estado.linea = analogRead(SENSOR_LINEA);
  return estado;
}

void moverAdelante(int velocidad) {
  analogWrite(PWM_LEFT, velocidad);
  analogWrite(PWM_RIGHT, velocidad);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moverAtras(int velocidad) {
  analogWrite(PWM_LEFT, velocidad);
  analogWrite(PWM_RIGHT, velocidad);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void girarIzquierda(int velocidad) {
  analogWrite(PWM_LEFT, 0);
  analogWrite(PWM_RIGHT, velocidad);
}

void girarDerecha(int velocidad) {
  analogWrite(PWM_LEFT, velocidad);
  analogWrite(PWM_RIGHT, 0);
}

void detener() {
  analogWrite(PWM_LEFT, 0);
  analogWrite(PWM_RIGHT, 0);
}

// Estrategia de Ataque
void estrategiaAtaque(EstadoSensores sensores) {
  if (sensores.proximidadIzquierda == LOW && sensores.proximidadDerecha == LOW) {
    avanzar(150);  // Si no detecta al oponente, avanza hacia adelante
  } else if (sensores.proximidadIzquierda == HIGH) {
    girarDerecha(120);  // Si detecta al oponente a la izquierda, gira a la derecha
  } else if (sensores.proximidadDerecha == HIGH) {
    girarIzquierda(120);  // Si detecta al oponente a la derecha, gira a la izquierda
  }
}

// Estrategia de Defensa
void estrategiaDefensa(EstadoSensores sensores) {
  if (sensores.proximidadIzquierda == HIGH || sensores.proximidadDerecha == HIGH) {
    retroceder(100);  // Si detecta al oponente cerca, retrocede
    delay(300);
    girarDerecha(150);  // Luego gira rápidamente para evitar el ataque
    delay(200);
  } else {
    detener();  // Si no hay detección, se queda en espera
  }
}

// Estrategia de Búsqueda
void estrategiaBusqueda(EstadoSensores sensores) {
  if (sensores.proximidadIzquierda == LOW && sensores.proximidadDerecha == LOW) {
    avanzar(100);  // Avanza lentamente buscando al oponente
  } else {
    estrategiaAtaque(sensores);  // Si detecta al oponente, cambia a ataque
  }
}

// Estrategia de Rotación
void estrategiaRotacion(EstadoSensores sensores) {
  if (sensores.proximidadIzquierda == LOW && sensores.proximidadDerecha == LOW) {
    girarDerecha(80);  // Gira constantemente a la derecha buscando al oponente
  } else {
    estrategiaAtaque(sensores);  // Si detecta al oponente, cambia a ataque
  }
}

// Estrategia de Embestida
void estrategiaEmbestida(EstadoSensores sensores) {
  if (sensores.proximidadIzquierda == HIGH || sensores.proximidadDerecha == HIGH) {
    avanzar(255);  // Si detecta al oponente, avanza a máxima velocidad
  } else {
    estrategiaBusqueda(sensores);  // Si no hay detección, sigue en búsqueda
  }
}

// Estrategia de Esquiva
void estrategiaEsquiva(EstadoSensores sensores) {
  if (sensores.proximidadIzquierda == HIGH) {
    girarDerecha(200);  // Si el oponente está a la izquierda, esquiva hacia la derecha
    delay(500);
  } else if (sensores.proximidadDerecha == HIGH) {
    girarIzquierda(200);  // Si el oponente está a la derecha, esquiva hacia la izquierda
    delay(500);
  } else {
    estrategiaDefensa(sensores);  // Si no hay detección, mantiene posición defensiva
  }
}

void loop() {
  // Permitir al usuario seleccionar la estrategia antes de activar el módulo de arranque
  seleccionarEstrategia();

  // Bucle principal: ejecuta las estrategias mientras el módulo de arranque esté activado
  while (digitalRead(MODULO_ARRANQUE) == HIGH) {
    // Leer el estado actual de los sensores
    EstadoSensores sensores = leerSensores();

    // Comprobar si el robot ha detectado la línea límite del área de combate
    if (sensores.linea <= 60) {  // Si está cerca de la línea
      moverAtras(100);           // Retrocede para evitar salirse del área
      delay(500);                 // Pausa para retroceder
      continue;                   // Salta a la siguiente iteración del bucle
    }

    // Ejecuta la estrategia seleccionada en función de los sensores
    switch (estrategiaSeleccionada) {
      case ATAQUE:
        estrategiaAtaque(sensores);
        break;
      case DEFENSA:
        estrategiaDefensa(sensores);
        break;
      case BUSQUEDA:
        estrategiaBusqueda(sensores);
        break;
      case ROTACION:
        estrategiaRotacion(sensores);
        break;
      case EMBESTIDA:
        estrategiaEmbestida(sensores);
        break;
      case ESQUIVA:
        estrategiaEsquiva(sensores);
        break;
      default:
        detener();  // En caso de que no haya una estrategia válida, detener el robot
        break;
    }

    // Verifica si el módulo de arranque se ha desactivado, lo que indica que debe detenerse
    if (digitalRead(MODULO_ARRANQUE) == LOW) {
      detener();  // Detener el robot si el módulo de arranque está apagado
      break;      // Salir del bucle
    }
    delay(1);  // Pausa breve para evitar sobrecarga del bucle
  }
}

