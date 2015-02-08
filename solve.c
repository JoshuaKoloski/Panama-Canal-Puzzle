#include <stdio.h>
#include <stdlib.h>

#define hashSize 100003
#define queueSize 50000
/*Position data type.*/
typedef struct pnode {
	struct pnode *next; /*next pointer for the hash bucket*/
	struct pnode *back; /*position from which this one came*/
	int cost; /*number of moves to get to this position*/
	char piece; /*piece that moved to this position*/
	int dir; /*direction moved to enter this position*/
	char board[12]; /*this position's board*/
} PositionBody;
typedef PositionBody *TypePosition;

//allocated memory for a new position
TypePosition NEWPOSITION() {
	TypePosition p = (TypePosition) malloc(sizeof(PositionBody));
		if (p==NULL) {
			printf("Malloc for a new position failed."); exit(1);
		}
	return p;
}

//sets the goal position "panama canal"
void setGoal(TypePosition p){
	p->board[0]='p';
	p->board[1]='a';
	p->board[2]='n';
	p->board[3]='a';
	p->board[4]='m';
	p->board[5]='a';
	p->board[6]='c';
	p->board[7]='a';
	p->board[8]='n';
	p->board[9]='a';
	p->board[10]='l';
	p->board[11]=(char)0;
}
//sets the start position "canama panal"
void setStart(TypePosition p){
	p->board[0]='c';
	p->board[1]='a';
	p->board[2]='n';
	p->board[3]='a';
	p->board[4]='m';
	p->board[5]='a';
	p->board[6]='p';
	p->board[7]='a';
	p->board[8]='n';
	p->board[9]='a';
	p->board[10]='l';
	p->board[11]=(char)0;
}
//prints a given position's board
void printBoard(TypePosition p){
	printf("\n");
	int i;
	for (i = 0; i < 12; i++){
		if (p->board[i]==(char)0 && i==5){
			printf("_\n");
		}else if(p->board[i]==(char)0){
			printf("_");
		}else if(i==5){
			printf("%c\n", p->board[i]);
		}else {
			printf("%c", p->board[i]);
		}
	}
	printf("\n");
}


//START HASH**************************************
//strucure for a hash table
typedef struct hashTable{
	TypePosition hash[hashSize];//array of positions
	int size;//amount of position elements in the hash table
}*TypeHash;
//allocates memory for a hash table
TypeHash NEWHASH(){
	TypeHash q = (TypeHash)malloc(sizeof(struct hashTable));
	if (q==NULL){printf("Malloc for a new hash failed."); exit(1);}
	return q;
}

//hash function that returns a value to use as an index where a postiion will be added to the hsh table
int H(TypePosition p){
	int i=0;
	unsigned int Hvalue=0;
	while(i<12){
		Hvalue=((Hvalue*128)+(int)(p->board[i++]))%hashSize;
	}

	return(Hvalue);

}

//debugging function that prints a postion's hash list
/*void printHashList(TypePosition p, TypeHash h) {
	int place = H(p);
	TypePosition v = h->hash[place];
	if (v==NULL){
		printf("empty list\n");
	} else {
		int i =1;
		while (v!=NULL){
			printf("\nSpot: %d", i);
			printBoard(v);
			v=v->next;
			i++;
		}
	}
}*/

//compares the boards of two different positions. Returns 1 if they are equal and 0 if they are not
int compare(TypePosition a, TypePosition b){
	int i;
	for (i=0; i < 12; i++){
		if (a->board[i]!=b->board[i]){
			return 0;
		} 
	}
	return 1;
}

//checks to see if a position is found in a hash table. Return 1 if it is found and 0 if it is not 
int member(TypePosition p, TypeHash h){
	int place = H(p);
	//printf("\nplace:%d\n", place);
    TypePosition track = h->hash[place];
    //printf("\nGot here\n");
    if (track==NULL){
    	return 0;
    }
    //printf("\nPointer did not point to nothing\n");
	
	while (track!=NULL){
		//printf("\nEntered while loop\n");
		if (compare(track, p)==1){
			return 1;
		}
		track=track->next;
	}
	return 0;
}

//inserts a position into the hash table if it is not yet a member of the hash table
void insert(TypePosition p, TypeHash h){
	if (member(p,h)==0){
		//printf("\nNEW POSITION\n");
		h->size++;
		int place = H(p);
		/*TypePosition start = NEWPOSITION();
		start=p;*/

		p->next = h->hash[place];
		h->hash[place]=p;
		
	}
}

/*void elementsInRows(TypeHash h){
	int sum;
	int i;
	for (i=0; i<hashSize; i++){
		TypePosition v = h->hash[i];
		while (v!=NULL){
			sum++;
			v=v->next;
		}
		printf("\nROW %d CONTAINS %d ELEMENTS", i, sum);
		sum=0;
	}
	
}*/
//searches through the hash table and increments a counter whenever a index contains an empty list. Returns the amount of empty lists in hash table
int empty(TypeHash h){
	int sum=0;
	int i;
	for (i=0;i<hashSize;i++){
		TypePosition v = h->hash[i];
		if (v==NULL){sum++;}
	}
	return sum;
}
//searches through the hash table and increments a counter each time a list contains only one element. Returns the amount of 1 element lists in hash table
int one(TypeHash h){
	int sum=0;
	int one=0;
	int i;
	for (i=0; i<hashSize; i++){
		TypePosition v = h->hash[i];
		while (v!=NULL){
			sum++;
			v=v->next;
		}
		if (sum==1){one++;}
		sum=0;
	}
	return one;
}
//searches through the hash table and keeps track of the list with the greates amount of elements. Returns the lists with maximum amount of elements
int max(TypeHash h){
	int sum=0;
	int max = 0;
	int i;
	for (i=0; i<hashSize; i++){
		TypePosition v = h->hash[i];
		while (v!=NULL){
			sum++;
			v=v->next;
		}

		if (sum>max){
			max=sum;
		} 
		sum=0;
	}
	return max;
}
//END HASH**********************************************

//QUEUES*****************************************************
//defines a queue structure
typedef struct queue {
	TypePosition queue[queueSize];//array of positions
	int front, rear, size, max;//tracks the front, rear, current size, and maximum size of the queue
}*TypeQueue;
//allocates memory for the queue
TypeQueue NEWQUEUE(){
	TypeQueue q = (TypeQueue)malloc(sizeof(struct queue));
	if (q==NULL){printf("Malloc for a new queue failed."); exit(1);}
	return q;
}
//QUEUE FUNCTIONS*********************************
//debugging function which prints out the element in the queue
/*
void printQueue(TypeQueue q){
	int i;
	printf("\nfront: %d\nrear: %d", q->front, q->rear);
	for (i=q->front; i<=q->rear; i++){
		
		printf("i: %d", i);
		printBoard(q->queue[i]);
	}
}*/
//adds a position to the queue
void enqueue(TypeQueue q, TypePosition p){
	//printf("\n%d\n", q->queue[q->first]);
	if (q->size>=queueSize){
		printf("\nERROR: QUEUE OVERFLOW\n");
	} else{
		q->queue[q->rear]=p;
		q->rear=(q->rear)+1;
		if (q->rear==queueSize){
			q->rear=0;
		}
		q->size=(q->size)+1;
	}
}
//removes a position from the queue and returns that position
TypePosition dequeue(TypeQueue q){
	if ((q->size)>(q->max)){
		(q->max)=(q->size);
	}
	if (q->size==0){
		printf("\nERROR: QUEUE UNDERFLOW\n");
	} else{
		int temp = q->front;
		q->front=(q->front)+1;
		if (q->front==queueSize){
			q->front=0;
		}
		q->size=(q->size)-1;
		return q->queue[temp];
	}
	TypePosition temp = NEWPOSITION();
	return temp;
}

//END OF QUEUE FUNCTIONS******************************
void setPosition(TypePosition a, TypePosition b){
	int i;
	for (i=0; i < 12; i++){
		b->board[i]=a->board[i];
	}	
	b->cost=a->cost;
	b->dir=a->dir;
	b->piece=a->piece;	
}

//enqueues all the possible positions that derive from a given position as long as they are not in the hash table
void move(TypePosition p, TypeQueue a, TypeHash h){
 	int north=0;
	int south=1;
	int east=2;
	int west=3;
	int j;
	char temp;

	for (j=0; j<12; j++){
		if (p->board[j]==(char)0){	
			//checks if the blank space can move up. If so, moves the position above the empty space south			
			if(j+6<12){
				TypePosition m = NEWPOSITION();
				setPosition(p,m);
				temp=m->board[j+6];
				m->board[j+6]=m->board[j];
				m->board[j]=temp;
				m->back=p;
				m->dir=north;
				m->piece=p->board[j+6];
				m->cost=(p->cost)+1;
				if (member(m,h)==0){
					enqueue(a, m);
					insert(m,h);
				}
				

			} 
			//checks to see if the blank space can move left. If so, moves position to the left of the blank space east
			if(j!=0 && j!=6){
				TypePosition q = NEWPOSITION();
				setPosition(p, q);
				temp=q->board[j-1];
				q->board[j-1]=q->board[j];
				q->board[j]=temp;
				q->back=p;
				q->dir=east;
				q->piece=p->board[j-1];
				q->cost=(p->cost)+1;
				if (member(q,h)==0){
					enqueue(a, q);
					insert(q, h);
				}

			}
			//checks if the blank space can move down. If so, moves the position below the empty space north	
			if (j-6>=0){
				TypePosition n = NEWPOSITION();
				setPosition(p, n);
				temp=n->board[j-6];
				n->board[j-6]=n->board[j];
				n->board[j]=temp;
				n->back=p;
				n->dir=south;
				n->piece=p->board[j-6];
				n->cost=(p->cost)+1;
				if (member(n,h)==0){
					enqueue(a, n);
					insert(n,h);
				}
				

			}
			//checks to see if the blank space can move right. If so, moves position to the right of the blank space west
			if(j!=5 && j!=11){
				TypePosition l = NEWPOSITION();
				setPosition(p,l);
				temp=l->board[j+1];
				l->board[j+1]=l->board[j];
				l->board[j]=temp;
				l->back=p;
				l->dir=west;
				l->piece=p->board[j+1];
				l->cost=(p->cost)+1;
				if (member(l,h)==0){
					enqueue(a, l);
					insert(l, h);
				}				
			} 								
		}
	}
}
//recursively prints the chain of position from the start position to the given position by following the back pointers
void printMoves(TypePosition p){
	if (p!=NULL){
		printMoves(p->back);
		printf("\nStep %d: ", p->cost);
		char *direction;
		if (p->dir==0){
			direction="up";
		} else if(p->dir==1){
			direction="down";
		} else if(p->dir==2){
			direction="right";
		} else {
			direction="left";
		}
		if (p->piece!=(char)0){
			printf("\npiece '%c' is moved %s", p->piece, direction);
		}
		printBoard(p);
	}
}

int main() {
	TypeHash h = NEWHASH();//declare hash
	TypeQueue q = NEWQUEUE();//declare queue
	TypePosition p0 = NEWPOSITION();//declare start position
	setStart(p0);//set start position
	TypePosition goal = NEWPOSITION();//declare goal position
	setGoal(goal);//set goal position	
	insert(p0, h);//insert starting position into hash table
	enqueue(q, p0);//enqueue starting position
	int check=0;//check will be set to 1 if the current position matches the goal position
	TypePosition temp = NEWPOSITION();
	while(check!=1){
		temp=dequeue(q);
		check=compare(temp,goal);//compares the current position with the goal position
		if(check==1){
			printMoves(temp);//finish the program and print the steps
		}
		if(check==0){
			move(temp, q, h);
		}
	}
	printf("\n********************QUEUE STATISTICS**********************:");
	printf("\nqCapacity: %d", queueSize);
	printf("\nqFront: %d", q->front);
	printf("\nqRear: %d", q->rear);
	printf("\nqSize: %d", q->size);
	printf("\nqMax: %d", q->max);
	printf("\n\n*********************HASH STATISTICS**********************:");
	printf("\nhCapacity: %d", hashSize);
	printf("\nhCount: %d", h->size);
	printf("\nhEmpty: %d", empty(h));
	printf("\nhOne: %d", one(h));
	printf("\nhMax: %d\n", max(h));
	return (0.0);
}
