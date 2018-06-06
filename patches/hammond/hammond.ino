#include <Wire.h>
#include "Adafruit_Trellis.h"
// Connect Trellis Vin to 5V and Ground to ground.
// Connect the INT wire to pin #A2 (can change later!)
// Connect I2C SDA pin to your Arduino SDA line A4
// Connect I2C SCL pin to your Arduino SCL line A5
#define ORGAN 0
#define LATCHING 1
#define MODE ORGAN
#define NUMTRELLIS 3
#define numKeys (NUMTRELLIS * 16)
#define INTPIN A2
int db[9];
Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_Trellis matrix1 = Adafruit_Trellis();
Adafruit_Trellis matrix2 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0, &matrix1, &matrix2);

//map trellis index to rows and columns, vice versa
int i2r(int i){
  return 4-(((i%16)-(i%4))/4);
}
int i2c(int i){
  return 12-(((i-(i%16))/4)+(i%4));
}
int rc2i(int r, int c){
  return 47-((((c-1)-((c-1)%4))/4)*16+(r-1)*4 + (c-1)%4);
}

void setup() {
  Serial.begin(38400);
  //pullup intpin
  pinMode(INTPIN, INPUT);
  pinMode(A0, OUTPUT);
  digitalWrite(INTPIN, HIGH);
  digitalWrite(A0, HIGH);
  trellis.begin(0x70, 0x71, 0x72);  // or four!

  // light up all the LEDs in order
  for (uint8_t i=0; i<numKeys+24; i = i + 4) {
    for (uint8_t j=0; j<4; j++){
      if(i<numKeys){
        trellis.setLED(i+3-j);
        trellis.writeDisplay(); 
      }
      if(i >= 24){
        trellis.clrLED(i+3-j-24);
        trellis.writeDisplay(); 
      }
    }
    delay(50);
  }
}



void loop() {
  delay(30); // 30ms delay is required, dont remove me!
  if (MODE == ORGAN) {
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
      // if it was pressed, turn it on
  if (trellis.justPressed(i)) {
    if(i2c(i) > 3){
      switch (i2r(i)){
        case 1:
        db[i2c(i)-4] = 8;
        break;
        case 2:
        if( ( ++db[i2c(i)-4] )==9){db[i2c(i)-4]--;};
        break;
        case 3:
        if( ( --db[i2c(i)-4] )== (-1)){db[i2c(i)-4]++;};
        break;
        default:
        db[i2c(i)-4] = 0;
      }
      for(int j = 1; j<5; j++){
        if((db[i2c(i)-4] % (1<<j)) - (db[i2c(i)-4] % (1<<(j-1))) != 0 ) {
          trellis.setLED(rc2i(5-j,i2c(i)));
        }else{
          trellis.clrLED(rc2i(5-j,i2c(i)));
        }    
      }
      Serial.write(db[i2c(i)-4] + ((i2c(i)-4)<<4));
    }else if (i2c(i)==1){
      if (trellis.isLED(i)){
        trellis.clrLED(i);
        Serial.write(256-2*i2r(i));
      }else{
        trellis.setLED(i);
        Serial.write(257-2*i2r(i));
      }
    }else if (i2c(i) == 2){
        if (trellis.isLED(i)){
        trellis.clrLED(i);
        Serial.write(248-2*i2r(i));
      }else{
        trellis.setLED(i);
        Serial.write(249-2*i2r(i))
        ;
      }
    }
   }
  } 
      }
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
    }
}
