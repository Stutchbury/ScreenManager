#ifndef SCREEN_ROUTER_H
#define SCREEN_ROUTER_H

#include <stdint.h>
#include "ScreenTransition.h"

/**
 * @brief Abstract class that resolves screen transitions (ie moving Next, Back or to a new screen). Called by ScreenManager if requestScreen is set.
 * 
 */
class ScreenRouter {
public:

    public:

    virtual ~ScreenRouter() = default;

    /**
     * @brief Resolve a screen transition intent's validity. Can return 0 or the the requested id (no objection), or a redirect id or the current id.
     * 
     * @param current 
     * @param intent 
     * @return std::string 
     */
    virtual uint8_t resolveScreen(uint8_t current, const TransitionIntent& intent) = 0;

    private:
    friend class ScreenManager;

    ScreenRouter* nextRouter = nullptr;
    ScreenRouter* prevRouter = nullptr;


                                
};

#endif
