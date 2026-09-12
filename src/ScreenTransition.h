#ifndef SCREEN_TRANSITION_H
#define SCREEN_TRANSITION_H

#include <stdint.h>

using ScreenId = uint8_t;

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
 * @brief The type of transition and an optional requested ScreenId
 * 
 */
struct TransitionIntent {
    /**
     * @brief Must have a constructor to use defaults in C++11 because 'aggregate` \_O_/
     * 
     * @param TransitionIntentType Transition intent types are:
        -  `None`  - No transition, stay on current screen
        -  `Auto`  - The IScreenRouter decides which screen to transition to.
        -  `Next`  - The IScreenRouter can accept a provided screen name or redirect
        -  `Back`  - The IScreenRouter determines what the previous screen should be
        -  `Init`  - Only the first IScreenRouter can resolve this.
        -  `Force` - Force a screen change. Current screen's end() will be called but the result ignored. Use with caution.
     * 
     * @param ScreenId - An explicitly requested `ScreenId` (alias for a `uint8_t`)
     */
    // cppcheck-suppress noExplicitConstructor ; because I want to use `= {}`
    constexpr TransitionIntent( TransitionIntentType intentType = TransitionIntentType::None, uint8_t ScreenId = 0)
        : type(intentType), 
          requested(ScreenId) 
        {}

    TransitionIntentType type; ///< The requested transition type
    ScreenId requested;  ///< Explicit requested ScreenId
};

#endif
