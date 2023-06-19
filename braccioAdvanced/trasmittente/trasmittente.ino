#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<Wire.h>
#include <NewPing.h>

RF24 myRF24 (7, 8);
byte address[6] = "00001";

//ultrasuoni
#define PIN_TRIGGER 13
#define PIN_ECHO 12
#define MASSIMO 20

NewPing sonar(PIN_TRIGGER, PIN_ECHO, MASSIMO);

//gyroscopes
#define DEVICE_A (0x1D)    //first ADXL345 device address
#define DEVICE_B (0x53)    //second ADXL345 device address
#define TO_READ (6)        //num of bytes we are going to read each time (two bytes for each axis)

byte buff[TO_READ] ;      //6 bytes buffer for saving data read from the device
char str[512];            //string buffer to transform data before sending it to the serial port

void setup () {
  //serial begin
  Serial.begin(9600);
  Wire.begin();        // join i2c bus (address optional for master)

  //RC trasmission begin
  myRF24.begin();
  myRF24.openWritingPipe(address);
  myRF24.setPALevel(RF24_PA_MIN);
  myRF24.stopListening();

  //set speed to 10
  String sp = "sp10";
  myRF24.write(&sp, sizeof(sp));

  //Turning on the both ADXL345s
  writeTo(DEVICE_A, 0x2D, 24);
  writeTo(DEVICE_B, 0x2D, 24);
  int regAddress = 0x32;      //first axis-acceleration-data register on the ADXL345
int xa = 0, ya = 0, za = 0;  
int xb = 0, yb = 0, zb = 0;
}

void loop () {
  //ultrasuoni
  int readSonar = sonar.ping_cm();
  //tranform cm in °, on 10cm
  readSonar *= 18;
  Serial.println(readSonar);
  String posA = "s1" + String(readSonar);

readFrom(DEVICE_A, regAddress, TO_READ, buff); //read the acceleration data from the ADXL345  
   //each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
   //thus we are converting both bytes in to one int
  xa = (((int)buff[1]) << 8) | buff[0];   
  ya = (((int)buff[3])<< 8) | buff[2];
  za = (((int)buff[5]) << 8) | buff[4];
  
  readFrom(DEVICE_B, regAddress, TO_READ, buff); //read the acceleration data from the second ADXL345
  xb = (((int)buff[1]) << 8) | buff[0];   
  yb = (((int)buff[3])<< 8) | buff[2];
  zb = (((int)buff[5]) << 8) | buff[4];
  
  //we send the x y z values as a string to the serial port
  sprintf(str, "%d %d %d %d %d %d", xa, ya, za, xb, yb, zb);  
  Serial.print(str);
  Serial.write(10);

  String s2 = "s2" + String(xa);
  String s3 = "s3" + String(ya);
  String s4 = "s4" + String(xb);
  String s5 = "s5" + String(yb);
  //RC
  char text[5] = "Prova";

  myRF24.write(&text, sizeof(text));

  Serial.println(text);
  delay(2000);
}

void writeTo(int device, byte address, byte val) {
   Wire.beginTransmission(device); //start transmission to device 
   Wire.write(address);        // send register address
   Wire.write(val);        // send value to write
   Wire.endTransmission(); //end transmission
}

void readFrom(int device, byte address, int num, byte buff[]) {
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        //sends address to read from
  Wire.endTransmission(); //end transmission
  
  Wire.beginTransmission(device); //start transmission to device
  Wire.requestFrom(device, num);    // request 6 bytes from device
  
  int i = 0;
  while(Wire.available())    //device may send less than requested (abnormal)
  { 
    buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); //end transmission
}
