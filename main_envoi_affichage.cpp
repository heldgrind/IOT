#include "MicroBit.h"
#include "ssd1306.h"
#include <stdio.h>
#include <stdlib.h>
#include "bme280.h"


MicroBit uBit; // Create an instance of the MicroBit runtime class
MicroBitThermometer thermometer; // Create an instance of the thermometer class
MicroBitI2C i2c(I2C_SDA0,I2C_SCL0); // Create an instance of the I2C class
MicroBitPin P0(MICROBIT_ID_IO_P0, MICROBIT_PIN_P0, PIN_CAPABILITY_DIGITAL_OUT); // Create an instance of the pin class


int x = 0 ; // Valeur Globale pour le changement de l'affichage

void onData(MicroBitEvent)
{
    
    ManagedString b = ""; // Création d'une chaine de caractère vide

    b = uBit.radio.datagram.recv(); // On récupère la chaine de caractère reçu par le radio
    uBit.display.scroll(b);  // On affiche la chaine de caractère reçu par le radio

    if (b == "HT"){ // Si la chaine de caractère reçu par le radio est égale à HT

        x = 1;  // On active le mode HT (Humidité et Température) avec la chaine de caractère 

    }else if (b == "TH"){  // Si la chaine de caractère reçu par le radio est égale à TH

        x = 2; // On active le mode TH (Température et Humidité) avec la chaine de caractère 
      
    }

}


int main() 
{

    uBit.init(); // Initialise the micro:bit runtime.

    
    bme280 bme(&uBit,&i2c); // Create an instance of the bme280 class
    uint32_t pressure = 0; // Create a variable to store the pressure value
    int32_t temp = 0; // Create a variable to store the temperature value
    uint16_t humidite = 0; // Create a variable to store the humidity value

    ssd1306 screen(&uBit, &i2c, &P0); // Create an instance of the ssd1306 class
    
    uBit.radio.enable(); // Enable the radio
    uBit.radio.setGroup(22); // Set the radio group to 22

    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData); // Listen for radio events

    
    PacketBuffer b(16); // Create a buffer to store the data to send


    while(1){

        bme.sensor_read(&pressure, &temp, &humidite); // Read the sensor values

        int tmp_int = bme.compensate_temperature(temp); // Calculate the temperature value
        int hum_int = bme.compensate_humidity(humidite); // Calculate the humidity value

        if (x == 0 or x == 2 ){ // Si x est égale à 0 ou 2 on affiche la température et l'humidité

            ManagedString display = "Temp:" + ManagedString(tmp_int/100) + "." + (tmp_int > 0 ? ManagedString(tmp_int%100): ManagedString((-tmp_int)%100))+" C";
            
            screen.display_line(2,0 , display.toCharArray()); // Display the temperature value on the screen

            display = "Humi:" + ManagedString(hum_int/100) + "." + ManagedString(hum_int%100)+" rH";

            screen.display_line(4,0 , display.toCharArray()); // Display the humidity value on the screen

        }else if (x == 1){ // Si x est égale à 1 on affiche l'humidité et la température


            ManagedString display = "Humi:" + ManagedString(hum_int/100) + "." + ManagedString(hum_int%100)+" rH";

            screen.display_line(2,0 , display.toCharArray()); // Display the humidity value on the screen
            
             display = "Temp:" + ManagedString(tmp_int/100) + "." + (tmp_int > 0 ? ManagedString(tmp_int%100): ManagedString((-tmp_int)%100))+" C";
            
            screen.display_line(4,0 , display.toCharArray()); // Display the temperature value on the screen

          
        
        
        }

    
       int increase = 0; // Create a variable to store the increase value

        memcpy(b.getBytes(), &tmp_int, sizeof(tmp_int)); // Copy the temperature value to the buffer

        increase += sizeof(b.getBytes()); // Increase the increase value
        
        memcpy((b.getBytes()+increase), &hum_int, sizeof(hum_int)); // Copy the humidity value to the buffer
           
        screen.update_screen(); // Update the screen

        uBit.radio.datagram.send(b); // Send the buffer
                 
        uBit.sleep(1000); // Sleep for 1 second
    
    }

     release_fiber(); // Release the fiber
}



 








