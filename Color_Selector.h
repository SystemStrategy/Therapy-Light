#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_Pin, NEO_GRB + NEO_KHZ800);

float output_red;
float output_green;
float output_blue;

unsigned long Starting_Time;
  unsigned long Blink_Time;
void SetupLED() {
  pixels.begin();  // This initializes the NeoPixel library.  for(int i=0;i<NUMPIXELS;i++)
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));  // Moderately bright green color.
    pixels.show();                                   // This sends the updated pixel color to the hardware.
    delay(100);                                      // Delay for a period of time (in milliseconds).
  }
}



void Color_Splitter(String Hex_Color, int &red, int &green, int &blue) {
  Hex_Color = Hex_Color.substring(1);

  // Parse the hex string into a long integer
  unsigned long hexValue = strtoul(Hex_Color.c_str(), nullptr, 16);

  // Extract the red, green, and blue color components
  red = (hexValue >> 16) & 0xFF;
  green = (hexValue >> 8) & 0xFF;
  blue = hexValue & 0xFF;
}

void Ramp_LED(int red, int green, int blue) {
  //integrate-"ramp"
  output_red = output_red + ((red - output_red) * LED_Rate);
  output_green = output_green + ((green - output_green) * LED_Rate);
  output_blue = output_blue + ((blue - output_blue) * LED_Rate);

  // Serial.print("Red   : ");
  // Serial.println(output_red);
  // Serial.print("Green : ");
  // Serial.println(output_green);
  // Serial.print("Blue  : ");
  // Serial.println(output_blue);

  for (int i = 0; i < NUMPIXELS; i++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(output_red, output_green, output_blue));  // Moderately bright green color.
    pixels.show();                                                                 // This sends the updated pixel color to the hardware.
    delay(5);                                                                     // Delay for a period of time (in milliseconds).
  }
}



void Color_Timer(bool Run_LED_Sequence) {
  bool Last_Run_LED_Sequence;
  int red;
  int green;
  int blue;
  unsigned long currentMillis = millis();

  //Store the Starting_Timer on the transition of Run_LED_Sequence from (Off to ON)
  if (!Run_LED_Sequence) {
    Starting_Time = currentMillis;
  }

  //Starting LED Color
  
  if (Run_LED_Sequence) {
    //turn off 
    if ((currentMillis - Starting_Time) >= (60000UL * (Duration + 5))) {
      LED_Rate = 1;
      Ramp_LED(0, 0, 0);
    }
    //Overtime LED Color
    else if ((currentMillis - Starting_Time) >= (60000UL * (Duration))) {
      if ((currentMillis - Blink_Time) >= 10000UL) Blink_Time = currentMillis;
      else if ((currentMillis - Blink_Time) >= 5000UL) {
        Color_Splitter(Hex_Color4, red, green, blue);
        LED_Rate = 0.03;
        Ramp_LED(red, green, blue);
      } else if ((currentMillis - Blink_Time) >= 0UL) {
        LED_Rate = 0.03;
        Ramp_LED(0, 0, 0);
      }
    }
    //5 Minutes Remaining LED Color
    else if ((currentMillis - Starting_Time) >= (60000UL * (Duration- 5))) {
      LED_Rate = 0.01;
      Color_Splitter(Hex_Color3, red, green, blue);
      Ramp_LED(red, green, blue);
    }
    //Half Time LED Color
    else if ((currentMillis - Starting_Time) >= (60000UL * (Duration / 2))) {
      LED_Rate = 0.01;
      Color_Splitter(Hex_Color2, red, green, blue);
      Ramp_LED(red, green, blue);
    }
    else {
      LED_Rate = 0.01;
      Color_Splitter(Hex_Color1, red, green, blue);
      Ramp_LED(red, green, blue);
    }
  }  else {
    LED_Rate = 1;
    Ramp_LED(0, 0, 0);
  }
}

