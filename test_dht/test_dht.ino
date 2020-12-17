#include <DHT.h>
#define dhtPin 8
#define dhtType DHT22

DHT dht(dhtPin, dhtType);
float t, h, t_media, h_media;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  t_media = 0;
  h_media = 0;
  for(byte i=0; i<30; i++)
  {
    delay(2000); //minimum measure time for this sensor
    t = dht.readTemperature();
    h = dht.readHumidity();
    //Serial.println(t); //uncomment this with you want to visualize every measure
    //Serial.println(h); //uncomment this with you want to visualize every measure
    t_media += t;
    h_media += h;
    //Serial.println(t_media); //uncomment this with you want to visualize the sum of measures
    //Serial.println(h_media); //uncomment this with you want to visualize the sum of measures
    //Serial.println(i); //uncomment this with you want to see the number of iteration
  }
  t_media = t_media/30;
  h_media = h_media/30;
  Serial.print("Temperatura média: ");
  Serial.println(t_media);
  Serial.print("Umidade média: ");
  Serial.println(h_media);
}
