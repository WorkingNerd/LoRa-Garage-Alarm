#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

RH_RF95 rf95;// Singleton instance of the radio driver
float frequency = 868.0;

int pirPin = 2; 
int pirStat = 0; 

void setup() 
{
  pinMode(pirPin, INPUT); 
  
  Serial.begin(9600);
  Serial.println("Start LoRa Client");
  
  if (!rf95.init())
    Serial.println("init failed");
 
  rf95.setFrequency(frequency); // Setup ISM frequency
  rf95.setTxPower(13); // Setup Power,dBm 
  rf95.setSpreadingFactor(7); //Spreading Factor (6 ~ 12) 
  rf95.setSignalBandwidth(125000); //BandWidth option: 7800,10400,15600,20800,31200,41700,62500,125000,250000,500000 (lower BandWidth for longer distance!) 
  rf95.setCodingRate4(5); //Coding Rate:5(4/5),6(4/6),7(4/7),8(4/8)
  
}

void loop()
{
  pirStat = digitalRead(pirPin); 
  
  if (pirStat == HIGH) 
  {
    Serial.println("Sending to LoRa Server");
   
    uint8_t data[] = "ALERT! Garage is open!"; // Send a message to LoRa Server
    rf95.send(data, sizeof(data));
    
    rf95.waitPacketSent();
    
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
  
    if (rf95.waitAvailableTimeout(5000))
    { 
         
      if (rf95.recv(buf, &len))
     {
        Serial.print("got reply: ");
        Serial.println((char*)buf);
        Serial.print("RSSI: ");
        Serial.println(rf95.lastRssi(), DEC);    
      }
      else
      {
        Serial.println("recv failed");
      }
    }
    else
    {
      Serial.println("No reply, is LoRa server running?");
    }
    delay(5000);
  }
}