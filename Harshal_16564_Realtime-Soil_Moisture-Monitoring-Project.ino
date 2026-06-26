#define BLYNK_TEMPLATE_ID "TMPL3Eaz1RL4q"
#define BLYNK_TEMPLATE_NAME "soli mositure monitoring"
#define BLYNK_AUTH_TOKEN "X48l4SlY9nExTsL9--ARvvj9DsjGuTGs"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

#define SOIL_PIN 34
#define LED_PIN 2

BlynkTimer timer;
bool alertSent= false;

void sendSensor()
{
  int sensorValue = analogRead(SOIL_PIN);

  int moisture = map(sensorValue,0, 4095, 0, 100);

  Serial.print("Moisture: ");
  Serial.print(moisture);
  Serial.println("%");

  // Send moisture value to Blynk V0
  Serial.println("Sending to blynk");
  Blynk.virtualWrite(V1, moisture);

  // Alert
  if (moisture < 20) {
    digitalWrite(LED_PIN, HIGH);
    Blynk.virtualWrite(V2,1);
    if (!alertSent){
      Blynk.logEvent("moisture_low","Warning:Soil Moisture is below 20%!");
      alertSent = true;
    }
  } 
  else {
    digitalWrite(LED_PIN, LOW);
    Blynk.virtualWrite(V2,0);
    alertSent = false;
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Blynk Connected!");

  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}