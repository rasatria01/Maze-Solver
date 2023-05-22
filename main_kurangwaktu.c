#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define BILLION  1000000000L;


struct QNode {
    int i;
    int j;
    int dist;
    struct QNode* next;
};
  

struct Queue {
    struct QNode *front;
    struct QNode *rear;
};
struct QNode* newNode(int i,int j,int dist)
{
    struct QNode* temp = (struct QNode*) malloc(sizeof(struct QNode));
    temp->i = i;
    temp->j = j;
    temp->dist=dist;
    temp->next = NULL;
    return temp;
}
void invalidFIleError()
{
	printf("Error: file invalid!");
}
int isEmpty(struct QNode* root)
{
    return !root;
}
struct Queue* createQueue()
{
    struct Queue* q = (struct Queue*) malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

void push(struct QNode** root, int i,int j,int dist)
{
    struct QNode* stackNode = newNode(i,j,dist);
    stackNode->next = *root;
    *root = stackNode;
}
void peek(struct QNode *root)
{
	printf("\n%d\n%d\n%d",root->i,root->j,root->dist);
}
void pop(struct QNode** root, int* i,int* j,int* dist)
{

    *i=(*root)->i;
    *j=(*root)->j;
    *dist=(*root)->dist;     
    *root = (*root)->next;   
}
int isSafe(char **arr,char** arri,int i,int j ,int maxcol,int maxrow)
{
	if (i<maxrow && i>=0 && j>=0 && j<maxcol && arri[i][j]!='1' && (arr[i][j]=='-' || arr[i][j]=='e'))
	{
		return 1;
	}
	return 0;
}
void enQueue(struct Queue* q, int i, int j,int dist)
{

    struct QNode* temp = newNode(i,j,dist);

    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

void deQueue(struct Queue* q,int* i, int* j,int* dist)
{
	*i=q->front->i;
	*j=q->front->j;
	*dist=q->front->dist;
    if (q->front == NULL)
        return;
  
    q->front = q->front->next;

    if (q->front == NULL)
        q->rear = NULL;
}
int checkFile(char* filename)
{
	struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0)
		if (buffer.st_size<=1)
			return 3;
		else
			return 2;
	else
		return 1;
}
char **createArr(int row_in, int col_in)
{
    char **grid;
    grid = malloc(row_in * sizeof(int *));
    for (int i = 0; i < row_in; i++)
    {
        grid[i] = malloc(col_in * sizeof(int));
    }
    return grid;
}
char **parseData(int *matrixRow, int *matrixCol, char filename[50])
{
    char temp[100];
    FILE *stream = fopen(filename, "r");
    fgets(temp, 30, stream);
    *matrixRow = atoi(temp);
    fgets(temp, 30, stream);
    *matrixCol = atoi(temp);
    if(*matrixRow==0 || *matrixCol==0)
    {
		atexit(invalidFIleError);
		exit(0);
	}
    char **mat = createArr(*matrixRow, *matrixCol);
    for (int i = 0; i < *matrixRow; i++)
    {
        fgets(temp, 100, stream);
        for (int j = 0; j < *matrixCol; j++)
        {
            mat[i][j] = temp[j];
        }
    }
    fclose(stream);
    return mat;
}
void displayImage(char **grid, int row, int col)
{
    int i,j;
    for (i=0;i<row;i++)
    {
		for(j=0;j<col;j++)
		{
			printf("%c",grid[i][j]);
		} 
		printf("\n");
	}
}
void backTrack(char** grid,struct QNode **Qnode,int row, int col)
{
	int i,j,disk,tempI,tempJ,tempDisk;
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			grid[i][j]='-';
		}
	}
	pop(Qnode,&i,&j,&disk);
	grid[i][j]='*';
	while(!isEmpty(*Qnode))
	{
		pop(Qnode,&tempI,&tempJ,&tempDisk);
		if((i+1)==tempI && tempJ==j && disk==(tempDisk+1))
		{
			grid[tempI][tempJ]='*';
			i=tempI;
			j=tempJ;
			disk=tempDisk;
		}
		if((i-1)==tempI && tempJ==j && disk==(tempDisk+1))
		{
			grid[tempI][tempJ]='*';
			i=tempI;
			j=tempJ;
			disk=tempDisk;
		}
		if(i==tempI && tempJ==(j+1) && disk==(tempDisk+1))
		{
			grid[tempI][tempJ]='*';
			i=tempI;
			j=tempJ;
			disk=tempDisk;
		}
		if(i==tempI && tempJ==(j-1) && disk==(tempDisk+1))
		{
			grid[tempI][tempJ]='*';
			i=tempI;
			j=tempJ;
			disk=tempDisk;
		}
	}
}
int bfs(char **grid,struct QNode **Qnode, int row, int col)
{
	int i,j,dist=0;
	struct Queue *q=createQueue();
	
	char **visited=createArr(row, col);
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			if(grid[i][j]=='#')
			{
				visited[i][j]='3';
			}else if(grid[i][j]=='s')
			{
				enQueue(q,i,j,dist);
				push(Qnode,i,j,dist);
			}else
			{
				visited[i][j]='0';
			}
			
		}
	}

	while(q!=NULL)
	{
		deQueue(q,&i,&j,&dist);
		visited[i][j]='1';
		push(Qnode,i,j,dist);
		if(grid[i][j]=='e')
		{
			return dist;
		}
		if(isSafe(grid,visited,i+1,j,col,row))
		{
			enQueue(q,i+1,j,dist+1);
			
		}
		
		if(isSafe(grid,visited,i,j+1,col,row))
		{
			enQueue(q,i,j+1,dist+1);
		}
		
		if(isSafe(grid,visited,i-1,j,col,row))
		{
			enQueue(q,i-1,j,dist+1);
		}
		
		if(isSafe(grid,visited,i,j-1,col,row))
		{
			enQueue(q,i,j-1,dist+1);
		}
	}
	return -1;
}

int main()
{
	char filename[50];
	int k,dist;
	char **matrix,**sol;
    int matrixRow, matrixCol;
    struct QNode* root = NULL;
    printf("Enter maze file path: ");
	scanf("%s",filename);
	k=checkFile(filename);
	clock_t begin = clock();
	if (k==3)
	{
		printf("Error: file is empty!");
	}else if (k==2)
	{
		matrix = parseData(&matrixRow, &matrixCol, filename);
		sol=createArr(matrixRow,matrixCol);
		dist=bfs(matrix,&root,matrixRow,matrixCol);
		backTrack(sol,&root,matrixRow,matrixCol);
		clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("\nSolution :\n");
		displayImage(sol, matrixRow, matrixCol);
		printf("Solution distance: %d cells\n",dist+1);
		printf("Time elapsed: %.10f s\n\n",time_spent);
		
	}else
	{
		printf("Error: file invalid!");
	}

    return 0;
}

