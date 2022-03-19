#include <SPI.h>
#include <LoRa.h>
#include <CheapLCD.h>

CheapLCD lcd;

char temp;

double packet_duration;
uint32_t temp_number = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  LoRa.setPins(3, 2);
  
  if (!LoRa.begin(869525000)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSignalBandwidth(250E3);
  LoRa.setSpreadingFactor(12);
  LoRa.setGain(6);

  lcd.begin();
  lcd.clear();
  //lcd.write("Hello world!");
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    lcd.clear();

    // read packet
    //while (LoRa.available()) {
    //  temp = (char)LoRa.read();
    //  Serial.print(temp);
      //lcd.write(temp);
    //}

    uint8_t i = 0;
    // read packet
    while (LoRa.available()) {
      temp_number |= ((uint32_t)LoRa.read() << (i * 8));
      //Serial.println(LoRa.read());
      i ++;
    }
    Serial.println(i);
    //packet_duration = (double)temp_number;
    Serial.println(temp_number);

    float packet_SNR = LoRa.packetSnr();
    int packet_RSSI = LoRa.packetRssi();  

    Serial.print(" SNR: ");
    Serial.println(packet_SNR);

    lcd.print("SNR: ");
    lcd.print(packet_SNR);
    lcd.print(" dB");
    
    lcd.setCursor(0, 1);

    // print RSSI of packet
    Serial.print(" RSSI: ");
    Serial.println(packet_RSSI);

    lcd.print("RSSI: ");
    lcd.print(packet_RSSI);
    lcd.print(" dBm");
  }
}
