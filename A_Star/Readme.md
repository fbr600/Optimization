
# A*, a routing problem

Computing an optimal path (according to distance) from Basílica de Santa Maria del Mar (Plaça de Santa Maria) in Barcelona to the Giralda (Calle Mateos Gago) in Sevilla by using the AStar algorithm. To this end one has to implement the AStar algorithm and compute and write the optimal path.

http://mat.uab.cat/~alseda/MasterOpt/spain.csv.zip

## Program 

Model of storage information

I have defined a Structure to store all the graph information,

```c
struct node
{
	unsigned long int	id;
	double 			g,h;		   //distances for the distance functions
	double			lat,lon;	   //latitude and longitude of node
	int			open, close;	   //if the node is (1), or not (2), on open and closed list
	int 			valn;		   //number of neighbours
	struct node		**nb;		   //pointers to the neighbours
	struct node 		*parent;           //pointer to the parent, applied on Astar
};
```

The singular or interessting thing about this structure is that the whole information that we need about the graph, including the information of nodes and the ways as well, is stored in each node, as follows:

 * ** nb 	The field ** nb (neighbours) is an array of pointers to each of the neighbours of the node, so 	pointers to other struct node.
	+ N2 is neighbour of n1:
	+ If exist one way in which the list of nodes is  ……/idn*/idn*/idn1/idn2/idn*/………
	+ Then, in the node[n1]->nb[i] = &node[n2].
	+ And if the way is not oneway, then we put the symetric case Node[n2]->nb[i] = &node[n1].

* Valn 	+ means the number of neighbours that the node has.
	+ It is calculated while we are scanning all the ways and storing the nb-pointers information.
	+ This number will be very usefull in the Astar.

## Sequence of actions

### 1 Read file ‘spain.csv’

You can download the file [here (348.5 MB)](http://mat.uab.cat/~alseda/MasterOpt/spain.csv.zip) 

### 2 Create a file called *‘nodes.txt’*
	Function: WriteNodes
	This function reads the file ‘spain.csv’ and return the number of nodes that has the graph.
	Format: idnode / latnode / lonnode
	
### 3 Create a file called *‘ways.txt’*
	Function: WriteWays
	Format:nº nodes in the way / idway / 1 if is oneway, 0 if not / idnode1 / idnod2 / ......
	One important information that is got in this function is the number of nodes that there are in 	each way. This will be usefull to scann the ways in order to assign the neighbours to each 	node.
	Also, as in the Nodes case, this function return the number of  ways.

If steps 1 , 2, 3 has been already done, there is the possibility of skip this steps and start at 4. 
Taking into account that if we did not run this function, we would not know the number of nodes and ways, so I have imposed the dimensions in the case of not running this function, for accidental cases. 
