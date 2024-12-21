#ifndef ANGKA2_H
#define ANGKA2_H

#include <LedControl.h>  // Memasukkan library LedControl

// Deklarasi fungsi untuk menampilkan angka 0-9 pada digit pertama dan digit kedua
void AngkaMirror0(LedControl lc);
void AngkaMirror1(LedControl lc);
void AngkaMirror2(LedControl lc);
void AngkaMirror3(LedControl lc);
void AngkaMirror4(LedControl lc);
void AngkaMirror5(LedControl lc);
void AngkaMirror6(LedControl lc);
void AngkaMirror7(LedControl lc);
void AngkaMirror8(LedControl lc);
void AngkaMirror9(LedControl lc);

// Deklarasi fungsi untuk menampilkan angka 0-9 pada digit kedua
void AngkaMirror0DigitKedua(LedControl lc);
void AngkaMirror1DigitKedua(LedControl lc);
void AngkaMirror2DigitKedua(LedControl lc);
void AngkaMirror3DigitKedua(LedControl lc);
void AngkaMirror4DigitKedua(LedControl lc);
void AngkaMirror5DigitKedua(LedControl lc);
void AngkaMirror6DigitKedua(LedControl lc);
void AngkaMirror7DigitKedua(LedControl lc);
void AngkaMirror8DigitKedua(LedControl lc);
void AngkaMirror9DigitKedua(LedControl lc);
//void jumlahsetmirror1(LedControl lc) ;
//void jumlahsetmirror2(LedControl lc) ;
//void jumlahsetmirror3(LedControl lc);
//void jumlahsetmirror4(LedControl lc) ;

// Fungsi untuk menampilkan angka 0-20 dengan membagi angka menjadi dua digit
void AngkaMirror(LedControl lc, int angka);
//void tampilkanSetmirror(LedControl lc, int set) ;
#endif
