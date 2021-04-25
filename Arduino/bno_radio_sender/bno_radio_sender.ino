#include <Wire.h>
#include <MirfHardwareSpiDriver.h> // Pour la communication SPI
#include <Mirf.h>     // Pour la gestion de la communication
#include <nRF24L01.h> // Pour les définitions des registres du nRF24L01
#include <SPI.h>      // Pour la communication via le port SPI
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
  
Adafruit_BNO055 bno = Adafruit_BNO055(55);
 
void setup(void) 
{
  Serial.begin(9600);
  Mirf.cePin = 8; //Set the CE pin to D9
  Mirf.csnPin = 7; //Set the CE pin to D10
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init(); //initialization nRF24L01

  Mirf.channel = 2; // Choix du canal de communication (128 canaux disponibles, de 0 à 127)
  Mirf.payload = sizeof(float); // Taille d'un message (maximum 32 octets)
  Mirf.config(); // Sauvegarde la configuration dans le module radio

  Mirf.setTADDR((byte *) "nrf01"); // Adresse de transmission
  Mirf.setRADDR((byte *) "nrf02"); // Adresse de réception
  
  Wire.begin(); // initialisation librairire 
  Serial.println("Orientation Sensor Test"); Serial.println("");
  bno.begin();
  delay(5000);
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);
}
 
void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  
  float x;
  x = euler.x();
  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(euler.x(), 4);
  Serial.print("\tY: ");
  Serial.print(euler.y(), 4);
  Serial.print("\tZ: ");
  Serial.print(euler.z(), 4);
  Serial.println("");
  delay(100);

  Mirf.send((byte *) &x); // On envoie le message à la voiture
  while(Mirf.isSending());
  // On attend la fin de l'envoi
  Serial.write((byte)x);
}
