#include "MicroBit.h"


MicroBit uBit; // Create an instance of the MicroBit runtime class
MicroBitThermometer thermometer; // Create an instance of the thermometer class


void onData(MicroBitEvent)
{
    
    PacketBuffer b(16); // Create a buffer to store the data to send
    b = 0; // Clear the buffer
    b = uBit.radio.datagram.recv(); // On récupère la chaine de caractère reçu par le radio
    

    int temperature; // Create a variable to store the temperature value
    int humidity; // Create a variable to store the humidity value

    ////////////////////////////////////////////////////////////
    //sudo minicom -D /dev/ttyACM0 -b 115200
    ////////////////////////////////////////////////////////////

    memcpy(&temperature, b.getBytes(), 4); // 4 Octets 
    memcpy(&humidity, (b.getBytes()+4), 4); // 4 Octets
    
    ManagedString tmp = "tmp: " + ManagedString(temperature) + "; " ; // Création d'un chaine de caractère avec la température
    ManagedString hum = "hmd: " + ManagedString(humidity) + "; " ; // Création d'un chaine de caractère avec l'humidité

    uBit.serial.send( tmp); // On envoie la température sur le port série
    uBit.serial.send( hum); // On envoie l'humidité sur le port série

    ManagedString env = uBit.serial.read(4,ASYNC); // On lit les 4 caractères reçu sur le port série
    uBit.sleep(2000); // On attend 2 secondes
    uBit.display.scroll(env); // On affiche les 4 caractères reçu sur le port série
    uBit.radio.datagram.send(env);//envoi de l'ordre d'affichage à la carte principale
   
}


int main()
{
    uBit.init(); // Initialise the micro:bit runtime.


    uBit.radio.enable(); // Enable the radio
    uBit.radio.setGroup(22); // Set the radio group to 22
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData); // Listen for messages from the radio
    
    while (1)
    {
        uBit.sleep(2000);  
    }
        
}

