#include <stdlib.h>
#include <time.h>
#include <limits.h>
#define PLAYER1 2
#define PLAYER2 3
#define PLAYER3 4
#define PLAYER4 5
#define PLAYER5 6
#define PLAYER6 7
#define BUTTON_CONT 9
#define BUTTON_SET 8
#define SPEAKER 10

int players[] = {0, 0, 0, 0, 0, 0};
int num_players = 0;
int small_blind, big_blind;

void beeper(){
  for (int i = 0; i < 3; i++){
    for(int j = 2; j < 8; j++){
      digitalWrite(j, HIGH);
    }
    tone(SPEAKER, 300);
    delay(300);
    for(int j = 2; j < 8; j++){
      digitalWrite(j, LOW);
    }
    noTone(SPEAKER);
    delay(300);
  }
}

void blind_logic(int big_blind){
  Serial.print("Big Blind: ");
  Serial.println(big_blind);
  //small-blind logic: loop through prev players and check if 1
  if (big_blind == 0) {
    small_blind = num_players - 1;
  }
  else {
    small_blind = big_blind - 1;
  }
  while(players[small_blind] == 0) {
    small_blind--;
    if (small_blind < 0) {
      small_blind = num_players - 1;
    }
    if (players[small_blind] == 1) {
      break;
    }
  }

  Serial.print("Small Blind: ");
  Serial.println(small_blind);

  for(int i = 2; i < 8; i++){
    digitalWrite(i, LOW);
  }

  digitalWrite((small_blind+2), HIGH);
  digitalWrite((big_blind+2), HIGH);
  
  if (big_blind == small_blind) {
    for(int i = 150; i < 851; i+=100) {
      for(int j = 2; j < 8; j++){
        digitalWrite(j, HIGH);
      }
      tone(SPEAKER, i);
      delay(100);
    }
    delay(500);
    noTone(SPEAKER);
    for(int j = 2; j < 8; j++){
      digitalWrite(j, LOW);
    }
  }
}

void setup() {
  Serial.begin(9600);
  // Initializing outputs and inputs
  for(int i = 2; i < 11; i++) {
    if (i < 8){
      pinMode(i, OUTPUT);
    }
    else {
      pinMode(i, OUTPUT);
    }
  }
  
  // allows for nice lighting :) jacob's contribution!!!!!!!!!!!!
  for(int i = 7; i > 1; i--){
    digitalWrite(i, HIGH);
    tone(SPEAKER, (8-i)*200);
    delay(100);
    noTone(SPEAKER);
  }
  delay(200);
  for(int i = 2; i < 8; i++){
    digitalWrite(i, LOW);
    tone(SPEAKER, (8-i)*200);
    delay(100);
    noTone(SPEAKER);
  }

  int new_setval = digitalRead(BUTTON_SET);
  int prev_setval = 0;
  int new_contval = digitalRead(BUTTON_CONT);
  int prev_contval = 0;

  delay(1000);

  beeper();

  const unsigned long increment = 1000.0*60*15;
  unsigned long multiplier = 0;
  unsigned long timer;

  // Set timer
  while(1) {
    prev_setval = new_setval;
    new_setval = digitalRead(BUTTON_SET);

    if (new_setval == 0 && prev_setval == 1) {
      multiplier += 1;
      if (multiplier > 6) {
        for(int i = 2; i < 8; i++){
          digitalWrite(i, LOW);
        }
        multiplier = 0;
      }
      digitalWrite(multiplier+1, HIGH);
    }

    prev_contval = new_contval;
    new_contval = digitalRead(BUTTON_CONT);
    if (new_contval == 0 && prev_contval == 1){
      for(int i = 2; i < 8; i++) {
        digitalWrite(i, LOW);
      }
      if (multiplier == 0){
        timer = 1000.0*60*15;
      }
      else {
        timer = multiplier*increment;
      }
      break;
    }
  }

  // Setup for Choosing Players
  while(1){
    prev_setval = new_setval;
    new_setval = digitalRead(BUTTON_SET);
    if (new_setval == 0 && prev_setval == 1) {
      num_players++;
      if (num_players > 6) {
        for(int i = 4; i < 8; i++){
          digitalWrite(i, LOW);
        }
        num_players = 2;
      }
      digitalWrite((num_players+1), HIGH);
      
      for(int i = 0; i < 6; i++){
        if (i < num_players){
          players[i] = 1;
        }
        else{
          players[i] = 0;
        }
      }
    }

    prev_contval = new_contval;
    new_contval = digitalRead(BUTTON_CONT);
    if (new_contval == 0 && prev_contval == 1){
      for(int i = 2; i < 8; i++){
        digitalWrite(i, LOW);
      }
      break;
    }
  }

  big_blind = 0;
  blind_logic(big_blind);
  int blink = 0;
  unsigned long start_time = millis();
  unsigned long curr_time;

  // The Gameplay
  while(1){
    curr_time = millis();
    if ((curr_time - start_time) > timer){ // jcob helped devu here
      start_time = millis();
      beeper();
      digitalWrite((big_blind)+2, HIGH);
    }
    prev_contval = new_contval;
    new_contval = digitalRead(BUTTON_CONT);
    prev_setval = new_setval;
    new_setval = digitalRead(BUTTON_SET);
    blink = !blink;
    digitalWrite((big_blind+2), blink);
    delay(100);

    if (prev_contval == 0 && new_contval == 1){
      big_blind++;
      big_blind%=num_players;
      while (players[big_blind] == 0) {
        big_blind++;
        big_blind %= num_players;
        if (players[big_blind] == 1) {
          break;
        }
      }
      blind_logic(big_blind);
    }

    if (prev_setval == 0 && new_setval == 1){
      digitalWrite((big_blind+2), LOW);
      players[big_blind] = 0;
      for(int i = 450; i > 149; i-=50){
        tone(SPEAKER, i);
        delay(100);
      }
      noTone(SPEAKER);
    }
  }
}

void loop() {
  // Unused
}
