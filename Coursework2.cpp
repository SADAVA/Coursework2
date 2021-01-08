
// S.

#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"

class DataStructure
{
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

    }

    ITEM2* GetItem(char* pID)
    {   // Returns pointer to item with the specified ID.If the item was not found, returns 0.

    }

    void operator+=(ITEM2* i)
    {   // Operator function to add a new item into data structure.

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
    {   // Printsall the items into command prompt window. 

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
    std::cout << dataFromFile << std::endl << std::endl;
}

// EOF