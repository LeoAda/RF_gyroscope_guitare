#include <Wire.h>
#include <MirfHardwareSpiDriver.h> // Pour la communication SPI
#include <Mirf.h>     // Pour la gestion de la communication
#include <nRF24L01.h> // Pour les définitions des registres du nRF24L01
#include <SPI.h>      // Pour la communication via le port SPI

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  Mirf.cePin = 8; //Set the CE pin to D9
  Mirf.csnPin = 7; //Set the CE pin to D10
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init(); //initialization nRF24L01

  Mirf.channel = 2; // Choix du canal de communication (128 canaux disponibles, de 0 à 127)
  Mirf.payload = sizeof(float); // Taille d'un message (maximum 32 octets)
  Mirf.config(); // Sauvegarde la configuration dans le module radio

  Mirf.setTADDR((byte *) "nrf02"); // Adresse de transmission
  Mirf.setRADDR((byte *) "nrf01"); // Adresse de réception
  
  Wire.begin(); // initialisation librairire 
}

void loop() {
  // put your main code here, to run repeatedly:
  float pitch;
  if(Mirf.dataReady()){
    Mirf.getData((byte*)&pitch);
    Serial.println(pitch);
  }
}
