#include "sound.h"
#include <Arduino.h>
void smashSound(int buzzerPin) {
  tone(buzzerPin, NOTE_E5, 150); // Nada E5, 150 ms
  delay(150); 
  tone(buzzerPin, NOTE_G5, 150); // Nada G5, 150 ms
  delay(150); 
  tone(buzzerPin, NOTE_C6, 150); // Nada C6, 150 ms
  delay(150); 
  tone(buzzerPin, NOTE_D6, 150); // Nada D6, 150 ms
  delay(150); 
  tone(buzzerPin, NOTE_B5, 150); // Nada B5, 150 ms
  delay(150); 
  tone(buzzerPin, NOTE_E6, 150); // Nada E6, 150 ms
  delay(150); 

  // Mengakhiri dengan nada lebih rendah
  tone(buzzerPin, NOTE_A4, 200); // Nada A4, 200 ms
  delay(200); 
  
  // Matikan suara setelah selesai
  noTone(buzzerPin);
}

void setSound(int buzzerPin) {
  // Nada-nada yang digunakan untuk efek kemenangan, dengan variasi yang lebih meriah
  tone(buzzerPin, NOTE_E5, 150);  // Nada E5, 100 ms
  delay(150);
  tone(buzzerPin, NOTE_G5, 150);  // Nada G5, 100 ms
  delay(150);
  tone(buzzerPin, NOTE_C6, 150);  // Nada C6, 100 ms
  delay(150);
  tone(buzzerPin, NOTE_D6, 150);  // Nada D6, 100 ms
  delay(150);
  tone(buzzerPin, NOTE_F6, 150);  // Nada F6, 100 ms
  delay(150);
  tone(buzzerPin, NOTE_E6, 150);  // Nada E6, 100 ms
  delay(150);

  // Penutupan dengan nada lebih rendah untuk menandakan kemenangan
  tone(buzzerPin, NOTE_C5, 200);  // Nada C5, 150 ms
  delay(200);

  noTone(buzzerPin);  // Matikan suara setelah selesai
}

void victory(int buzzerPin) {
  // Nada-nada yang digunakan untuk efek kemenangan, dengan variasi yang lebih meriah
  tone(buzzerPin, NOTE_E5, 150);  // Nada E5, 100 ms
  delay(150);
  tone(buzzerPin, NOTE_G5, 150);  // Nada G5, 100 ms
  delay(150);
  tone(buzzerPin, NOTE_C6, 150);  // Nada C6, 100 ms
  delay(150);
  tone(buzzerPin, NOTE_D6, 150);  // Nada D6, 100 ms
  delay(150);

  // Penutupan dengan nada lebih rendah untuk menandakan kemenangan
  tone(buzzerPin, NOTE_C5, 250);  // Nada C5, 200 ms
  delay(200);

  noTone(buzzerPin);  // Matikan suara setelah selesai
}

