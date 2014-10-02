# Impetus Spheroid
## A rapid-paced game about spheroid with high momentum and a target

### TODO

* Global
** TODO Clean code in order to be sure it isn't necessary to check if a dynCast<> is correct (currently, paused objects can lead some EntityManager's function to return some kind of 'empty' container which contains, I presume, some nullptr that leads to SEGFAULT)

* Utility
** TODO Add some functionnality to Time (just redirection of sf::seconds)

* ECS
** Add all the "pause" mechanics
*** TODO Decide wether the pause system must accumulate

* TODO *TEST EVERYTHING*

* Engine
** PhysicEngine
*** TODO catch the exception in addEntity()