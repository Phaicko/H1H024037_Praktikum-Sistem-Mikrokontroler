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
  
  vTaskStartScheduler();
}

void loop() {
  // Kosong. Scheduler FreeRTOS mengambil alih eksekusi
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
    
    // Anda bisa membuat rasio delay yang berbeda di sini jika diperlukan
    // Misalnya, waktu off lebih lama dari waktu on
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