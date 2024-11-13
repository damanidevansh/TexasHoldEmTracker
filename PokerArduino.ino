// Definitions and Variable Initializations
#include <stdlib.h>
#include <time.h>
#define CONTINUE
#define SET
#define PLAYER1 2
#define PLAYER2 3
#define PLAYER3 4
#define PLAYER4 5
#define PLAYER5 6
#define PLAYER6 7
#define BUTTONPRESSVOLT 9
#define DIGINP2 2
#define SPEAKER 10
#define BUTTON_CONT 8

int players[] = {0, 0, 0, 0, 0, 0};
int num_player = 0;
int whileloop = 1;
int ante, chuck, pin, del_happened;

void ante_chuck_logic(int ante) {

    chuck = (ante+1)%num_player;
    //if chuck does not exist, move forward until done
    while (players[chuck] == 0) {
      if (players[++chuck])
        break;
    }

    Serial.print("chuck = ");
    Serial.println(chuck);
    Serial.print("ante = ");
    Serial.println(ante);

    digitalWrite(PLAYER1, LOW);
    digitalWrite(PLAYER2, LOW);
    digitalWrite(PLAYER3, LOW);
    digitalWrite(PLAYER4, LOW);
    digitalWrite(PLAYER5, LOW);
    digitalWrite(PLAYER6, LOW);

    if (ante == 0)
        digitalWrite(PLAYER1, HIGH);
    if (ante == 1)
        digitalWrite(PLAYER2, HIGH);
    if (ante == 2)
        digitalWrite(PLAYER3, HIGH);
    if (ante == 3)
        digitalWrite(PLAYER4, HIGH);
    if (ante == 4)
        digitalWrite(PLAYER5, HIGH);
    if (ante == 5)
        digitalWrite(PLAYER6, HIGH);

    if (chuck == 0)
        digitalWrite(PLAYER1, HIGH);
    if (chuck == 1)
        digitalWrite(PLAYER2, HIGH);
    if (chuck == 2)
        digitalWrite(PLAYER3, HIGH);
    if (chuck == 3)
        digitalWrite(PLAYER4, HIGH);
    if (chuck == 4)
        digitalWrite(PLAYER5, HIGH);
    if (chuck == 5)
        digitalWrite(PLAYER6, HIGH);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PLAYER1, OUTPUT);
  pinMode(PLAYER2, OUTPUT);
  pinMode(PLAYER3, OUTPUT);
  pinMode(PLAYER4, OUTPUT);
  pinMode(PLAYER5, OUTPUT);
  pinMode(PLAYER6, OUTPUT);
  pinMode(BUTTONPRESSVOLT, INPUT);
  pinMode(BUTTON_CONT, INPUT);


  // allows for nice lighting :) jacob's contribution!!!!!!!!!!!!
  digitalWrite(PLAYER6, HIGH);
  delay(100);
  digitalWrite(PLAYER5, HIGH);
  delay(100);
  digitalWrite(PLAYER4, HIGH);
  delay(100);
  digitalWrite(PLAYER3, HIGH);
  delay(100);
  digitalWrite(PLAYER2, HIGH);
  delay(100);
  digitalWrite(PLAYER1, HIGH);
  delay(300);
  digitalWrite(PLAYER1, LOW);
  delay(100);
  digitalWrite(PLAYER2, LOW);
  delay(100);
  digitalWrite(PLAYER3, LOW);
  delay(100);
  digitalWrite(PLAYER4, LOW);
  delay(100);
  digitalWrite(PLAYER5, LOW);
  delay(100);
  digitalWrite(PLAYER6, LOW);

  
  // pinMode(PLAYER_BUTTON, INPUT);
  // pinMode(CONT_BUTTON, INPUT);
  // pinMode(SPEAKER, OUTPUT);
  // pinMode(A0, INPUT);
  // pinMode(2, OUTPUT);
  int ButtonPresses = 0;
  int prev_sensorval = digitalRead(BUTTONPRESSVOLT);
  int new_sensorval = digitalRead(BUTTONPRESSVOLT);
  int prev_contvolt = digitalRead(BUTTON_CONT);
  int contvolt = digitalRead(BUTTON_CONT);

  while(1) {
    prev_sensorval = new_sensorval;
    new_sensorval = digitalRead(BUTTONPRESSVOLT);
    // Serial.println(new_sensorval);
    if (new_sensorval == 0 && prev_sensorval == 1){
      num_player++
      if (num_player > 6) {
        num_player = 2;
      }
      // Serial.println(num_player);
      if (num_player == 1) {
        digitalWrite(PLAYER6, LOW);
        digitalWrite(PLAYER5, LOW);
        digitalWrite(PLAYER4, LOW);
        digitalWrite(PLAYER3, LOW);
        digitalWrite(PLAYER2, LOW);
        digitalWrite(PLAYER1, HIGH);
        for(int i = 0; i < 6; i++){
          players[i] = 0;
        }
        players[0] = 1;
      }
      if (num_player == 2) {
        digitalWrite(PLAYER6, LOW);
        digitalWrite(PLAYER5, LOW);
        digitalWrite(PLAYER4, LOW);
        digitalWrite(PLAYER3, LOW);
        digitalWrite(PLAYER2, HIGH);
        digitalWrite(PLAYER1, HIGH);
      }
      if (num_player == 3) {
        digitalWrite(PLAYER6, LOW);
        digitalWrite(PLAYER5, LOW);
        digitalWrite(PLAYER4, LOW);
        digitalWrite(PLAYER3, HIGH);
        digitalWrite(PLAYER2, HIGH);
        digitalWrite(PLAYER1, HIGH);
      }
      if (num_player == 4) {
        digitalWrite(PLAYER6, LOW);
        digitalWrite(PLAYER5, LOW);
        digitalWrite(PLAYER4, HIGH);
        digitalWrite(PLAYER3, HIGH);
        digitalWrite(PLAYER2, HIGH);
        digitalWrite(PLAYER1, HIGH);
      }
      if (num_player == 5) {
        digitalWrite(PLAYER6, LOW);
        digitalWrite(PLAYER5, HIGH);
        digitalWrite(PLAYER4, HIGH);
        digitalWrite(PLAYER3, HIGH);
        digitalWrite(PLAYER2, HIGH);
        digitalWrite(PLAYER1, HIGH);
      }
      if (num_player == 6) {
        digitalWrite(PLAYER6, HIGH);
        digitalWrite(PLAYER5, HIGH);
        digitalWrite(PLAYER4, HIGH);
        digitalWrite(PLAYER3, HIGH);
        digitalWrite(PLAYER2, HIGH);
        digitalWrite(PLAYER1, HIGH);
      }

      for(int i = 0; i < 6; i++){
          if (i<num_player) {
            players[i] = 1;
          }
          else {
            players[i] = 0;
          }
      }
    }
    prev_contvolt = contvolt;
    contvolt = digitalRead(BUTTON_CONT);

      // Serial.println(contvolt);
    if (contvolt == 0 && prev_contvolt == 1){
      digitalWrite(PLAYER6, LOW);
      digitalWrite(PLAYER5, LOW);
      digitalWrite(PLAYER4, LOW);
      digitalWrite(PLAYER3, LOW);
      digitalWrite(PLAYER2, LOW);
      digitalWrite(PLAYER1, LOW);
      // Serial.println("Reached cont");
      break;
    }


  }
  pin = 2;
  ante_chuck_logic(0);

  while (1) {
      prev_contvolt = contvolt;
      contvolt = digitalRead(BUTTON_CONT);
      digitalWrite(pin, HIGH);
      delay(100);
      digitalWrite(pin, LOW);
      delay(100);


      // Serial.println(new_sensorval);
      if (contvolt == 0 && prev_contvolt == 1){
        // Serial.println(ante);
        // Serial.println(chuck);
        if (!del_happened) 
          ante = (ante + 1)%num_player;
        else
          del_happened = 0;
        while (players[ante] == 0) {
          if (players[(++ante)%num_player])
            break;
        }
        ante_chuck_logic(ante);
        pin = ante + 2;
      }
        prev_sensorval = new_sensorval;
        new_sensorval = digitalRead(BUTTONPRESSVOLT);
        // Serial.println(new_sensorval);
        if (new_sensorval == 0 && prev_sensorval == 1){
            if (chuck == 0)
                digitalWrite(PLAYER1, LOW);
            if (chuck == 1)
                digitalWrite(PLAYER2, LOW);
            if (chuck == 2)
                digitalWrite(PLAYER3, LOW);
            if (chuck == 3)
                digitalWrite(PLAYER4, LOW);
            if (chuck == 4)
                digitalWrite(PLAYER5, LOW);
            if (chuck == 5)
                digitalWrite(PLAYER6, LOW);
          players[chuck] = 0;
          del_happened = 1;
          num_player--;
        }
      
  }

}




void loop() {

}
