#ifndef IMANAGED_SCREEN_H
#define IMANAGED_SCREEN_H

#include <stdint.h>

 /**
 * @brief Interface for a screen that can be managed by ScreenManager. A 'Screen' is a combination of the 'View' part of MVC (Model, View, Controller) pattern with a portion of 'Controller mixed in.
 * 
 * @details Widgets and Icons are the true 'View' components but they are called from a Screen.
 * 
 */
//class IManagedScreen : public BaseWidget {
class IManagedScreen {

    public:

    virtual ~IManagedScreen() = default;

    /**
     * @brief Called after a screen is created but before `start()`
     *  
     * @details An `IManagedScreen`s `begin()` will be called as follows:
     * For external screens:
     * - From `ScreenManager::begin()` or
     * - On registration if `ScreenManager::begin()` has already been called.
     * For managed screens:
     * - Immediately after being created.
     * 
     */
    virtual void begin() = 0;

    /**
     * @brief Called by `ScreenManager` when this screen is set active. This is where the screen should become the app's active context.
     * 
     */
    virtual void start() = 0;

    /**
     * @brief Called at defined FPS only whan screen is active
     */
    virtual void draw() = 0;

    /**
     * @brief Called by `ScreenManager` before the next screen is set active
     * 
     * @return true: Progress to next screen
     * @return false: Stay on this screen (ignored if `TransitionIntent is `Force`)
     */

    virtual bool end() = 0;

    /**
     * @brief Reserved for future use. Pause a screen and hand context to an overlay
     * 
     */
    virtual void pause() {}

    /**
     * @brief Reserved for future use. Resume a screen (screen should re-acquire context)
     * 
     */
    virtual void resume() { 
        start(); 
    }


};



#endif