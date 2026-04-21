# Percobaan 1
![alt text](<foto percobaan 1 pertemuan 3.jpg>)
## Jelaskan proses dari input keyboard hingga LED menyala/mati
Proses dari Input Keyboard hingga LED Menyala/Mati dimana mengetik angka '1' atau '0' di Serial Monitor komputer dan menekan Enter akan membuat komputer mengubah karakter tersebut menjadi kode ASCII (contohnya, '1' adalah 49) dan mengirimkannya melalui kabel USB ke chip antarmuka USB-to-Serial di papan Arduino
##  Mengapa digunakan Serial.available() sebelum membaca data? Apa yang terjadi jika baris tersebut dihilangkan?
Serial.available() digunakan untuk memeriksa berapa banyak byte data yang sedang mengantre (tersedia) di dalam Serial Buffer untuk dibaca.
## Modifikasi program agar LED berkedip (blink) ketika menerima input '2' dengan kondisi jika ‘2’ aktif maka LED akan terus berkedip sampai perintah selanjutnya
``` c++
// Menentukan pin 12 sebagai pin untuk LED. Menggunakan 'const' agar nilai tidak berubah.
const int PIN_LED = 12;
// Menyimpan status perintah terakhir yang diberikan. Default-nya '0' (Mati).
char currentCommand = '0';
// Variabel untuk menyimpan waktu terakhir LED berubah status (untuk logika blink tanpa delay).
unsigned long previousMillis = 0;
// Menentukan durasi kedipan LED (dalam milidetik). 500ms = 0.5 detik.
const long interval = 500;
// Menyimpan status LED saat ini (HIGH atau LOW) untuk keperluan kedipan.
int ledState = LOW;
void setup() {
  // Menginisialisasi komunikasi serial dengan kecepatan 9600 bit per detik.
  Serial.begin(9600);
  // Menampilkan instruksi ke pengguna di Serial Monitor.
  Serial.println("Ketik '1' (ON), '0' (OFF), atau '2' (BLINK)");
  // Mengatur pin 12 agar berfungsi sebagai pin OUTPUT.
  pinMode(PIN_LED, OUTPUT);
}
void loop() {
  // Mengecek apakah ada data yang masuk dari Serial Monitor.
  if (Serial.available() > 0) {
    // Membaca karakter yang masuk dan menyimpannya di variabel sementara 'data'.
    char data = Serial.read();
    // Menyaring input: hanya memperbarui currentCommand jika inputnya valid ('0', '1', atau '2').
    // Hal ini mencegah karakter newline dari tombol Enter merusak status sistem.
    if (data == '0' || data == '1' || data == '2') {
      currentCommand = data;
    }
  }
  // LOGIKA EKSEKUSI BERDASARKAN STATE 
  if (currentCommand == '1') {
    // Jika status adalah '1', nyalakan LED secara konstan.
    digitalWrite(PIN_LED, HIGH);
  } else if (currentCommand == '0') {
    // Jika status adalah '0', matikan LED secara konstan.
    digitalWrite(PIN_LED, LOW);
  } else if (currentCommand == '2') {
    // Jika status adalah '2', jalankan logika BLINK menggunakan millis().
    // Mencatat waktu sistem saat ini.
    unsigned long currentMillis = millis();
    // Mengecek apakah selisih waktu saat ini dan waktu terakhir LED berubah sudah melebihi interval (500ms).
    if (currentMillis - previousMillis >= interval) {
      // Jika ya, simpan waktu saat ini sebagai acuan untuk kedipan berikutnya.
      previousMillis = currentMillis;
      // Membalikkan status LED: Jika LOW jadikan HIGH, jika HIGH jadikan LOW.
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      // Terapkan status baru tersebut ke pin LED.
      digitalWrite(PIN_LED, ledState);
    }
  }
}
```
## 4. Tentukan apakah menggunakan delay() atau milis()! Jelaskan pengaruhnya terhadap sistem

Untuk kasus di mana sistem harus berkedip (menjalankan tugas berulang) sambil tetap siap menerima input baru, pilihan yang digunakan adalah millis(). Berikut adalah penjelasan pengaruhnya terhadap sistem:
Jika menggunakan delay() (Blocking Code):
Fungsi delay(500) akan menghentikan seluruh kerja mikrokontroler selama 500 milidetik. Sistem akan "membeku". Jika pengguna mengetik '0' (perintah mati) tepat saat Arduino sedang berada di dalam masa delay(), Arduino tidak akan bisa membaca Serial.available() secara responsif. Akibatnya, sistem terasa lambat, tidak responsif (lagging), atau bahkan bisa melewatkan input dari pengguna.
Jika menggunakan millis() (Non-Blocking Code):
Fungsi millis() bertindak seperti sebuah stopwatch yang terus berjalan di latar belakang. Dengan mencatat waktu dan mencari selisih waktu (currentMillis - previousMillis >= interval), fungsi loop() dapat terus berjalan tanpa henti, ratusan ribu kali per detik. Ini memungkinkan Arduino untuk terus memantau apakah ada data serial baru dari keyboard di setiap putarannya, sambil secara akurat menyalakan atau mematikan LED setiap 500ms. Sistem menjadi responsif dan bisa melakukan multitasking.


# Percobaan 2
![alt text](<gif percobaan 2 pertemuan 3.gif>)
## Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian tersebut

## Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan pin kanan tertukar
Pin potensiometer tidak boleh dipasang secara sembarangan, tetapi memiliki aturan tersendiri. Dimana:
Pin Tengah (Wiper) wajib dihubungkan ke pin Analog Arduino (A0), pin ini bertugas membaca pembagian tegangan yang berubah-ubah saat tuas diputar. Pin Kiri dan Kanan untuk dihubungkan ke VCC (5V) dan GND.
## Modifikasi program dengan menggabungkan antara UART dan I2C
``` c++
/// Memuat library yang digunakan
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
// Inisialisasi LCD pada alamat 0x27 dengan ukuran 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int pinPot = A0;
void setup() {
  // Inisialisasi komunikasi UART pada baud rate 9600
  Serial.begin(9600);
  // Inisialisasi komunikasi I2C untuk LCD
  lcd.init();
  lcd.backlight();
}
void loop() {
  // Membaca nilai analog dari potensiometer (0 - 1023)
  int nilaiADC = analogRead(pinPot);
  // Kalkulasi Voltase, Persentase, dan Panjang Bar
  float voltase = (nilaiADC * 5.0) / 1023.0;
  int persen = map(nilaiADC, 0, 1023, 0, 100);
  int panjangBar = map(nilaiADC, 0, 1023, 0, 16);
  // 1. Output ke UART (Serial Monitor)
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print("\tVolt: ");
  Serial.print(voltase);
  Serial.print(" V\tPersen: ");
  Serial.print(persen);
  Serial.println("%");
  // 2. Output ke I2C (LCD) baris atas Nilai ADC dan Persentase
  lcd.setCursor(0, 0);
  lcd.print("ADC: ");
  lcd.print(nilaiADC);
  lcd.print("    "); // Menghapus sisa karakter (padding)
  lcd.setCursor(11, 0); // Mengatur posisi agar persentase sejajar di kanan
  lcd.print(persen);
  lcd.print("%  "); // Menghapus sisa karakter
  // 3. Output ke I2C (LCD) baris bawah Bar Progress
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) {
      lcd.print((char)255); // Karakter ASCII blok penuh 
    } else {
      lcd.print(" ");       // Karakter kosong
    }
  }
  // Jeda pembacaan
  delay(200);
}
```
## Tabel Hasil

|ADC    |Volt (V)   |Persen (%) |
|---|---|---|
|1|0.00 V|0%|
|21|0.10 V|2%|
|49|0.24 V|4%|
|74|0.36 V|7%|
|96|0.47 V|9%|
