![Logo](../master/media/images/logo.png)

## A rapid-paced game about spheroid with high momentum and a target


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

* Gameplay
  * Add some indications (objective, points, money, etc)
  	* More explicit beggining: Cannon sprite w/ aiming visualized (__DONE!__), cross-hair for mouse's cursor (__DONE!__) (, maybe arc trajectory __DONE!__ for the first shot and maybe a glowing target __DONE!__).
	* Alternate beetween cross-hair and "normal" cursor __DONE!__ | Replace the grey cross-hair by something more visible(?)
	* More explicit Events: Icons, Particule Effects, other Graphic Effects...
	* More explicit PowerUps: Icons, _selected_ visualization __DONE!__, Particule Effects, other graphics Effects
	* More explicit scoring: Little indication each time a ball touch the target (+1, +10, +100, ...) __DONE!__
  * Make the game more addictive
  	* Permanent Power-Ups __DONE!__
	* Unlocking Power-Ups __DONE!__
	* Achievement? Ranking System (Bronze, Silver, Gold, etc)?
	* Rewarding: animation __DONE!__? sound?

* Graphics
  * Particle Effects (for Balls, PowerUps, Events, etc...)
  	* Base (ParticleSystem, ParticleEmitter, Emitter) __DONE!__
	* Affector __DONE!__
	* Apply
	  * Ball _DONE!_
	  * Events _DONE!_
	  * PowerUps _DONE!_
  * Transition
  	* Transition __DONE!__
	* TransitionFunctions __DONE!__
	* TransitionDeque __DONE!__
	* Apply Transition Deque to Cannon (with a new class?) and to Difficulty Manager __DONE!__

* Sound (just little sound effects)

* Bugs
  * Prevent balls from being created when an other ball is in the generation post. __DONE__
  * World's wall mess-up __DONE__

* Clean-up
  * Fusion addWindWorld and reverseGravWorld in EventFunctions.cpp

* (Heavy) Refactoring
  * FontHolder (or a general ResourceHolder<>) __DONE!__
  	* Now replace every convenient ```const std::string&``` by the ResourceHolder managment
  * Transition from simple position movement to complex Transformation __DONE!__
  * Separate DifficultyManager into two classes
  * Remove gui::EffectColor<T> if it is only used for Market, you can make a little utility function instead of this behemoth __DONE!__
  * Refactor TransitionDeque to have one an only Transition?
	
* Optimization (if necessary)
  * Search where are the bottlenecks
  * ecs/EntityManager's entityExists() / componentExists() (called 504,360 times for a small game!).
The function itself is optimized, but it is called to many times...
**Solution:** Make a quad-tree for collision detection
  * Reduce all the Balls' draw calls, as one should be enough: they are all the same (minus the color)!
  * For smooth balltrail: adapt duration and frequency of particles w/ ball's velocity
  * ECS: Optimize to be more cache-friendly
  * Use a single draw call for all the balls, as it is done for Particles


### Trivia
* __What are the naming convention?__ I use the [Java naming convention](https://en.wikipedia.org/wiki/Naming_convention_%28programming%29#Java), as I think it makes sense in OOP
* __Why do you (almost) use no encryption for you data files? The players can cheat!__ Let them cheat then! If it was a competitive game, I'll understand the concerns, but here, they may learn two or three things doing so =).
* __Do you know what 'MVC' means?__ I didn't know its existence at the creation of this project, and it will take a long time to refactor everything into this principle. As my personal deadline is September 2015, I don't have enough time.
