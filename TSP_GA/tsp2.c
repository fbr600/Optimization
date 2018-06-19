/*
	Travelling Salesman Problem

	It requires the files 'dist.txt' which contains the distances
	between the cities as a square matrix, and the list of the cities
	ordered alphabetical in the file 'cities.txt'.
	The first is used to evaluate the distance function (fitness function).
	And the second one is just to know the cities of the problem and to have
	the solution route.

	This is the code which calculates a solution for each combination of the 
	parameters acting in a certain range.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int 				m 		= 37;	//dim of the problem
int 				n 		= 200; 	//population (required to be an even number)
unsigned long int 	seed 	= 1;
int 				t 		= 5;	//number of elements to do the parents selec
int 				posCr 	= 17;	//numb of positions choosed in crossover
int 				maxiter = 200;	//number max of iterations
double				mutq	= 0.0;

unsigned int Dist(int**,int*);
void Initrand(int *v);
void write(int *a);
void Cross(int *p1,int *p2,int *c);
void ga(int **pop, int *best, int **dist);
void Select(int**p,int*par,int **a);
void Mutate(int *c);

int main ()
{
	int **a,i,j,**p,*best,*rebest;
	unsigned int d,min,b;
	int in,it,iposCr;
	FILE *f1,*fres;
	
	fres = fopen("tspend.txt","w");	
	
	//Allocating for cities distances
	a = (int**)malloc(m*sizeof(int*));
	for(i=0;i<m;i++)
		a[i] = (int*)malloc(m*sizeof(int));
	
	srand(time(NULL));	

	//Allocating for the solution
	best = (int*)malloc(m*sizeof(int));
	rebest = (int*)malloc(m*sizeof(int));
	//Initializing distances
	f1 = fopen("dist.txt","r");
	for(i=0;i<m;i++)
		for(j=0;j<m;j++)
			fscanf(f1,"%d\t",&a[i][j]);
	min = 50000;
	for(n=250;n<500;n+=50)
	{
		//Allocating population
		p = (int**)malloc(n*sizeof(int*));
		for(i=0;i<n;i++)
			p[i] = (int*)malloc(m*sizeof(double));
		for(t=2;t<9;t+=1)
			for(posCr=13;posCr<17;posCr+=2)
			{
				//Inizialization the population randomly
				for(i=0;i<n;i++)
					Initrand(p[i]); 
				ga(p,best,a);
				b=Dist(a,best);
				if(b <min)
				{
					min = b;
					for(i=0;i<m;i++) rebest[i] = best[i];
					printf("%d\n",min);
				}
				fprintf(fres,"n=%3d t=%2d pC=%2d D= %5d ", n,t,posCr,b);
				for(i=0;i<m;i++) fprintf(fres,"%2d ",best[i]);
				fprintf(fres,"\n");
				
			}
		free(p);
	}
	write(rebest);
	d = Dist(a,rebest);
	printf("LA DIST ÉS %d\n",d);
	
	free(a);free(best);free(f1);
}
void Writesol(int *a,FILE *fcit,FILE *fsol)
{
	char c;
	int i,j;
	for(i=0;i<m;i++)	
	{
		for(j=0;j<a[i];j++)
			while(fgetc(fcit)!='\n');
		do{ c=fgetc(fcit); fprintf(fsol,"%c",c);}while(c!='\n');
		//fprintf(fsol,"\n");
		rewind(fcit);	
	}
	fprintf(fsol,"Barcelona\n");
}
void writepop(int **p,FILE *fres)
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
			fprintf(fres,"%2d ",p[i][j]);
		fprintf(fres,"\n");
	}
	return;
}
//Ga function
void ga(int **pop, int *best, int **a)
{
	int i,j,iter=0,k;
	int *p1,*p2,*c1,*c2,**q,*best1; 	//parents and children
	unsigned int *f,fit,fit2;
	
	p1 = (int*)malloc(m*sizeof(int));
	p2 = (int*)malloc(m*sizeof(int));
	c1 = (int*)malloc(m*sizeof(int));
	c2 = (int*)malloc(m*sizeof(int));
	best1 = (int*)malloc(m*sizeof(int));
	f = (unsigned int*)malloc(n*sizeof(unsigned int));

	//New population	
	q = (int**)malloc(n*sizeof(int*));
	for(i=0;i<n;i++) 
		q[i] = (int*)malloc(m*sizeof(int));

	fit = 50000;
	fit2= 50000;
	for(i=0;i<n;i++) 
	{
		f[i] = Dist(a,pop[i]);
		if(fit>f[i])
		{
			k = i;
			fit = f[i];
		}
	}
	do
	{
		iter++;
		
		fit = 50000;
		for(i=0;i<n;i++) 
		{

			f[i] = Dist(a,pop[i]);

			if(fit>f[i])
			{
				if(f[i]<fit2)	{fit2 = f[i]; for(j=0;j<m;j++) best1[j] = pop[k][j];}
				k = i;
				fit = f[i];
			}
		}
		//printf("iter %d dist is %d\n",iter,fit);
		for(i=0;i<m;i++) best[i] = pop[k][i];
		for(i=0;i<n/2;i++)
		{
			Select(pop,p1,a);		Select(pop,p2,a);
			Cross(p1,p2,c1);		Cross(p1,p2,c2);
			if(rand()%10 / 10 < mutq) { Mutate(c1);	Mutate(c2);}

			//Copying the new population to the current
			for(j=0;j<m;j++)
			{
				q[2*i][j]   = c1[j];
				q[2*i+1][j] = c2[j];
			}
		}

		for(i=0;i<n;i++)
			for(j=0;j<m;j++)
				pop[i][j] = q[i][j];
		
	}while(fit>10000 && iter <maxiter);
	
	if(Dist(a,best1) < Dist(a,best)) for(i=0;i<m;i++) best[i]=best1[i];	

	free(p1);free(p2);free(c1);free(c2);
	free(q); free(f); free(best1);
	return;
}

//Mutation function
void Mutate(int *c)
{
	int k1,k2,aux;
	seed++;
	//srand(seed); 
	k1 = rand()%(m-1)+1;
	k2 = rand()%(m-1)+1;
	if(k1==k2) 	k1 = 10;
	
	aux = c[k1];
	c[k1] = c[k2];
	c[k2] = aux;
	return;
}

//position based crossover
void Cross(int *p1,int *p2,int *c)
{
	int i,j,a;
	int *pos,*aux,*p3;

	p3  = (int*)malloc(m*sizeof(int));
	pos = (int*)malloc(posCr*sizeof(int));
	aux = (int*)malloc(m*sizeof(int));

	for(i=0;i<m;i++) aux[i] = i;

	//selecting posCr positions randomly
	for(i=0;i<posCr;i++) 
	{
		a = rand()%(m-i);
		pos[i] = aux[a];
		for(j=a;j<m-1;j++) aux[j] = aux[j+1];
	}

	//adding info to the offspring
	for(i=0;i<m;i++) {c[i] = -1; p3[i]=p2[i];}
					
	for(i=0;i<posCr;i++) c[pos[i]] = p1[pos[i]];

	for(i=0;i<m;i++)
		for(j=0;j<m;j++)
			if(c[i]!=-1 && c[i] == p3[j]) 
				p3[j] = -1;

	for(i=0;i<m;i++)
	{
		kkk:if(p3[i]!=-1)
			for(j=0;j<m;j++)
				if(c[j]==-1)
				{
					c[j] = p3[i];
					i++;
					goto kkk;
				}
	}		
	free(p3); free(aux); free(pos);
	return;
}
//we choose the fitest ind from t randomly indiv of the pop
void Select(int**p,int*par,int **a)
{
	int i,j,b;
	int *p2,*aux;
	unsigned int fit,*fval;
	fval = (unsigned int*)malloc(t*sizeof(unsigned int));
	p2 = (int*)malloc(t*sizeof(int));
	aux= (int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++) aux[i] = i;
	seed++;
	//srand(seed); 

	// selecting t parents randomly
	for(i=0;i<t;i++) 
	{
		b = rand()%(n-i);
		p2[i] = aux[b];
		for(j=b;j<n-1;j++) aux[j] = aux[j+1];
	}
	for(i=0;i<t;i++)
		fval[i] = Dist(a,p[p2[i]]);
	fit = 50000;
	for(i=0;i<t;i++)
	{
		if(fval[i]<fit)
		{
			fit = fval[i];
			j=i;
		}
	}
	for(i=0;i<m;i++)
		par[i] = p[p2[j]][i];

	free(fval);
	free(p2);
	free(aux);
	return;
}
//Initialization of the population
void Initrand(int *v)
{
	int i,a,j;
	int *aux;
	aux = (int*)malloc((m-1)*sizeof(int));
	seed++;
	v[0]=3;
	for(i=0;i<3;i++)	aux[i] = i;
	for(i=3;i<m-1;i++)	aux[i] = i+1;
	//srand(seed+2);
	for(i=1;i<m;i++)
	{
		a = rand()%(m-i);
		v[i] = aux[a];
		for(j=a;j<m-2;j++)	aux[j] = aux[j+1];
	}
}

//printing one the solution on the console 
void write(int *a)
{
	int i;
	printf("\n");
	for(i=0;i<m;i++) printf("%2d, ",a[i]);
	printf("3\n");
	return;
}

//evaluation of fitness function
unsigned int Dist(int **a, int *v)
{
	int i;
	unsigned int sum=0;
	for(i=0;i<m-1;i++)
		sum = sum + a[v[i]][v[i+1]];
	sum = sum + a[v[36]][3];
	return sum;
}
