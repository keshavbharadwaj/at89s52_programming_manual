/**
PROGRAMMING AN ATMEL AT89S51/52 USING ARDUINO
RELEASED AS IS WITHOUT WARRANTY 
I AM NOT LIABLE FOR ANY DAMAGE DONE TO YOUR HARDWARE
THIS PROJECT IS FOR EDUCATIONAL PURPOSES ONLY
Credits to NICK PABLO for the Arduino Sketch
TIKTAK (C) 2014

**/

#define dummyData 0xAA
#define RDY 75
#define NRDY 76

const int _MISO = 4;
const int _MOSI = 5;
const int _CLK = 3;
const int RST = 2;

/* Variable definition block */

byte data;
byte AL,AH; // 16-bit address
byte lockByte; // embed lock bits here
byte SigH,SigL; // Signature Bytes

void setup()
{
pinMode(_MISO, INPUT);
pinMode(_MOSI, OUTPUT);
pinMode(_CLK, OUTPUT);
pinMode(RST, OUTPUT);
Serial.begin(115200); // depends on the setting of the host PC

}



void loop()
{ 
  while (!Serial.available()); // wait for character
        if (Serial.available() > 0)
        switch (Serial.read())
        {
        case 'p': Serial.write(progEnable());
                  break; 
        case 'r': readProgmem(); 
                  Serial.write(data);
                  break;
        case 'a': while(!Serial.available());
                  AL = Serial.read();
                  break; 
        case 'A': while(!Serial.available()); 
                  AH = Serial.read();
                  break; 
        case 'd': while(!Serial.available()); 
                  data = Serial.read();
                  break; 
        case 'S': AH = 0;
                  AL = 0;
                  SigH = readSign();
                  Serial.write(SigH);
                  break; 
        case 's': AH = 2;
                  AL = 0;
                  SigL = readSign();
                  Serial.write(SigL);
                  AH = 1;
                  AL = 0;
                  SigL = readSign();
                  Serial.write(SigL);
                  break; // read SigL
        case 'o': digitalWrite(RST,1);break; 
        case 'c': digitalWrite(RST,0);break; 
        case 'e': eraseChip();
                  Serial.write(RDY);
                  break; 
        case 'j': break;
        case 'w': writeProgmem();
                  break;
        }



}


unsigned char SendSPI(unsigned char data)
{ 
  uint8_t retval = 0;
  uint8_t intData = data;
  int t;
  
    for (int ctr = 0; ctr < 7; ctr++)
      { 
         if (intData & 0x80) digitalWrite(_MOSI,1);
            else digitalWrite(_MOSI,0);

        digitalWrite(_CLK,1); 
        delayMicroseconds(1); 

        t = digitalRead(_MISO); 
        digitalWrite(_CLK,0);

        if (t) retval |= 1; else retval &= 0xFE;
            retval<<=1;
            intData<<= 1;
        delayMicroseconds(1); 
    }


  if (intData & 0x80) digitalWrite(_MOSI,1);
    else digitalWrite(_MOSI,0);
  
    digitalWrite(_CLK,1);
    delayMicroseconds(1); 

    t = digitalRead(_MISO);
    digitalWrite(_CLK,0);
    
    if (t) retval |= 1; 
    else retval &= 0xFE;

    return retval; 
}


byte progEnable()
{ 
    SendSPI(0xAC);
    SendSPI(0x53);
    SendSPI(dummyData);
    
   return SendSPI(dummyData);
}

void eraseChip()
{
     SendSPI(0xAC);
     SendSPI(0x9F);
     SendSPI(dummyData);
     SendSPI(dummyData);
     
     delay(520);
}

void readProgmem()
{

    SendSPI(0x20);
    SendSPI(AH);
    SendSPI(AL);
    data = SendSPI(dummyData);
}

void writeProgmem()
{
    SendSPI(0x40);
    SendSPI(AH);
    SendSPI(AL);
    SendSPI(data);
}

void writeLockBits()
{
    SendSPI(0xAC);
    SendSPI(lockByte);
    SendSPI(dummyData);
    SendSPI(dummyData);
}

void readLockBits()
{ 
    SendSPI(0x24);
    SendSPI(dummyData);
    SendSPI(dummyData);
    lockByte = SendSPI(dummyData);
}

byte readSign()
{
    SendSPI(0x28);
    SendSPI(AH);
    SendSPI(AL);
    return SendSPI(dummyData);
}

