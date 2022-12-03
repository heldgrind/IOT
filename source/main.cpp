/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "MicroBit.h"

MicroBit uBit;
MicroBitThermometer thermometer;
MicroBitSerial serial(USBTX, USBRX); // tx, rx

void onData(MicroBitEvent)
{
    
    PacketBuffer b(16);
    b = uBit.radio.datagram.recv();
    

    int temperature;
    int humidity;

    ////////////////////////////////////////////////////////////
    //sudo minicom -D /dev/ttyACM0 -b 115200
    ////////////////////////////////////////////////////////////
    memcpy(&temperature, b.getBytes(), 4); // 4 Octets 
    for (int i = 0; i < 4; i++) { // 4 Octets 
        memcpy(&humidity, &b[i+4], 4); // 1 octets 
    }

    uBit.display.scroll(temperature);
    uBit.display.scroll(humidity);


    ManagedString s = ManagedString(temperature );
    //uBit.serial.printf("Temperature: %d\n", temperature);
    uBit.serial.send(temperature);
}
int main()
{
    uBit.init();
    uBit.radio.enable();
    uBit.radio.setGroup(22);
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);


    
    while (1)
    {
        uBit.sleep(1000);
        uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);
    }
    
}

