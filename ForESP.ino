void setup() {
  Serial.begin(9600);  // Начать прием данных через Serial порт
}

void loop() {
  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n');  // Прочитать данные до символа новой строки
    
    // Разделить строку на подстроки, используя разделитель "_"
    char *token = strtok((char *)receivedData.c_str(), "_");
    if (token != NULL) {
      // Первое значение - температура
      int temperature = atoi(token);
      
      // Перейти к следующей подстроке
      token = strtok(NULL, "_");
      
      // Второе значение - влажность
      int humidity = atoi(token);
      
      // И так далее для других значений
      // В этом примере, предполагается, что все значения являются целыми числами
      // Если есть нецелые числа, используйте функцию atof() для преобразования в float
      
      // Теперь у вас есть доступ к отдельным данным: temperature, humidity и так далее
      Serial.print("Temperature: ");
      Serial.println(temperature);
      Serial.print("Humidity: ");
      Serial.println(humidity);
    }
  }
}
