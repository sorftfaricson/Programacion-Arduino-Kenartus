#include <Servo.h>
Servo miServo1;
Servo miServo2;


// Variable para guardar la posición actual del servo
int grado1 = 90;
int grado2 = 180; 

int PWMVelocidad = 0;

int cicloServo1 = 0;
int cicloServo2 = 0;

char dato;
char buffer[6];
byte index = 0;
int valorRecibido = 0;

int StateX = 0, StateY = 0, StateZ = 0;

int X1 = 2, X2 = 3, X3 = 4;
int Y1 = 5, Y2 = 6, Y3 = 7;
int Z1 = 8, Z2 = 9, Z3 = 10;

int PWMmotor = 11;

const int pinServo1 = 12;
const int pinServo2 = 13;

int XS1 = 22, XS2 = 23;
int LecturaXSen1 = 0, LecturaXSen2 = 0;

int YS1 = 24, YS2 = 25;
int LecturaYSen1 = 0, LecturaYSen2 = 0;

int ZS1 = 27, ZS2 = 26;
int LecturaZSen1 = 0, LecturaZSen2 = 0;

int tiempoX = 1, tiempoY = 1, tiempoZ = 1;

int Movs = 999;
int candadoX = 0, candadoY = 0, candadoZ = 0;

int RPierna = 0;
int ContadorRP = 0;

int RBrazo = 0;
int ContadorRB = 0;


int RCaminador = 0;
int ContadorRC = 0;


int RPSentado = 0;
int ContadorRPS = 0;

int ResRobot = 0;
int resX = 0, resY = 0, resZ = 0;

int pasoActual = 0;
void setup() {
  Serial2.begin(9600);
  Serial1.begin(9600); 
  Serial.begin(9600);
  miServo1.attach(pinServo1);
  miServo2.attach(pinServo2);
  
  pinMode(PWMmotor, OUTPUT);
  
  // Inicializar el servo en 0 grados
  miServo1.write(90);
  miServo2.write(180);
  
  
  pinMode(X1, OUTPUT); pinMode(X2, OUTPUT); pinMode(X3, OUTPUT);
  pinMode(Y1, OUTPUT); pinMode(Y2, OUTPUT); pinMode(Y3, OUTPUT);
  pinMode(Z1, OUTPUT); pinMode(Z2, OUTPUT); pinMode(Z3, OUTPUT);

  pinMode(XS1, INPUT); pinMode(XS2, INPUT);
  pinMode(YS1, INPUT); pinMode(YS2, INPUT);
  pinMode(ZS1, INPUT); pinMode(ZS2, INPUT);
  
  memset(buffer, 0, sizeof(buffer));
  
}


void loop() {
    actualizarSensores();

    // Control manual por estados (si no hay rutina activa)
    if (Movs == 999) {
      if(LecturaXSen1 == HIGH && StateX == 1){ estadoXNada(); }
      if(LecturaXSen1 == LOW  && StateX == 1){ estadoXPositivo(); }
      if(LecturaXSen2 == HIGH && StateX == 2){ estadoXNada(); }
      if(LecturaXSen2 == LOW  && StateX == 2){ estadoXNegativo(); }
      if(StateX == 0){ estadoXNada(); }

      if(LecturaYSen1 == HIGH && StateY == 1){ estadoYNada(); }
      if(LecturaYSen1 == LOW  && StateY == 1){ estadoYPositivo(); }
      if(LecturaYSen2 == HIGH && StateY == 2){ estadoYNada(); }
      if(LecturaYSen2 == LOW  && StateY == 2){ estadoYNegativo(); }
      if(StateY == 0){ estadoYNada(); }

      if(LecturaZSen1 == HIGH && StateZ == 1){ estadoZNada(); }
      if(LecturaZSen1 == LOW  && StateZ == 1){ estadoZPositivo(); }
      if(LecturaZSen2 == HIGH && StateZ == 2){ estadoZNada(); }
      if(LecturaZSen2 == LOW  && StateZ == 2){ estadoZNegativo(); }
      if(StateZ == 0){ estadoZNada(); }
    }

    // Escucha de puertos Seriales
    if (Serial1.available() > 0) ActividadSerial(Serial1);
    if (Serial.available() > 0)  ActividadSerial(Serial);
    if (Serial2.available() > 0) ActividadSerial(Serial2);



    // Ejecución secuencial de rutinas automáticas (un paso por vuelta de loop)
    if(Movs == 0){ cicloServo1 = 0; cicloServo2 = 0; rutinaStop(); RPierna = 0; ContadorRP = 0; RBrazo=0; ContadorRB = 0; RCaminador = 0; ContadorRC = 0; RPSentado = 0; ContadorRPS = 0; Movs = 999; }
    if(Movs == 1){ rutinaPierna(); }
    if(Movs == 2){ rutinaBrazo(); }
    if(Movs == 3){ /*rutinaCaminador();*/ rutinaCaminar(); }
    if(Movs == 4){ rutinaPieSentado(); }
    if(Movs == 5){ puntoInicio();}

    analogWrite(PWMmotor,25 * PWMVelocidad);
}


void ActividadSerial(Stream &puerto){
    dato = puerto.read();

    if (isDigit(dato)) {
      if (index < 4) { // Evita desbordar el buffer de 6 posiciones
        buffer[index++] = dato;
        buffer[index] = '\0';
      }
    } 
    else {
      valorRecibido = atoi(buffer);

      switch (dato) {
        case 'M':
          if(valorRecibido == 0) {Movs = 0;}
          if(valorRecibido == 1) {RPierna = 1;Movs = 1;}
          if(valorRecibido == 2) {RBrazo = 1;Movs = 2;}
          if(valorRecibido == 3) {cicloServo1 = 1; cicloServo2 = 1; RCaminador = 1;Movs = 3;}
          if(valorRecibido == 4) {RPSentado = 1;Movs = 4;}
          if(valorRecibido == 5) {ResRobot = 1;Movs = 5;}
          break;
        
        case '*':
          if(valorRecibido == 0) candadoX = 0;
          if(valorRecibido == 1) candadoX = 1;
          if(valorRecibido == 2) candadoY = 0;
          if(valorRecibido == 3) candadoY = 1;
          if(valorRecibido == 4) candadoZ = 0;
          if(valorRecibido == 5) candadoZ = 1;
          break;

        case 't': tiempoX = valorRecibido; break;
        case 'u': tiempoY = valorRecibido; break;
        case 'v': tiempoZ = valorRecibido; break;
        
        case 'C': PWMVelocidad = valorRecibido; break;
        
        case 'X': moverPasos('X', valorRecibido); break;
        case 'Y': moverPasos('Y', valorRecibido); break;
        case 'Z': moverPasos('Z', valorRecibido); break;
        case 'x': moverPasos('x', valorRecibido); break;
        case 'y': moverPasos('y', valorRecibido); break;
        case 'z': moverPasos('z', valorRecibido); break;

        case 'a': StateX = 0; puerto.println("NadaX"); break;
        case 'b': StateX = 1; puerto.println("X+");break;
        case 'c': StateX = 2; puerto.println("X-");break;
        
        case 'd': StateY = 0; puerto.println("NadaY"); break;
        case 'e': StateY = 1; puerto.println("Y+"); break;
        case 'f': StateY = 2; puerto.println("Y-"); break;

        case 'g': StateZ = 0; puerto.println("NadaZ"); break;
        case 'h': StateZ = 1; puerto.println("Z+"); break;
        case 'i': StateZ = 2; puerto.println("Z-"); break;

        case '\n':
        case '\r': 
          break;

        default:
          puerto.print("Error: Caracter '");
          puerto.print(dato);
          puerto.println("' no reconocido.");
          break;
      }
      
      index = 0;
      memset(buffer, 0, sizeof(buffer));
    }
}

 //esta funcion solo se llama una ves y ya 
void estadoXNada() { 
  if(candadoX == 0){
  digitalWrite(X1, 1); 
  digitalWrite(X2, 0); 
  digitalWrite(X3, 0); 
  }
  if(candadoX == 1){
  digitalWrite(X1, 0); 
  digitalWrite(X2, 0); 
  digitalWrite(X3, 0);
  }
  
  }
  
void estadoXPositivo() {
  digitalWrite(X1, 0); 
  digitalWrite(X2, 1); 
  digitalWrite(X3, 0); 
  delay(tiempoX);
  digitalWrite(X1, 0); 
  digitalWrite(X2, 1); 
  digitalWrite(X3, 1); 
  delay(tiempoX);
}

void estadoXNegativo() {
  digitalWrite(X1, 0); 
  digitalWrite(X2, 0); 
  digitalWrite(X3, 0); 
  delay(tiempoX);
  digitalWrite(X1, 0); 
  digitalWrite(X2, 0); 
  digitalWrite(X3, 1); 
  delay(tiempoX);
}
//esta funcion solo se llama una ves y ya 
void estadoYNada() {
  if(candadoY == 0){
  digitalWrite(Y1, 1); 
  digitalWrite(Y2, 0); 
  digitalWrite(Y3, 0); } 
  if(candadoY == 1){
  digitalWrite(Y1, 0); 
  digitalWrite(Y2, 0); 
  digitalWrite(Y3, 0); 
  } 
  }
  
void estadoYPositivo() {
  digitalWrite(Y1, 0); 
  digitalWrite(Y2, 1); 
  digitalWrite(Y3, 0); 
  delay(tiempoY);
  digitalWrite(Y1, 0); 
  digitalWrite(Y2, 1); 
  digitalWrite(Y3, 1); 
  delay(tiempoY);
}

void estadoYNegativo() {
  digitalWrite(Y1, 0); 
  digitalWrite(Y2, 0); 
  digitalWrite(Y3, 0); 
  delay(tiempoY);
  digitalWrite(Y1, 0); 
  digitalWrite(Y2, 0); 
  digitalWrite(Y3, 1); 
  delay(tiempoY);
}

//esta funcion solo se llama una ves y ya 
void estadoZNada() { 
  if(candadoZ == 0){
  digitalWrite(Z1, 1); 
  digitalWrite(Z2, 0); 
  digitalWrite(Z3, 0); }
  if(candadoZ == 1){
  digitalWrite(Z1, 0); 
  digitalWrite(Z2, 0); 
  digitalWrite(Z3, 0); 
  }
  }
  
void estadoZPositivo() {
  digitalWrite(Z1, 0); 
  digitalWrite(Z2, 1); 
  digitalWrite(Z3, 0); 
  delay(tiempoZ);
  digitalWrite(Z1, 0); 
  digitalWrite(Z2, 1); 
  digitalWrite(Z3, 1); 
  delay(tiempoZ);
}

void estadoZNegativo() {
  digitalWrite(Z1, 0); 
  digitalWrite(Z2, 0); 
  digitalWrite(Z3, 0); 
  delay(tiempoZ);
  digitalWrite(Z1, 0); 
  digitalWrite(Z2, 0); 
  digitalWrite(Z3, 1); 
  delay(tiempoZ);
}



void actualizarSensores() {
  LecturaXSen1 = digitalRead(XS1);
  LecturaXSen2 = digitalRead(XS2);
  LecturaYSen1 = digitalRead(YS1);
  LecturaYSen2 = digitalRead(YS2);
  LecturaZSen1 = digitalRead(ZS1);
  LecturaZSen2 = digitalRead(ZS2);
}



// FUNCIÓN MOVER PASOS REVISADA Y SEGURA
void moverPasos(char eje, int pasos) {
  for (int i = 0; i < pasos; i++) {
    actualizarSensores(); // Monitoreo en tiempo real

    if (eje == 'X') { if (LecturaXSen1 == LOW) { estadoXPositivo(); resX = 0; } else { estadoXNada(); resX = 1; break; } }
    if (eje == 'x') { if (LecturaXSen2 == LOW) { estadoXNegativo(); } else { estadoXNada(); break; } }
    if (eje == 'Y') { if (LecturaYSen1 == LOW) { estadoYPositivo(); resY = 0; } else { estadoYNada(); resY = 1; break; } }
    if (eje == 'y') { if (LecturaYSen2 == LOW) { estadoYNegativo(); } else { estadoYNada(); break; } }
    if (eje == 'Z') { if (LecturaZSen1 == LOW) { estadoZPositivo(); resZ = 0; } else { estadoZNada(); resZ = 1; break; } }
    if (eje == 'z') { if (LecturaZSen2 == LOW) { estadoZNegativo(); } else { estadoZNada(); break; } }
    
    delay(1); // Pequeña pausa física indispensable para el motor
  }
}




void rutinaPierna() {
delay(1);
if (RPierna == 1)
{
  moverPasos('Z',1);
  moverPasos('y',1);
  ContadorRP++;
  if(ContadorRP >= 150){delay(500); RPierna = 2;}
  }

if (RPierna == 2)
{
  moverPasos('z',1);
  moverPasos('Y',1);
  ContadorRP--;
  if(ContadorRP <= 0){delay(500); RPierna = 1;}
  }

  if (RPierna == 0){ContadorRP = 0;}
}




void rutinaBrazo() {
  delay(1);
  if(RBrazo == 1){moverPasos('Z',1);moverPasos('y',1);moverPasos('X',1);moverPasos('Z',1);moverPasos('y',1);ContadorRB++;if(ContadorRB == 50){delay(500);RBrazo = 2;}}
  if(RBrazo == 2){moverPasos('z',1);moverPasos('Y',1);moverPasos('x',1);moverPasos('z',1);moverPasos('Y',1);ContadorRB++;if(ContadorRB == 100){delay(500);RBrazo = 3;}}
  if(RBrazo == 3){moverPasos('Z',1);moverPasos('y',1);moverPasos('x',1);moverPasos('Z',1);moverPasos('y',1);ContadorRB++;if(ContadorRB == 150){delay(500);RBrazo = 4;}}
  if(RBrazo == 4){moverPasos('z',1);moverPasos('Y',1);moverPasos('X',1);moverPasos('z',1);moverPasos('Y',1);ContadorRB++;if(ContadorRB == 200){delay(500);ContadorRB = 0;RBrazo = 1;}}
  if(RBrazo == 0){ContadorRB == 0;}
}


void rutinaCaminador() {
  delay(1);
  if(RCaminador == 1){moverPasos('y',1);moverPasos('X',1); moverPasos('y',1);moverPasos('X',1);moverPasos('y',1);moverPasos('y',1);ContadorRC++;if(ContadorRC == 50){delay(500);RCaminador = 2;}}
  if(RCaminador == 2){moverPasos('y',1);moverPasos('x',1); moverPasos('y',1);moverPasos('x',1);moverPasos('y',1);moverPasos('y',1);ContadorRC++;if(ContadorRC == 100){delay(500);RCaminador = 3;}}
  if(RCaminador == 3){moverPasos('y',1);moverPasos('X',1); moverPasos('y',1);moverPasos('X',1);moverPasos('y',1);moverPasos('y',1);ContadorRC++;if(ContadorRC == 150){delay(500);RCaminador = 4;}}
  if(RCaminador == 4){moverPasos('y',1);moverPasos('x',1); moverPasos('y',1);moverPasos('x',1);moverPasos('y',1);moverPasos('y',1);ContadorRC++;if(ContadorRC == 200){delay(500);RCaminador = 5;}}
  if(RCaminador == 5){moverPasos('Y',1);moverPasos('X',1); moverPasos('Y',1);moverPasos('X',1);moverPasos('Y',1);moverPasos('Y',1);ContadorRC++;if(ContadorRC == 250){delay(500);RCaminador = 6;}}
  if(RCaminador == 6){moverPasos('Y',1);moverPasos('x',1); moverPasos('Y',1);moverPasos('x',1);moverPasos('Y',1);moverPasos('Y',1);ContadorRC++;if(ContadorRC == 300){delay(500);RCaminador = 7;}}
  if(RCaminador == 7){moverPasos('Y',1);moverPasos('X',1); moverPasos('Y',1);moverPasos('X',1);moverPasos('Y',1);moverPasos('Y',1);ContadorRC++;if(ContadorRC == 350){delay(500);RCaminador = 8;}}
  if(RCaminador == 8){moverPasos('Y',1);moverPasos('x',1); moverPasos('Y',1);moverPasos('x',1);moverPasos('Y',1);moverPasos('Y',1);ContadorRC++;if(ContadorRC == 400){delay(500);ContadorRC = 0;RCaminador = 1;}}
  if(RCaminador == 0){ContadorRC = 0;}
}

void rutinaCaminar(){
  if(cicloServo1 == 0){miServo1.write(90);}
  if(cicloServo1 == 1){miServo1.write(grado1);delay(10);grado1 = grado1 - 1;
     if(grado1 <= 0){cicloServo1 = 2;}}
  if(cicloServo1 == 2){miServo1.write(grado1);delay(10);grado1 = grado1 + 1;
     if(grado1 >= 90){cicloServo1 = 1;}}
/////////////////////////////////////////////////////////
  if(cicloServo2 == 0){miServo2.write(180);}
  if(cicloServo2 == 1){miServo2.write(grado2);delay(10);grado2 = grado2 - 1;
     if(grado2 <= 90){cicloServo2 = 2;}}
  if(cicloServo2 == 2){miServo2.write(grado2);delay(10);grado2 = grado2 + 1;
     if(grado2 >= 180){cicloServo2 = 1;}}
     }

void rutinaNoCaminar(){miServo1.write(90);miServo2.write(180);}

void rutinaPieSentado() {
  delay(1);
  if(RPSentado == 1){moverPasos('Z',1);/*moverPasos('y',1);*/ContadorRPS++;if(ContadorRPS == 300){delay(500);RPSentado = 2;}}
  if(RPSentado == 2){moverPasos('z',1);/*moverPasos('Y',1);*/ContadorRPS++;if(ContadorRPS == 600){delay(500);ContadorRPS = 0; RPSentado = 1;}}
  if(RPSentado == 0){ContadorRPS = 0;}
}

void puntoInicio(){
  delay(1);
  if(ResRobot == 1){
    if(resX == 1 && resY == 1 && resZ == 1){
    Movs = 999;
    ResRobot = 0;
    resX = 0; resY = 0; resZ = 0;
    }
    else{
    moverPasos('Z',1);
    moverPasos('Y',1);
    moverPasos('X',1);}
    }
  }

void rutinaStop(){
  estadoXNada(); estadoYNada(); estadoZNada();rutinaNoCaminar();

}
