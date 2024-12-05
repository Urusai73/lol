#include <microDS18B20.h>
#include <UIPEthernet.h>

// Задаем сетевые параметры.
byte mac[] = { 0xDE, 0x05, 0xB6, 0x27, 0x39, 0x19 }; //  MAC
byte ip[] = { 10, 98, 90, 200 }; // IP address 
String readString = String(20);
byte addr[8];

#define DS_PIN A2                  // пин передачи данных
EthernetServer server(80);       //  порт web страницы

// Присвоение ID датчика переменного адреса.
// s1_addr - переменная датчика
// 0x28, 0x31, 0x61, 0x81, 0xE3, 0xE0, 0x3C, 0x5  - ID датчика
uint8_t s1_addr[8] = { 0x28, 0x31, 0x61, 0x81, 0xE3, 0xE0, 0x3C, 0x5 };
uint8_t s2_addr[8] = { 0x28, 0x67, 0x72, 0x81, 0xE3, 0xD6, 0x3C, 0x2C };
uint8_t s3_addr[8] = { 0x28, 0xE9, 0x16, 0x57, 0x4, 0xE1, 0x3C, 0x2D };
uint8_t s4_addr[8] = { 0x28, 0xF4, 0xDB, 0x81, 0xE3, 0xAE, 0x3C, 0xC4 };
uint8_t s5_addr[8] = { 0x28, 0x50, 0x39, 0x57, 0x4, 0xE1, 0x3C, 0x2B };
uint8_t s6_addr[8] = { 0x28, 0x53, 0x46, 0x81, 0xE3, 0x4F, 0x3C, 0xF7 };
uint8_t s7_addr[8] = { 0x28, 0x61, 0xF3, 0x81, 0xE3, 0xC4, 0x3C, 0xE5 };

// Задаем переменную полученной информации с пина и датчика.
MicroDS18B20<DS_PIN, s1_addr> sensor1;
MicroDS18B20<DS_PIN, s2_addr> sensor2;
MicroDS18B20<DS_PIN, s3_addr> sensor3;
MicroDS18B20<DS_PIN, s4_addr> sensor4;
MicroDS18B20<DS_PIN, s5_addr> sensor5;
MicroDS18B20<DS_PIN, s6_addr> sensor6;
MicroDS18B20<DS_PIN, s7_addr> sensor7;

int val = 0;

void setup()
{
  // Запускаем соеденение и сервер
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);

  //Плата сообшает в com свой ip
  Serial.print(" IP server ");
  Serial.println(Ethernet.localIP());
}
void loop()
{
  // Прослушивание входящих клиентов http
  EthernetClient client = server.available();
  if (client) {

    // http-запрос заканчивается пустой строкой
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        // Ждем завершения get запроса и отправляем ответ http
        if (c == '\n' && currentLineIsBlank) {

          // отправляеv ответ браузеру
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: ext/plain; version=0.0.4; charset=utf-8");
          client.println("Connnection: close");

          // Запрашиваем опрос температуры с датчиков
          sensor1.requestTemp();
          sensor2.requestTemp();
          sensor3.requestTemp();
          sensor4.requestTemp();
          sensor5.requestTemp();
          sensor6.requestTemp();
          sensor7.requestTemp();

          // промежутки в секундах между опросами датчиков
          delay(1000);

          // Создае переменные для обработки http страницы
          float celsius1;
          float celsius2;
          float celsius3;
          float celsius4;
          float celsius5;
          float celsius6;
          float celsius7;

          // Присваиваем переменной http страницы действие по опросу датчика.
          celsius1 = sensor1.getTemp();
          celsius2 = sensor2.getTemp();
          celsius3 = sensor3.getTemp();
          celsius4 = sensor4.getTemp();
          celsius5 = sensor5.getTemp();
          celsius6 = sensor6.getTemp();
          celsius7 = sensor7.getTemp();
          
          // Собираем страницу http
          client.print("data_center_temp{sensor=\"1\"} " + String(celsius1) + "\n");
          client.print("data_center_temp{sensor=\"2\"} " + String(celsius2) + "\n");
          client.print("data_center_temp{sensor=\"3\"} " + String(celsius3) + "\n");
          client.print("data_center_temp{sensor=\"4\"} " + String(celsius4) + "\n");
          client.print("data_center_temp{sensor=\"5\"} " + String(celsius5) + "\n");
          client.print("data_center_temp{sensor=\"6\"} " + String(celsius6) + "\n");
          client.print("data_center_temp{sensor=\"7\"} " + String(celsius7) + "\n");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
        if (Serial.available() > 0) {
          val = Serial.read();
          if (val=='1') Serial.println(sensor1.getTemp());  // Выводит температуру 1 датчика при отправке  com port цифры 1
          if (val=='2') Serial.println(sensor2.getTemp());  // Выводит температуру 2 датчика при отправке  com port цифры 2
          if (val=='3') Serial.println(sensor3.getTemp());  // Выводит температуру 3 датчика при отправке  com port цифры 3
          if (val=='4') Serial.println(sensor4.getTemp());  // Выводит температуру 4 датчика при отправке  com port цифры 4
          if (val=='5') Serial.println(sensor5.getTemp());  // Выводит температуру 5 датчика при отправке  com port цифры 5
          if (val=='6') Serial.println(sensor6.getTemp());  // Выводит температуру 6 датчика при отправке  com port цифры 6
          if (val=='7') Serial.println(sensor7.getTemp());  // Выводит температуру 7 датчика при отправке  com port цифры 7
        }
      }
    }
  }
  // Ждем ХХ секунд для получения данных
  delay(10);
  // закрыть соединение
  client.stop();
}
