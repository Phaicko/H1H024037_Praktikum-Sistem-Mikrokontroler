# Percobaan 1
![alt text](<adc-servo.jpg>)
![alt text](<adc-servo.gif>)
## Apa fungsi perintah analogRead() pada rangkaian praktikum?
Membaca input nilai tegangan analog yang masuk ke salah satu pin analog mikrokontroler (A0 - A5) yang akan diubah menjadi digital pada mikrokontroler untuk diproses.
## Mengapa diperlukan fungsi map() dalam program tersebut?
Untuk mengatur batasan perbedaan rentang nilai input ADC (0 - 1023) dan batasan output pada servo (0 - 255).
## Modifikasi program berikut agar servo hanya bergerak dalam rentang 30° hingga 150°, meskipun potensiometer tetap memiliki rentang ADC 0–1023.
``` C++
#include <Servo.h> // library untuk servo motor
Servo myservo; // membuat objek servo

// ===================== PIN SETUP =====================
// Tentukan pin yang digunakan untuk potensiometer dan servo
const int potensioPin = A0;   // pin analog input (contoh A0)
const int servoPin = 3;      // pin digital untuk servo (PWM)

// ===================== VARIABEL =====================
// Variabel untuk menyimpan data ADC dan sudut servo
int pos = 0; // tipe data dan inisialisasi awal
int val = 0; // tipe data dan inisialisasi awal
void setup() {
  // Hubungkan servo ke pin yang sudah ditentukan
  myservo.attach(servoPin); // servoPin

  // Aktifkan komunikasi serial untuk monitoring
  Serial.begin(9600); // baud rate (contoh 9600)

}
void loop() {
  // ===================== PEMBACAAN ADC =====================
  // Baca nilai dari potensiometer (rentang 0–1023)
  val = analogRead(potensioPin); // potensioPin

  // ===================== KONVERSI DATA =====================
  // Ubah nilai ADC menjadi sudut servo (0–180 derajat)
  pos = map(val,
             0,   // nilai minimum ADC
             1023,   // nilai maksimum ADC
             30,   // sudut minimum servo 30
             150);  // sudut maksimum servo 150

  // ===================== OUTPUT SERVO =====================
  // Gerakkan servo sesuai hasil mapping
  myservo.write(pos); // variabel sudut

  // ===================== MONITORING DATA =====================
  // Tampilkan data ADC dan sudut servo ke Serial Monitor
  Serial.print("ADC Potensio: ");
  Serial.print(val); // variabel ADC

  Serial.print(" | Sudut Servo: ");
  Serial.println(pos); // variabel sudut

  // ===================== STABILISASI =====================
  // Delay untuk memberi waktu servo bergerak stabil
  delay(100); // isi dalam milidetik
}
```

# Percobaan 2
![alt text](<adc-led.jpg>)
![alt text](<adc-led.gif>)
## Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi analogWrite()!
Karena pengaturan kecerahan lampu LED menggunakann teknik PWM (Pulse Width Modulation) karena pin digital tidak bisa mengatur tegangan analog murni.
## Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?
Hubungannya adalah proporsionalitas linear perbedaan antara perbedaan rentang nilai kemampuan perangkat keras di dalam mikrokontroler Arduino dimana rentang nilai pembacaan sensor (ADC) persis empat kali lebih besar daripada rentang nilai keluaran (PWM).
## Modifikasilah program berikut agar LED hanya menyala pada rentang kecerahan sedang, yaitu hanya ketika nilai PWM berada pada rentang 50 sampai 200.
``` C++
// ===================== PIN SETUP =====================
// Tentukan pin yang digunakan untuk potensiometer dan LED PWM
const int potPin = A0;   // dengan pin analog (contoh A0)
const int ledPin = 9;   // dengan pin digital PWM (contoh 9)
// ===================== VARIABEL =====================
// Variabel untuk menyimpan hasil pembacaan dan konversi PWM
int nilaiADC = 0;  // dengan nilai awal (default 0)
int pwm = 0;       // dengan nilai awal (default 0)
void setup() {
  // ===================== OUTPUT SETUP =====================
  // Atur pin LED sebagai output
  pinMode(ledPin, 9);

  // ===================== SERIAL MONITOR =====================
  // Aktifkan komunikasi serial untuk melihat data pembacaan
  Serial.begin(9600); // isi baud rate (contoh 9600)
}

void loop() {
  // ===================== PEMBACAAN SENSOR =====================
  // Baca nilai analog dari potensiometer (rentang 0–1023)
  nilaiADC = analogRead(potPin); // dengan potPin

  // ===================== PEMROSESAN DATA (SCALING) =====================
  // Ubah nilai ADC (0–1023) menjadi nilai PWM (0–255)
  pwm = map(nilaiADC,
            0,   // nilai minimum ADC
            1023,   // nilai maksimum ADC
            0,   // PWM minimum
            255);  // PWM maksimum

  // ===================== OUTPUT PWM =====================
  // Kirim sinyal PWM ke LED (mengatur kecerahan)
  analogWrite(ledPin, pwm); // isi dengan variabel PWM
  // ===================== OUTPUT PWM =====================
  // Cek apakah nilai PWM berada di rentang 50 hingga 200
  if (pwm <= 50 || pwm >= 200) {
    // Jika PWM dibawah 50 atau diatas 200, matikan LED
    analogWrite(ledPin, 0); 
  } else {
    // Jika tidak, menyalakan LED sesuai nilai PWM
    analogWrite(ledPin, pwm);   
  }
  // ===================== MONITORING DATA =====================
  // Tampilkan data ADC dan PWM ke Serial Monitor
  Serial.print("ADC: ");
  Serial.print(nilaiADC); // variabel ADC pada Serial Monitor

  Serial.print(" | PWM: ");
  Serial.println(pwm); // variabel PWM pada Serial Monitor

  // ===================== STABILISASI SISTEM =====================
  // Delay untuk menstabilkan pembacaan dan tampilan data
  delay(100); // isi dalam milidetik (contoh 50)
}
```
