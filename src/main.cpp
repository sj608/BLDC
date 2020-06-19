#include <Arduino.h>


unsigned long curr_time = 0;
unsigned long prev_time = 0;
unsigned int deb_delay = 100;
uint8_t step = 0;

bool button_read = 0;
bool seq_active = false;

void sequence(){
  // pin 2 3 4 are lows  ABC
  // pin 5 6 7 are highs ABC
  if(seq_active){
  switch (step)
  {
    case 0:
      // HIGH side
      digitalWrite(7, LOW);
      digitalWrite(5, HIGH);
      // LOW side
      digitalWrite(3, HIGH);
      // only write once
      seq_active = false;
      Serial.println(step);
      break;

    case 1:
      // HIGH side
      digitalWrite(5, HIGH);
      // LOW side
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      // only write once
      seq_active = false;
      Serial.println(step);
      break;

    case 2:
      // HIGH side
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      // LOW side
      digitalWrite(4, HIGH);
      // only write once
      seq_active = false;
      break;

    case 3:
      // HIGH side
      digitalWrite(6, HIGH);
      // LOW side
      digitalWrite(4, LOW);
      digitalWrite(2, HIGH);
      // only write once
      seq_active = false;
      break;

    case 4:
      // HIGH side
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      // LOW side
      digitalWrite(2, HIGH);
      // only write once
      seq_active = false;
      break;

    case 5:
      // HIGH side
      digitalWrite(7, HIGH);
      // LOW side
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      // only write once
      seq_active = false;
      break;

    default:
      break;
    }
  }
}

void buttonDebounce(){
  if(curr_time - prev_time > deb_delay){
    step++;
    if(step > 6){
      step = 0;
    }
    prev_time = curr_time;
  }
}

void setup (){
  Serial.begin(9600);
  for(int i = 2; i < 8; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  pinMode(8, INPUT);
  pinMode(A5, INPUT);
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  TCNT1 = 0;
  TCCR1B |= (1<<WGM12) | (1<<CS12) | (1<<CS10);
  TIMSK1 |= (1<<OCIE1A);
  sei();
}

ISR(TIMER1_COMPA_vect){
  OCR1A = map(analogRead(A5), 0, 1024, 7813, 1);
  step++;
  if(step>6){
    step = 0;
  }
  seq_active = true;
}

void loop (){
  // button_read = digitalRead(8);
  // if(button_read == LOW){
  //   curr_time = millis();
  // }
  // else{
  //   buttonDebounce();
  // }
  sequence();

}