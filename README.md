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
* Global
  * Graphics
  * Sound
  * Clean-up
  	* core/Event system
	* World
	* Market
	* Inventory
	* PowerUp
  * (Heavy) Refactoring
  	* FontHolder (or a general ResourceHolder<>)
	* Separate DifficultyManager into two classes
	
* Optimization (if necessary)
  * Search where are the bottlenecks
  	* ecs/EntityManager's entityExists() / componentExists() (called 504,360 times for a small game!).
The function itself is optimized, but it is called to many times...
**Solution:** Make a quad-tree for collision detection
  * ECS: Optimize to be more cache-friendly


### Trivia
* __What are the naming convention?__ I use the [Java naming convention](https://en.wikipedia.org/wiki/Naming_convention_%28programming%29#Java), as I think it makes sense in OOP
* __Why do you (almost) use no encryption for you data files? The players can cheat!__ Let them cheat then! If it was a competitive game, I'll understand the concerns, but here, they may learn two or three things doing so =).
* __Do you know what 'MVC' means?__ I didn't know its existence at the creation of this project, and it will take a long time to refactor everything into this principle. As my personal deadline is September 2015, I don't have enough time.
