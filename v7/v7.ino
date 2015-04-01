/** By Ryan Millner
Spring 2015
**/


byte commandByte;
byte noteByte;
byte velocityByte;

byte noteOn = 144;
byte noteOff = 128;
byte pitchBend = 224;
byte modulation = 176;
int lastColor;
int rPin = 10; // set to correct value
int gPin = 6;  // set to correct value
int bPin =11;  // set to correct value
int rLED = 0;
int gLED = 0;
int bLED = 0;
int rLED_old = -1;
int gLED_old = -1;
int bLED_old = -1;
int r;            // remainder
int mod;          // pitchbend mod
int onOff = 0;
int ons = -1; // number of consecutive ons
char maxC; // brightest color
char lastCommand;
byte pbVelocity;
byte lastNote;

void setup(){
  Serial.begin(31250);
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  digitalWrite(rPin,LOW);
  digitalWrite(gPin,LOW);
  digitalWrite(bPin,LOW);
 
}

void Lights(){
  do{
    if (Serial.available()){
      commandByte = Serial.read();  //read first byte
      noteByte = Serial.read(); //read next byte
      velocityByte = Serial.read(); //read final byte

      if (commandByte == noteOn){
         onOff = 1;
         ons = ons + 1;
         lastNote = noteByte;
         r = noteByte % 12;
        if(rLED != colorR(noteByte)) rLED_old = rLED; // consecutive note on
        if(gLED != colorG(noteByte)) gLED_old = gLED; // consecutive note on
        if(bLED != colorB(noteByte)) bLED_old = bLED; // consecutive note on
           rLED = colorR(noteByte); // get red LED value
           gLED = colorG(noteByte); // get green LED value
           bLED = colorB(noteByte); // get blue LED value
           setColor(rLED, gLED, bLED);
      }

      if (commandByte == noteOff){
        if(ons > 0){
          setColor(rLED_old, gLED_old, bLED_old); // set to old color
          ons = 0;
        }
        else{
          digitalWrite(rPin,LOW);//turn led off
          digitalWrite(gPin,LOW);//turn led off
          digitalWrite(bPin,LOW);//turn led off 
          ons = -1;        
       }
        onOff = 0;  
        
      }
      
      if (commandByte == pitchBend && onOff == 1){
         pbWrite(velocityByte); // write using pitch bend func
      }

//      if (commandByte == modulation && onOff == 1){
//         modWrite(velocityByte); // write using pitch bend func
//      }
         
    }
  }
  while (Serial.available() > 2); //when at least three bytes available
}

    

void loop(){
  Lights();
  delay(10); // change for optimal results: 0 > d > 100
}

int colorR(int noteByte){  //finding red value on color wheel
  int red;
  if (r == 1 || r == 7){ 
    red = 64;
  }
  else if (r > 1 && r < 7){
    red = 127; 
  }
  
  else{
    red = 0;
  }
  
  return red;
}

int colorG(int noteByte){  //finding green value on color wheel
  int green;
  if (r == 5 || r == 11){
    green = 64;
  }
  else if (r >= 0 && r < 5){
    green = 0; 
  }
  
  else{
    green = 127;
  }
  
  return green;
}

int colorB(int noteByte){  //finding blue value on color wheel
  int blue;
  if (r < 3 || r > 9){
    blue = 127;
  }
  else if (r == 3 || r == 9){
    blue = 64; 
  }
  else{
    blue = 0;
  }
  return blue;
}

void setColor(int r, int g, int b){
         analogWrite(rPin, r);
         analogWrite(gPin, g);
         analogWrite(bPin, b); 
}

char topColor(int r, int g, int b){
	if(r > g && r > b){
		maxC = 'r';
	}
	else if(g > b){
		maxC = 'g';
	}
	else{
		maxC = 'b';
	}
	return maxC;
}


//Pitch Bend Function - room for improvement
void pbWrite(byte velocity){
   char pin = topColor(rLED, gLED, bLED);
//  if(rLED > 0 && maxC != 'rPin'){
//    digitalWrite(rLED, 0);
//  }
//  
//  if(gLED > 0 && maxC != 'gPin'){
//    digitalWrite(gLED, 0);
//  }
//  
//  if(bLED > 0 && maxC != 'bPin'){
//    digitalWrite(bLED, 0);
//  }
  if(pin=='r'){
    analogWrite(rPin, velocity * 2);
  }
  if(pin=='g'){
    analogWrite(gPin, velocity * 2);
  }
  if(pin=='b'){
    analogWrite(bPin, velocity * 2);
  }  
}

////Mod Function - coming soon
//void modWrite(byte velocity){
//  while(velocity > 0 && onOff >= 1){
//    setColor(rLED, gLED, bLED);
//    delay(velocity * 5);
//    setColor(0, 0, 0);
//    delay(velocity * 5);
//  }
//}


