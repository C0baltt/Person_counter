/*
  Подключения к пинам Arduino:
  Кнопки: d2, d3;
  реле: d5, d6
  //================================  -       ==>   -
*/
#include <Wire.h>

#include <SoftwareSerial.h>// Подключаем библиотеку SoftwareSerial.h

//==============================================
#define LIGHT_PIN 7          // определяем пин освещения
#define SensorRight_pin 8            // определяем пин правого сенсора
#define SensorLeft_pin 9            // определяем пин левого сенсора
#define LEDSensorRight 10        // определяем пин диода правого сенсора =>
// =>(для визуального контроля обработки сенсора ардуиной)
#define LEDSensorLeft 11        // определяем пин диода левого сенсора=>
// =>(для визуального контроля обработки сенсора ардуиной)
#define power_pin 12            // подача питания на сенсоры

//======================== Переменные ======================
boolean light;//переменная, хранящая состояние освещения (вкл/выкл)

boolean flagActivateSensorRight, flagActivateSensorLeft;//флаг, показывающий, что сенсор активирован
boolean flagCount; //флаг, позволяющий обратиться к счетчику человек 1 раз с момента=>
//=> активации обоих сенсоров (исключает избыточные срабатывания)
unsigned long activateTimeSensorRight, activateTimeSensorLeft;//точное время срабатывания сенсоров
int countActivateSensorRight, countActivateSensorLeft;//счетчик активаций сенсоров
int countPerson;//количество человек в комнате

boolean flag_butt_set;
//=======================================================================================
void setup() {
 Serial.begin(9600);
  Serial.println("Serial.begin");
  
  //==============================================
  pinMode(LIGHT_PIN, OUTPUT);          // пин освещения как выход
  pinMode(SensorRight_pin, INPUT);         // пин сенсора как вход
  pinMode(SensorLeft_pin, INPUT);         // пин сенсора как вход
  pinMode(LEDSensorRight, OUTPUT);          // пин диода как выход
  pinMode(LEDSensorLeft, OUTPUT);          // пин диода как выход
  pinMode(power_pin, OUTPUT);
}
//=======================================================================================
void loop() {
  sensorControl();//вызов функции контроля сенсоров
  lightControl();     //вызов функции контроля освещения
}
//====================================== КОНЕЦ LOOP =========================================================================================
//===============================================================================================================================
//================================= ФУНКЦИИ =======================================================================================
void lightControl() { //управляет освещением, на основании=>
  //=>количества человек, вошедших в помещение
  if (countPerson > 0) {
    //analogRead(pin);
    digitalWrite(LIGHT_PIN, HIGH); // если кто-то вошел в комнату, включаем свет
  } else {
    digitalWrite(LIGHT_PIN, LOW); // если никого нет, выключает свет
  }
}
//===============================================================================================================================
void sensorControl() {//функция, опрашивающая датчики движения,
  //и на основании их показаний определяет зашел человек
  //или вышел
  if (digitalRead(SensorRight_pin) == 1 && flagActivateSensorRight == 0) {//если=>
    //=> сработал сенсор, но флаг активации не поднят,то=>
    activateTimeSensorRight = millis();//=>значит сенсор сработал только что=>
    flagActivateSensorRight = 1;//=>записываем время срабатывания и устанавливаем 
    //=>флаг активации сенсора
  }
  if (digitalRead(SensorLeft_pin) == 1 && flagActivateSensorLeft == 0) {//если
    //=> сработал сенсор, но флаг активации не поднят,то
    activateTimeSensorLeft = millis();//значит сенсор сработал только что
    //=> записываем время срабатывания
    flagActivateSensorLeft = 1;//устанавливаем флаг активации сенсора
  }
  if  (flagCount == 0) {     //((millis() - activateTimeSensorLeft > 1000) && ( millis() - activateTimeSensorRight > 1000)) 
    if (flagActivateSensorLeft == 1 && flagActivateSensorRight == 1) {//если
      //=> флаг активации сенсоров 1 и 2 установлен, то=>
      flagCount = 1;//=>устанавливаем флаг срабатывания двух сенсоров=>
      if (activateTimeSensorRight > activateTimeSensorLeft) {//=>сравниваем время
        //=> активации сенсоров: если 1-ый сработал раньше,то=>
        countPerson++;//=>это значит, что человек вошел, увеличиваем счетчик=>
        //=> на 1
        countActivateSensorRight++;//счетчик активации правого сенсора
        //digitalWrite(SensorRight_pin, HIGH);
      } else {//иначе - человек вышел
        if (countPerson > 0) countPerson--;//если счетчик больше 0 
        //=> отнимаем 1(такая проверка введена чтобы не стало отрицательным=>
        //=>количество человек, т.к. переменная безнакового типа, )
        countActivateSensorLeft++;//счетчик активации левого сенсора
      }
    }
    //digitalWrite(power_pin, LOW);//подача питания на сенсоры
  }
  
  if (digitalRead(SensorRight_pin) == 0 && flagActivateSensorRight == 1) {//если сигнал с сенсора не приходит,
    flagActivateSensorRight = 0;// значит сенсор отключен, сбрасываем флаг активации
  }
  if (digitalRead(SensorLeft_pin) == 0 && flagActivateSensorLeft == 1) {//если сигнал с сенсора не приходит,
    flagActivateSensorLeft = 0;//значит сенсор отключен, сбрасываем флаг активации
  }

  if (flagActivateSensorRight == 0 && flagActivateSensorLeft == 0) {
    flagCount = 0;//если сенсоры не активны, то сбрасываем флаг активации обоих сенсоров
  }

  digitalWrite(LEDSensorRight, flagActivateSensorRight);//присваиваем диоду состояние датчика
  digitalWrite(LEDSensorLeft, flagActivateSensorLeft);//присваиваем диоду состояние датчика
}
//===============================================================================================================================
