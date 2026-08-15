# ScreenManager

A lightweight C++ library for managing application screens, navigation, transitions and display timing.

ScreenManager is designed for applications composed of multiple screens or views, on embedded systems/microcontrollers. It provides a simple, predictable framework for screen navigation without depending on any graphics, widget or input library.

Rather than having screens create, destroy or directly navigate to one another, ScreenManager can act as the central coordinator, keeping screens independent and making application flow easier to understand and maintain.

> **ScreenManager is GFX agnostic** - it has no dependency on a graphics library. It does not draw anything itself or dictate *how* a screen is rendered - it only determines *when* `draw()` is called, according to the configured display refresh rate.

So what *does* it do? ScreenManager manages the lifecycle and activation of screens. When a screen becomes current, its `start()` method provides the opportunity to acquire the application's input context, ensuring that user interactions (touch, buttons etc) are directed to the current screen. Additionally, optional ScreenManager Routers provide a powerful way to manage transitions between screens or create history trees & wizards.

---

## Features

* Register screens with a simple numeric `ScreenId` (must be > 0)
* Transition between screens by ID or transition intent (Next, Prev etc).
* Optional screen routers for navigation, validation and redirection.
* Support for both application-owned and ScreenManager-owned screens.
* Lazy creation and destruction of infrequently used screens.
* Track the current and previous screen.
* Suitable for resource-constrained microcontrollers or large embedded apps.

---

## Why ScreenManager?

Many embedded applications consist of a collection of independent screens but as applications grow, navigation logic often becomes distributed throughout the codebase, with screens directly creating or switching to other screens.

`ScreenManager` centralises that responsibility.

Screens simply request a transition. The (optional) Routers registered with  `ScreenManager` decide where to go, whether the transition is allowed, and ensures the appropriate screen is started.

This keeps screens independent and makes navigation easier to modify, test and maintain.

For me, this is the 'missing piece' of embedded app development -  I cannot find another library that does this. To quote Eric S. Raymond's [Lesson1:](http://www.catb.org/~esr/writings/cathedral-bazaar/):
> Every good work of software starts by scratching a developer's personal itch.


---

## What is a Screen?

In the `ScreenManager` library, a 'Screen' encapsulates the behaviour associated with one logical part of the application - for example a home screen, settings screen etc. `ScreenManager`'s primary role is to provide the ability to manage the input context so user actions are directed to the current screen.

A Screen therefore primarily fulfils the role of a controller for a single application state. When a screen is current (typically the screen being displayed), it defines how the application responds to user interactions, and may coordinate updates to the application's state and request transitions to other screens. A screen may also be responsible for rendering its own view, but this is entirely optional.

Rather than just allowing Screens to navigate directly to one another, they can request transitions. `ScreenManager` then coordinates the navigation by consulting any registered routers, managing screen lifetimes where necessary, and activating the next screen.

ScreenManager deliberately places no restrictions on how a Screen is rendered or how input events are delivered. A Screen may use any graphics library, widget framework or input system. The primary responsibility of each Screen is to define how the application behaves while it is current.

Although the library has been written with displayed Screens in mind, it can also be used as *without* a display as a very powerful *context* manager.

---

## Screen Ownership

ScreenManager supports two ownership models.

### External Screens

The application owns the screen object and is responsible for its lifetime.

```cpp
HomeScreen home;
StatusScreen status;

manager.registerScreen(HOME_SCREEN, &home);
manager.registerScreen(STATUS_SCREEN, &status);
```

The ScreenManager keeps a reference to each screen but never creates or destroys it.

This is ideal for screens that are shown frequently or should remain resident throughout the application's lifetime.

### Managed Screens

The ScreenManager owns the screen object.

```cpp
manager.registerScreen<SettingsScreen>(SETTINGS_SCREEN);
manager.registerScreen<KeyboardScreen>(KEYBOARD_SCREEN);
```

Managed screens are created automatically when required and are destroyed when longer active. They must have a default constructor.

This allows rarely used screens with a larger memory footprint to consume RAM only while active.

---

## Screen Routers

Screen Routers provide an *optional* mechanism for implementing application specific navigation logic.

Without routers, a requested transition simply activates the requested screen.  While this is sufficient for many applications, others require navigation decisions that depend on the current screen or the application state.

For example, a transition may need to be redirected to a login screen, prevented while a machine is running, or determine which screen "Back" or "Next" should actually navigate to.

Rather than embedding this logic within individual screens, Screen Routers centralise navigation decisions and keep screens independent of one another. Screens simply request a transition; routers decide whether that transition should proceed, be redirected, or be rejected.

Routers are consulted in the order they are registered. The first router to return a valid destination determines the next screen. If no router modifies the request, the original transition proceeds unchanged.

Applications that do not require custom navigation logic can simply omit routers entirely.

A router is given the opportunity to examine every requested transition before it occurs.

A router may:

* allow the requested transition
* redirect to another screen
* reject the transition

Routers are called in the order they are registered. The first router that returns a valid destination determines the next screen.

---

## Installation


### Arduino IDE

Install ScreenManager via the Arduino IDE Libary Manager.

### PlatformIO

Add the following to your `platformio.ini` file:

```
lib_deps = 
	stutchbury/ScreenManager@^1.0.0
```
Or search and install from PlatformIO's Library Manager.

---

## Support

For further information, please see the [full Doxygen generated API docs](https://stutchbury.github.io/ScreenManager/docs/api/) which includes the Developer Notes.

Feedback, [bug reports](https://github.com/Stutchbury/ScreenManager/issues) and pull requests are very welcome or chat on [Discord](https://discord.gg/GDcEcWPKKm) if you have any questions.


---

## Basic Usage

```cpp
ScreenManager manager;

HomeScreen home;
StatusScreen status;

void setup() {
    manager.registerScreen(HOME_SCREEN, &home); //ScreenId must not be 0
    manager.registerScreen(STATUS_SCREEN, &status); //ScreenId must not be 0
    manager.registerScreen<SettingsScreen>(SETTINGS_SCREEN); //ScreenId must not be 0

    manager.begin();
}

void loop() {
    manager.update(millis()); //Must be called before accessing `getCurrent()`
}
```

Navigate directly to another screen (router is optional):

```cpp
manager.requestScreen(SETTINGS_SCREEN);
```

Or request a transition by intent (requires a router to resolve the intent):

```cpp
manager.requestScreen({ TransitionIntentType::Back });
```

---


## Design Principles

`ScreenManager` has:

* Small memory footprint.
* Predictable execution.
* No dependency on graphics libraries.
* No dependency on input libraries.
* No dependency on a particular framework.
* Suitable for AVR, STM32, ESP32, RP2040, Teensy and other platforms.
* Straightforward to unit test.
* Minimal public API.

## Architecture

`ScreenManager` deliberately separates responsibilities.

* **Application**  -  creates and/or registers Screens and can requests transitions.
* **ScreenManager**  -  manages registration, navigation and Screen lifetime.
* **Screens**  -  implement application behaviour and can request transitions.
* **Routers**  -  optionally influence navigation decisions.

Keeping these responsibilities separate allows implemention behaviour within a Screen while `ScreenManager` coordinates the overall application flow.


## License

GPL v2
