/*
	FILE SPAIN.CSV required to run this program.

	This program starts converting the file spain.csv to two new smaller files   
	nodes.txt and edges.txt, that will just have the information we need. 	
*/

/*
    Author  FRANCESC BADIA ROCA
    
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

unsigned long int 	nn; //23895681
unsigned long int 	ne;	// 1417363
int 				c;

struct node
{
	unsigned long int 	id;
	double				lat,lon;
	double 				g,h;
	int					open, close;
	int					valn;		//valence of the node
	struct node			**nb;		//array of pointers to each neighbours, the dim of this array is valn
	struct node 		*parent;
};

//FUNCTIONS HEADERS


void WriteNodes			(FILE *,FILE *,unsigned long int *);
void WriteWays			(FILE *,FILE *,unsigned long int *);
void ReadNodes			(FILE *, struct node *);
void ReadNeighbours		(FILE *, struct node *);
struct node *Searchnode	(struct node *, unsigned long int,unsigned long int*,unsigned long int);
void Astar				(struct node*,unsigned long int, unsigned long int);
double H				(struct node *,struct node*);
void Printpath			(struct node *gn,FILE *f,struct node *);

//MAIN FUNCTION
int main()
{
	int j;
	char aux;
	unsigned long int source,goal,i;
	FILE *f1,*fn,*fe,*fp;
	clock_t init,curr;	

	struct node *gn;

	f1 = fopen("spain.csv","r");		//input file
	
	printf("\nThis program creates two files the first time, it will takes a few minutes\n");
	printf("If you have already created this files previously you can save this part of\n");
	printf("computation.\n\nIs this time the first time you run the program? (y/n):");
	scanf("%c",&aux);

	init = clock();

	if(aux=='y')
	{
		fn = fopen("nodes.txt","w");	//rewrite the nodes and edges in the correct form and counting the dimension (1st time)
		fe = fopen("edges.txt","w");
		WriteNodes(f1,fn,&nn);
		WriteWays(f1,fe,&ne);	
		printf("\nFinished writing: time now:%f seconds",((float)clock()-(float)init)/CLOCKS_PER_SEC);
	}
	else
	{
		fn = fopen("nodes.txt","r+");		
		fe = fopen("edges.txt","r+");	//reading the nodes and edges if we have already created this files previously (2nd and more times)
		nn = 23895681;
		ne = 1417363;
	}

	printf("\nnodes\t%ld",nn);
	printf("\nways\t%ld\n",ne);
	
	//allocating and charging the nodes information
	gn = (struct node*)malloc(nn*sizeof(struct node));		

	//Storing the nodes information
	ReadNodes(fn,gn);

	printf("\nNodes stored: time now:%f\n",((float)clock()-(float)init)/CLOCKS_PER_SEC);

	//allocating and charging the neighbours information to the nodes
	ReadNeighbours(fe,gn);
	
	printf("\nNeighbours stored: time now:%f\n",((float)clock()-(float)init)/CLOCKS_PER_SEC);

	source  = 240949599;	//coordenates 41.383442, 2.181815
	goal	= 195977239;	//coordenates 37.386099,-5.991742

	//AStar
	Astar(gn,source,goal);

	printf("\nAStar completed: time now:%f\n",((float)clock()-(float)init)/CLOCKS_PER_SEC);

	free(gn);
	fclose(f1);
	fclose(fn);
	fclose(fe);

}

//for the first time that it runs, this function rewrite the
//nodes information, just id / lat / lon
void WriteNodes(FILE *f,FILE *fn,unsigned long int *nn)
{
	int j,k,i;
	unsigned long int count=0;
	fpos_t pos;
	c = fgetc(f);
	while(c != 'n') { while (fgetc(f) != '\n'); c = fgetc(f);};
	
	while( c=='n' )
	{
	
		fgetpos(f,&pos); 
		j = 0;
		while(c != '\n') {j++; c = fgetc(f);};
		fsetpos(f,&pos);
		k = 0;
		for(i=0;i<j;i++)
		{
		
			c = fgetc(f);
			if(c == '|') 
			{
				k++;
				if(k == 9 || k == 10)	fprintf(fn,"\t");
			}
			else 
			{
				if(k == 1 || k == 9 || k == 10) fprintf(fn,"%c",c);
			}
		}
		c = fgetc(f);
		count++;
	};
	*nn = count;
}
//This function charges the information of the file to the struct node array
void ReadNodes(FILE *fn, struct node *gn)
{
	unsigned long int i;	
	rewind(fn);
	
	for(i=0;i<nn;i++)
	{
		fscanf(fn,"%ld\t%lf\t%lf",&gn[i].id,&gn[i].lat,&gn[i].lon);
		gn[i].parent 	= NULL;
		gn[i].valn 	 	= 0;
		gn[i].close 	= 0;
		gn[i].open 		= 0;
		gn[i].nb		= (struct node**)malloc(1*sizeof(struct node*));
	}	
	return;
}

//for the first time that it runs, this function rewrite the
//ways information, just id /number of nodes in the way / oneway or not / nodes list
void WriteWays(FILE *f,FILE *fe, unsigned long int *ne)
{
	int k,l,m,onew;
	unsigned long int count=0,j,i;
	fpos_t pos;
	
	while(c != 'w') { while (fgetc(f) != '\n'); c=fgetc(f);};
	
	while( c== 'w' )
	{
	
		fgetpos(f,&pos); 
		j = 0;
		l = 0;
		while(c != '\n') {j++; c = fgetc(f); if(c == '|') l++;};
		fsetpos(f,&pos);
	
		k = 0;
		fprintf(fe,"%d",l-8);	//num of nodes per way
		for(i=0;i<j;i++)
		{
			c = fgetc(f);
			if(c == '|') 
			{
				k++;
				if(k == 1 || k == 7 || k > 8)	fprintf(fe,"\t");
				if(k == 7) onew = i;
			}
			else 
			{
				if(k == 1 || k > 8) fprintf(fe,"%c",c);
				if(k == 7 && i == onew+1 && c == 'o') fprintf(fe,"%d",1);
			}
			if(i == onew+1 && k == 8) fprintf(fe,"%d",0);
		}
		c = fgetc(f);
		count++;
	};
	*ne = count;
}

//This function charges the information of the file to the struct node neighbours array
void ReadNeighbours(FILE *fe, struct node *gn)
{
	int numnod,oneway,j,l=0;
	unsigned long int i,idnode,dimi,k,*h,idway,idnode1,nna = 0,nnb=0;
	struct node *a,*b;
	double dim;	
	
	//Creating the hash vector
	dim		= sqrt((double) nn);
	dimi 	= (int)dim;	
	h 		= (unsigned long int*)malloc((dimi+1)*sizeof(unsigned long int));
	for(k=0;k<=dimi;k++)
		h[k] = gn[k*dimi].id;
	
	rewind(fe);
	a = NULL;
	for(i=0;i<ne;i++)
	{	
		
		fscanf(fe,"%d\t%ld\t%d\t",&numnod,&idway,&oneway);
		if(numnod == 1) { while(fgetc(fe) != '\n'); continue; }
		while ( a ==NULL) 
		{		
			fscanf(fe,"%ld\t",&idnode1);
			a = Searchnode(gn,idnode1,h,dimi);
			if(a == NULL) nna++;
			numnod -= 1;
		};
		for(j=0;j<numnod;j++)
		{	
			fscanf(fe,"%ld\t",&idnode);
			b = Searchnode(gn,idnode,h,dimi);
			
			if(b == NULL) 	continue;
			
			if(a->valn > 0) a->nb = (struct node**)realloc(a->nb,(a->valn+1)*sizeof(struct node *));
			a->nb[a->valn] 	=  b;
			a->valn   		+= 1;
			
			if(oneway==0) 	
			{
				if(b->valn > 0)	b->nb = (struct node**)realloc(b->nb,(b->valn+1)*sizeof(struct node *));
				b->nb[b->valn] = a;
				b->valn += 1;
			}
			a = b;
		}
		a = NULL;
	}
	free(h);
	
	return;
}

//This function search the node with id idn(input) and returns the pointer to the same
//In this function is used the hash vector strategy to improve the search time
struct node *Searchnode(struct node *gn, unsigned long int idn,unsigned long int *h, unsigned long int dimi)
{
	unsigned long int i,j;
	
	i = 0;	
	while(idn >= h[i]) {i++; if(i == dimi) break;}; //after that h[i-1]<idn<=h[i+1]
	
	if(i == 0)
		for(j=0;j<dimi+1;j++)
		{
			printf("\n%d < %ld < %ld",0,idn,h[1]);
			exit(1);
			if(gn[j].id == idn) 			
				return(&gn[j]); 
		}

	for(j=(i-1)*dimi;j<dimi*i && j<nn;j++)
		if(gn[j].id == idn) 				
			return(&gn[j]); 

	if(i==dimi)					
		for(j=dimi*dimi;j<nn;j++)	
			if(gn[j].id == idn) 
				return(&gn[j]); 
	
	return(NULL);
}
void Astar(struct node *gn,unsigned long int ids, unsigned long int idg)
{
	FILE *f,*q;
	double dim,fmin,tentg;
	int dimi,k,j,numop;
	unsigned long int *hash,i;
	struct node *source,*goal,*curr,*bestn;
	struct node **openlist;

	f = fopen("path.txt","w");
	q = fopen("expanded.txt","w");
	openlist = (struct node **)malloc(1*sizeof(struct node *));
	
	//Creating the hash vector
	dim 	= sqrt((double) nn);
	dimi 	= (int)dim;	//4888
	hash 	= (unsigned long int*)malloc((dimi+1)*sizeof(unsigned long int));

	for(k=0;k<=dimi;k++)
		hash[k] = gn[k*dimi].id;
	
	source  = Searchnode(gn,ids,hash,dimi);
	goal	= Searchnode(gn,idg,hash,dimi);
	curr 	= goal;

	if (source ==NULL)
	{
		printf("Source node not found\n");
		free(hash);
		return;
	}
	if (goal == NULL)
	{
		printf("Goal node not found\n");
		free(hash);
		return;
	}

	source->open 	= 1;
	source->g		= 0;
	source->h		= H(source,goal);
	openlist[0] 	= source;
	numop 			= 1;

	while (numop > 0)
	{
		fmin = 1000000000000000;
		for(i=0;i<numop;i++)
			if(openlist[i]->g+openlist[i]->h < fmin)
			{
				curr = openlist[i];
				fmin = openlist[i]->g+openlist[i]->h;
			}
		if(curr->id == idg) {printf("\nGOAL REACHED\n"); break;}
		
		
		fprintf(q,"%lf\t%lf\n",curr->lat,curr->lon);
		for(i=0;i<curr->valn;i++)
		{
			if(curr->parent==curr->nb[i]) continue;
			tentg = curr->g + H(curr,curr->nb[i]);
			if(curr->nb[i]->open == 1)
			{
				if(curr->nb[i]->g <= tentg) {goto kkk;}
			}			
			else
			{
				if(curr->nb[i]->close == 1)
				{
					if(curr->nb[i]->g <= tentg) {goto kkk;}
					curr->nb[i]->close 	= 0;
					curr->nb[i]->open  	= 1;
					numop 				+= 1;
					openlist = (struct node**)realloc(openlist,numop*sizeof(struct node *));
					openlist[numop-1] = curr->nb[i];
				}
				else
				{
					curr->nb[i]->open = 1;
					numop += 1;
					openlist 			= (struct node**)realloc(openlist,numop*sizeof(struct node *));
					openlist[numop-1] 	= curr->nb[i];
 					curr->nb[i]->h 		= H(curr->nb[i],goal);
				}
			}
			curr->nb[i]->g 		= tentg;
			curr->nb[i]->parent = curr;	
		}
		kkk:curr->close = 1;
		curr->open 		= 0;
		numop 			-= 1;
		j				= 0;
		
		while (curr != openlist[j]) {j++;}; 
		//removing the expanded node from the opened list
		openlist[j] = openlist[numop];
		openlist = (struct node**)realloc(openlist,numop*sizeof(struct node *));
	};
	if (numop == 0) printf("\nERROR\n");
	else
	{
		printf("DIST BCN - SEVILLA %lf\n",goal->g); //DIST BCN - SEVILLA 988793.732264
		Printpath(gn,f,goal);
	}	
	free(hash);
	fclose(f);
	fclose(q);
	return;
}

// The usual PI/180 constant
#define DEG_TO_RAD 0.017453292519943295769236907684886
// Earth's quadratic mean radius for WGS-84
#define EARTH_RADIUS_IN_METERS 6372797.560856
// Using Spherical Low of Cosine:

double H(struct node *a,struct node *b)
{
	//version of lluis alsedà	

   return acos(sin(a->lat * DEG_TO_RAD) * sin(b->lat * DEG_TO_RAD) + \
               cos(a->lat * DEG_TO_RAD) * cos(b->lat * DEG_TO_RAD) * cos((a->lon-b->lon) * DEG_TO_RAD) \
	      ) * EARTH_RADIUS_IN_METERS;
}

//Print on a file the coordenates of the optimal path nodes
void Printpath(struct node *gn, FILE *f,struct node *goal)
{
	struct node *a;
	a = goal;	
	while(a->parent!=NULL)
	{
		fprintf(f,"%lf\t%lf\n",a->lat,a->lon);
		a = a->parent;
	};
	return;
}

