
// S.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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

void PrintItem(std::ostream& ostr, ITEM2* i)
{
    if (i == NULL)
    {
        ostr << "[ERROR] Item is NULL" << std::endl;
        return;
    }
    ostr << std::setfill(' ') << std::setw(16) << i->pID;
    ostr << " ";
    ostr << std::setfill(' ') << std::setw(10) << i->Code;
    ostr << " ";
    PrintTimeStructure(ostr, i->pTime);
    ostr << std::endl;
}

bool StringsAreTheSame(char* a, char* b)
{
    return strcmp(a, b) == 0;
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

    bool ItemWithIDExists(char* pItemID)
    {
        // First D header
        HEADER_D* d = firstHeaderD;

        // Iterate D headers
        while (d != NULL)
        {
            // First A header
            HEADER_A* a = d->pHeaderA;

            // Itertate A headers
            while (a != NULL)
            {
                // First item
                ITEM2* i = (ITEM2*)a->pItems;

                // Iterate items
                while (i != NULL)
                {
                    // Match item's ID to ID we are looking for
                    if (StringsAreTheSame(i->pID, pItemID) == true)
                        return true; // FOUND IT

                    // Switch to next item
                    i = i->pNext;
                }

                // Switch to next header A
                a = a->pNext;
            }

            // Switch to next header D
            d = d->pNext;
        }

        // No matches, means none of the items has this given ID
        return false;
    }

    TIME* ReplicateTime(TIME* t)
    {
        TIME* newTime = new TIME;

        newTime->Hour = t->Hour;
        newTime->Min  = t->Min;
        newTime->Sec  = t->Sec;

        return newTime;
    }

    ITEM2* ReplicateItem(ITEM2* original)
    {
        ITEM2* newItem = new ITEM2;
        newItem->Code = original->Code;
        newItem->pTime = ReplicateTime(original->pTime);
        
        char* copy = (char*)malloc(strlen(original->pID) + 1);
        strcpy_s(copy, strlen(original->pID) + 1, original->pID);
        newItem->pID = copy;

        return newItem;
    }

    std::string SerializeTime(TIME* t)
    {
        std::ostringstream res;

        res << t->Hour << ":" << t->Min << ":" << t->Sec;

        return res.str();
    }

    std::string SerializeItem2(ITEM2* i)
    {
        std::ostringstream res;

        res << i->pID << "*" << i->Code << "*" << SerializeTime(i->pTime);

        return res.str();
    }

    TIME* DeserializeTime(std::string line)
    {
        TIME* time = new TIME;


        std::string stringyHour = line.substr(0, line.find("*", 0)); // Slice Hour
        line = line.substr(line.find(":", 0) + 1, line.length()); // Slice off the Hour

        std::string stringyMin = line.substr(0, line.find("*", 0)); // Slice Min
        line = line.substr(line.find(":", 0) + 1, line.length()); // Slice off the Min

        std::string stringySec = line; // What's left is Sec

        time->Hour = std::stoi(stringyHour);
        time->Min  = std::stoi(stringyMin);
        time->Sec  = std::stoi(stringySec);

        return time;
    }

    ITEM2* DeserializeItem2(std::string line)
    {
        ITEM2* item = new ITEM2;


        std::string stringyID = line.substr(0, line.find("*", 0)); // Slice ID
        char* charID = (char*)stringyID.c_str();
        line = line.substr(line.find("*", 0) + 1, line.length()); // Slice off the ID

        std::string stringyCode = line.substr(0, line.find("*", 0)); // Slice Code
        line = line.substr(line.find("*", 0) + 1, line.length()); // Slice off the Code

        // What's left is Time
        item->pNext = NULL;
        char* copy = (char*)malloc(strlen(charID) + 1);
        strcpy_s(copy, strlen(charID) + 1, charID);
        item->pID = copy;
        item->Code = std::stoul(stringyCode);
        item->pTime = DeserializeTime(line);

        return item;
    }

    void DeleteEverything()
    {   // Deletes every item, every A header and every D header
        HEADER_D* d = firstHeaderD;
        HEADER_D* dNext = NULL;

        while (d != NULL)
        {
            HEADER_A* a = d->pHeaderA;
            HEADER_A* aNext = NULL;

            while (a != NULL)
            {
                ITEM2* i = (ITEM2*)a->pItems;
                ITEM2* iNext = NULL;

                while (i != NULL)
                {
                    iNext = i->pNext;
                    delete i;
                    i = iNext;
                }

                aNext = a->pNext;
                delete a;
                a = aNext;
            }

            dNext = d->pNext;
            delete d;
            d = dNext;
        }

        firstHeaderD = NULL;
    }
public:
    DataStructure()
    {   // Constructor that creates empty data structure.
    }

    DataStructure(char* pFilename)
    {   // Constructor that reads data from a binary file.
        std::ifstream file(pFilename);

        std::string line;

        while (std::getline(file, line))
        {
            ITEM2* i = DeserializeItem2(line);
            (*this) += i;
        }
    }

    ~DataStructure()
    {   // Destructor that deletes all the items, vectors of pointersand headers.

    }


    DataStructure(const DataStructure& Original)
    {   // Copy constructor.
        // Simply iterate over all items in `Original` and add them to `this`, structure will be recreated.
        HEADER_D* d = Original.firstHeaderD;

        while (d != NULL)
        {
            HEADER_A* a = d->pHeaderA;

            while (a != NULL)
            {
                ITEM2* i = (ITEM2*)a->pItems;

                while (i != NULL)
                {
                    (*this) += ReplicateItem(i);
                    i = i->pNext;
                }

                a = a->pNext;
            }

            d = d->pNext;
        }
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
    {   // Returns pointer to item with the specified ID. If the item was not found, returns 0.
        HEADER_D* d = firstHeaderD;

        while (d != NULL)
        {
            HEADER_A* a = d->pHeaderA;

            while (a != NULL)
            {
                ITEM2* i = (ITEM2*)a->pItems;

                while (i != NULL)
                {
                    if (StringsAreTheSame(i->pID, pID) == true) return i;
                    i = i->pNext;
                }

                a = a->pNext;
            }

            d = d->pNext;
        }

        return NULL;
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

    void operator-=(char* pRemoveItemID)
    {   // Operator function to removeand destroy item with the specified ID.
        if (pRemoveItemID == NULL)
        {
            std::cout << "In order to remove an item ID must be specified" << std::endl;
            return;
        }
        if (ItemWithIDExists(pRemoveItemID) != true)
        {
            std::cout << "Item with such ID DOES NOT exist!" << std::endl;
            return;
        }

        // First D header
        HEADER_D* d = firstHeaderD;
        HEADER_D* dPrevious = NULL;

        // Iterate D headers
        while (d != NULL)
        {
            // First A header
            HEADER_A* a = d->pHeaderA;
            HEADER_A* aPrevious = NULL;

            // Itertate A headers
            while (a != NULL)
            {
                // First item
                ITEM2* i = (ITEM2*)a->pItems;
                ITEM2* iPrevious = NULL;

                // Iterate items
                while (i != NULL)
                {
                    // Match item's ID to ID we are looking for
                    if (StringsAreTheSame(i->pID, pRemoveItemID) == true)
                    {
                        // REMOVING ITEM
                        ITEM2* iNext = i->pNext;
                        delete i;

                        if (iPrevious == NULL) // This is the first item in header A
                        {
                            if (iNext != NULL)
                            {
                                a->pItems = iNext;
                            }
                            else // No next and no previous
                            {
                                // REMOVING A HEADER
                                HEADER_A* aNext = a->pNext;
                                delete a;

                                if (aPrevious == NULL) // This is the first A header
                                {
                                    if (aNext == NULL) // No next and no previous
                                    {
                                        // REMOVING D HEADER
                                        HEADER_D* dNext = d->pNext;
                                        delete d;

                                        if (dPrevious == NULL) // This is the first d header
                                        {
                                            firstHeaderD = dNext;
                                        }
                                        else
                                        {
                                            dPrevious->pNext = dNext;
                                        }
                                    }
                                    else
                                    {
                                        d->pHeaderA = aNext;
                                    }
                                }
                                else
                                {
                                    aPrevious->pNext = aNext;
                                }

                            }
                        }
                        else
                        {
                            iPrevious->pNext = iNext;
                        }

                        // Item deleted, return
                        return;
                    }

                    // Switch to next item
                    iPrevious = i;
                    i = i->pNext;
                }

                // Switch to next header A
                aPrevious = a;
                a = a->pNext;
            }

            // Switch to next header D
            dPrevious = d;
            d = d->pNext;
        }

        std::cout << "Failed to remove item, ID not found" << std::endl;
    }

    DataStructure& operator=(const DataStructure& Right)
    {   // Operator function for assignment.
        DeleteEverything();

        // Iterate all items in `Right`
        // Replicate item and add it to `this`
        HEADER_D* d = Right.firstHeaderD;

        while (d != NULL)
        {
            HEADER_A* a = d->pHeaderA;

            while (a != NULL)
            {
                ITEM2* i = (ITEM2*)a->pItems;

                while (i != NULL)
                {
                    (*this) += ReplicateItem(i);
                    i = i->pNext;
                }

                a = a->pNext;
            }

            d = d->pNext;
        }

        return *this;
    }

    int operator==(DataStructure& Other)
    {   // Operator function for comparison. Returs 0 (not equal) or 1 (equal)./
        // Just iterate over all items in `Other`, and check if the item exists in `this` DataStructure

        if (GetItemsNumber() != Other.GetItemsNumber())
            return false; // Number of items is different

        HEADER_D* d = Other.firstHeaderD;

        while (d != NULL)
        {
            HEADER_A* a = d->pHeaderA;

            while (a != NULL)
            {
                ITEM2* i = (ITEM2*)a->pItems;

                while (i != NULL)
                {
                    if (ItemWithIDExists(i->pID) != true)
                        return false; // This item does not exist, structs are different
                    i = i->pNext;
                }

                a = a->pNext;
            }

            d = d->pNext;
        }

        // All items checked and all of them exist
        // structs are the same
        return true;
    }

    void Write(char* pFilename)
    {   // Serializes the data structureand writes into the specified file.
        std::ofstream file(pFilename, std::ios::trunc);

        HEADER_D* d = firstHeaderD;

        while (d != NULL)
        {
            HEADER_A* a = d->pHeaderA;

            while (a != NULL)
            {
                ITEM2* i = (ITEM2*)a->pItems;

                while (i != NULL)
                {
                    file << SerializeItem2(i) << std::endl;
                    i = i->pNext;
                }

                a = a->pNext;
            }

            d = d->pNext;
        }

        file.close();
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
                    ostr << d->cBegin << "" << a->cBegin << " ";
                    PrintItem(ostr, i);
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

    const char* getItemID = "Light Cyan";
    std::cout << "######### Get item: " << getItemID << " #########" << std::endl;
    ITEM2* getItem = data.GetItem((char*)getItemID);
    PrintItem(std::cout, getItem);

    const char* getItemID2 = "X X";
    std::cout << "######### Get NON-EXISTING item: " << getItemID2 << " #########" << std::endl;
    ITEM2* getItem2 = data.GetItem((char*)getItemID2);
    PrintItem(std::cout, getItem2);

    std::cout << "######### Creating copy of DataStructure #########" << std::endl;
    DataStructure dataCopy = DataStructure(data);

    std::cout << "######### Removing from initial data #########" << std::endl;
    const char* itemsToRemove[] = { "Banana Mania", "Persian Green", "Vegas Gold" };
    for (const char* itemToRemove : itemsToRemove)
        data -= (char*)itemToRemove;

    std::cout << "######### Compare inital data to copy #########" << std::endl;
    if (data == dataCopy)
        std::cout << "DATAs ARE THE SAME" << std::endl;
    else
        std::cout << "DATAs ARE DIFFERENT" << std::endl;

    const char* filename = "datastructure.txt";
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
    std::cout << "######### data #########" << std::endl;
    std::cout << data << std::endl << std::endl;

    std::cout << "######### dataFromFile #########" << std::endl;
    std::cout << dataFromFile << std::endl << std::endl;

    std::cout << "######### dataCopy #########" << std::endl;
    std::cout << dataCopy << std::endl << std::endl;

    std::cout << "######### FINISH #########" << std::endl;
}

// EOF