#include <Servo.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//servo
Servo servoa;
Servo servob;
Servo servoc;
Servo servod;
Servo servoe;
Servo servof;

int servoaPos, servobPos, servocPos, servodPos, servoePos, servofPos;
int servo1PPos, servo2PPos, servo3PPos, servo4PPos, servo5PPos, servo6PPos;
int sp;

String dataIn = "";
String dataInS;

//LED RGB
int L_R = 11;
int L_G = 10;
int L_B = 9;

//stato
int stato = LOW;

//RC
RF24 myRF24 (12, 13);
byte address[6] = "00001";

void setup() {
  //serial monitor
  Serial.begin(9600);

  //button
  pinMode(8, INPUT);

  //servo setup pinze
  servoa.attach(7);
  servo1PPos = 180;
  servoa.write(servo1PPos);

  //movimento pinze
  servob.attach(6);
  servo2PPos = 15;
  servob.write(servo2PPos);

  //angolo pinze
  servoc.attach(5);
  servo3PPos = 90;
  servoc.write(servo3PPos);

  //giro secondo pezzo braccio
  servod.attach(4);
  servo4PPos = 60;
  servod.write(servo4PPos);

  //primo pezzo braccio
  servoe.attach(3);
  servo5PPos = 100;
  servoe.write(servo5PPos);

  servof.attach(2);
  servo6PPos = 0;
  servof.write(servo6PPos);

  //setup RC
  myRF24.begin();
  myRF24.openReadingPipe(0, address);
  myRF24.setPALevel(RF24_PA_MIN);
  myRF24.startListening();

  //RGB Setup
  pinMode(L_R, OUTPUT);
  pinMode(L_G, OUTPUT);
  pinMode(L_B, OUTPUT);
}

void loop() {
  if (digitalRead(8) == HIGH) {
    stato = !stato;
    delay(100);
  }

  if (stato == HIGH) {
    if ( myRF24.available()) {
      char text[5] = "";
      myRF24.read( &text, sizeof(text));
      dataIn = text;
      Serial.println(dataIn);
    }
  } else {
    if (Serial.available() <= 0) {
      digitalWrite(L_G, LOW);
      digitalWrite(L_B, LOW);
      digitalWrite(L_R, HIGH);
    }

    //check data in
    if (Serial.available() > 0) {
      dataIn = Serial.readString();
      Serial.print(dataIn);
      digitalWrite(L_R, LOW);
      digitalWrite(L_B, LOW);
      digitalWrite(L_G, HIGH);
    }
  }

//-------------------------------\Motor movement\-----------------------------//

  //set the speed
  if (dataIn.startsWith("sp")) {
    dataInS = dataIn.substring(2, dataIn.length());
    sp = dataInS.toInt();
  }

  if (dataIn.startsWith("s1")) {
    dataInS = dataIn.substring(2, dataIn.length()); // Extract only the number
    servoaPos = dataInS.toInt();  // Convert the string into integer
    // We use for loops so we can control the speed of the servo
    // If previous position is bigger then current position
    if (servo1PPos > servoaPos) {
      for ( int j = servo1PPos; j >= servoaPos; j--) {   // Run servo down
        servoa.write(j);
        delay(sp);    // defines the speed at which the servo rotates
      }
    }
    // If previous position is smaller then current position
    if (servo1PPos < servoaPos) {
      for ( int j = servo1PPos; j <= servoaPos; j++) {   // Run servo up
        servoa.write(j);
        delay(sp);
      }
    }
    servo1PPos = servoaPos;
  }

  if (dataIn.startsWith("s2")) {
    dataInS = dataIn.substring(2, dataIn.length());
    servobPos = dataInS.toInt();

    if (servo2PPos > servobPos) {
      for ( int j = servo2PPos; j >= servobPos; j--) {
        servob.write(j);
        delay(sp);
      }
    }

    if (servo2PPos < servobPos) {
      for ( int j = servo2PPos; j <= servobPos; j++) {
        servob.write(j);
        delay(sp);
      }
    }
    servo2PPos = servobPos;
  }

  if (dataIn.startsWith("s3")) {
    dataInS = dataIn.substring(2, dataIn.length());
    servocPos = dataInS.toInt();

    if (servo3PPos > servocPos) {
      for ( int j = servo3PPos; j >= servocPos; j--) {
        servoc.write(j);
        delay(sp);
      }
    }

    if (servo3PPos < servocPos) {
      for ( int j = servo3PPos; j <= servocPos; j++) {
        servoc.write(j);
        delay(sp);
      }
    }
    servo3PPos = servocPos;
  }

  if (dataIn.startsWith("s4")) {
    dataInS = dataIn.substring(2, dataIn.length());
    servodPos = dataInS.toInt();

    if (servo4PPos > servodPos) {
      for ( int j = servo4PPos; j >= servodPos; j--) {
        servod.write(j);
        delay(sp);
      }
    }

    if (servo4PPos < servodPos) {
      for ( int j = servo4PPos; j <= servodPos; j++) {
        servod.write(j);
        delay(sp);
      }
    }
    servo4PPos = servodPos;
  }

  if (dataIn.startsWith("s5")) {
    dataInS = dataIn.substring(2, dataIn.length());
    servoePos = dataInS.toInt();

    if (servo5PPos > servoePos) {
      for ( int j = servo5PPos; j >= servoePos; j--) {
        servoe.write(j);
        delay(sp);
      }
    }

    if (servo5PPos < servoePos) {
      for ( int j = servo5PPos; j <= servoePos; j++) {
        servoe.write(j);
        delay(sp);
      }
    }
    servo5PPos = servoePos;
  }

  if (dataIn.startsWith("s6")) {
    dataInS = dataIn.substring(2, dataIn.length());
    servofPos = dataInS.toInt();

    if (servo6PPos > servofPos) {
      for ( int j = servo6PPos; j >= servofPos; j--) {
        servof.write(j);
        delay(sp);
      }
    }

    if (servo6PPos < servofPos) {
      for ( int j = servo6PPos; j <= servofPos; j++) {
        servof.write(j);
        delay(sp);
      }
    }
    servo6PPos = servofPos;
  }

}
