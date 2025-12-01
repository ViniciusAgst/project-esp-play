#include "pitches.h"

#define buttonPin 2
#define ledPin 6
#define SPEAKER_PIN 10

volatile bool buttonFlag = false; 

// INTERRUPÇÃO 
void IRAM_ATTR handleButton() {
  buttonFlag = true;
}

// MUSICA QUE SERA TOCADA
int currentTheme = 0;

// TEMPO DE EXECUTACAO DAS NOTAS
int tempo = 100;

// MELODIA 1 
const int melody1[] = {
  // Jingle Bells
  // "Jingle bells, jingle bells, jingle all the way."
  NOTE_B4, 4, NOTE_B4, 4, NOTE_B4, 2,
  NOTE_B4, 4, NOTE_B4, 4, NOTE_B4, 2,
  NOTE_B4, 4, NOTE_D5, 4, NOTE_G4, 4, NOTE_A4, 4,
  NOTE_B4, 1,

  // "Oh, what fun it is to ride in a one-horse open sleigh, hey!"
  NOTE_C5, 4, NOTE_C5, 4, NOTE_C5, -4, NOTE_C5, 8,
  NOTE_C5, 4, NOTE_B4, 4, NOTE_B4, -4, NOTE_B4, 8,
  NOTE_A4, 4, NOTE_A4, 4, NOTE_B4, 4, NOTE_A4, 4,
  NOTE_D5, 1,

  // Repetição da primeira parte
  // "Jingle bells, jingle bells, jingle all the way."
  NOTE_B4, 4, NOTE_B4, 4, NOTE_B4, 2,
  NOTE_B4, 4, NOTE_B4, 4, NOTE_B4, 2,
  NOTE_B4, 4, NOTE_D5, 4, NOTE_G4, 4, NOTE_A4, 4,
  NOTE_B4, 1,

  // Repetição da segunda parte com final diferente
  // "Oh, what fun it is to ride in a one-horse open sleigh!"
  NOTE_C5, 4, NOTE_C5, 4, NOTE_C5, -4, NOTE_C5, 8,
  NOTE_C5, 4, NOTE_B4, 4, NOTE_B4, -4, NOTE_B4, 8,
  NOTE_A4, 4, NOTE_A4, 4, NOTE_D5, 4, NOTE_B4, 4,
  NOTE_G4, 1,
};

// MELODIA 2
const int melody2[] = {  

  NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_F4,-4,
  NOTE_E4,-2, NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_G4,-4,
  NOTE_F4,-2, NOTE_C4,4, NOTE_C4,8,

  NOTE_C5,-4, NOTE_A4,-4, NOTE_F4,-4, 
  NOTE_E4,-4, NOTE_D4,-4, NOTE_AS4,4, NOTE_AS4,8,
  NOTE_A4,-4, NOTE_F4,-4, NOTE_G4,-4,
  NOTE_F4,-2, 
};


// QUANTAS NOTAS A MELODIA 1 TEM
int notes1 = sizeof(melody1) / sizeof(melody1[0]) / 2;

// QUANTAS NOTAS A MELODIA 2 TEM
int notes2 = sizeof(melody2) / sizeof(melody2[0]) / 2;

// VARIAVEIS COM DURACAO
int divider = 0, noteDuration = 0;

// SETUP INICIAL DO PROJETO
void setup() {
  Serial.begin(9600); 

  pinMode(ledPin, OUTPUT); 
  pinMode(buttonPin, INPUT_PULLUP); 

  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButton, FALLING);

  pinMode(SPEAKER_PIN, OUTPUT);
}

//FUNCAO QUE TOCA UMA MELODIA
void playSound(const int soundList[], int notes){
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    if(buttonFlag){
      buttonFlag = false;
      noTone(SPEAKER_PIN);
      return;
    }

    int wholenote = (60000 * 4) / tempo;

    divider = soundList[thisNote + 1];

    if (divider > 0) {

      noteDuration = (wholenote) / divider;

    } else if (divider < 0) {

      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; 

    }

    tone(SPEAKER_PIN, soundList[thisNote], noteDuration*0.9);

    delay(noteDuration);
    
    noTone(SPEAKER_PIN);
  }
}

// LOOP PRINCIPAL DO PROGRAMA
void loop() {
  if (buttonFlag) {
    buttonFlag = false; 

    digitalWrite(ledPin, HIGH);
    if (currentTheme == 0) {
       currentTheme = 1; 
       tempo = 120;
       playSound(melody1, notes1);
     } else {
       currentTheme = 0;
       tempo = 140; 
       playSound(melody2, notes2);
     } 
    digitalWrite(ledPin, LOW); 
  } 
  
  delay(20); 
}
