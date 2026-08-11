#pragma once

#include "IManagedScreen.h"


using ScreenId = uint8_t;

/**
 * @brief A registered screen. Only ever accessed by ScreenManager.
 * 
 */
class ScreenEntry {
    friend class ScreenManager;

    public: 
    
    virtual ~ScreenEntry() = default;
    
    protected:

    ScreenEntry* nextScreenEntry = nullptr; ///< The next screen in the intrusive list
    ScreenEntry* prevScreenEntry = nullptr; ///< The previous screen in the intrusive list

    IManagedScreen* instance = nullptr; ///< The reference to an instance of an external or managed IManagedScreen - 
    ScreenId id = 0;    ///< The identified given during registration


    bool isManaged = true;  ///< True = Managed, false = External
    bool isOverlay = false; ///< Reserved for future use

    /**
     * @brief Create an IManagedScreen if required and return it.
     * 
     * @return IManagedScreen* 
     */
    virtual IManagedScreen* load() = 0;

    /**
     * @brief Delete an IManagedScren if required.
     * 
     */
    virtual void unload() = 0;
};

/**
 * @brief A registration entry for an External IManagedScreen
 * 
 * @tparam T 
 */
template<typename T>
class ScreenEntryExternal final : public ScreenEntry {
    friend class ScreenManager;
    
    public:

    /**
     * @brief Construct a ScreenEntry for an External IManagedScreen
     * 
     * @param screen 
     * @param id 
     */
    explicit ScreenEntryExternal(T* screen, ScreenId id) {
        this->instance = screen;
        this->id = id;
        this->isManaged = false;
    }

    protected:
    IManagedScreen* load() override {
        return this->instance;
    }

    void unload() override {
    }
};

/**
 * @brief A registration entry for a Managed IManagedScreen
 * 
 * @tparam T 
 */
template<typename T>
class ScreenEntryManaged final : public ScreenEntry {
    friend class ScreenManager;
    
    public:

    /**
     * @brief Construct a ScreenEntry for Managed IManagedScreen
     * 
     * @param id 
     */
    //explicit ScreenEntryManaged<T>(ScreenId id) { 
    explicit ScreenEntryManaged(ScreenId id) { 
        this->id = id;
        this->isManaged = true;
    }

    ~ScreenEntryManaged() override {
            this->unload();
    }

    protected:
    IManagedScreen* load() override {
        if (!this->instance) {
            this->instance = new T;
            this->instance->begin();
        }
        return this->instance;
    }

    void unload() override {
        delete this->instance;
        this->instance = nullptr;
    }
};

