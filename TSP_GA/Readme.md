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
* Each individual of the population is represented by an array of 37 components and all of them starts with
the number 3, that is the number corresponding to Barcelona.
* Each component of the array represents the number of the city in the alphabetical ordered list of the cities,
from 0 (Amsterdam) to 36 (Zurich).
* All the numbers from 0 to 36 must appear in each individual and consequently, no repeated numbers are
accepted.

This restrictions are important and define the problem, in consequence is important to taking it account in all 
the manipulations of the members of the population. 
Fitness Function

The fitness function is the function that we want to minimize, and this is the main goal of the problem.
The function evaluates the distance of the way that is represented in each individual, and it is: if p is an 
individual, then *p = (3,21,23,2,6,.........,4,17,16)* , then the function F act as follows;

  *F(p)= dist(3,21)+dist(21,23)+dist(23,2)+.......+d(17,16)+d(16,3)*

Normal distance computed. Observe that the last summand represents the distance of coming back to 
Barcelona, this information is not stored in the individual because is the same for all of them, but we have to 
add it to get the real distance from Barcelona and close the route. 

### Creating a whole population

The size of the population, as we will see later on the results, is one of the trascendent parameters of the 
problem. We have started for n = 100 individuals, and to get different results, we have incresed it up to 550 
or 600 individuals.
On the explanation of the GA (Genetic Algorithm), the population must be initialized randomly. To take into 
account of the restriction of individuals, I have created a specific function in the code of the program in order 
to get this:
For each individual p, we put first of all p(0) = 3 , this means starting at Barcelona.
We conside a list of aux = (0,1,2,4,5,6............36) that has 36 elements, observe the abscence of 3.
we take a number a randomly from 0 to 36, and put p(1) = aux(a), this is the a-th component of aux.
Then we delete this component of the aux list, by copying the laters components from a one step 
forward, and now we start again by taking a number randomly from 0 to 35, and doing this until we 
get the whole individual. 
With this we ensure to not repeat any number in the result list. 
It is harder to explain with words than show the piece of the algorithm:
    ```
    // m is 37 and the resultant individual is v[i]
    for(i=0;i<3;i++) aux[i] = i; 
      for(i=3;i<m-1;i++) aux[i] = i+1; 
        for(i=1;i<m;i++) 
        { 
          a = random()%(m-i); 
          v[i] = aux[a]; 
          for(j=a;j<m-2;j++) aux[j] = aux[j+1]; 
         }
    ```
