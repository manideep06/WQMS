#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#define FIREBASE_HOST "wqms-ad41c.firebaseio.com"
#define FIREBASE_AUTH "XakKC0MYgfBEYxOqQKHNyqNBs4VtcIZlbZBozkQC"
#define WIFI_SSID "Guest"
#define WIFI_PASSWORD "Srix@123"


//float x;
/*Turbidity*/
int turbiditysensor = 32;
float volt;
float ntu;
/*Temperature*/
#include <OneWire.h> 
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);

   sensors.begin();
  

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}


void loop() {
 
 sensors.requestTemperatures(); // Send the command to get temperature readings 
 //Serial.println("DONE"); 
 Serial.print("Temperature is: "); 
 Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?




 volt = 0;
    for(int i=0; i<800; i++)
    {
        volt += ((float)analogRead(turbiditysensor)/4095)*5;
    }
    volt = volt/800;
    Serial.print("Voltage : ");
    Serial.println(volt);
    if(volt < 2.5){
      ntu = 3000;
    }else{
      ntu = -1120.4*(volt*volt)+5742.3*volt-4353.8; 
    }
   Serial.print(" Turbidity : ");
    Serial.print(ntu);
   Serial.println(" NTU");





int x=analogRead(33);
int y=0.00342*x;
Serial.println("analog value:");
Serial.println(x);
Serial.println("pH:");
Serial.println(y);






 Firebase.setInt("Temperature",sensors.getTempCByIndex(0));
 Firebase.setFloat("Voltage",volt);
 Firebase.setFloat("Turbidity",ntu);
 Firebase.setFloat("pH", y);

 if(y<6.5)
{
  Serial.println("Use Lime and Alkaline Fertilizer");
  Firebase.setString("Status1","\"Use Lime and Alkaline Fertilizer\"");
}
else if(y>8.5)
{
  Serial.println("Use Acid Fertilizer");
  Firebase.setString("Status1","\"Use Acid Fertilizer\"");
}

if(ntu>0)
{
  Serial.println("Use Animal Manual accordingly");
  Firebase.setString("Status2","\"Use Animal Manure accordingly\"");
  
}
else
{
  Firebase.setString("Status2","-");
}

 
 delay(1000);

}
