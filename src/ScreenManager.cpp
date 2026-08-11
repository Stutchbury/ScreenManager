#include <stdint.h>
#include "ScreenManager.h"
#include "ScreenRouter.h"
#include "ScreenEntry.h"
#include "ScreenTransition.h"


using ScreenId = uint8_t;

ScreenManager::~ScreenManager() {
    ScreenEntry* se = firstScreenEntry;
    while (se) {
        ScreenEntry* nextScreenEntry = se->nextScreenEntry;
        delete se;
        se = nextScreenEntry;
    }
}

void ScreenManager::begin() { 
    begun = true;
    for (ScreenEntry* entry = firstScreenEntry; entry; entry = entry->nextScreenEntry) {
        if ( !entry->isManaged ) {
            entry->instance->begin();
        }
    }
    //pendingIntent = { TransitionIntentType::Init };
    requestScreen({TransitionIntentType::Init});
}

void ScreenManager::update(uint32_t nowMs) {
    if ( pendingIntent.type != TransitionIntentType::None ) {
        resolveTransition(pendingIntent);
        pendingIntent = {};
    }
    if ( displayRefreshMs == 0 || (uint32_t)(nowMs - lastDisplayRefresh) < displayRefreshMs ) return;
    lastDisplayRefresh = nowMs;
    if (current) current->instance->draw();
}


// IManagedScreen* ScreenManager::getScreen(ScreenId id) {
//     for (ScreenEntry* entry = firstScreenEntry; entry; entry = entry->nextScreenEntry) {
//         if ( entry->id == id ) {
//             entry->load();
//             return entry->instance;
//         }
//     }
//     return nullptr;
// }

bool ScreenManager::isCurrent(const ScreenId id) {
    if ( !current ) return false;
    return ( id == current->id );
}

bool ScreenManager::isPrevious(ScreenId id) {
    if ( !previous ) return false;
    return ( id == previous->id );
}

IManagedScreen* ScreenManager::getCurrent() {
    if (!current) {
        return nullptr; 

    }
    return current->instance;
}


ScreenId ScreenManager::getPreviousId() {
    if (!previous) return 0; 
    return previous->id;
}



bool ScreenManager::haveScreen(ScreenId id) {
    for (ScreenEntry* entry = firstScreenEntry; entry; entry = entry->nextScreenEntry) {
        if ( entry->id == id ) {
            return true;
        }
    }
    return false;
}

void ScreenManager::requestScreen(const ScreenId nextScreen) {
    requestScreen( { TransitionIntentType::Next, nextScreen } );
}

void ScreenManager::requestScreen(const TransitionIntent& intent) { 
    pendingIntent = intent; 
}

void ScreenManager::addRouter(ScreenRouter* router) { 
    router->prevRouter = lastRouter;
    router->nextRouter = nullptr;
    if (lastRouter) {
        lastRouter->nextRouter = router;
    } else {
        firstRouter = router;
    }
    lastRouter = router;
}

bool ScreenManager::setInitialScreen(ScreenId id) {
    if ( !haveScreen(id ) ) return false;
    initialScreenId = id;
    return true;
}



ScreenEntry* ScreenManager::getScreenEntry(ScreenId id) {
    for (ScreenEntry* entry = firstScreenEntry; entry; entry = entry->nextScreenEntry) {
        if ( entry->id == id ) {
            return entry;
        }
    }
    return nullptr;
}


void ScreenManager::addScreenEntry(ScreenEntry* entry) {
    //Do the intrusive list stuff 
    entry->prevScreenEntry = lastScreenEntry;
    entry->nextScreenEntry = nullptr;
    if (lastScreenEntry) {
        lastScreenEntry->nextScreenEntry = entry;
    } else {
        firstScreenEntry = entry;
        if ( !initialScreenId ) initialScreenId = entry->id;
    }
    lastScreenEntry = entry;
}



void ScreenManager::resolveTransition(const TransitionIntent& intent) {

    if ( !firstScreenEntry ) return; //No screens have been registered!

    ScreenId resolved = (intent.type == TransitionIntentType::Init || !current )
                        ? initialScreenId
                        : intent.requested;

    if ( firstRouter ) { //Have router(s) 
        for (ScreenRouter* router = firstRouter; router; router = router->nextRouter) {
            ScreenId redirect = router->resolveScreen(( current ?  current->id : 0 ), intent);
            if ( redirect ) {
                resolved = redirect;
                break;
            }                
        }
    }

    if ( !resolved ) return; //final sanity check neither intent or router gave a screen id
    // Find the screen in the ScreenEntries
    ScreenEntry* nextScreen = getScreenEntry(resolved);
    if ( !nextScreen ) return; // Screen not registered
    if (current) {
        if ( nextScreen == current ) return; // Already on this screen
        if ( !current->instance->end() ) {
            if ( intent.type != TransitionIntentType::Force ) return; //Check if we can exit current screen
        }
        current->unload();
        previous = current;
    }
    current = nextScreen;
    current->load()->start();
}

