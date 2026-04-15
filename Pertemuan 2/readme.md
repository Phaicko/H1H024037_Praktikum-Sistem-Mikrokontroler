# Percobaan 1
![alt text](<gif percobaan 1.gif>)
## Gambarkan rangkaian schematic yang digunakan pada percobaan!
![alt text](<schematic percobaan 1.png>)
## Apa yang terjadi jika nilai num lebih dari 15?
Jika nilai num lebih dari 15 (misalnya 16, 17, dst.), program akan memanggil indeks yang berada di luar batas array atau digitPattern.
## Apakah program ini menggunakan common cathode atau common anode? Jelaskan alasanya!
Program ini menggunakan Common Anode (CA). Pada fungsi displayDigit, terdapat perintah digitalWrite(segmentPins[i], !digitPattern[num][i]);. Di dalam array digitPattern, angka 1 menandakan segmen yang "aktif". Namun, pada tipe Common Anode, segmen akan menyala jika diberi logika LOW (0). Oleh karena itu, kode menggunakan operator negasi/NOT (!) untuk membalik nilai 1 (HIGH) menjadi 0 (LOW) agar segmen tersebut benar-benar menyala.
## Modifikasi program agar tampilan berjalan dari F ke 0 dan berikan penjelasan disetiap baris
``` C++
##include <Arduino.h> // Memasukkan library Arduino
// Mendefinisikan nomor pin Arduino yang terhubung ke segmen: a, b, c, d, e, f, g, dp
const int segmentPins[8] = {7, 6, 5, 10, 11, 8, 9, 4};
// Mendeklarasikan array 2D untuk menyimpan bit pola nyala lampu 0-9 dan A-F
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, // Pola untuk karakter 0
  {0,1,1,0,0,0,0,0}, // Pola untuk karakter 1
  {1,1,0,1,1,0,1,0}, // Pola untuk karakter 2
  {1,1,1,1,0,0,1,0}, // Pola untuk karakter 3 
  {0,1,1,0,0,1,1,0}, // Pola untuk karakter 4
  {1,0,1,1,0,1,1,0}, // Pola untuk karakter 5
  {1,0,1,1,1,1,1,0}, // Pola untuk karakter 6
  {1,1,1,0,0,0,0,0}, // Pola untuk karakter 7
  {1,1,1,1,1,1,1,0}, // Pola untuk karakter 8
  {1,1,1,1,0,1,1,0}, // Pola untuk karakter 9
  {1,1,1,0,1,1,1,0}, // Pola untuk karakter A
  {0,0,1,1,1,1,1,0}, // Pola untuk karakter b
  {1,0,0,1,1,1,0,0}, // Pola untuk karakter C
  {0,1,1,1,1,0,1,0}, // Pola untuk karakter d
  {1,0,0,1,1,1,1,0}, // Pola untuk karakter E
  {1,0,0,0,1,1,1,0}  // Pola untuk karakter F
};
void displayDigit(int num) // Fungsi untuk menampilkan digit pada 7-segment berdasarkan angka yang dilempar (num)
{
  for(int i=0; i<8; i++) // Melakukan perulangan 8 kali untuk ke-8 pin segmen
  {
    // Mengirim sinyal ke pin. Operator '!' digunakan untuk membalik logika (1 jadi 0, 0 jadi 1).
    // Hal ini karena Common Anode membutuhkan sinyal LOW (0) agar lampu menyala.
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}
void setup() // Fungsi inisialisasi yang dijalankan satu kali saat Arduino pertama kali dinyalakan
{
  for(int i=0; i<8; i++) // Melakukan perulangan ke-8 pin
  {
    pinMode(segmentPins[i], OUTPUT); // Mengatur setiap pin segmen sebagai OUTPUT
  }
}
void loop() // Fungsi looping yang akan berjalan secara terus-menerus berulang kali
{
  // Melakukan perulangan hitung mundur, dimulai dari indeks 15 (huruf F) hingga 0
  for(int i=15; i>=0; i--)
  {
    displayDigit(i); // Memanggil fungsi displayDigit untuk menampilkan nilai 'i' saat ini
    delay(1000);     // Memberikan jeda waktu 1000 milidetik (1 detik) sebelum berganti ke angka berikutnya
  }
}
```
# Percobaan 2
![alt text](<gif percobaan 2.gif>)
## Gambarkan rangkaian schematic yang digunakan pada percobaan!
![alt text](<schematic percobaan 2.png>)
## Mengapa pada push button digunakan mode INPUT_PULLUP pada Arduino Uno? Apa keuntungannya dibandingkan rangkaian biasa?
Pin mode INPUT_PULLUP digunakan untuk mengaktifkan resistor pull-up internal yang bernilai sekitar 20k hingga 50k Ω (Ohm) yang sudah ada di dalam mikrokontroler (untuk kasus ini adalah ATmega)
## Jika salah satu LED segmen tidak menyala, apa saja kemungkinan penyebabnya dari sisi hardware maupun software?
Jika satu segmen (contohnya hanya pada segment 'b' atau 'g') tidak menyala, masalahnya bisa terjadi di sisi hardware seperti pin pada Arduino rusak atau kabel jumper yang rusak atau pada bagian software seperti terdapat typo dalam code atau salah mendeklarasikan pin.
## Modifikasi rangkaian dan program dengan dua push button yang berfungsi sebagai penambahan (increment) dan pengurangan (decrement) pada sistem counter dan berikan penjelasan disetiap baris
``` C++
##include <Arduino.h> // Memasukkan library Arduino
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4}; // Mendeklarasikan array konstan berukuran 8 untuk menyimpan pin Arduino (urutan: a, b, c, d, e, f, g)
// Mendeklarasikan pin untuk tombol Increment (Tambah) di pin 3
const int buttonIncPin = 3; 
// Mendeklarasikan pin untuk tombol Decrement (Kurang) di pin 2
const int buttonDecPin = 2; 
// Membuat variabel integer bernama 'counter' dengan nilai awal 0
int counter = 0;
// Menyimpan status tombol Increment sebelumnya, default HIGH (karena PULLUP)
bool lastIncState = HIGH;
// Menyimpan status tombol Decrement sebelumnya, default HIGH
bool lastDecState = HIGH;
// Array 2 dimensi untuk menyimpan pola nyala/mati segmen (1=nyala, 0=mati). Pola ini memuat 16 indeks: Angka 0-9 dan Huruf A-F.
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, // Indeks 0: Angka 0
  {0,1,1,0,0,0,0,0}, // Indeks 1: Angka 1
  {1,1,0,1,1,0,1,0}, // Indeks 2: Angka 2
  {1,1,1,1,0,0,1,0}, // Indeks 3: Angka 3
  {0,1,1,0,0,1,1,0}, // Indeks 4: Angka 4
  {1,0,1,1,0,1,1,0}, // Indeks 5: Angka 5
  {1,0,1,1,1,1,1,0}, // Indeks 6: Angka 6
  {1,1,1,0,0,0,0,0}, // Indeks 7: Angka 7
  {1,1,1,1,1,1,1,0}, // Indeks 8: Angka 8
  {1,1,1,1,0,1,1,0}, // Indeks 9: Angka 9
  {1,1,1,0,1,1,1,0}, // Indeks 10: Huruf A
  {0,0,1,1,1,1,1,0}, // Indeks 11: Huruf b
  {1,0,0,1,1,1,0,0}, // Indeks 12: Huruf C
  {0,1,1,1,1,0,1,0}, // Indeks 13: Huruf d
  {1,0,0,1,1,1,1,0}, // Indeks 14: Huruf E
  {1,0,0,0,1,1,1,0}, // Indeks 15: Huruf F
};
// Fungsi khusus untuk menampilkan angka di 7-segment berdasarkan nomor indeks
void displayDigit(int num) {
  // Looping dari 0 sampai 7 untuk mengontrol 8 pin segmen
  for(int i=0; i<8; i++) {
    // Menuliskan logika ke pin Arduino. Tanda '!' membalik logika (1 jadi 0, 0 jadi 1).
    // Asumsinya 7-segment jenis Common Anode (butuh LOW untuk menyala).
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}
// Fungsi setup yang berjalan hanya satu kali saat Arduino dinyalakan
void setup() {
  // Looping untuk menyetel kedelapan pin segmen
  for(int i=0; i<8; i++) {
    // Menetapkan setiap pin di segmentPins sebagai OUTPUT (keluaran sinyal)
    pinMode(segmentPins[i], OUTPUT);
  }
  // Menetapkan pin tombol Increment sebagai input dengan resistor pull-up internal aktif
  pinMode(buttonIncPin, INPUT_PULLUP);
  // Menetapkan pin tombol Decrement sebagai input dengan resistor pull-up internal aktif
  pinMode(buttonDecPin, INPUT_PULLUP);
  // Menampilkan angka awal (0) di 7-segment saat pertama menyala
  displayDigit(counter);
}
// Fungsi loop yang berjalan terus-menerus
void loop() {
  // Membaca status logika saat ini dari pin tombol Increment (HIGH/LOW)
  bool currentIncState = digitalRead(buttonIncPin);
  // Membaca status logika saat ini dari pin tombol Decrement (HIGH/LOW)
  bool currentDecState = digitalRead(buttonDecPin);
  // Mengecek apakah tombol Increment ditekan (transisi dari HIGH ke LOW)
  if (lastIncState == HIGH && currentIncState == LOW) {
    // Menambah nilai counter sebanyak 1
    counter++;
    // Jika counter melebihi batas indeks tertinggi (16), kembalikan nilainya ke 0 (reset/rollover)
    if(counter > 16) counter = 0;
    // Perbarui tampilan 7-segment dengan nilai counter yang baru
    displayDigit(counter);
    // Beri jeda 200 milidetik sebagai debounce untuk mencegah pembacaan tombol ganda akibat getaran mekanis
    delay(200); 
  }
  // Mengecek apakah tombol Decrement ditekan (transisi dari HIGH ke LOW)
  if (lastDecState == HIGH && currentDecState == LOW) {
    // Mengurangi nilai counter sebanyak 1
    counter--;
    // Jika counter kurang dari 0 (negatif), kembalikan ke batas tertinggi (16) (rollover mundur)
    if(counter < 0) counter = 16;
    // Perbarui tampilan 7-segment dengan nilai counter yang baru
    displayDigit(counter);
    // Beri jeda 200 milidetik sebagai debounce
    delay(200);
  }
  // Menyimpan status tombol Increment saat ini sebagai acuan untuk putaran loop berikutnya
  lastIncState = currentIncState;
  // Menyimpan status tombol Decrement saat ini sebagai acuan untuk putaran loop berikutnya
  lastDecState = currentDecState;
}
```
