#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

#define BUZZER_PIN 8
#define LED_VERDE 11
#define LED_AMARELO 10
#define LED_VERMELHO 9

#define NIVEAU_PIN A0
#define CHUVA_PIN A1

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 5000;

void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
}

void loop() {
  if (millis() - lastUpdate > updateInterval) {
    lastUpdate = millis();

    int nivelAgua = analogRead(NIVEAU_PIN) / 10;
    int chuva = analogRead(CHUVA_PIN);
    float umidade = dht.readHumidity();
    if (isnan(umidade)) umidade = 50;

    int risco = 0;
    if (nivelAgua > 70 || chuva > 400) risco = 2;
    else if (nivelAgua > 40 || chuva > 250) risco = 1;

    // Controle de LED
    digitalWrite(LED_VERDE, risco == 0 ? HIGH : LOW);
    digitalWrite(LED_AMARELO, risco == 1 ? HIGH : LOW);
    digitalWrite(LED_VERMELHO, risco == 2 ? HIGH : LOW);

    // Controle do buzzer
    if (risco == 2) tone(BUZZER_PIN, 1000);
    else noTone(BUZZER_PIN);

    // LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    if (risco == 2) lcd.print("ALERTA VERMELHO");
    else if (risco == 1) lcd.print("Alerta Amarelo ");
    else lcd.print("Risco Baixo     ");

    lcd.setCursor(0, 1);
    lcd.print("N:");
    lcd.print(nivelAgua);
    lcd.print("cm I:");
    lcd.print(chuva / 10);
    lcd.print("%");

    // Serial (simulando envio de dados)
    Serial.print("Dados Enviados --- ");
    Serial.print("Nivel da Agua:");
    Serial.print(nivelAgua);
    Serial.print("cm, Intensidade da Chuva:");
    Serial.print(chuva);
    Serial.print("%, Umidade:");
    Serial.print(umidade);
    Serial.print("%, Risco:");
    Serial.println(risco);
  }
}

