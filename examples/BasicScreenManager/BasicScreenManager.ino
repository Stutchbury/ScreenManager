#include <Arduino.h>
#include "IManagedScreen.h"
#include "ScreenManager.h"

/**
 * @brief The most basic example usage of ScreenManager
 * 
 */


// A very simple Screen class that just prints to Serial for each method
class ExampleScreen : public IManagedScreen {
    public:
    ExampleScreen() {
        Serial.println("ExampleScreen constructor");
    }

    ~ExampleScreen() override {
        Serial.println("ExampleScreen destructor");
    }
    void begin() override { 
        Serial.println("ExampleScreen begin()");
    }

    void start() override { 
        Serial.println("ExampleScreen start()");
    }

    void draw() override { 
        Serial.println("ExampleScreen draw()");
    }

    bool end() override {
        Serial.println("ExampleScreen end");
        return true;
    }
};


ScreenManager screenManager;

ExampleScreen exampleScreen;

void setup() {
    Serial.begin(9600);
    delay(1000);

    screenManager.registerScreen(1, &exampleScreen);

    screenManager.setFps(1); //This will make the draw() output print once per second
    
    screenManager.begin();

    Serial.println("Setup complete.");
}


void loop() {
    screenManager.update(millis());
}
