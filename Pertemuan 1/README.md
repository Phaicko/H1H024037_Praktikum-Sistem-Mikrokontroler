# Jawaban dari Pertanyaan pada Modul
##Percabangan
1.	Lampu akan reset waktu ke awal saat timeDelay sampai 100
2.	Lampu akan berkedip lebih cepat seiring berkedip sampai kesepuluh kalinya.
3.	timeDelay berfungsi untuk memberi jeda pada setiap lampu dinyalakan dan dimatikan. 
4.
const int ledPin = 6;
int timeDelay = 1000;
int step = -100; //
void setup() {
  pinMode(ledPin, OUTPUT);}
void loop() {
  digitalWrite(ledPin, HIGH);
  delay(timeDelay); 
  digitalWrite(ledPin, LOW);
  delay(timeDelay);
  // ubah arah delay menjadi bertambah
  timeDelay += step;
  // kecepatan max
  if (timeDelay <= 100) {
    timeDelay = 100; 
    step = 100; 
  }
  // kecepatan min
  if (timeDelay >= 1000) {
    timeDelay = 1000;
    step = -100;}}

##Perulangan
1.	![alt text](<rangkaian 5 led.png>)
2.	Saat memasuki loop for pertama yang bersifat increment dari pin 2 sampai 7 sehingga lampu menyala berurutan dari kiri ke kanan
3.	Saat memasuki loop for kedua yang bersifat decrement dari pin 7 sampai 2 sehinga lampu menyala kembali berurutan dari kanan ke kiri
4.
int timer = 500;
void setup() {
  for (int ledPin = 2; ledPin < 8; ledPin++) {
    pinMode(ledPin, OUTPUT);
 }}
void loop() {
// Menyalakan 3 LED dari kiri (Pin 2, 3, 4)
  for (int ledPin = 2; ledPin <= 4; ledPin++) {
    digitalWrite(ledPin, HIGH);
  }
  delay(timer);
  // Matikan semua sebelum pindah ke kanan
  for (int ledPin = 2; ledPin <= 4; ledPin++) {
    digitalWrite(ledPin, LOW);
  }
  // Menyalakan 3 LED dari kanan (Pin 5, 6, 7)
  for (int ledPin = 5; ledPin <= 7; ledPin++) {
    digitalWrite(ledPin, HIGH);
  }
  delay(timer);
  // Matikan semua sebelum mengulang
  for (int ledPin = 5; ledPin <= 7; ledPin++) {
    digitalWrite(ledPin, LOW);
 }}
