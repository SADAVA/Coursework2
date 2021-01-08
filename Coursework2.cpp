
// S.

#include <iostream>
#include <iomanip>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"

void PrintTimeStructure(std::ostream& ostr, TIME* p)
{   // Print in format: 01:23:45
    if (p == NULL)
    {
        ostr << "Can't print Time structure because it is empty";
        return;
    }

    ostr << std::setfill('0') << std::setw(2) << p->Hour;
    ostr << ":";
    ostr << std::setfill('0') << std::setw(2) << p->Min;
    ostr << ":";
    ostr << std::setfill('0') << std::setw(2) << p->Sec;
}

class DataStructure
{
private:
    HEADER_D* firstHeaderD = NULL;

    char FindDID(char* id)
    {   // Returns first char, or NULL
        return id[0];
    }

    char FindAID(char* id)
    {   // Returns char after space, or NULL
        bool nextCharIsWhatWeNeed = false;

        for (char c = *id; c; c = *++id)
        {
            if (nextCharIsWhatWeNeed == true) return c;
            if (c == ' ') nextCharIsWhatWeNeed = true;
        }

        // Char not found
        return NULL;
    }

    HEADER_D* FindHeaderD(char id)
    {   // Finds D header by ID, or else NULL
        HEADER_D* d = firstHeaderD;

        while (d != NULL)
        {
            if (d->cBegin == id) return d;
            d = d->pNext;
        }

        // Not found
        return NULL;
    }

    HEADER_D* CreateNewEmptyHeaderD()
    {
        HEADER_D* d = new HEADER_D;
        d->cBegin = NULL;
        d->pHeaderA = NULL;
        d->pNext = NULL;
        d->pPrior = NULL;
        return d;
    }

    HEADER_D* CreateHeaderD(char id)
    {   // Creates and inserts in appropriate place a new D header, or else NULL
        HEADER_D* d = firstHeaderD;

        // Our new D Header
        HEADER_D* newD = CreateNewEmptyHeaderD();
        newD->cBegin = id;

        // If no first header or first header is already higher than new ID
        if (firstHeaderD == NULL || id < firstHeaderD->cBegin)
        {
            newD->pNext = firstHeaderD;
            firstHeaderD = newD;
            return newD;
        }

        // Iterate D Headers
        while (d != NULL)
        {
            if (d->cBegin < id)
            {
                if (d->pNext == NULL)
                {   // We are at the end of all D headers
                    // Add new header as last
                    d->pNext = newD;
                    return newD;
                }
                if (id < d->pNext->cBegin)
                {   // We have the PERFECT spot for new header!
                    newD->pNext = d->pNext; // Change pNext to next D header
                    d->pNext = newD;        // Change current's next header to new header
                    return newD;            // Done.
                }
            }

            // Next D header please!
            if (d->pNext == NULL)
            {   // We are at the end of all D headers
                // Add new header as last
                d->pNext = newD;
                return newD;
            }
            else {
                // More headers to check, keep iterating
                d = d->pNext;
            }
        }

        // D headers missing
        return NULL;
    }

    HEADER_A* FindHeaderA(HEADER_D* d, char id)
    {   // Finds A header by ID, or else NULL
        HEADER_A* a = d->pHeaderA;

        while (a != NULL)
        {
            if (a->cBegin == id) return a;
            a = a->pNext;
        }

        // Not found
        return NULL;
    }

    HEADER_A* CreateNewEmptyHeaderA()
    {
        HEADER_A* a = new HEADER_A;
        a->cBegin = NULL;
        a->pItems = NULL;
        a->pNext = NULL;
        return a;
    }

    HEADER_A* CreateHeaderA(HEADER_D* d, char id)
    {   // Creates and inserts in appropriate place a new A header, or else NULL
        HEADER_A* a = d->pHeaderA;

        // Our new A Header
        HEADER_A* newA = CreateNewEmptyHeaderA();
        newA->cBegin = id;

        // If no first header or first header is already higher than new ID
        if (d->pHeaderA == NULL || id < d->pHeaderA->cBegin)
        {
            newA->pNext = d->pHeaderA;
            d->pHeaderA = newA;
            return newA;
        }

        // Iterate A Headers
        while (a != NULL)
        {
            if (a->cBegin < id)
            {
                if (a->pNext == NULL)
                {   // We are at the end of all A headers
                    // Add new header as last
                    a->pNext = newA;
                    return newA;
                }
                if (id < a->pNext->cBegin)
                {   // We have the PERFECT spot for new header!
                    newA->pNext = a->pNext; // Change pNext to next A header
                    a->pNext = newA;        // Change current's next header to new header
                    return newA;            // Done.
                }
            }

            // Next A header please!
            if (a->pNext == NULL)
            {   // We are at the end of all A headers
                // Add new header as last
                a->pNext = newA;
                return newA;
            }
            else {
                // More headers to check, keep iterating
                a = a->pNext;
            }
        }

        // A headers missing
        return NULL;
    }

    void AppendItem(HEADER_A* a, ITEM2* i)
    {   // Adds item to top of A header
        i->pNext = (ITEM2*)a->pItems;
        a->pItems = i;
    }


public:
    DataStructure()
    {   // Constructor that creates empty data structure.
    }

    DataStructure(char* pFilename)
    {   // Constructor that reads data from a binary file.

    }

    ~DataStructure()
    {   // Destructor that deletes all the items, vectors of pointersand headers.

    }

    DataStructure(const DataStructure& Original)
    {   // Copy constructor.

    }

    int GetItemsNumber()
    {   // Returns the current number of items indata structure.

        int counter = 0;

        HEADER_D* d = firstHeaderD;

        while (d != NULL)
        {
            HEADER_A* a = d->pHeaderA;

            while (a != NULL)
            {
                ITEM2* i = (ITEM2*)a->pItems;

                while (i != NULL)
                {
                    counter++;
                    i = i->pNext;
                }

                a = a->pNext;
            }

            d = d->pNext;
        }

        return counter;
    }

    ITEM2* GetItem(char* pID)
    {   // Returns pointer to item with the specified ID.If the item was not found, returns 0.

    }

    void operator+=(ITEM2* i)
    {   // Operator function to add a new item into data structure.
        char dID = FindDID(i->pID);
        char aID = FindAID(i->pID);

        HEADER_D* d = FindHeaderD(dID);
        if (d == NULL)
            d = CreateHeaderD(dID);

        HEADER_A* a = FindHeaderA(d, aID);
        if (a == NULL)
            a = CreateHeaderA(d, aID);

        AppendItem(a, i);
    }

    void operator-=(char* pID)
    {   // Operator function to removeand destroy item with the specified ID.

    }

    DataStructure& operator=(const DataStructure& Right)
    {   // Operator function for assignment.

    }

    int operator==(DataStructure& Other)
    {   // Operator function for comparison. Returs 0 (not equal) or 1 (equal).

    }

    void Write(char* pFilename)
    {   // Serializes the data structureand writes into the specified file.

    }

    friend std::ostream& operator<<(std::ostream& ostr, const DataStructure& str)
    {   // Prints all the items into command prompt window. 
        HEADER_D* d = str.firstHeaderD;

        while (d != NULL)
        {
            HEADER_A* a = d->pHeaderA;

            while (a != NULL)
            {
                ITEM2* i = (ITEM2*)a->pItems;

                while (i != NULL)
                {
                    ostr << d->cBegin << "  " << a->cBegin << " ";
                    ostr << std::setfill(' ') << std::setw(16) << i->pID;
                    ostr << " ";
                    ostr << std::setfill(' ') << std::setw(10) << i->Code;
                    ostr << " ";
                    PrintTimeStructure(ostr, i->pTime);
                    ostr << std::endl;
                    i = i->pNext;
                }

                a = a->pNext;
            }

            d = d->pNext;
        }

        return ostr;
    }
};

int main()
{
    std::cout << "######### Creating new DataStructure #########" << std::endl;
    DataStructure data;

    int numberOfItems = 10;
    std::cout << "######### Adding " << numberOfItems << " new items #########" << std::endl;

    for (int i = 0; i < numberOfItems; i++)
        data += (ITEM2*)GetItem(2);

    std::cout << "######### Print current DataStructure #########" << std::endl;
    std::cout << data << std::endl << std::endl;

    std::cout << "######### Size of DataStructure: " << data.GetItemsNumber() << std::endl;

    /*const char* getItemID = "Light Cyan";
    std::cout << "######### Get item: " << getItemID << " #########" << std::endl;
    ITEM2* getItem = data.GetItem((char*)getItemID);

    const char* getItemID2 = "X X";
    std::cout << "######### Get NON-EXISTING item: " << getItemID2 << " #########" << std::endl;
    ITEM2* getItem2 = data.GetItem((char*)getItemID2);

    std::cout << "######### Creating copy of DataStructure #########" << std::endl;
    DataStructure dataCopy = data;

    std::cout << "######### Removing from initial data #########" << std::endl;
    const char* itemsToRemove[] = { "Banana Mania", "Persian Green", "Vegas Gold" };
    for (const char* itemToRemove : itemsToRemove)
        data -= (char*)itemToRemove;

    std::cout << "######### Compare inital data to copy #########" << std::endl;
    if (data == dataCopy)
        std::cout << "DATAs ARE THE SAME" << std::endl;
    else
        std::cout << "DATAs ARE DIFFERENT" << std::endl;

    const char* filename = "datastructure.bin";
    std::cout << "######### Writing initial data structure TO file: " << filename << " #########" << std::endl;
    data.Write((char*)filename);

    std::cout << "######### Creating new data structure FROM file: " << filename << " #########" << std::endl;
    DataStructure dataFromFile = DataStructure((char*)filename);

    std::cout << "######### Compare inital data to data from file #########" << std::endl;
    if (data == dataFromFile)
        std::cout << "DATAs ARE THE SAME" << std::endl;
    else
        std::cout << "DATAs ARE DIFFERENT" << std::endl;

    std::cout << "######### Assign copy to fromFile data structure #########" << std::endl;
    dataFromFile = dataCopy;

    std::cout << "######### Print the result #########" << std::endl;
    std::cout << dataFromFile << std::endl << std::endl; //*/

    std::cout << "FINISH" << std::endl;
}

// EOF