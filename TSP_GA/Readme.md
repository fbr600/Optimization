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

```
for(n=250;n<550;n+=10) 
{
  p = (int**)malloc(n*sizeof(int*)); 
  for(i=0;i<n;i++) 
    p[i] = (int*)malloc(m*sizeof(double)); 
  for(t=2;t<11;t+=1) 
    for(posCr=13;posCr<23;posCr+=1) 
    { 
      //Inizialization the population randomly 
      for(i=0;i<n;i++) 
        Initrand(p[i]); 
      ga(p,best,a,fres); 
      b=Dist(a,best); 
      if(b <min) 
      { 
        min = b; 
        for(i=0;i<m;i++) 
          rebest[i] = best[i]; 
      } 
      fprintf(fres,"n=%3d t=%2d pC=%2d D= %5d ", n,t,posCr,b); 
      for(i=0;i<m;i++) 
        fprintf(fres,"%2d ",best[i]); 
      fprintf(fres,"\n"); 
    } 
  free(p); 
}
```

I've wrote all the solutions in a file, and the program itselfs print on the console the best solution.
At the end we obtain a long list of ways as the following:

```
n=350 t= 8 pC=13 D= 17404 3 16 20 19 12 33 13 28 24 34 2 35 23 27 4 14 9 11 8 0 32 29 1 6 10 17 7 15 26 18 30 31 36 5 22 25 21 
n=350 t= 8 pC=15 D= 18607 3 25 28 24 34 2 23 27 4 14 9 8 0 32 29 1 6 10 17 7 15 26 18 11 35 31 30 36 5 12 33 22 13 21 19 16 20 
n=400 t= 2 pC=13 D= 17891 3 26 15 7 17 10 6 1 29 32 0 18 8 9 14 4 27 11 31 30 5 36 23 35 2 34 28 24 13 25 33 22 12 19 21 16 20 
n=400 t= 2 pC=15 D= 17431 3 26 15 7 17 10 6 1 29 32 0 18 8 9 14 4 27 35 2 23 31 11 30 36 5 12 19 33 22 34 28 24 13 25 21 20 16 
n=400 t= 3 pC=13 D= 17703 3 20 16 19 25 33 22 13 24 28 34 2 35 27 23 31 36 30 18 11 4 14 9 8 29 0 32 6 1 10 17 7 15 26 5 12 21 
n=400 t= 3 pC=15 D= 17466 3 21 25 13 24 28 34 22 33 19 12 5 36 23 27 35 2 31 30 11 4 14 9 8 18 0 32 29 1 6 7 17 10 15 26 16 20 
n=400 t= 4 pC=13 D= 17955 3 20 16 15 18 8 9 14 4 0 32 29 1 6 10 17 7 26 11 31 30 22 13 24 28 34 2 35 27 23 36 5 12 19 33 25 21 
n=400 t= 4 pC=15 D= 17488 3 21 25 13 24 28 33 22 34 2 35 27 23 36 5 12 30 31 11 4 14 9 8 18 0 32 29 1 6 10 17 7 15 26 19 16 20 
```

### Results and conclusions

The three bests solutions that i got are the following

```
/*16515*/ best1[37]={3,21,25,33,22,13,24,28,34,2,35,27,4,14,9,8,0,32,29,1,6,10,17,7,15,26,18,11,30,31,23,36,5,12,19,16,20}, 
/*16533*/ best2[37]={3,20,16,19,12,5,36,23,31,30,18,26,15,7,17,10,6,1,29,32,0,8,11,9,14,4,27,35,2,34,24,28,13,22,33,25,21},
/*16548*/ best3[37]={3,20,16,19,12,5,36,23,31,30,11,8,18,26,15,7,17,10,6,1,29,32,0,14,9,4,27,35,2,34,24,28,13,22,33,25,21};
```

#### Observations

If we apply each of this combinations of parameters to the simple program 'tsp.c', we would not get this best 
solution.

This is due to the combinatorial fact, or randomness of the algorithm. I got this results performing tsp2.c and, 
so, running GA more than one thausand “shots” every time, this makes changing the initialization of the 
population very important, and adds a fact that I have not comented still now, that is the difficult of generates 
random numbers with computation.

In consequence, is not sure that this are the best solutions that ever this algorithm is able to calculate. But for
the statistical of the quantity of solutions I have seen performing this program, I think this are not too bad 
solutions.
