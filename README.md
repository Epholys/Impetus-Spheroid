
![Logo](../master/media/images/logo.png)

## A rapid-paced game about spheroid with high momentum and a target

## GET IT [HERE](http://epholys.itch.io/impetus-spheroid)

## Hacking

You'll only need two things :

* A C++11-compliant compiler like ![gcc](https://gcc.gnu.org/)

* SFML 2.1 or later, available ![here](http://www.sfml-dev.org/download/sfml/2.1/) and with some excellent setup tutorials ![there](http://www.sfml-dev.org/tutorials/2.1/).

If you're on GNU/Linux, you can then just type 

```
$ sudo apt-get install g++ make libsfml-dev
$ git clone https://github.com/Epholys/Impetus-Spheroid
$ cd Impetus-Spheroid
$ make
$ ./Impetus-Spheroid
```

and you'll have a game freshly installed from source!

### TODO

* URGENT OPTIMISATION: ecs::EntityManager::getObjectTable()/getComponent()/etc called too many times!
  * Create operations on objectTable(?)
  * Cache common table as vectors

* Gameplay
  * Tutorial
  	* Full-fledged tutorial for begginers at the first game __NEEDS TESTING__
  * Balancing
  	* Cheaper bonuses _NEEDS TESTING_
	* Faster base auto-fire __DONE!__
  * Add some indications (objective, points, money, etc)
	* Replace the grey cross-hair by something more visible(?)
	* More explicit Events: Icons, Other Graphic Effects...
	  * Gravity Changes
	  	* Arrows __DONE!__
	  * Stop Time
	  	* Slowing balls __DONE!__
		* Hourglass Icons __DONE!__
		* Awesome effect like radial blur(?)
	  * Crazy Cannon __DONE!__
	* More explicit PowerUps: Icons, Particule Effects, other graphics Effects
  * Make the game more addictive
	* Rewarding: sound?
  * Add some content
  	* More Events!
	  * Spinning Target
	  * Morphing Target
	  * Crazy/Teleporting(?) Target
	* More Power-Ups!
	  * Several ball / shot?
	  * Heavy Balls
	* More Improvement!
	  * Improves power-ups?
	* "Variety of environments and targets with different behaviors. Different weapons. In some levels an ability to move."

* User Experience
  * Close Menus, restart game with buttons

* Graphics
  * TargetSun's Highlight more visible __DONE!__
  * Balls' Trail particles duration longer __DONE!__
  * Round particles for "Shards" __DONE!__
  
* Sound (just little sound effects)

* Bugs
  * Auto-adjust the speed of the next balls left bar
  * Ball lost when LostFocus Event __DONE!__
  * Crash when CancelEvent is called twice in a execution __DONE!__

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

### Trivia
* __What are the naming convention?__ I use the [Java naming convention](https://en.wikipedia.org/wiki/Naming_convention_%28programming%29#Java), as I think it makes sense in OOP
* __Why do you (almost) use no encryption for you data files? The players can cheat!__ Let them cheat then! If it was a competitive game, I'll understand the concerns, but here, they may learn two or three things doing so =).
* __Do you know what 'MVC' means?__ I didn't know its existence at the creation of this project, and it will take a long time to refactor everything into this principle. As my personal deadline is September 2015, I don't have enough time.
