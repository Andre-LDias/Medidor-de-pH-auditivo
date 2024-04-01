#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
bool botao = 0;


// Initialize software serial on pins 10 and 11
SoftwareSerial mySoftwareSerial(10, 11);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;
String line;
char command;
int pause = 0;
int repeat = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
 
float valor_calibracao = 21.34;   // Fator de calibração
 
int contagem = 0;           // Variável de contagem
float soma_tensao = 0;      // Variável para soma de tensão
float media = 0;            // Variável que calcula a media
float entrada_A0;           // Variável de leitura do pino A0
float tensao;               // Variável para conversão em tensão
unsigned long tempo;        // Float tempo
 

void setup() {
  // Serial communication with the module
  mySoftwareSerial.begin(9600);
  // Initialize Arduino serial
  Serial.begin(115200);
  // Check if the module is responding and if the SD card is found
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini"));
  Serial.println(F("Initializing DFPlayer module ... Wait!"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Not initialized:"));
    Serial.println(F("1. Check the DFPlayer Mini connections"));
    Serial.println(F("2. Insert an SD card"));
    while (true)
      ;
  }
  Serial.println();
  Serial.println(F("DFPlayer Mini module initialized!"));
  // Initial settings
  myDFPlayer.setTimeOut(500);  // Serial timeout 500ms
  myDFPlayer.volume(30);        // Volume 30
  myDFPlayer.EQ(2);            // Normal equalization


  pinMode(2,INPUT_PULLUP);

  lcd.init();                   // Inicializa o display
  lcd.begin(16, 2);             // Inicializa todos os caracteres
  lcd.backlight();              // Inicializa o backlight
  lcd.setCursor(0, 0);          // Posiciona o cursor na posição
  lcd.print("Projeto Integrador");       // Escreve no monitor serial
  lcd.setCursor(0, 1);          // Posiciona o cursor na posição
  lcd.print("Leitura de pH");   // Escreve no monitor serial
  delay(2000);
  lcd.clear();                  // Limpa o conteúdo do display
  


}

void loop() {
  soma_tensao = 0;   // Inicia soma_tensão em 0
  contagem = 0;      // Inicia a contagem em 0
 
  while (contagem < 5) {                   // Executa enquanto contagem menor que 10
    tempo = millis();                       // Define o tempo em microssegundos
    entrada_A0 = analogRead(A0);            // Lê a entrada analógica
    tensao = (entrada_A0 * 5) / 1024.0;   // Converte em tensão, o valor lido
    soma_tensao = (soma_tensao + tensao);   // Soma a tensão anterior com a atual
    contagem++;                             // Soma 1 à variável de contagem
    delay(100);                             // Aguarda para próxima leitura
  }
 
  media = soma_tensao / 5;                 // Calcula a média das leituras
 
  float valor_pH = -5.70 * media + valor_calibracao;    // Calcula valor de pH
 
  lcd.setCursor(0, 0);            // Posiciona o cursor no display
  lcd.print("Valor pH: ");        // Escreve no display
  lcd.setCursor(11, 0);           // Posiciona o cursor no display
  lcd.print(valor_pH, 2);         // Escreve o pH com uma casa decimal
  delay(1000);

  // Define data type for valor (choose float or double)
  double valor = valor_pH;

  // Separate integer and decimal parts
  int parteInteira = static_cast<int>(valor);
  double parteDecimal = valor - parteInteira;

  // Create a vector to store values
  int vetor[3]; // Alterado para 3 elementos para armazenar a parte inteira, dezena e centésima da parte decimal

  // Add values to the vector
  vetor[0] = parteInteira;
  vetor[1] = static_cast<int>(parteDecimal * 100); // Dezena da parte decimal

  botao = digitalRead(2);

  // Print the vector values and play corresponding audio files based on button state
  for (int i = 0; i < 2; i++) {
    Serial.print(" "); // Adiciona um espaço entre os valores impressos
    if (botao == LOW) {
      if (i == 0) {
        myDFPlayer.playFolder(1, 100);
        delay(1500);
        myDFPlayer.playFolder(1, (vetor[i]));
        delay(500);
        Serial.print(vetor[i]);
      }
      if (i == 1) {
        myDFPlayer.playFolder(1, 120);
        delay(1000);
        myDFPlayer.playFolder(1, (vetor[i]));
        delay(500);
        Serial.print(vetor[i]);
      }
    }
  }
  Serial.println();
}
