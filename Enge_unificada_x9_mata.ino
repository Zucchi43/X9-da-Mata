#include <Adafruit_SleepyDog.h>
#include <Adafruit_MLX90614.h>
#include <Servo.h>
#include <Wire.h>

#define Servo_Pin 10
#define Buzzer_Pin 3

unsigned int Modo_Operacao = 0;
int pos_servo = 0;
float temperatura_alvo;
int dir_servo = 0, dir_alarme = 0, frequencia = 600;
int tempo = 0, tempo_alarme_max = 0;
int sleepMS;
Adafruit_MLX90614 Sensor_temp = Adafruit_MLX90614();
Servo Servo_motor;

void setup() {
  // put your setup code here, to run once:
Sensor_temp.begin();
Servo_motor.attach(Servo_Pin);
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
if(Modo_Operacao == 0) { //TASK DE PROCURAR ALGO
  if (dir_servo == 0) {
    Servo_motor.write(pos_servo++);
    delay(60);
  }
  else{
    Servo_motor.write(pos_servo--);
    delay(60);
  }
  temperatura_alvo = Sensor_temp.readObjectTempC();
  //Serial.println(temperatura_alvo);
  
  if (pos_servo >= 180) dir_servo = 1;
  if(pos_servo <= 0){
    dir_servo = 0;
    Modo_Operacao = 1;
  }
  
  if(temperatura_alvo > 40){
    Modo_Operacao = 2;
    tempo = millis();
    tempo_alarme_max = millis();
  }
  
}

if(Modo_Operacao == 1){
  sleepMS = Watchdog.sleep(10000);  // Sleep for up to 5 second.
  Modo_Operacao = 0;
}

if (Modo_Operacao == 2){
  if(millis() - tempo > 15){
    Serial.println("TA AQUI");
    if(dir_alarme == 0) {
      tone(Buzzer_Pin,frequencia);
      frequencia++;
    }
    else {
      tone(Buzzer_Pin,frequencia);
      frequencia--;
    }
    if(frequencia >= 800){
      dir_alarme = 1;
    }
    if(frequencia <= 650) dir_alarme = 0;
    tempo = millis();
  }
  if(millis() - tempo_alarme_max > 8000) {
    if ( Sensor_temp.readObjectTempC() < 40 || 1){
      Modo_Operacao = 0;
      frequencia = 700;
      noTone(Buzzer_Pin);
      tempo_alarme_max = millis();
    }
    else {
      Modo_Operacao = 2;
      tempo_alarme_max = millis();
    }
    
  }
}
Serial.print(Modo_Operacao);
Serial.print("         ");
Serial.println(temperatura_alvo);
}
