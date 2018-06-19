# Travelling Sales Problem 

## Genetic Algorithm

A traveling salesman must visit all cities appearing in the table of distances between
European cities starting and ending in Barcelona. An optimum route must be computed
using GA, according to driving distance.

### Contents
* Data structure and particular stuff
* Creating a whole population
* Genetic Operations
* Renovation of population strategy
* Program
* Delivered files
* Results and Conclusions

### Data structure and particular stuff
#### Individuals
Since we have a list of 37 cities, and we have to start and end in Barcelona, I have used this restriction as
follows:
* 1. Each individual of the population is represented by an array of 37 components and all of them starts with
the number 3, that is the number corresponding to Barcelona.
* 2. Each component of the array represents the number of the city in the alphabetical ordered list of the cities,
from 0 ->Amsterdam to 36->Zurich
* 3. All the numbers from 0 to 36 must appear in each individual and consequently, no repeated numbers are
accepted.

This restrictions are important and define the problem, in consequence is important to taking it account in all 
the manipulations of the members of the population. 
Fitness Function

The fitness function is the function that we want to minimize, and this is the main goal of the problem.
The function evaluates the distance of the way that is represented in each individual, and it is: if p is an 
individual, then **p = (3,21,23,2,6,.........,4,17,16)** , then the function F act as follows;
F(p)= dist(3,21)+dist(21,23)+dist(23,2)+.......+d(17,16)+d(16,3)
Normal distance computed. Observe that the last summand represents the distance of coming back to 
Barcelona, this information is not stored in the individual because is the same for all of them, but we have to 
add it to get the real distance from Barcelona and close the route. 
