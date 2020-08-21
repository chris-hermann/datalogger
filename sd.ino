#include<SD.h>
#include<SPI.h>
#include<stdlib.h> 
#include<DHT.h>
#include<SoftwareSerial.h>
#include<TinyGPS.h>


#define DHTPIN 8
#define DHTTYPE DHT22
#define GPS_RX 3
#define GPS_TX 4
SoftwareSerial ss(GPS_TX, GPS_RX);
TinyGPS gps;
DHT dht(DHTPIN, DHTTYPE);
File myFile;
const int CS = 10;
char temp_ar[8], umid_ar[8], w_lat[16], w_lon[16], w_data[16], w_hora[16], w_realfeel[8];
float celsius, humidity;
float f_lat, f_lon, valor_umidsolo;
int ano, PIN_SOLO = A0, read_umidsolo;
byte mes, dia, hora, minuto, seg;
bool newData = false;


void setup() 
{
  Serial.begin(9600);
  pinMode(CS, OUTPUT);
  dht.begin();
  ss.begin(9600);
  
  if(!SD.begin(CS))
  {
    Serial.println("Falha na inicialização do SD!");
    while(true);
  }
  
  Serial.println("SD inicializado com sucesso!");
  myFile = SD.open("test.txt", FILE_WRITE);
  myFile.println("Data;Hora;Temperatura do Ar(C);Umidade do Ar(%);Sensacao Termica(C);Umidade do Solo(%);Latitude;Longitude;");
  myFile.close();
}

void loop() 
{ 
  for (unsigned long start = millis(); millis() - start < 10000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    gps.f_get_position(&f_lat, &f_lon);
    gps.crack_datetime(&ano, &mes, &dia, &hora, &minuto, &seg);
    hora -= 3;
  }
  
  celsius = dht.readTemperature();
  humidity = dht.readHumidity();
  read_umidsolo = analogRead(PIN_SOLO);
  valor_umidsolo = 100 * ((1024 - read_umidsolo)/1024);
  float realfeel = dht.computeHeatIndex(celsius, humidity, false);

  
  if(isnan(celsius) || isnan(humidity))
  {
    Serial.println("Falha no sensor de temperatura e umidade do ar!");
    loop;
  }
  
  dtostrf(f_lat, 10, 6, w_lat);
  dtostrf(f_lon, 10, 6, w_lon); 
  dtostrf(celsius, 4, 2, temp_ar);
  dtostrf(humidity, 4, 2, umid_ar);
  dtostrf(realfeel, 4, 2, w_realfeel);
  sprintf(w_data, "%02d/%02d/%02d", dia, mes, ano);
  sprintf(w_hora, "%02d:%02d:%02d", hora, minuto, seg);
  String dataString = "";

  dataString += w_data;
  dataString += ";";
  dataString += w_hora;
  dataString += ";";
  dataString += temp_ar;
  dataString += ";";
  dataString += umid_ar;
  dataString += ";";
  dataString += w_realfeel;
  dataString += ";";
  dataString += valor_umidsolo;
  dataString += ";";
  dataString += w_lat;
  dataString += ";";
  dataString += w_lon;
  dataString += ";";
  
  
  myFile = SD.open("test.txt", FILE_WRITE);
  myFile.println(dataString);
  myFile.close();
  Serial.print(dataString);
  Serial.println(" | Gravado com sucesso!");  
}
