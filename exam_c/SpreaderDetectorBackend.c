
//
// Created by ziv on 01/08/2020.
//

/****************************************************************************
 *                             const and libs                               *
 ***************************************************************************/

// _______________________________ includes ________________________________

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SpreaderDetectorParams.h"

// _______________________________ defines _______________________________

// num of args: num of input files + 1
#define ARG_NUM 3

// boolean const
#define FALSE 0
#define TRUE 1

// start index and initialization const
#define START 0

// parsing consts
#define MAX_LEN 1024
#define WHITE_SPACE " "
#define BATCH 10

// file handling consts
#define READ "r"
#define WRITE "w"
#define EMPTY -1

// file types index
#define PEOPLE_IDX 1
#define MEETINGS_IDX 2

// crna infected patient zero crna
#define ROOT_CRNA 1

// error messages consts
#define INPUT_ERR "Usage: ./SpreaderDetectorBackend <Path to People.in> <Path to Meetings.in>\n"
#define INPUT_FILES_ERR "Error in input files.\n"
#define OUTPUT_FILES_ERR "Error in output file.\n"


/****************************************************************************
 *                              declaration                                 *
 ***************************************************************************/

// _______________________________ structs _______________________________


/**
 * an edges struct. a directed
 * edge in tree of infection
 * contains the dist and the weights
 * from which we calculate the crna
 * between two people.
 * the successor is the node to
 * which the edge is directed to.
 */
typedef struct Edge Edge;


/**
 * a node struct. anode in
 * the directed tree graph of
 * infections. contains the ID,
 * name, crna, edges from the
 * node and the num of edges
 * from the node.
 */
typedef struct Node Node;


// _______________________________ free _______________________________

void freeNode(Node *node);

void freeArr(Node **arr, int len);

// _______________________________ init _______________________________

int initNode(Node **nodesArr, char *name, char *ID, int idx);

int addEdgeToNode(Node **nodesArr, int *indicesArr, Edge *edge, int ID1, int length);

int initEdge(Node **nodesArr, int *indicesArr, char *strID1, char *strID2, char *strDist, char
			 *strTime, int length);

int *createIndicesArr(int length);

// _______________________________ find _______________________________

int findIndex(Node **nodesArr, int *indicesArr, int start, int end, int ID);

// ___________________________ input files ____________________________

int errorHandler(int success, char *msg, Node **arr, int length);

int readPeople(Node ***pNodesArr, FILE *peopleInputFile);

int readMeetings(Node **nodesArr, int *indicesArr, FILE *meetingsInputFile, int length);

int buildData(Node ***pNodesArr, char *peoplePath, char *meetingsPath, int *len);

// _______________________________ sort _______________________________

void merge2Arrays(Node **nodesArr, int *indicesArr, int *Left, int *Right, int leftLen, int
				  rightLen, int start, int byID);

int merge(Node **nodesArr, int *indicesArr, int start, int middle, int end, int byID);

int mergeSortOnIndices(Node **nodesArr, int *indicesArr, int start, int end, int byID);

// _______________________________ crna _______________________________

float calcCrna(const float dist, const float time);

int calcCrnaForTree(Node *head, const float ancestorCrna, const Edge *edge);

// ______________________________ output ______________________________

int output(Node **nodesArr, const int *indicesArr, const int length);

// _______________________________ main _______________________________

int run(char *peoplePath, char *meetingsPath);

int validInput(const int argc);

int main(int argc, char *argv[]);


/****************************************************************************
 *                               structs                                    *
 ***************************************************************************/


/**
 * an edges struct. a directed
 * edge in tree of infection
 * contains the dist and the weights
 * from which we calculate the crna
 * between two people.
 * the successor is the node to
 * which the edge is directed to.
 */
struct Edge
{
	float dist, time;
	Node *successor;
};


/**
 * a node struct. anode in
 * the directed tree graph of
 * infections. contains the ID,
 * name, crna, edges from the
 * node and the num of edges
 * from the node.
 */
struct Node
{
	float crna;
	int ID;
	int numOfEdges;
	char *name;
	Edge **edges;
};


/****************************************************************************
 *                                   free                                   *
 ***************************************************************************/


/**
 * free all allocated memory for a node
 * @param node the pointer to the node struct
 */
void freeNode(Node *node)
{
	free(node->name);
	// if there is no edges of the person
	if (node->edges != NULL)
	{
		// free all edges from the node
		for (int i = START; i < node->numOfEdges; i++)
		{
			free(node->edges[i]);
		}
		free(node->edges);
	}
	free(node);
}


/**
 * free all the nodes in the array and their edges.
 * free the array.
 * @param arr array of nodes
 */
void freeArr(Node **arr, int len)
{
	// free all nodes in the array
	for (int i = START; i < len; i++)
	{
		freeNode(arr[i]);
	}
	// free the array
	free(arr);
}


/****************************************************************************
 *                                   init                                   *
 ***************************************************************************/


/**
 * init the values of the node in the index idx in the array
 * with the values given and null values where needed
 * @param nodesArr array of nodes to init a new node
 * 		in the index idx
 * @param name name param for the node
 * @param ID ID param for the node
 * @param idx idx of the new node in the array
 * @return TRUE for success FALSE for failure
 */
int initNode(Node **nodesArr, char *name, char *ID, int idx)
{
	// allocate memory
	nodesArr[idx] = (Node *) malloc(sizeof(Node));
	if (nodesArr[idx] == NULL)
	{
		return FALSE;
	}
	//init values
	nodesArr[idx]->edges = NULL;
	nodesArr[idx]->crna = START;
	nodesArr[idx]->ID = atoi(ID);
	nodesArr[idx]->numOfEdges = START;

	nodesArr[idx]->name = (char *) malloc((strlen(name) + 1) * sizeof(char));
	if (nodesArr[idx]->name == NULL)
	{
		free(nodesArr[idx]);
		return FALSE;
	}
	strcpy(nodesArr[idx]->name, name);
	return TRUE;
}


/**
 * add new edge to the graph in the edge
 * list of the ancestor
 * @param nodesArr array to update an
 * 		edge list of a specific node
 * 		in the array
 * @param indicesArr the array of the indices sorted
 * @param edge the edge to add to the
 * 		edge list
 * @param ID1 the ID of the ancestor node
 * @return TRUE for success FALSE for failure
 */
int addEdgeToNode(Node **nodesArr, int *indicesArr, Edge *edge, int ID1, int length)
{
	Node *node1 = nodesArr[findIndex(nodesArr, indicesArr, START, length - 1, ID1)];
	Edge **tmp = (node1->numOfEdges) ?
				 (Edge **) realloc(node1->edges, (node1->numOfEdges + 1) * sizeof(Edge *)) :
				 (Edge **) malloc((node1->numOfEdges + 1) * sizeof(Edge *));
	if (tmp == NULL)
	{
		free(edge);
		return FALSE;
	}
	// memory allocation went successfully
	node1->edges = tmp;
	node1->edges[node1->numOfEdges] = edge;
	node1->numOfEdges++;
	return TRUE;
}


/**
 * create an edge and add it to the ancestor node
 * @param nodesArr the nodes array to update
 * @param indicesArr the array of the indices sorted
 * @param strID1 the string of the id of the first node
 * @param strID2 the string of the id of the second node
 * @param strDist the string representation of teh dist
 * 		between the nodes
 * @param strTime the time the two nodes where close
 * @return TRUE for success FALSE for failure
 */
int initEdge(Node **nodesArr, int *indicesArr, char *strID1, char *strID2, char *strDist, char
			 *strTime, int length)
{
	int ID1 = atoi(strID1);
	int ID2 = atoi(strID2);
	float dist = atof(strDist);
	float time = atof(strTime);

	// allocate memory for the edge
	Edge *edge = (Edge *) malloc(sizeof(Edge));
	if (edge == NULL)
	{
		return FALSE;
	}
	int idx = findIndex(nodesArr, indicesArr, START, length - 1, ID2);
	Node *node2 = nodesArr[idx];
	edge->successor = node2;
	edge->dist = dist;
	edge->time = time;
	return addEdgeToNode(nodesArr, indicesArr, edge, ID1, length);
}


/**
 * create an indices array for the sort
 * @param length the length of the array
 * @return pointer to the array
 */
int *createIndicesArr(int length)
{
	int *arr = (int *) malloc(length * sizeof(int));
	if (arr != NULL)
	{
		for (int i = START; i < length; i++)
		{
			arr[i] = i;
		}
	}
	return arr;
}


/****************************************************************************
 *                              find index                                  *
 ***************************************************************************/


/**
 * find the index of the edge node
 * with the id ID in teh nodes arr
 * using binary search in sorted by
 * ID indices array
 * @param nodesArr the nodes array
 * @param indicesArr the array with
 * 		the sorted indices
 * @param start the start index
 * 		to search in
 * @param end the end index to
 * 		search in
 * @param ID the id to search for
 * @return the index of the node
 * 		with the id ID
 */
int findIndex(Node **nodesArr, int *indicesArr, int start, int end, int ID)
{
	if (end >= start)
	{
		int mid = start + (end - start) / 2;

		return (nodesArr[indicesArr[mid]]->ID == ID) ? indicesArr[mid] :
			   ((nodesArr[indicesArr[mid]]->ID > ID) ?
				findIndex(nodesArr, indicesArr, start, mid - 1, ID) :
				findIndex(nodesArr, indicesArr, mid + 1, end, ID));
	}
	return start;
}


/****************************************************************************
 *                          input files handling                            *
 ***************************************************************************/


/**
 * handle errors in the process
 * output to stderr the problem
 * and frees memory if needed
 * @param success indicator of success
 * @param msg message if success is FALSE
 * @param arr pointer to the array to free
 * @param length the length of the valid
 * value in the array
 * @return success
 */
int errorHandler(int success, char *msg, Node **arr, int length)
{
	if (!success)
	{
		fprintf(stderr, "%s", msg);
		if (arr != NULL)
		{
			freeArr(arr, length);
		}
	}
	return (!success);
}


/**
 * read the people.in file and initialize the nodes array
 * @param pNodesArr the pointer to the node array
 * @param peopleInputFile the people.in file
 * @return TRUE for success FALSE for failure and EMPTY for empty file
 */
int readPeople(Node ***pNodesArr, FILE *peopleInputFile)
{
	int success = TRUE;
	// line buffer
	char line[MAX_LEN];
	// first memory allocation
	int count = START, len = BATCH;
	(*pNodesArr) = (Node **) malloc(len * sizeof(Node *));
	if (*pNodesArr == NULL)
	{
		return FALSE;
	}
	// while not EOF
	while (fgets(line, MAX_LEN, peopleInputFile))
	{
		if (count == len) // if there is more memory allocation needed
		{
			len += BATCH;
			Node **tmp = (Node **) realloc((*pNodesArr), len * sizeof(Node *));
			if (tmp == NULL)
			{
				// we have only #count number of elements initialized in the array
				freeArr((*pNodesArr), count);
				(*pNodesArr) = NULL;
				return FALSE;
			}
			(*pNodesArr) = tmp;
		}
		//init the new node
		char *name, *ID;
		name = strtok(line, WHITE_SPACE);
		ID = strtok(NULL, WHITE_SPACE);

		success = initNode((*pNodesArr), name, ID, count);
		// if problem have accured while initializing the new node
		if (!success)
		{
			freeArr((*pNodesArr), count);
			(*pNodesArr) = NULL;
			break;
		}
		count++;
	}
	if (!count)
	{
		freeArr((*pNodesArr), count);
		(*pNodesArr) = NULL;
		return EMPTY;
	}
	return (!success) ? success : count;
}


/**
 * read the meetings file's content and create the edge of the graph
 * @param nodesArr the nodes array which should update its' nodes edges
 * @param indicesArr the array of sorted indices
 * @param meetingsInputFile the file with the meetings data
 * @param length the length of valid node in teh nodes array
 * @return headID for success FALSE for failure EMPTY for empty file
 */
int readMeetings(Node **nodesArr, int *indicesArr, FILE *meetingsInputFile, int length)
{
	int success = TRUE;
	// line buffer
	char line[MAX_LEN];
	if (fgets(line, MAX_LEN, meetingsInputFile) == NULL)
	{
		freeArr(nodesArr, length);
		return EMPTY;
	}
	int headID = atoi(line);

	// add new edges
	while (fgets(line, MAX_LEN, meetingsInputFile))
	{
		char *strID1, *strID2, *strDist, *strTime;
		strID1 = strtok(line, WHITE_SPACE);
		strID2 = strtok(NULL, WHITE_SPACE);
		strDist = strtok(NULL, WHITE_SPACE);
		strTime = strtok(NULL, WHITE_SPACE);
		success = initEdge(nodesArr, indicesArr, strID1, strID2, strDist, strTime, length);
		if (!success)
		{
			freeArr(nodesArr, length);
			break;
		}
	}
	return (!success) ? success : headID;
}


/**
 * read the people and the meetings files
 * and initialize the array and returns
 * the index of the root node
 * @param pNodesArr the pointer to the
 * 		array of node
 * @param peoplePath the path to the
 * 		people.in file
 * @param meetingsPath the path to
 * 		the meetings.in file
 * @return EXIT_FAILURE for failure
 * 		EXIT_SUCCESS for EMPTY file
 * 		index of the root if otherwise
 */
int buildData(Node ***pNodesArr, char *peoplePath, char *meetingsPath, int *len)
{
	// read the people.in file
	FILE *people = fopen(peoplePath, READ);
	if (errorHandler((people != NULL), INPUT_FILES_ERR, NULL, FALSE))
	{
		(*pNodesArr) = NULL;
		return EXIT_FAILURE;
	}
	int length = readPeople(pNodesArr, people);
	fclose(people);
	if (errorHandler(length, STANDARD_LIB_ERR_MSG, NULL, FALSE))
	{
		(*pNodesArr) = NULL;
		return EXIT_FAILURE;
	}
	if (length == EMPTY)
	{
		FILE *meetings = fopen(meetingsPath, READ);
		// check the case of a empty file and then a invalid file
		if (errorHandler((meetings != NULL), INPUT_FILES_ERR, NULL, FALSE))
		{
			(*pNodesArr) = NULL;
			return EXIT_FAILURE;
		}
		fclose(meetings);
		output(NULL, NULL, EMPTY);
		(*pNodesArr) = NULL;
		return EXIT_SUCCESS;
	}
	int *indicesArr = createIndicesArr(length);
	if (errorHandler(mergeSortOnIndices((*pNodesArr), indicesArr, START, length - 1, TRUE),
		STANDARD_LIB_ERR_MSG, (*pNodesArr), length))

	{
		free(indicesArr);
		(*pNodesArr) = NULL;
		return EXIT_FAILURE;
	}
	// read the meeting.in file
	FILE *meetings = fopen(meetingsPath, READ);
	if (errorHandler((meetings != NULL), INPUT_FILES_ERR, (*pNodesArr), length))
	{
		free(indicesArr);
		(*pNodesArr) = NULL;
		return EXIT_FAILURE;
	}

	int headID = readMeetings((*pNodesArr), indicesArr, meetings, length);
	fclose(meetings);
	if (errorHandler(headID, INPUT_FILES_ERR, NULL, FALSE))
	{
		free(indicesArr);
		(*pNodesArr) = NULL;
		return EXIT_FAILURE;
	}
	if (headID == EMPTY)
	{
		free(indicesArr);
		output(NULL, NULL, EMPTY);
		(*pNodesArr) = NULL;
		return EXIT_SUCCESS;
	}
	int idx = findIndex((*pNodesArr), indicesArr, START, length - 1, headID);
	*len = length;
	free(indicesArr);
	return idx;
}


/****************************************************************************
 *                                  sort                                    *
 ***************************************************************************/

// I chose to use merge sort for all sort usages
// the merge sort will use a merge function with
// option to merge based on ID or crna


/**
 * preform the merge of left and
 * right back into indices array
 * in order
 * @param nodesArr the array of the values
 * @param indicesArr the array of indices
 * 		to work on
 * @param Left the left hand array
 * @param Right the right hand array
 * @param leftLen the length of Left
 * @param rightLen the length of Right
 * @param start the start index of the
 * 		part of the array to put the
 * 		values into
 * @param byID indicator wether to
 * 		sort by ID or by crna
 */
void merge2Arrays(Node **nodesArr, int *indicesArr, int *Left, int *Right, int leftLen, int
				  rightLen, int start, int byID)
{
	// Merge the temp arrays back
	int i = START, j = START, k = start;
	while (i < leftLen && j < rightLen)
	{
		// compare between the values of the two places
		// (not indexes which are the values of the array)
		if (((!byID) && (nodesArr[Left[i]]->crna <= nodesArr[Right[j]]->crna)) ||
			((byID) && (nodesArr[Left[i]]->ID <= nodesArr[Right[j]]->ID)))
		{
			indicesArr[k] = Left[i];
			i++;
		}
		else
		{
			indicesArr[k] = Right[j];
			j++;
		}
		k++;
	}
	// copy the remaining
	while (i < leftLen)
	{
		indicesArr[k] = Left[i];
		i++;
		k++;
	}
	while (j < rightLen)
	{
		indicesArr[k] = Right[j];
		j++;
		k++;
	}
}


/**
 * merge two parts of an array into one.
 * @param nodesArr the array of the
 * 		values to compare by
 * @param indicesArr the indices array
 * 		that is the array which will
 * 		be changed.
 * @param start first element index
 * @param middle middle element index
 * @param end last element index
 * @return TRUE for success FALSE
 * 		for failure
 */
int merge(Node **nodesArr, int *indicesArr, int start, int middle, int end, int byID)
{
	// the len of each halve of the array
	int leftLen = middle - start + 1;
	int rightLen = end - middle;
	// create two tmp arrays to merge
	int *Left, *Right;
	//allocate memory for the two parts
	Left = (int *) malloc((leftLen * sizeof(int)));
	if (Left == NULL)
	{
		return FALSE;
	}
	Right = (int *) malloc((rightLen * sizeof(int)));
	if (Right == NULL)
	{
		return FALSE;
	}

	// copy the two new values
	for (int i = START; i < leftLen; i++)
	{
		Left[i] = indicesArr[start + i];
	}
	for (int j = START; j < rightLen; j++)
	{
		Right[j] = indicesArr[middle + 1 + j];
	}
	merge2Arrays(nodesArr, indicesArr, Left, Right, leftLen, rightLen, start, byID);
	free(Left);
	free(Right);
	return TRUE;
}


/**
 * merge sort as thought in data structure course
 * using the merge function (as explained in data
 * structure course).
 * @param nodesArr array of node structs to
 * 		sort based on
 * @param indicesArr indices which represents
 * 		the array's order
 * @param start the first index in the array
 * @param end the last index in the array
 * @return TRUE for success FALSE for failre
 * (can happen only because of allocation in merge)
 */
int mergeSortOnIndices(Node **nodesArr, int *indicesArr, int start, int end, int byID)
{
	int success = TRUE;
	if (start < end)
	{
		int middle = (end + start) / 2;

		// Sort first then second halves and last, merge
		success = (mergeSortOnIndices(nodesArr, indicesArr, start, middle, byID) &&
				   mergeSortOnIndices(nodesArr, indicesArr, middle + 1, end, byID) &&
				   merge(nodesArr, indicesArr, start, middle, end, byID));

	}
	return success;
}


/****************************************************************************
 *                                  crna                                    *
 ***************************************************************************/


/**
 * calculate the crna factor between 2 persons.
 * (the edge weight)
 * @param dist the distance in meters between both
 * @param time the time they where next to each other
 * @return the crna factor for those persons
 */
float calcCrna(const float dist, const float time)
{
	return (time * MIN_DISTANCE) / (dist * MAX_TIME);
}


/**
 * calculate the crna factor for the whole tree recursively
 * @param head the current node we calculate the crna for
 * 		head of the subtree we are recursively calculating
 * 		from now on.
 * @param ancestorCrna the crna factor of the
 * 		ancestor of the node in the tree
 * @param edge the edge between two nodes in the tree
 * 		it contains the dist and time for calculating the
 * 		crna factor and the successor we can reach by using this edge
 * @return 1 for success else 0
 */
int calcCrnaForTree(Node *head, const float ancestorCrna, const Edge *edge)
{
	// calc crna for head if root crna is 1
	head->crna = (edge == NULL) ? ROOT_CRNA : (ancestorCrna * calcCrna(edge->dist, edge->time));

	// if has successors
	if (head->edges != NULL)
	{
		for (int i = START; i < head->numOfEdges; i++)
		{
			if (!calcCrnaForTree(head->edges[i]->successor, head->crna, head->edges[i]))
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}



/****************************************************************************
 *                                output                                    *
 ***************************************************************************/


/**
 * write the output file in order
 * @param personsArr the array of nodes
 * @param indicesArr the array of indices
 * @param length the length of relevant indices
 * @return TRUE for success else FALSE
 */
int output(Node **nodesArr, const int *indicesArr, const int length)
{
	FILE *out = fopen(OUTPUT_FILE, WRITE);
	if (out == NULL)
	{
		return FALSE;
	}
	if (nodesArr != NULL)
	{
		for (int i = length - 1; i >= START; i--)
		{
			if (nodesArr[indicesArr[i]]->crna >= MEDICAL_SUPERVISION_THRESHOLD)
			{
				fprintf(out, MEDICAL_SUPERVISION_THRESHOLD_MSG, nodesArr[indicesArr[i]]->name,
						(long unsigned int) nodesArr[indicesArr[i]]->ID);
			}
			else if (nodesArr[indicesArr[i]]->crna >= REGULAR_QUARANTINE_THRESHOLD)
			{
				fprintf(out, REGULAR_QUARANTINE_MSG, nodesArr[indicesArr[i]]->name,
						(long unsigned int) nodesArr[indicesArr[i]]->ID);
			}
			else
			{
				fprintf(out, CLEAN_MSG, nodesArr[indicesArr[i]]->name,
						(long unsigned int) nodesArr[indicesArr[i]]->ID);
			}
		}
	}
	fclose(out);
	return TRUE;
}


/****************************************************************************
 *                                 main                                     *
 ***************************************************************************/


/**
 * the running function.
 * it runs the whole program.
 * @param peoplePath the path to the people.in file
 * @param meetingsPath the path to the meetings.in file
 * @return EXIT_FAILURE or EXIT_SUCCESS
 * 		for failure and success of the program
 */
int run(char *peoplePath, char *meetingsPath)
{
	int success = TRUE;
	// init stuff
	Node **arr = NULL;
	int length = START;
	int rootIdx = buildData(&arr, peoplePath, meetingsPath, &length);
	if (arr == NULL)
	{
		return rootIdx;
	}

	// alg stuff
	success = calcCrnaForTree(arr[rootIdx], START, NULL);
	if (errorHandler(success, STANDARD_LIB_ERR_MSG, arr, length))
	{
		return EXIT_FAILURE;
	}

	int *indices = createIndicesArr(length);
	if (errorHandler((indices != NULL), STANDARD_LIB_ERR_MSG, arr, length))
	{
		return EXIT_FAILURE;
	}

	success = mergeSortOnIndices(arr, indices, START, length - 1, FALSE);
	if (errorHandler(success, STANDARD_LIB_ERR_MSG, arr, length))
	{
		return EXIT_FAILURE;
	}

	success = output(arr, indices, length);
	if (errorHandler(success, OUTPUT_FILES_ERR, arr, length))
	{
		return EXIT_FAILURE;
	}
	free(indices);
	freeArr(arr, length);
	return EXIT_SUCCESS;
}


/**
 * check if the length of the input array is valid
 * @param argc length of input array
 * @return TRUE if valid else FALSE
 */
int validInput(const int argc)
{
	if ((argc != ARG_NUM))
	{
		fprintf(stderr, INPUT_ERR);
		return FALSE;
	}
	return TRUE;
}


/**
 * if the number of input strings
 * is valid run the program
 * @param argc num of input args
 * @param argv the strings of the input args
 * @return
 */
int main(int argc, char *argv[])
{
	if (validInput(argc))
	{
		return run(argv[PEOPLE_IDX], argv[MEETINGS_IDX]);
	}
	return EXIT_FAILURE;
}




