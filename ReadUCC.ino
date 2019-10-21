/*
 Программа измерения внутреннего опорного напряжения
 Дата создания: 12.08.2019
 Автор: Субботин А.С.
 Версия 1.0.0
 
 */
//==============================================================
// Подключение библиотек
//==============================================================

//==============================================================
// Константы
//==============================================================
#define DELAY_OUTPUT 1000

//==============================================================
//------------------------- АППАРАТНАЯ КОНФИГУРАЦИЯ ------------
//==============================================================
#define SERIAL_SPEED 115200    //Скорость передачи по каналу последовательного порта


//==============================================================
// Глобальные переменные
//==============================================================
long currentMillis;
long previousMillis;

//==============================================================
// Программа начальной установки
//==============================================================
void setup() 
{
 Serial.begin(SERIAL_SPEED);
 Serial.println("Read UCC programm");

 currentMillis = millis();
 previousMillis = currentMillis;
 
}//End of void setup()

//==============================================================
// Главный цикл пограммы
//==============================================================
void loop() 
{
  currentMillis = millis();
  if(currentMillis - previousMillis > DELAY_OUTPUT)
  {
    previousMillis = currentMillis;

    Serial.println("Ucc = " + String(readUcc()));
  }
}//End of void loop()

//==============================================================
// Подпрограммы
//==============================================================

long readUcc()
{
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
      ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined(__AVR_ATiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
      ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined(__AVR_ATiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
      ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
      ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif

  delay(75);//Wait for Uref to set
  ADCSRA |= _BV(ADSC);//Start conversion

  while (bit_is_set(ADCSRA, ADSC));//Measuring

  uint8_t low = ADCL;
  uint8_t high = ADCH;
  
  long result = (high << 8) | low;

  result = 1125300L / result;//Calculate Ucc (in mv) 
  
  return result;
          
}//End of long readUcc()

