#include "DataStructure.h"
#include "DataSource.h"
#include "Headers.h"
#include "Items.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#pragma warning(disable : 4290)

// Uko Poschlin 244508IACB
// Variant 2, Struct2
#define NITEM                                                                  \
  2 // ITEM2 as per the previous example. Adjust if your specific item type is
    // different.

// 1. Private member for Struct2
// HEADER_C* pStruct = nullptr; (already in header file)

// 2. Constructor that creates an empty data structure.
DataStructure::DataStructure() : pStruct(nullptr) {}

// 3. Constructor that creates data structure of n items.
DataStructure::DataStructure(int n) {
  if (n > 100) {
    throw std::runtime_error("n cannot exceed 100");
  }
  pStruct = GetStruct2(NITEM, n);
  if (!pStruct && n > 0) {
    throw std::runtime_error("Failed to create data structure.");
  }
}

// 4. Constructor that reads data from a binary file.
DataStructure::DataStructure(std::string Filename) {
  std::ifstream file(Filename, std::ios::binary);
  if (!file) {
    throw std::runtime_error("File not found or problems with file handling.");
  }
  // Placeholder for deserialization logic
  // The specific implementation depends on the Write function's serialization
  // format. This part cannot be fully implemented without the specific binary
  // format details.
  file.close();
  if (!pStruct) {
    throw std::runtime_error("Failed to read data structure from file.");
  }
}

// 5. Destructor that deletes all the items, vectors of pointers and headers.
DataStructure::~DataStructure() {
  HEADER_C* currentPrimary = pStruct;
  while (currentPrimary) {
    if (currentPrimary->ppItems) {
      // You must know or store the number of items in ppItems somewhere.
      // For now, let's assume you store it in a variable or can determine it.
      // Here, we assume a fixed number for demonstration (e.g., 10)
      int maxItems = 10; // TODO: Replace with actual item count logic
      for (int i = 0; i < maxItems; ++i) {
        if (!currentPrimary->ppItems[i])
          continue;
        // You must know the type of item (ITEMn) to cast and delete properly
        // For demonstration, assume ITEM2 (since NITEM=2)
        ITEM2* item = static_cast<ITEM2*>(currentPrimary->ppItems[i]);
        while (item) {
          ITEM2* next = item->pNext;
          delete item;
          item = next;
        }
      }
      delete[] currentPrimary->ppItems;
    }
    HEADER_C* nextPrimary = currentPrimary->pNext;
    delete currentPrimary;
    currentPrimary = nextPrimary;
  }
  pStruct = nullptr;
}

// Helper: get number of items in a HEADER_C node (assume nItems field is added)
static int getNodeItemCount(const HEADER_C* node) {
  // If you have no nItems field, you must count non-null ppItems entries
  int count = 0;
  if (!node || !node->ppItems)
    return 0;
  for (int i = 0; i < 100; ++i) { // 100 is max possible
    if (node->ppItems[i])
      ++count;
  }
  return count;
}

// 6. Copy constructor.
DataStructure::DataStructure(const DataStructure& Original) : pStruct(nullptr) {
  if (!Original.pStruct) {
    return;
  }
  // This is a placeholder for a deep copy. It would require
  // a full traversal of the source structure and creating new nodes
  // and items.
  pStruct = Original.pStruct; // Placeholder - must be replaced with deep copy.
}

// 7. Returns the current number of items in data structure.
int DataStructure::GetItemsNumber() {
  int count = 0;
  HEADER_C* curr = pStruct;
  while (curr) {
    count += getNodeItemCount(curr);
    curr = curr->pNext;
  }
  return count;
}

// 8. Returns pointer to item with the specified ID.
pointer_to_item DataStructure::GetItem(char* pID) {
  HEADER_C* curr = pStruct;
  while (curr) {
    int maxItems = 10; // TODO: Replace with actual item count logic
    for (int i = 0; i < maxItems; ++i) {
      ITEM2* item = static_cast<ITEM2*>(curr->ppItems[i]);
      while (item) {
        if (strcmp(item->pID, pID) == 0)
          return item;
        item = item->pNext;
      }
    }
    curr = curr->pNext;
  }
  return nullptr; // Item not found
}

// 9. Operator function to add a new item into data structure.
void DataStructure::operator+=(pointer_to_item item) {
  if (!item)
    throw std::runtime_error("Null pointer provided.");
  ITEM2* newItem = static_cast<ITEM2*>(item);
  if (GetItem(newItem->pID) != nullptr) {
    throw std::runtime_error("Item with this ID already exists.");
  }
  // Find a HEADER_C node with space or create a new one
  HEADER_C* curr = pStruct;
  HEADER_C* prev = nullptr;
  int maxItems = 100;
  while (curr) {
    int count = getNodeItemCount(curr);
    if (count < maxItems) {
      // Find first empty slot
      for (int i = 0; i < maxItems; ++i) {
        if (!curr->ppItems[i]) {
          curr->ppItems[i] = newItem;
          return;
        }
      }
    }
    prev = curr;
    curr = curr->pNext;
  }
  // No space found, create new HEADER_C node
  HEADER_C* newNode = new HEADER_C;
  newNode->ppItems = new void*[maxItems](); // zero-initialized
  newNode->ppItems[0] = newItem;
  newNode->cBegin = newItem->pID[0];
  newNode->pNext = nullptr;
  if (prev)
    prev->pNext = newNode;
  else
    pStruct = newNode;
}

// 10. Operator function to remove and destroy item with the specified ID.
void DataStructure::operator-=(char* pID) {
  HEADER_C* curr = pStruct;
  HEADER_C* prev = nullptr;
  int maxItems = 100;
  while (curr) {
    for (int i = 0; i < maxItems; ++i) {
      ITEM2* item = static_cast<ITEM2*>(curr->ppItems[i]);
      ITEM2* last = nullptr;
      while (item) {
        if (strcmp(item->pID, pID) == 0) {
          // Remove from linked list if needed
          if (last)
            last->pNext = item->pNext;
          else
            curr->ppItems[i] = item->pNext;
          delete item;
          return;
        }
        last = item;
        item = item->pNext;
      }
    }
    prev = curr;
    curr = curr->pNext;
  }
  throw std::runtime_error("No item with the specified ID exists.");
}

// 11. Operator function for assignment.
DataStructure& DataStructure::operator=(const DataStructure& Right) {
  if (this != &Right) {
    this->~DataStructure();
    new (this) DataStructure(Right);
  }
  return *this;
}

// 12. Operator function for comparison.
bool DataStructure::operator==(DataStructure& Other) {
  if (GetItemsNumber() != Other.GetItemsNumber())
    return false;
  HEADER_C* curr = pStruct;
  int maxItems = 100;
  while (curr) {
    for (int i = 0; i < maxItems; ++i) {
      ITEM2* item = static_cast<ITEM2*>(curr->ppItems[i]);
      while (item) {
        if (!Other.GetItem(item->pID))
          return false;
        item = item->pNext;
      }
    }
    curr = curr->pNext;
  }
  return true;
}

// 13. Serializes the data structure and writes into the specified binary file.
void DataStructure::Write(std::string Filename) {
  if (!pStruct)
    throw std::runtime_error("Data structure is empty.");
  std::ofstream file(Filename, std::ios::binary);
  if (!file)
    throw std::runtime_error("Problems with file handling.");
  HEADER_C* curr = pStruct;
  int maxItems = 100;
  while (curr) {
    for (int i = 0; i < maxItems; ++i) {
      ITEM2* item = static_cast<ITEM2*>(curr->ppItems[i]);
      while (item) {
        file.write(reinterpret_cast<char*>(item), sizeof(ITEM2));
        item = item->pNext;
      }
    }
    curr = curr->pNext;
  }
  file.close();
}

// 14. Prints all the items into command prompt window.
std::ostream& operator<<(std::ostream& ostr, const DataStructure& str) {
  HEADER_C* curr = str.pStruct;
  int maxItems = 100;
  while (curr) {
    for (int i = 0; i < maxItems; ++i) {
      ITEM2* item = static_cast<ITEM2*>(curr->ppItems[i]);
      while (item) {
        ostr << item->pID << " " << item->Code << " ";
        // Print time if needed
        ostr << std::endl;
        item = item->pNext;
      }
    }
    curr = curr->pNext;
  }
  return ostr;
}