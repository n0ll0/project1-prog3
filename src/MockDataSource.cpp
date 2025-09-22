// Mock DataSource implementation for cross-platform development
// This is a simplified implementation that provides basic functionality
// for testing when the original Windows DataSource.dll is not available

#include "DataSource.h"
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "platform_compat.h"
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>

#ifdef _WIN32
// On Windows, don't compile this - use the real DLL
#else

// Simple color names for mock implementation
static const char* mock_colors[] = {
    "Deep Red", "Blue Green", "Light Yellow", "Dark Purple",
    "Bright Orange", "Cool Blue", "Warm Pink", "Forest Green",
    "Sky Blue", "Rose Red", "Golden Yellow", "Violet Purple",
    "Ocean Blue", "Grass Green", "Fire Red", "Ice Blue"
};
static const int num_colors = sizeof(mock_colors) / sizeof(mock_colors[0]);

// Mock implementations - minimal functionality for testing
extern "C" {

HEADER_B* GetStruct1(int iItem, int nItems) {
    std::cout << "Mock GetStruct1 called - not implemented\n";
    return nullptr;
}

HEADER_C* GetStruct2(int iItem, int nItems) {
    std::cout << "Mock GetStruct2 called - not implemented\n";
    return nullptr;
}

HEADER_A** GetStruct3(int iItem, int nItems) {
    std::cout << "Mock GetStruct3 called - not implemented\n";
    return nullptr;
}

HEADER_D* GetStruct4(int iItem, int nItems) {
    std::cout << "Mock GetStruct4 called - not implemented\n";
    return nullptr;
}

HEADER_E* GetStruct5(int iItem, int nItems) {
    std::cout << "Mock GetStruct5 called - not implemented\n";
    return nullptr;
}

void* GetItem(int iItem, char* pID) {
    // Only implement ITEM2 for basic testing
    if (iItem != 2) {
        std::cout << "Mock GetItem: Only ITEM2 supported\n";
        return nullptr;
    }
    
    ITEM2* item = new ITEM2;
    
    // Set ID
    const char* id_to_use;
    if (pID && strlen(pID) > 0) {
        id_to_use = pID;
    } else {
        // Use random color
        srand(time(nullptr) + rand());
        id_to_use = mock_colors[rand() % num_colors];
    }
    
    size_t len = strlen(id_to_use);
    item->pID = new char[len + 1];
    STRCPY_SAFE(item->pID, len + 1, id_to_use);
    
    // Set random code
    item->Code = rand() % 100000;
    
    // Set time
    item->pTime = new TIME;
    item->pTime->Hour = rand() % 24;
    item->pTime->Min = rand() % 60;
    item->pTime->Sec = rand() % 60;
    
    item->pNext = nullptr;
    
    return item;
}

void SetInstructorMode() {
    std::cout << "Mock SetInstructorMode called\n";
}

} // extern "C"

#endif // _WIN32