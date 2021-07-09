#include <Adafruit_DotStar.h>
#include <SPI.h>
#define NUMPIXELS 1 // Number of LEDs in strip
#define DATAPIN 7
#define CLOCKPIN 8
Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

const byte led_pin_1 = 1;
const byte decoder_pin_1 = 3;
const byte decoder_pin_2 = 4;
const byte onoff_pin = 2;

const int pulsewidth = 100; // in millisecs
const long cycletime_1 = 700; // in millisecs
const long cycletime_2 = 1200; // in millisecs
const long cycletime_3 = 1700; // in millisecs
const long cycletime_4 = 2200; // in millisecs
long cycletime; // % Cycle time

// Flag
bool led = false;
bool gate1,gate2,gate1now,gate2now;

// Time variables
unsigned long t0, tnow;


void setup() {
  // put your setup code here, to run once:
  pinMode(led_pin_1, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(decoder_pin_1, INPUT_PULLDOWN);
  pinMode(decoder_pin_2, INPUT_PULLDOWN);
  pinMode(onoff_pin, INPUT_PULLDOWN);   

  Serial.begin(9600);
  t0 = millis();

  // Initialize
  gate1 = digitalRead(decoder_pin_1);
  gate2 = digitalRead(decoder_pin_2);
  
  if (!gate1){
    if (!gate2){
      cycletime = cycletime_1;
      strip.setPixelColor(0, 0x000080); // blue
    }
    else{
      cycletime = cycletime_4;
      strip.setPixelColor(0, 0x404040); // white
    }
  }
  else{
    if (!gate2){
      cycletime = cycletime_2;
      strip.setPixelColor(0, 0x008000); // green
    }
    else{
      cycletime = cycletime_3;
      strip.setPixelColor(0, 0x800000); // red
    }
  }
  Serial.println(cycletime);

  // Onboard LED
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
}

void loop() {
  tnow = millis() - t0;
  
  // LED off
  if ((tnow > pulsewidth) && (led == true)){
    led = false;
    digitalWrite(13, LOW);
    digitalWrite(led_pin_1, LOW);
  }

  // LED on
  if ((tnow >= cycletime) && (led == false)){
    led = true;
    t0 = millis();
    digitalWrite(13, HIGH);
    
    if (digitalRead(onoff_pin) == HIGH){
      digitalWrite(led_pin_1, HIGH);
    }
  }

  // Get new gates
  gate1now = digitalRead(decoder_pin_1);
  gate2now = digitalRead(decoder_pin_2);
  
  // Cycle time
  if ((gate1 != gate1now) || (gate2 != gate2now)){
    if (!gate1now){
      if (!gate2now){
        cycletime = cycletime_1;
        Serial.println(cycletime);
        strip.setPixelColor(0, 0x404040); // white
      }
      else{
        cycletime = cycletime_4;
        Serial.println(cycletime);
        strip.setPixelColor(0, 0x000080); // blue
      }
    }
    else{
      if (!gate2now){
        cycletime = cycletime_2;
        Serial.println(cycletime);
        strip.setPixelColor(0, 0x800000); // red
      }
      else{
        cycletime = cycletime_3;
        Serial.println(cycletime);
        strip.setPixelColor(0, 0x008000); // green
      }
    }

    // Renew gates
    gate1 = gate1now;
    gate2 = gate2now;
    strip.show();
  }
  
  delayMicroseconds(100);
  // Serial.println(tnow);
}
