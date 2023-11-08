  #include <Arduino.h>

  #define MAX_GAME_LENGTH 100

  // Defina os pinos para LEDs e botões
  const int ledPins[] = {16, 17, 5, 18};  // Pinos dos LEDs
  const int buttonPins[] = {27, 26, 33, 32};  // Pinos dos botões
  const int resetPin = 34;

  uint8_t gameSequence[MAX_GAME_LENGTH] = {0};
  uint8_t gameIndex = 0;

  // int sequence[4];  // Sequência de LEDs a ser lembrada
  // int playerSequence[4];  // Sequência do jogador
  int roundCount = 0;  // Número da rodada

  void setup() {
    // Inicialize os pinos dos LEDs como saída
    Serial.begin(9600);

    for (int i = 0; i < 4; i++) {
      pinMode(ledPins[i], OUTPUT);
      pinMode(buttonPins[i], INPUT_PULLUP);
    }
    pinMode(resetPin, INPUT);
    // Inicie a semente do gerador aleatório
    randomSeed(analogRead(0));
    delay(1000);
  }


byte readButtons() {
  while (true) {
    for (byte i = 0; i < 4; i++) {
      byte buttonPin = buttonPins[i];
      if (digitalRead(buttonPin) == LOW) {
        return i;
      }
      if (digitalRead(resetPin) == HIGH) {
        return -1;
    }
    }
    delay(1);
  }
}


  void gameOver() {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
        digitalWrite(ledPins[j], HIGH);
      }
      delay(500);
      for (int j = 0; j < 4; j++) {
        digitalWrite(ledPins[j], LOW);
      }
      delay(500);
    }
    roundCount = 0;
    delay(1000);
    resetGame();
  }

  void gameWon(){
    for (int k=0; k<8; k++){
      digitalWrite(ledPins[k], HIGH);
      delay(20);
      digitalWrite(ledPins[k], LOW);
      delay(20);
    }
    int roundCount = 0;
    delay(1000);
    resetGame();
  }

bool checkUserSequence() {
  for (int i = 0; i < gameIndex; i++) {
    byte expectedButton = gameSequence[i];
    byte actualButton = readButtons();
    led(actualButton);
    if (expectedButton != actualButton) {
      return false;
    }
  }
  return true;
}

void playSequence() {
  for (int i = 0; i < gameIndex; i++) {
    byte currentLed = gameSequence[i];
    led(currentLed);
    delay(50);
  }
}

void led(byte ledIndex) {
  digitalWrite(ledPins[ledIndex], HIGH);
  delay(300);
  digitalWrite(ledPins[ledIndex], LOW);
}

void resetGame() {
  gameIndex = 0;
  roundCount = 0;
  for (int i = 0; i < MAX_GAME_LENGTH; i++) {
    gameSequence[i] = -1;
  }
  gameIndex = 0;
  loop();
}

  void loop() {
    // pisca os leds pra indicar início do jogo
    if (roundCount == 0) {
      for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], HIGH);
        delay(100);
        digitalWrite(ledPins[i], LOW);
      }
      delay(500);
    }

    if (digitalRead(resetPin) == HIGH) {
    resetGame();
  }

      // Add a random color to the end of the sequence
  gameSequence[gameIndex] = random(0, 4);
  gameIndex++;
  if (gameIndex >= MAX_GAME_LENGTH) {
    gameIndex = MAX_GAME_LENGTH - 1;
  }
  playSequence();
  if (!checkUserSequence()) {
    gameOver();
  }
  delay(300);
  roundCount++;

  if(roundCount == 4){
    gameWon();
  }

}