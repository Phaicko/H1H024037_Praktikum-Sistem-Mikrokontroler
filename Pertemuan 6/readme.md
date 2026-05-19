# Percobaan 1
![alt text](<interupt.jpg>)
![alt text](<interupt.gif>)
## Jelaskan proses bagaimana tombol dapat mengubah kondisi LED menggunakan interrupt
Program utama berjalan terus-menerus di dalam loop(), di mana mikrokontroler selalu menuliskan nilai dari variabel ledState ke pin LED (pin 13). Pin 2 dikonfigurasi sebagai input dengan pull-up resistor internal (INPUT_PULLUP), sehingga secara normal berlogika HIGH. Ketika tombol ditekan, mikrokontroler akan menghubungkan pin 2 ke Ground (GND), menyebabkan tegangan turun seketika dari HIGH ke LOW. Penurunan tegangan ini (disebut falling edge) terdeteksi oleh perangkat keras mikrokontroler. Sesuai pengaturan pada attachInterrupt(), sistem langsung menghentikan (menjeda) eksekusi program loop(). Mikrokontroler melompat ke fungsi khusus bernama tombolInterrupt(). Di dalam fungsi ini, nilai variabel ledState diubah secara berlawanan (dari false menjadi true, atau sebaliknya). Setelah eksekusi tombolInterrupt() selesai, mikrokontroler kembali ke loop() tepat di titik tinggal. Baris perintah digitalWrite(13, ledState); kemudian akan dieksekusi dengan nilai ledState yang baru, sehingga LED menyala atau mati.
## Apa fungsi attachInterrupt() pada program tersebut?
Fungsi attachInterrupt() digunakan untuk menghubungkan event perangkat keras eksternal dengan sebuah fungsi di dalam kode.
## Mengapa pada ISR tidak disarankan menggunakan delay() dan Serial.print()?
Interrupt Service Routine (ISR) harus dieksekusi secepat dan sesingkat mungkin, dimana delay() bekerja dengan mengandalkan interupsi timer internal dari mikrokontroler. Masalahnya, ketika mikrokontroler sedang berada di dalam sebuah fungsi ISR, akan secara otomatis menonaktifkan seluruh interupsi lainnya. Karena interupsi timer tidak bisa berjalan, fungsi delay() akan membuat program terhenti total. Masalah dengan Serial.print() adalah proses mengirim data serial yang sangat lambat dan sangat bergantung pada sistem interupsi di balik layar untuk memindahkan data dari buffer ke perangkat keras. Melakukan print data di dalam ISR berpotensi besar membuat sistem crash atau setidaknya menyebabkan program utama tertahan terlalu lama.
## Apa fungsi keyword volatile pada variabel ledState?
Variabel ledState dideklarasikan dengan tipe data boolean dan ditambah awalan volatile. Keyword volatile ini adalah instruksi khusus untuk compiler.
## Pada percobaan digunakan mode interrupt FALLING. Modifikasikan program menggunakan mode interrupt lain (RISING, CHANGE, atau LOW)
Pada mode RISING, interupsi dipicu hanya ketika ada lonjakan tegangan sinyal dari LOW naik ke HIGH. Pada mode CHANGE, interupsi dipicu setiap kali terjadi perubahan status logika pada pin, baik dari HIGH ke LOW maupun dari LOW ke HIGH. Pada mode LOW, interupsi akan terus-menerus terpicu dan menjalankan ISR berulang kali tanpa henti selama sinyal pada pin dipertahankan pada logika LOW.

# Percobaan 2
![alt text](<timer.jpg>)
![alt text](<timer.gif>)
## Jelaskan bagaimana fungsi millis() bekerja pada program tersebut
Fungsi millis() pada dasarnya bertindak seperti stopwatch internal yang mulai menghitung dalam satuan milidetik (ms) tepat sejak mikrokontroler Arduino pertama kali dihidupkan atau di-reset.
## Apa perbedaan utama antara delay() dan millis()?
delay() berfungsi sebagai jeda absolut, jika menggunakan delay(1000), mikrokontroler akan berhenti total dan diam selama 1 detik penuh, mikrokontroler tidak bisa melakukan instruksi apa pun, membaca tombol, atau menerima data sensor sampai waktu 1 detik tersebut habis.
millis() berfungsi sebagai sistem pengecekan waktu, mikrokontroler hanya melihat "jam" untuk memeriksa apakah sudah saatnya melakukan suatu tugas. Jika belum waktunya, mikrokontroler bisa mengerjakan tugas lain terlebih dahulu.
## Mengapa metode millis() disebut non-blocking?
Metode millis() disebut non-blocking atau tidak memblokir karena penggunaannya tidak memblokir alur eksekusi program secara keseluruhan. Karena tidak ada jeda yang menghentikan program, fungsi utama loop() dapat terus berputar ribuan kali per detik. Hal ini membuat program yang melakukan multitasking, seperti menyalakan LED yang berkedip sambil secara bersamaan membaca input dari sensor atau tombol tanpa ada keterlambatan respons.
## Modifikasi program:
### LED pertama berkedip setiap 1 detik
```C++
#include <Arduino.h>
// KONFIGURASI LED 1
const int ledPin1 = 13;             // Pin untuk LED pertama
unsigned long previousMillis1 = 0;  // Waktu terakhir LED 1 berubah
const long interval1 = 1000;        // Interval kedip LED 1 (1 detik)
bool ledState1 = false;             // Status LED 1
// KONFIGURASI LED 2
const int ledPin2 = 12;             // Pin untuk LED kedua (menggunakan pin 12)
unsigned long previousMillis2 = 0;  // Waktu terakhir LED 2 berubah
const long interval2 = 500;         // Interval kedip LED 2 (500 ms)
bool ledState2 = false;             // Status LED 2
void setup() {
  // Atur kedua pin sebagai OUTPUT
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}
void loop() {
  // Ambil waktu saat ini SATU KALI di awal loop
  unsigned long currentMillis = millis();
  // TUGAS 1: KONTROL LED 1
  // Cek apakah sudah 1000 ms berlalu untuk LED 1
  if (currentMillis - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis;  // Simpan waktu terakhir LED 1
    ledState1 = !ledState1;           // Toggle status LED 1
    digitalWrite(ledPin1, ledState1); // Eksekusi nyala/mati ke pin 13
  }
}
```
### LED kedua berkedip setiap 500 ms
```C++
#include <Arduino.h>
//  KONFIGURASI LED 1 
const int ledPin1 = 13;             // Pin untuk LED pertama
unsigned long previousMillis1 = 0;  // Waktu terakhir LED 1 berubah
const long interval1 = 1000;        // Interval kedip LED 1 (1 detik)
bool ledState1 = false;             // Status LED 1
//  KONFIGURASI LED 2 
const int ledPin2 = 12;             // Pin untuk LED kedua (menggunakan pin 12)
unsigned long previousMillis2 = 0;  // Waktu terakhir LED 2 berubah
const long interval2 = 500;         // Interval kedip LED 2 (500 ms)
bool ledState2 = false;             // Status LED 2
void setup() {
  // Atur kedua pin sebagai OUTPUT
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}
void loop() {
  // Ambil waktu saat ini SATU KALI di awal loop
  unsigned long currentMillis = millis();
  // TUGAS 2: KONTROL LED 2
  // Cek apakah sudah 500 ms berlalu untuk LED 2
  if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis;  // Simpan waktu terakhir LED 2
    ledState2 = !ledState2;           // Toggle status LED 2
    digitalWrite(ledPin2, ledState2); // Eksekusi nyala/mati ke pin 12
  }
  // Karena tidak ada delay(), mikrokontroler akan terus 
  // mengeksekusi pengecekan ini dengan sangat cepat.
}
```
### Tanpa menggunakan delay() 
```C++
#include <Arduino.h>
//  KONFIGURASI LED 1 
const int ledPin1 = 13;             // Pin untuk LED pertama
unsigned long previousMillis1 = 0;  // Waktu terakhir LED 1 berubah
const long interval1 = 1000;        // Interval kedip LED 1 (1 detik)
bool ledState1 = false;             // Status LED 1
//  KONFIGURASI LED 2 
const int ledPin2 = 12;             // Pin untuk LED kedua (menggunakan pin 12)
unsigned long previousMillis2 = 0;  // Waktu terakhir LED 2 berubah
const long interval2 = 500;         // Interval kedip LED 2 (500 ms)
bool ledState2 = false;             // Status LED 2
void setup() {
  // Atur kedua pin sebagai OUTPUT
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}
void loop() {
  // Karena tidak ada delay(), mikrokontroler akan terus mengeksekusi pengecekan ini dengan sangat cepat.
}
```

