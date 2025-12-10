#include <Adafruit_NeoPixel.h>

// Define the pin the NeoPixel strip is connected to
#define NEOPIXEL_PIN 3 
#define BUTTON_PIN 4


// Define the number of pixels in your strip
#define NUM_PIXELS 120
#define SECTION_SIZE 20
#define NUM_SECTIONS 6 

// Declare the NeoPixel object
Adafruit_NeoPixel pixelsA(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

int buttonState= 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int lastButtonState = HIGH;

struct Color {
  int red;
  int green;
  int blue;
};

// Available colors to choose from
Color availableColors[] = {
  {255, 0, 0},   // red
  {0, 255, 0},   // green  
  {0, 0, 255},   // blue
  {255, 255, 0}, // yellow
  {0, 255, 255}, // cyan
  {128, 0, 128}  // purple
};

const int numAvailableColors = sizeof(availableColors) / sizeof(availableColors[0]);

void randomize() {
  pixelsA.clear(); 

  delay(100);
  
  // Create an array to track which colors we've used
  bool usedColors[numAvailableColors] = {false};
  
  // Randomly select 6 unique colors
  Color selectedColors[NUM_SECTIONS];
  int colorsSelected = 0;
  
  while (colorsSelected < NUM_SECTIONS) {
    int randomIndex = random(numAvailableColors);
    
    // If this color hasn't been used yet
    if (!usedColors[randomIndex]) {
      selectedColors[colorsSelected] = availableColors[randomIndex];
      usedColors[randomIndex] = true;
      colorsSelected++;
    }
  }
  
  // Apply the randomly selected colors to the pixels
  int indvSect= NUM_PIXELS/NUM_SECTIONS;

  for (int section = 0; section < NUM_SECTIONS; section++) {
    int startPixel= section * SECTION_SIZE;
    int endPixel= startPixel + SECTION_SIZE;

    for(int pixel= startPixel; pixel< endPixel; pixel++){

      pixelsA.setPixelColor(pixel, 
        pixelsA.Color(
          selectedColors[section].red, 
          selectedColors[section].green, 
          selectedColors[section].blue
        )
      );
    }
  
    pixelsA.show(); 
   
    delay(100);
  }
  

}  

void setup() {
  // Initialize the NeoPixel strips
  pixelsA.begin(); 
 
  // Set the brightness (0-255)
  pixelsA.setBrightness(50);

  // Initialize random seed
  randomSeed(analogRead(0)); // Read from unconnected analog pin for randomness

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);
  
  // Debounce the button
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      
      if (buttonState == LOW) {
        randomize();
      }
    }
  }
  
  lastButtonState = reading;
}