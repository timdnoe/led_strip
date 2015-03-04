/** By Ryan Millner
Spring 2015
**/


byte commandByte;
byte noteByte;
byte velocityByte;

byte noteOn = 144;
byte noteOff = 128;
byte pitchBend = 224;
int lastColor;
int rPin = 10;
int gPin = 6;
int bPin =11;
int rLED;
int gLED;
int bLED;
int r;            // remainder
int mod;          // pitchbend mod
int onOff = 0;
char maxC;        // brightest color
byte pbVelocity;

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
        if (velocityByte > 0){
         onOff = 1;
         rLED = colorR(noteByte); // get red LED value
         gLED = colorG(noteByte); // get green LED value
         bLED = colorB(noteByte); // get blue LED value
         setColor(rLED, gLED, bLED);
        }
      }

      if (commandByte == noteOff){
        rLED = 0;
        gLED = 0;
        bLED = 0;
        digitalWrite(rPin,LOW);//turn led off
        digitalWrite(gPin,LOW);//turn led off
        digitalWrite(bPin,LOW);//turn led off  
        onOff = 0;      
      }
      
      if (commandByte == pitchBend && onOff == 1){
        maxC = topColor(rLED, gLED, bLED);
        if(rLED > 0){
         rLED = rLED * velocityByte;
         analogWrite(rPin, rLED * velocityByte);  
        }
       if(gLED > 0){
         gLED = gLED * velocityByte;
         analogWrite(gPin, gLED * velocityByte);
        }
       if(bLED > 0){
         bLED = bLED * velocityByte;
         analogWrite(bPin, bLED * velocityByte);
         }
      }
         
    }
  }
  while (Serial.available() > 2); //when at least three bytes available
}
    

void loop(){
  Lights();
  delay(10); // neccesary for arduino to read midi
}

int colorR(int noteByte){  //finding red value on color wheel
  r = noteByte % 12;
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
  r = noteByte % 12;
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
  r = noteByte % 12;
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
         analogWrite(rPin, rLED);
         analogWrite(gPin, gLED);
         analogWrite(bPin, bLED); 
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

// void pbWrite(int pin, int light, byte v){
//   if(v > 64){
//     analogWrite(pin, light + v);
//   }
//   else{
//     analogWrite(rPin, rLED - (v / 2));
//     analogWrite(gPin, gLED - (v / 2));
//     analogWrite(bPin, bLED - (v / 2));
//   }
// }



