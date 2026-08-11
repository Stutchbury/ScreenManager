#ifndef SCREEN_TRANSITION_H
#define SCREEN_TRANSITION_H

#include <stdint.h>


/**
 * @brief Types of screen transition
 * 
 */
enum class TransitionIntentType : uint8_t { 
    None, ///< No transition, stay on current screen
    Auto, ///< The IScreenRouter decides which screen to transition to.
    Next, ///< The IScreenRouter can accept a provided screen name or redirect
    Back, ///< The IScreenRouter determines what the previous screen should be
    Init, ///< Only the first IScreenRouter can resolve this.
    Force ///< Force a screen change. Current screen's end() will be called but the result ignored. Use with caution.
};

/**
 * @brief The type of transition and an optional requested screen name
 * 
 */
struct TransitionIntent {
     /**
      * @brief Must have a contructor to use defaults in C++11 because 'aggregate` \_O_/
      * 
      */
    constexpr TransitionIntent( TransitionIntentType t = TransitionIntentType::None, uint8_t r = 0)
        : type(t), 
          requested(r) 
        {}

    TransitionIntentType type; ///< The requested transition type
    uint8_t requested;  ///< Explicit requested ScreenId
};

#endif
