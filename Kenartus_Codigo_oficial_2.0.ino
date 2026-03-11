char dato;
char buffer[6];
byte index = 0;
int valorRecibido = 0;

int StateX = 0, StateY = 0, StateZ = 0;

int X1 = 2, X2 = 3, X3 = 4;
int Y1 = 5, Y2 = 6, Y3 = 7;
int Z1 = 8, Z2 = 9, Z3 = 10;


int XS1 = 22;
int XS2 = 23;
int LecturaXSen1 = 0;
int LecturaXSen2 = 0;

int YS1 = 24; 
int YS2 = 25;
int LecturaYSen1 = 0;
int LecturaYSen2 = 0;

int ZS1 = 27;
int ZS2 = 26;
int LecturaZSen1 = 0;
int LecturaZSen2 = 0;

int tiempoX = 1;
int tiempoY = 1;
int tiempoZ = 1;

int Movs = 0;
int candadoX = 0;
int candadoY = 0;
int candadoZ = 0;

void setup() {
  Serial1.begin(9600); // Usaremos Serial1 (pines 0 y 1)
  Serial.begin(9600);
  pinMode(X1, OUTPUT); 
  pinMode(X2, OUTPUT); 
  pinMode(X3, OUTPUT);
  
  pinMode(Y1, OUTPUT); 
  pinMode(Y2, OUTPUT); 
  pinMode(Y3, OUTPUT);
  
  pinMode(Z1, OUTPUT); 
  pinMode(Z2, OUTPUT); 
  pinMode(Z3, OUTPUT);

  pinMode(XS1,INPUT);
  pinMode(XS2,INPUT);
  
  pinMode(YS1, INPUT);
  pinMode(YS2, INPUT);
  pinMode(ZS1, INPUT);
  pinMode(ZS2, INPUT);
  
  memset(buffer, 0, sizeof(buffer));
}

void loop() {
   LecturaXSen1 = digitalRead(XS1);
   LecturaXSen2 = digitalRead(XS2);
   
   LecturaYSen1 = digitalRead(YS1);
   LecturaYSen2 = digitalRead(YS2);

   LecturaZSen1 = digitalRead(ZS1);
   LecturaZSen2 = digitalRead(ZS2);

    if(LecturaXSen1 == HIGH){if(StateX == 1){estadoXNada();}}
    if(LecturaXSen1 == LOW) {if(StateX == 1){estadoXPositivo();}}
    if(LecturaXSen2 == HIGH){if(StateX == 2){estadoXNada();}}
    if(LecturaXSen2 == LOW) {if(StateX == 2){estadoXNegativo();}}
    if(StateX == 0){estadoXNada();}

    if(LecturaYSen1 == HIGH){if(StateY == 1){estadoYNada();}}
    if(LecturaYSen1 == LOW) {if(StateY == 1){estadoYPositivo();}}
    if(LecturaYSen2 == HIGH){if(StateY == 2){estadoYNada();}}
    if(LecturaYSen2 == LOW) {if(StateY == 2){estadoYNegativo();}}
    if(StateY == 0){estadoYNada();}

    if(LecturaZSen1 == HIGH){if(StateZ == 1){estadoZNada();}}
    if(LecturaZSen1 == LOW) {if(StateZ == 1){estadoZPositivo();}}
    if(LecturaZSen2 == HIGH){if(StateZ == 2){estadoZNada();}}
    if(LecturaZSen2 == LOW) {if(StateZ == 2){estadoZNegativo();}}
    if(StateZ == 0){estadoZNada();}

    if (Serial1.available() > 0) {
        ActividadSerial(Serial1);
    }
    if (Serial.available() > 0) {
        ActividadSerial(Serial);
    }


 

    if(Movs == 0){rutinaStop();}
    if(Movs == 1){rutinaPierna();}
    if(Movs == 2){rutinaBrazo();}
    if(Movs == 3){rutinaCaminador();}
    if(Movs == 4){rutinaPieSentado();}
}

void ActividadSerial(Stream &puerto){
  
    dato = puerto.read();

    //Si es un número, lo acumulamos
    if (isDigit(dato) && index < 3) {
      buffer[index++] = dato;
      buffer[index] = '\0';
    } 
    //Si es una letra, procesamos
    else {
      valorRecibido = atoi(buffer); // Convertimos el número acumulado (si existe)

      switch (dato) {
        // COMANDOS DE PASOS ESPECÍFICOS (Ej: "50X")

        case 'M':
        if(valorRecibido == 0)
        {
          Movs = 0;
          }
         if(valorRecibido == 1)
        {
          Movs = 1;
          }
          if(valorRecibido == 2)
        {
          Movs = 2;
          }
          if(valorRecibido == 3)
        {
          Movs = 3;
          }
          if(valorRecibido == 4)
        {
          Movs = 4;
          }
        break;
        
        case '*':
        if(valorRecibido == 0)
        {
          candadoX = 0;
          }
        if(valorRecibido == 1)
        {
          candadoX = 1;
          }
        if(valorRecibido == 2)
        {
          candadoY = 0;
          }
        if(valorRecibido == 3)
        {
          candadoY = 1;
          }
        if(valorRecibido == 4)
        {
          candadoZ = 0;
          }
        if(valorRecibido == 5)
        {
          candadoZ = 1;
          }

         break;


        
        case 't':
        tiempoX = valorRecibido; 
        break;

        case 'u':
        tiempoY = valorRecibido; 
        break;

        case 'v':
        tiempoZ = valorRecibido; 
        break;

        
        case 'X':
        moverPasos('X', valorRecibido); 
        break;
        
        case 'Y': 
        moverPasos('Y', valorRecibido); 
        break;
        
        case 'Z': 
        moverPasos('Z', valorRecibido); 
        break;

        case 'x':
        moverPasos('x', valorRecibido); 
        break;
        
        case 'y': 
        moverPasos('y', valorRecibido); 
        break;
        
        case 'z': 
        moverPasos('z', valorRecibido); 
        break;

        // COMANDOS DE ESTADO (Tus letras originales)
        case 'a': 
        StateX = 0; 
        
        break;

        case 'b': 
        StateX = 1; 
        
        break;
        
        case 'c': 
        StateX = 2; 
         
        break;
        
        case 'd': 
        StateY = 0; 
        puerto.println("NadaY"); 
        break;
        case 'e': 
        StateY = 1; 
        puerto.println("Y+"); 
        break;
        
        case 'f': 
        StateY = 2; 
        puerto.println("Y-"); 
        break;

        case 'g': 
        StateZ = 0; 
        puerto.println("NadaZ"); 
        break;
        
        case 'h': 
        StateZ = 1; 
        puerto.println("Z+"); 
        break;
        
        case 'i': 
        StateZ = 2; 
        puerto.println("Z-"); 
        break;

         // SALTOS DE LÍNEA (Ignorar invisiblemente)
        case '\n':
        case '\r': 
          break;

        // CARÁCTER NO RECONOCIDO
        default:
          puerto.print("Error: Caracter '");
          puerto.print(dato);
          puerto.println("' no reconocido.");
          break;
      }
      
      // Limpiar buffer después de cualquier letra
      index = 0;
      memset(buffer, 0, sizeof(buffer));
    }
  
  }


void rutinaStop(){
  estadoXNada();
  estadoYNada();
  estadoZNada();
  
  
  }

void rutinaPierna(){
  for (int i = 0; i <50 ; i++) 
  {
    estadoZPositivo();
    estadoYPositivo();
    }
    delay(500);
  for (int i = 0; i <50 ; i++) 
  {
    estadoZNegativo();
    estadoYNegativo();
    }
    delay(500);
  
  }

  
  void rutinaBrazo(){
    
  for (int i = 0; i < 50 ; i++) 
  {
    estadoZPositivo();
    estadoYPositivo();
    estadoXPositivo();
    estadoZPositivo();
    estadoYPositivo();
    }
    delay(500);

  for (int i = 0; i < 50 ; i++) 
  {
    estadoZNegativo();
    estadoYNegativo();
    estadoXNegativo();
    estadoZNegativo();
    estadoYNegativo();
    }
    delay(500);

  for (int i = 0; i < 50 ; i++) 
  {
    estadoZPositivo();
    estadoYPositivo();
    estadoXNegativo();
    estadoZPositivo();
    estadoYPositivo();
    }
    delay(500);

  for (int i = 0; i < 50 ; i++) 
  {
    estadoZNegativo();
    estadoYNegativo();
    estadoXPositivo();
    estadoZNegativo();
    estadoYNegativo();
    }
    delay(500);
  
  }




void rutinaCaminador()
{
  

    //Inicio
    for (int i = 0; i < 50 ; i++) 
  {
    estadoYNegativo();
    estadoYNegativo();
    estadoXPositivo();
    estadoXPositivo();
    estadoYNegativo();
    estadoYNegativo();
    }
    delay(500);
    for (int i = 0; i < 50 ; i++) 
    {
    estadoYNegativo();
    estadoYNegativo();
    estadoXNegativo();
    estadoXNegativo();
    estadoYNegativo();
    estadoYNegativo();
    }
    delay(500);
    for (int i = 0; i < 50 ; i++) 
    {
    estadoYNegativo();
    estadoYNegativo();
    estadoXPositivo();
    estadoXPositivo();
    estadoYNegativo();
    estadoYNegativo();
    }
    delay(500);
    for (int i = 0; i < 50 ; i++) 
    {
    estadoYNegativo();
    estadoYNegativo();
    estadoXNegativo();
    estadoXNegativo();
    estadoYNegativo();
    estadoYNegativo();
    }
    delay(500);

    //regreso
    for (int i = 0; i < 50 ; i++) 
  {
    estadoYPositivo();
    estadoYPositivo();
    estadoXPositivo();
    estadoXPositivo();
    estadoYPositivo();
    estadoYPositivo();
    }
    delay(500);
    for (int i = 0; i < 50 ; i++) 
    {
    estadoYPositivo();
    estadoYPositivo();
    estadoXNegativo();
    estadoXNegativo();
    estadoYPositivo();
    estadoYPositivo();
    }
    delay(500);
    for (int i = 0; i < 50 ; i++) 
    {
    estadoYPositivo();
    estadoYPositivo();
    estadoXPositivo();
    estadoXPositivo();
    estadoYPositivo();
    estadoYPositivo();
    }
    delay(500);
    for (int i = 0; i < 50 ; i++) 
    {
    estadoYPositivo();
    estadoYPositivo();
    estadoXNegativo();
    estadoXNegativo();
    estadoYPositivo();
    estadoYPositivo();
    }
    delay(500);

  
  }

void rutinaPieSentado(){
    for (int i = 0; i < 50 ; i++) 
    {
    estadoZPositivo();
    estadoYNegativo();
    
    }
    delay(500);
    for (int i = 0; i < 50 ; i++) 
    {
    estadoZNegativo();
    estadoYPositivo();
    }
    delay(500);

    
    }




void moverPasos(char eje, int pasos) {

  
  for (int i = 0; i < pasos; i++) 
  {
    if (eje == 'X'){
      estadoXPositivo();
    }
    if (eje == 'Y'){
      estadoYPositivo();
    }
    if (eje == 'Z'){
      estadoZPositivo();
    }

    if (eje == 'x'){
      estadoXNegativo();
    }
    if (eje == 'y'){
      estadoYNegativo();
    }
    if (eje == 'z'){
      estadoZNegativo();
    }
  }

 

  
}



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
