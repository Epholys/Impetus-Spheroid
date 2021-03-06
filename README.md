
![Logo](../master/media/images/logo.png)

## A rapid-paced game about spheroid with high momentum and a target

## GET IT [HERE](http://epholys.itch.io/impetus-spheroid)

## Project on Hold ; Looking for a maintainer

This project is currently on hold... but there's still a lot to do! If you want to take over this project, send me a little email at epholys [AT] gmail [DOT] com :).

## Hacking

You'll only need two things :

* A C++11-compliant compiler like ![gcc](https://gcc.gnu.org/)

* SFML 2.1 or later, available ![here](http://www.sfml-dev.org/download/sfml/2.1/) and with some excellent setup tutorials ![there](http://www.sfml-dev.org/tutorials/2.1/).

If you're on GNU/Linux and on a Debian-like system, you can then just type 

```
$ sudo apt-get install g++ make libsfml-dev
$ git clone https://github.com/Epholys/Impetus-Spheroid
$ cd Impetus-Spheroid
$ make
```

and you'll have a game freshly installed from source!


### Trivia
* __What are the naming convention?__ I use the [Java naming convention](https://en.wikipedia.org/wiki/Naming_convention_%28programming%29#Java), as I think it makes sense in OOP
* __Why do you (almost) use no encryption for you data files? The players can cheat!__ Let them cheat then! If it was a competitive game, I'll understand the concerns, but here, they may learn two or three things doing so =).
* __Do you know what 'MVC' means?__ I didn't know its existence at the creation of this project, and it will take a long time to refactor everything into this principle. As my personal deadline is September 2015, I don't have enough time.


### TODO

* Long-Term Thinking:
  * Sound
  * Android Port
  * Graphics

* Gameplay
  * Tutorial
  	* Full-fledged tutorial for begginers at the first game _NEEDS MORE TESTING_
  * Balancing
  	* High-ball points value frequency
  	* Limits the manual fire rate
  	* Cheaper bonuses _NEEDS MORE TESTING_
  * Add some indications (objective, points, money, etc)
  	* Replace the grey cross-hair by something more visible(?)
	* More explicit Events: Icons, Other Graphic Effects...
	  * Stop Time
		* Awesome effect like radial blur(?)
	* More explicit PowerUps: Icons, Particule Effects, other graphics Effects
  * Make the game more addictive
	* Rewarding: sound
  * Add some content
  	* More Events
	  * Spinning Target?
	  * Crazy/Teleporting(?) Target
	* More Power-Ups
	  * Unlock unlimited manual fire
	  * Heavy Balls?
	* More Improvement
	  * Improves power-ups?
	* "Variety of environments and targets with different behaviors. Different weapons. In some levels an ability to move."

* User Experience
  * Transitions beetween phases
  * Close Menus, restart game with buttons **DONE!**

* Graphics
  * Windows vs GNU/Linux Alpha problem
  * MORE JUICE
  
* Sound (just little sound effects)

* Bugs
  * Crashing Windows
  * Auto-adjust the speed of the next balls left bar

* Other
  * Make a credit page

* Clean-up
  * Fusion addWindWorld and reverseGravWorld in EventFunctions.cpp

* (Heavy) Refactoring
  * Replace every convenient ```const std::string&``` by the ResourceHolder managment
  * Separate DifficultyManager into two classes
  * Refactor TransitionDeque to have one an only Transition?
	
* Optimization (if necessary)
  * Search where are the bottlenecks
  * ecs/EntityManager's entityExists() / componentExists() (called 504,360 times for a small game!).
The function itself is optimized, but it is called to many times...
**Solution:** Make a quad-tree for collision detection _WIP (It isn't called unnecessarily now)_
  * Reduce all the Balls' draw calls, as one should be enough: they are almost all the same (minus the color)!
  * For smooth balltrail: adapt duration and frequency of particles w/ ball's velocity
  * Particles: adapt particle frequency w/ number of balls on screen
  * ECS: Optimize to be more cache-friendly



----------------------------------------------------------------------------------------------------------------------------
BLACK HOLE ERROR
Warning: noted but unhandled ioctl 0x6458 with no size/direction hints.
==20497==    This could cause spurious value errors to appear.
==20497==    See README_MISSING_SYSCALL_OR_IOCTL for guidance on writing a proper wrapper.
ImpetusSpheroid.debug: Target.cpp:179: void Target::updateIndicators(Time): Assertion `projectileComp && targetComp' failed.
==20497== 
==20497== Process terminating with default action of signal 6 (SIGABRT)
==20497==    at 0x5B6C1C7: raise (raise.c:55)
==20497==    by 0x5B6DE29: abort (abort.c:89)
==20497==    by 0x5B650BC: __assert_fail_base (assert.c:92)
==20497==    by 0x5B65171: __assert_fail (assert.c:101)
==20497==    by 0x45D7C0: Target::updateIndicators(sf::Time) (Target.cpp:179)
==20497==    by 0x45CDE9: Target::update(sf::Time) (Target.cpp:80)
==20497==    by 0x463289: World::update(sf::Time) (World.cpp:357)
==20497==    by 0x496AC4: StateGame::update(sf::Time) (StateGame.cpp:35)
==20497==    by 0x49F285: StateStack::update(sf::Time) (StateStack.cpp:53)
==20497==    by 0x48D031: Application::update(sf::Time) (Application.cpp:95)
==20497==    by 0x48CFE1: Application::run() (Application.cpp:87)
==20497==    by 0x4A1FD1: main (main.cpp:9)


To correct this mistake, see src/core/EventFunctions.cpp:490
