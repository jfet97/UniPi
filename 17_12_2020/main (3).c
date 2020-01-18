#include <stdio.h>
#include <stdlib.h>

// a node to store values' occurences inside a matrix
typedef struct c{
	int value;
	int times;
	struct c* next;
} Counter;

typedef Counter* Values;

// read the number of rows and the number of columns for a matrix from the STDIN
void readMatrixDimensions(int* numOfRows, int* numOfColumns);
// dinamically alloc memory for a matrix
int** matrixAllocator(int numOfRows, int numOfColumns);
// read a matrix's values from the STDIN
void readMatrixData(int** matrix, int numOfRows, int numOfColumns);
// count how many occurences there are for each values into a matrix, returns a list of them
Values countValues(int** matrixA, int matrixARowLen, int matrixAColLen);
// insert a new node inside the values' occurences list
void insert(Values* valuesListPtrPtr, int value);
// check if all the values contained inside a list are contained into a matrix (it does check the number of occurences as well)
int checkPresenceOfValues(int** matrixB, int matrixBRowLen, int matrixBColLen, Values valuesListPtr);
// check the matrix's property that has to be checked following the exam guidelines (Ej[0,M).(Ei[0,N)...)
void checkProperty(int ** matrixA, int matrixARowLen, int matrixAColLen);
// dealloc the memory used for a matrix
void freeList(Values* valuesListPtrPtr);
// dealloc the memory used for a list
void freeMatrix(int ** matrix, int rowLen);


int main(void) {

	// read and store the dimensions for the first matrix
	int matrix1RowLen;
	int matrix1ColLen;
	readMatrixDimensions(&matrix1RowLen, &matrix1ColLen);

	// create the first matrix
	int **matrix1 = matrixAllocator(matrix1RowLen, matrix1ColLen);

	// read and store the values inside the first matrix
	readMatrixData(matrix1, matrix1RowLen, matrix1ColLen);
	
	// read and store the dimensions for the second matrix
	int matrix2RowLen;
	int matrix2ColLen;
	readMatrixDimensions(&matrix2RowLen, &matrix2ColLen);

	// create the second matrix
	int **matrix2 = matrixAllocator(matrix2RowLen, matrix2ColLen);

	// read and store the values inside the second matrix
	readMatrixData(matrix2, matrix2RowLen, matrix2ColLen);


	// determine who is matrix A and matrix B
	int** matrixA = (matrix1RowLen * matrix1ColLen)	<= (matrix2RowLen * matrix2ColLen) ? matrix1 : matrix2;
	int** matrixB = matrixA == matrix1 ? matrix2 : matrix1;
	int matrixARowLen = matrixA == matrix1 ? matrix1RowLen : matrix2RowLen;
	int matrixAColLen = matrixA == matrix1 ? matrix1ColLen : matrix2ColLen;
	int matrixBRowLen = matrixB == matrix1 ? matrix1RowLen : matrix2RowLen;
	int matrixBColLen = matrixB == matrix1 ? matrix1ColLen : matrix2ColLen;


	// count and store the values inside matrix A with their occurences as well
	Values valuesListPtr = countValues(matrixA, matrixARowLen, matrixAColLen);

	// if the matrix B respect its restriction (it has to contain all the values presents in the matrix A...)
	if(checkPresenceOfValues(matrixB, matrixBRowLen, matrixBColLen, valuesListPtr) == 1) {
		// check the property of the matrix A
		checkProperty(matrixA, matrixARowLen, matrixAColLen);
	} else {
		// else, print "NO"
		puts("NO");		
	}

	// dealloc the memory
	freeMatrix(matrix1, matrix1RowLen);
	matrix1 = NULL;
	freeMatrix(matrix2, matrix2RowLen);
	matrix2 = NULL;
	freeList(&valuesListPtr);
	
	return 0;
}

void readMatrixDimensions(int* numOfRows, int* numOfColumns) {
	scanf("%d %d", numOfRows, numOfColumns);	
}

int** matrixAllocator(int numOfRows, int numOfColumns) {
	// first alloc an array of 'numOfRows' pointers
	int **matrix = malloc(sizeof(*matrix) * numOfRows);
	
	if(matrix == NULL) {
		exit(1);	
	}

	// each pointer has to be a pointer to an array of int
	// how many elements are there in each array? 'numOfColumns'!
	for(int i = 0; i < numOfRows; i++) {
		matrix[i] = malloc(sizeof(**matrix) * numOfColumns);

		if(matrix[i] == NULL) {
			exit(1);
		}
	}

	return matrix;
}

void readMatrixData(int** matrix, int numOfRows, int numOfColumns) {
	for(int row = 0; row < numOfRows; row++) {
		for(int col = 0; col < numOfColumns; col++) {
			scanf("%d", &matrix[row][col]);		
		}
	}

}

Values countValues(int** matrixA, int matrixARowLen, int matrixAColLen) {
	Values valuesListPtr = NULL;
	for(int row = 0; row < matrixARowLen; row++) {
		for(int col = 0; col < matrixAColLen; col++) {
			// inser each value into the list of values
			insert(&valuesListPtr, matrixA[row][col]);		
		}
	}
	return valuesListPtr;
	
}

void insert(Values* valuesListPtrPtr, int value) {
	if(*valuesListPtrPtr == NULL) {
		// if we get here, it means the inserted value was a new one and we need a new node for it
		// because no corresponding node was found before
		Counter* newCounter = malloc(sizeof(*newCounter));
		if(newCounter == NULL) {
			exit(1);		
		}
		newCounter->times = 1;
		newCounter->value = value;
		newCounter->next = NULL;
		*valuesListPtrPtr = newCounter;
	} else if((*valuesListPtrPtr)->value == value) {
		// if we get here, it means the inserted value was already present into the list
		// so we have to increas the values' counter of the right node
		(*valuesListPtrPtr)->times++;
		return;	
	} else {
		// if we get here, it means the current node's value is not the inserted value
		// so we keep searching a corresponding node 
		insert(&(*valuesListPtrPtr)->next, value);
	}
}

int checkPresenceOfValues(int** matrixB, int matrixBRowLen, int matrixBColLen, Values valuesListPtr) {
	if(valuesListPtr == NULL) {
		// if we get here, it means all the values are present inside matrix B with enough occurences.
		// we have checked each node
		return 1;
	} else {
		// check if the value stored inside the current node is into matrix B enough times
		int currentCheckedValues = valuesListPtr->value;
		// counter
		int currentTimes = valuesListPtr->times;

		// check all the second matrix :(
		for(int row = 0; row < matrixBRowLen; row++) {
			for(int col = 0; col < matrixBColLen; col++) {
				if(matrixB[row][col] ==  currentCheckedValues) {
					currentTimes--;
				}	
			}
		}

		// are there enough occurences for the current node? if so, the counter is not greater than 0		
		if(currentTimes <= 0) {
			// in this case, we have to keep checking all the remaining nodes
			return checkPresenceOfValues(matrixB, matrixBRowLen, matrixBColLen, valuesListPtr->next);
		} else {
			// else we can stop, because for the current value there are no enough occurences
			return 0;
		}
		
	}
}

void checkProperty(int ** matrixA, int matrixARowLen, int matrixAColLen) {
	
	// "boolean" flag that knows if, in a certain row, at least one value that is <0 was already found
	// it is useful only for avoid to print the ';'
	int foundAtLeastOneInThisRow = 0;

	// it starts from the bottom of the matrix...
	for(int row = matrixARowLen -1; row >= 0; row--) {	
		for(int col = 0; col < matrixAColLen; col++) {	
			// ...printing values only if the last element of a column is <0 		
			if(matrixA[matrixARowLen-1][col] < 0) {
				
				// logic for print the ; when it should be printed
				if(foundAtLeastOneInThisRow == 1) {
					printf(";");
				} else {
					foundAtLeastOneInThisRow = 1;
				} 

				printf("%d", matrixA[row][col]);
			}
		}
		
		// yes, we need a newline right here
		puts("");
		
		// reset the ; flag for the next row  
		foundAtLeastOneInThisRow = 0;
	}
}

void freeMatrix(int ** matrix, int rowLen) {
	// first, free the memory for each sub-array
	for(int row = 0; row < rowLen; row++) {
		free(matrix[row]);
	}
	// then free the matrix pointer, the array of int* pointer
	free(matrix);
}

void freeList(Values* valuesListPtrPtr) {
	if(*valuesListPtrPtr == NULL) {
		return;
	} else {
		// first, free the rest of the list
		freeList(&(*valuesListPtrPtr)->next);
		// then, free the current node
		free(*valuesListPtrPtr);
		*valuesListPtrPtr = NULL;
	}
}


