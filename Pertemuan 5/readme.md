# Percobaan 1
![alt text](<rtos-led.jpg>)
![alt text](<rtos-led.gif>)
## Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya
Ketiga task berjalan secara bergantian atau konkruen. Karena mikrokontroler yang umum digunakan pada Arduino (seperti keluarga AVR ATmega pada Uno atau Nano) memiliki prosesor single-core (satu inti). Artinya, prosesor secara fisik hanya bisa mengeksekusi satu baris instruksi pada satu waktu. Contoh yang dilakukannya adalah context switching dimana scheduler FreeRTOS membagi waktu CPU untuk setiap task dengan vTaskDelay() yang akan mengubah statusnya dari running menjadi blocked selama waktu yang ditentukan. Saat task ini diblokir, scheduler akan langsung mengambil alih CPU dan memberikannya kepada task lain yang berstatus ready.
## Bagaimana cara menambahkan task keempat? Jelaskan langkahnya
Pertama dengan deklarasi prototipe, membuat prototipe fungsi task di bagian atas program sebelum fungsi setup() (misalnya: void TaskBlink3( void *pvParameters );).
Lalu registrasi task dengan daftarkan task baru tersebut di dalam blok setup() menggunakan fungsi xTaskCreate(). Perlu untuk memasukkan argumen berupa nama fungsi, label (string), ukuran stack memori (misalnya 128), parameter NULL, tingkat prioritas, dan handle NULL.  
Terakhir adalah definisi fungsi dengan tulis isi fungsi task di bagian bawah program. Pastikan logika di dalam fungsi ini dibungkus dengan perulangan tak terbatas while(1) dan wajib mengandung fungsi blocking seperti vTaskDelay() agar tidak menggunakan CPU secara permanen.
## Modifikasilah program dengan menambah sensor (misalnya potensiometer), lalu gunakan nilainya untuk mengontrol kecepatan LED
![alt text](<rtos-led-potensio.gif>)

```C++
#include <Arduino_FreeRTOS.h>
void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );
void Taskprint( void *pvParameters );
// Pin untuk potensiometer
const int POT_PIN = A0; 
void setup() {
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL);
  xTaskCreate(Taskprint, "task3", 128, NULL, 1, NULL);
  vTaskStartscheduler();
}
void loop() {
  // Kosong. scheduler FreeRTOS mengambil alih eksekusi
}
void TaskBlink1(void *pvParameters) {
  pinMode(8, OUTPUT);
  while(1) {
    // 1. Baca nilai potensiometer (ADC 10-bit: 0 - 1023)
    int potValue = analogRead(POT_PIN);
    // 2. Mapping nilai ADC ke milidetik (Contoh: 50ms paling cepat, 1000ms paling lambat)
    int blinkDelay = map(potValue, 0, 1023, 50, 1000);

    Serial.println("Task1");
    digitalWrite(8, HIGH);
    // 3. Gunakan variabel blinkDelay untuk vTaskDelay
    vTaskDelay( blinkDelay / portTICK_PERIOD_MS );
    digitalWrite(8, LOW);
    vTaskDelay( blinkDelay / portTICK_PERIOD_MS );
  }
}
void TaskBlink2(void *pvParameters) {
  pinMode(7, OUTPUT);
  while(1) {
    // Membaca potensiometer yang sama untuk Task 2
    int potValue = analogRead(POT_PIN);
    int blinkDelay = map(potValue, 0, 1023, 50, 1000);

    Serial.println("Task2");
    digitalWrite(7, HIGH);
    vTaskDelay( blinkDelay / portTICK_PERIOD_MS );
    digitalWrite(7, LOW);
    // Rasio delay
    vTaskDelay( (blinkDelay * 2) / portTICK_PERIOD_MS );
  }
}
void Taskprint(void *pvParameters) {
  int counter = 0;
  while(1) {
    counter++;
    Serial.println(counter);
    vTaskDelay(500 / portTICK_PERIOD_MS); 
  }
}
```

# Percobaan 2
![alt text](<rtos-serial.png>)
![alt text](<rtos-serial.gif>)
## Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya
Kedua task tersebut yang dikonfigurasi dengan prioritas yang sama yaitu 0 berjalan dengan bergantian atau konkruen dimana task read_data menetapkan nilai statis x.temp = 54 dan x.h = 30 , lalu mengirimkannya ke antrean menggunakan xQueueSend. Setelah itu, task memanggil vTaskDelay(100).
## Apakah program ini berpotensi mengalami race condition? Jelaskan
Tidak akan terkena race kondition karena perpindahan data ditangani menggunakan FreeRTOS Queue (QueueHandle_t my_queue) yang dibuat dengan ukuran 1. Queue pada dasarnya sudah thread-safe (memiliki mekanisme penguncian internal pada level OS), sehingga operasi xQueueSend dan xQueueReceive dijamin tidak akan saling bertabrakan.
## Modifikasilah program dengan menggunakan sensor DHT sesungguhnya sehingga informasi yang ditampilkan dinamis.
``` C++
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include "DHT.h"
// Konfigurasi Pin dan Tipe Sensor DHT
#define DHTPIN 2        // Hubungkan pin data DHT ke Pin 2 Digital
#define DHTTYPE DHT11   // Ubah menjadi DHT22
DHT dht(DHTPIN, DHTTYPE);
// Struktur data yang dikirimkan via Queue (diubah ke float untuk presisi desimal DHT)
struct readings {
  float temp;
  float h;
};
QueueHandle_t my_queue;
// Prototipe fungsi
void read_data(void *pvParameters);
void display(void *pvParameters);

void setup() {
  Serial.begin(9600);
  dht.begin(); // Inisialisasi sensor DHT
  // Membuat queue dengan kapasitas 1 elemen bertipe struct readings
  my_queue = xQueueCreate(1, sizeof(struct readings));
  // Membuat task
  xTaskCreate(read_data, "read sensors", 128, NULL, 0, NULL);
  xTaskCreate(display, "display", 128, NULL, 0, NULL);
}
void loop() {
  // Kosong karena menggunakan FreeRTOS scheduler
}
void read_data(void *pvParameters) {
  struct readings x;
  for(;;) {
    // Sensor DHT membutuhkan jeda setidaknya 2 detik (2000ms) antar pembacaan data
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    // Membaca suhu dalam Celcius dan kelembapan
    x.temp = dht.readTemperature();
    x.h = dht.readHumidity();
    // Mengirim data hasil pembacaan ke Queue
    xQueueSend(my_queue, &x, portMAX_DELAY);
  }
}
void display(void *pvParameters) {
  struct readings x;
  for(;;) {
    // Menerima data dari Queue (Task ini akan selalu berstatus 'Blocked' hingga ada data yang masuk)
    if(xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      // Validasi pembacaan sensor (menghindari hasil Not a Number / NaN)
      if (isnan(x.temp) || isnan(x.h)) {
        Serial.println("Gagal membaca dari sensor DHT!");
      } else {
        Serial.print("Suhu = ");
        Serial.print(x.temp);
        Serial.println(" *C");
        
        Serial.print("Kelembapan = ");
        Serial.print(x.h);
        Serial.println(" %");
        Serial.println("--------------------");
      }
    }
  }
}
```
