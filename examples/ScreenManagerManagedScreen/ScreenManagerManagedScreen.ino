#include <Arduino.h>
#include "IManagedScreen.h"
#include "ScreenManager.h"

/**
 * @brief An example showing the different lifecycles of External and Managed Screens.
 * 
 * @details When entering a Screen ID in the monitor, the slight delay is `Serial.parseInt();` waiting for you to enter a longer number before parsing 
 * You do not need to press <Return>, jus 1 or 2
 * 
 */


// A very simple Screen that just prints to Serial for each method
class ExampleScreen : public IManagedScreen {
    public:
    ExampleScreen() {
        screenName = "Example";
        //Don't print for ExampleScreen as it is created before Serial has begun but *would* print when derived ExternalScreen is constructed!
        //Serial.print(screenName); Serial.println("Screen constructor");
    }

    ~ExampleScreen() override {
        Serial.print(screenName); Serial.println("Screen destructor");
    }
    void begin() override { 
        Serial.print(screenName); Serial.println("Screen begin()");
    }

    void start() override { 
        Serial.print(screenName); Serial.println("Screen start()");
    }

    void draw() override { 
        //Serial.print(screenName); Serial.println("Screen draw()");
    }

    bool end() override {
        Serial.print(screenName); Serial.println("Screen end");
        return true;
    }
    protected:
    const char* screenName;
};

// An example screen that is going to be registered as a Managed Screen.
// It could also be used as an External Screen - ScreenManager doesn't care!
class ManagedScreen : public ExampleScreen {
    public:
    ManagedScreen() {
        screenName = "Managed";
        Serial.print(screenName); Serial.println("Screen constructor");
    }

};

ScreenManager screenManager;

ExampleScreen exampleScreen; //An external screen

void setup() {
    Serial.begin(9600);
    delay(1000);

    // Register and External Screen
    screenManager.registerScreen(1, &exampleScreen);
    // Register a Managed Screen
    screenManager.registerScreen<ManagedScreen>(2);

    screenManager.begin();

    Serial.println("Setup complete.");

    Serial.println("Enter 1 or 2 to change screens...");
}


void loop() {
        screenManager.update(millis());
    if (Serial.available() > 0) {
        int number = Serial.parseInt();
        if (number != 0) {
            Serial.print("You entered: ");
            Serial.println(number);
            screenManager.requestScreen(number);
        }    
    }
}
