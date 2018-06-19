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

### Genetic Operations

#### Selecting parents strategy

The selection of the parents in each step of the evolution of the population is the tournment one. It consist in 
take t individuals randomly, and choosing the fittest one of this t elements. And for the other parent, using the
same strategy again.

The parameter t is one of most determinants on the problem as we will see with the results. The reason is 
that if t is big, then we are being very exploitative, and in very few steps all the population will have the same 
“genetic sequence”. Contrary, if t is small we are being very explorative, this option will give us the bests 
solutions, as we will see later on.

#### Crossover

The crossover that I have used is one that is position based. We choose posCr positions randomly. First we 
copy from parent one tho the child this positions and information. And then we complete the information from 
the parent two filling the blanks of the offspring orederly.

This will be an other parameter of the problem, the number of positions that the parent 1 pass to the son.

#### Mutation

The mutation is simple, just take two components randomly and interchange them.
There is a paramenter to control the proportion of mutating, it is a number pm from 0 to 1, and it means that 
every member of offspring is mutated with probability pm.

Since we are taking two positions randomly, in the case that the two positions are the same, we interchange 
this position with the 10 th one. This is an arbitrary fact that may add some randomness to the mutation, and 
to the altgorithm.

An important observation is that looking only at the results, the program give the bests results without using 
mutation, i.e. Initialazing the parameter pm as 0.

Another observation is that if we put positive mutation probability, we have to make large the parameter of 
the parets selection, increasing the pm, we are becoming more explorative, and to compensate this fact we 
back to more exploitative increasing the parent selection parameter. 

### Renovation of population strategy

The strategy for renew the population is simple, I create n new individuals from the n originals, and in each 
step the whle population is replaced for the new one.

### Program

I have wrote some functions to program all this genetic operations and the important one that is ga, the 
genetic algorithm. I have seen that the result of the best obtained element changes a lot with changing the 
parameters.

To solve that, I have created a serial of iterations where we are computing the GA for each value of each 
parameter, acting in a certaing range that I have tried in previous testings.

```
int    m       =  37;        //dim of the problem, number of cities
int    n       =  250 - 550; //population (required to be an even number) 
int    t       =  2 - 11;    //number of elements to do the parents selec 
int    posCr   =  13 - 23;   //numb of positions choosed in crossover 
int    maxiter =  100;       //number max of iterations
double mutq    = 0.1;        //probability of mutation
```

This is the piece of code that iterates and Compute a solution for each combination of the parameters acting 
in the range explained before.
