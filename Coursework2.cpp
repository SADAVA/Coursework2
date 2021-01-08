
// S.

#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"

class DataStructure
{
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
    std::cout << "Hello World!\n";
    HEADER_D* p = GetStruct4(2, 20);
}

// EOF