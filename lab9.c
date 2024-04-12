#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 20  // Define hash table size

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType *next;  // Pointer to next record in chain (for chaining)
};

// HashType
struct HashType
{
    struct RecordType *chain;  // Pointer to the first element in the chain
};

// Hash function
int hash(int x)
{
    return x % HASH_SIZE;
}

// Parse input file to create RecordType array
int parseData(const char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL;  // Initialize next pointer to NULL
        }

        fclose(inFile);
    }

    return dataSz;
}

// Print records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n");
}

// Insert record into hash table
void insertIntoHash(struct HashType hashTable[], struct RecordType *record)
{
    int index = hash(record->id);

    // Create a new node for the record
    struct RecordType *newNode = (struct RecordType *)malloc(sizeof(struct RecordType));
    if (!newNode)
    {
        printf("Memory allocation error\n");
        exit(-1);
    }
    *newNode = *record;
    newNode->order = -1;  // Reset order to -1 (optional)

    // Insert into the beginning of the chain
    newNode->next = hashTable[index].chain;
    hashTable[index].chain = newNode;
}

// Build hash table from records
void buildHashTable(struct HashType hashTable[], struct RecordType *records, int recordSz)
{
    for (int i = 0; i < recordSz; ++i)
    {
        insertIntoHash(hashTable, &records[i]);
    }
}

// Display records stored in hash table
void displayRecordsInHash(struct HashType hashTable[], int hashSz)
{
    printf("\nRecords in Hash Table:\n");
    for (int i = 0; i < hashSz; ++i)
    {
        struct RecordType *current = hashTable[i].chain;
        if (current)
        {
            printf("Index %d -> ", i);
            while (current)
            {
                printf("%d %c %d -> ", current->id, current->name, current->order);
                current = current->next;
            }
            printf("\n");
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    recordSz = parseData("input.txt", &pRecords);


    printRecords(pRecords, recordSz);

    // Initialize hash table
    struct HashType hashTable[HASH_SIZE];
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        hashTable[i].chain = NULL;
    }

    // Build hash table from records
    buildHashTable(hashTable, pRecords, recordSz);

    // Display records stored in hash table
    displayRecordsInHash(hashTable, HASH_SIZE);
    free(pRecords);

    return 0;
}
