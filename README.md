![Logo](../master/media/images/logo.png)

## A rapid-paced game about spheroid with high momentum and a target


## Hacking

You'll only need two things :

* A C++11-compliant compiler like ![gcc](https://gcc.gnu.org/)

* SFML 2.1 or later, available ![here](http://www.sfml-dev.org/download/sfml/2.1/) and with some excellent setup tutorials ![there](http://www.sfml-dev.org/tutorials/2.1/).

If you're on GNU/Linux, you can then just type 

```
make install
```

and you'll have a game freshly installed from source!

### TODO
* Global
  * Graphics
  * Sound
  * Clean-up everything!
  	* That also mean replace all the std::map of Inventory by two std::map with one with special struct instead of several map for nothing

* Optimization (if necessary)
  * Search where are the bottlenecks
  * ECS: Optimize to be more cache-friendly
