//#pragma once
#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "ScreenRouter.h"
#include "ScreenEntry.h"
#include "ScreenTransition.h"


using ScreenId = uint8_t;

/**
 * @brief The core class that handles registration, transitions and of course, calling the screen's draw() method.
 * 
 */
class ScreenManager {

public:

    ~ScreenManager();

    /**
     * @brief Called once during setup(). Will call the `IManagedScreen::begin()` method of all *external* screens.that have been registered.
     * 
     */
    void begin();

    /**
     * @brief Called from your `loop()`. Checks if a screen transition has been requested and calls current screen's draw() at set FPS.
     * 
     * @details Receives a timer value - in the Arduino Framework this would be `millis()`.
     * 
     * @param nowMs A timer value used to determine when `draw()` is called (based on the `setFps()`)
     */
    void update(uint32_t nowMs);

    /**
     * @brief Register a screen created externally
     * 
     * @details Usage: `screenManager.registerScreen(&home, 11);`
     * 
     * @tparam T 
     * @param screen 
     * @param id A uint8_t used to identify this screen
     */
    template<typename T>
    bool registerScreen(ScreenId id, T* screen ) {
        if ( id == 0 ) return false;
        if ( haveScreen(id)) return false;
        auto* entry = new ScreenEntryExternal<T>(screen, id);
        entry->instance = screen;
        addScreenEntry(entry);
        if ( begun ) screen->begin();
        return true;
    }

    /**
     * @brief Register a managed screen (ScreenManager will create and delete the screen)
     * 
     * @details Usage: `screenManager.registerScreen<SettingsScreen>(20);`
     * 
     * @tparam T 
     * @param id A uint8_t used to identify this screen
     */
    template<typename T>
    bool registerScreen(ScreenId id) {
        if ( id == 0 ) return false;
        if ( haveScreen(id)) return false;
        auto* entry = new ScreenEntryManaged<T>(id);
        addScreenEntry(entry);
        return true;
    }

    /**
     * @brief Set the FPS (Frames Per Second) for draw() to be called (default is 10)
     * 
     * @param fps 1-255 FPS. 0 means `IManagedScreen::draw()` will never be called.
     */
    void setFps(uint8_t fps) {
        if ( fps == 0 ) {
            displayRefreshMs = 0;
        } else {
            displayRefreshMs = (uint16_t)(1000/fps);
        }
    }

    /**
     * @brief Get the currently set Fps
     * 
     * @return uint8_t 
     */
    uint8_t getFps() {
        return (uint8_t)(displayRefreshMs/1000);
    }


    // /**
    //  * @brief Get a Screen object (not necessarily the current one)
    //  * 
    //  * @param id The id used to register the screen
    //  * @return IManagedScreen*  or nullptr if screen doesn't exist
    //  */
    // IManagedScreen* getScreen(ScreenId id);


    /**
     * @brief Return true if passed id is that of the current screen
     * 
     * @param id 
     * @return true 
     * @return false 
     */
    bool isCurrent(const ScreenId id);

    /**
     * @brief Return true if passed id is that of the previous screen
     * 
     * @param id 
     * @return true 
     * @return false 
     */
    bool isPrevious(ScreenId id);

    /**
     * @brief Get the current screen
     * 
     * @return IManagedScreen* or nullptr
     */
    IManagedScreen* getCurrent();


    /**
     * @brief Get the previous screen ID
     * 
     * @return Previous screen ID or 0
     */
    ScreenId getPreviousId();

    /**
     * @brief Check if a screen has been added and exists.
     * 
     * @param id The name or id used to register the screen
     * @return true Screen is available to get.
     * @return false Screen does not exist/
     */
    bool haveScreen(ScreenId id);

    /**
     * @brief Request a transition to a named screen. 
     * 
     * @details Will be passed to the screen routers (if any) for validation and/or redirect.
     * 
     * @param nextScreen 
     */
    void requestScreen(const ScreenId nextScreen);

    /**
     * @brief Request a screen transition.
     * 
     * @details The request will be passed to the screen routers (if any) for validation and/or redirect.
     * 
     * @param intent Can be Init, Back, Next, Auto or a specific screen name. Screen router resolves screen name.
     */
    void requestScreen(const TransitionIntent& intent);

    /**
     * @brief Add a new router for screen transitions. Routers are called in the order they are added. First to answer wins.
     * 
     * @param router 
     */
    void addRouter(ScreenRouter* router);

    /**
     * @brief Set the Initial Screen
     * 
     * @details By default the first registered screen is the init screen
     * 
     * @param id 
     * @return true Initial screen has been set
     * @return false Initial screen not set (is not a registered screen)
     */
    bool setInitialScreen(ScreenId id);


    private:    
    /**
     * @brief Get a ScreenEntry object (not necessarily the current one)
     * 
     * @param id The id used to register the screen
     * @return ScreenEntry*  or nullptr if screen hasn't been registered
     */
    ScreenEntry* getScreenEntry(ScreenId id);



    private:

    /**
     * @brief Add a ScreenEntry to the list.
     * 
     * @param entry 
     * @return * void 
     */
    void addScreenEntry(ScreenEntry* entry);



    /**
     * @brief User the ScreenRouters to decide which (or if) the next screen should become the current one.
     * 
     * @param intent 
     */
    void resolveTransition(const TransitionIntent& intent);


    ScreenEntry* current = nullptr;
    ScreenEntry* previous = nullptr;
    //ScreenEntry* pausedScreen = nullptr;
    TransitionIntent pendingIntent = {};
    ScreenId initialScreenId = 0;
    bool begun = false;


    uint16_t displayRefreshMs = 100;
    uint32_t lastDisplayRefresh = 0;



    // List management
    ScreenEntry* firstScreenEntry = nullptr;
    ScreenEntry* lastScreenEntry = nullptr;

    ScreenRouter* firstRouter = nullptr;
    ScreenRouter* lastRouter = nullptr;


};


#endif