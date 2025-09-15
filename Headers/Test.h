#pragma once
#include "DataStructure.h"
#include "DataSource.h"

template <typename T> void EvaluationTest(int iItem, std::string filename) {
  using namespace std;
  // iTem - index of the item, 1...10
  // filename - name and path of the file used for storing the items from data
  // structure
  char c;
  const char* colours[] = {"Verekarva Punane", "Tumedam Sinine",
                           "Punakas Kollane",  "Heledam Roheline",
                           "Absoluutne Must",  "Pimestav Valge"};
  char buf[20];
  //
  // Test 1
  //
  std::cout << "Test constructor DataStructure(n), printout and number of items"
            << std::endl;
  DataStructure* pds1 = new DataStructure(35); // test constructor
  std::cout << *pds1 << std::endl;             // test printout
  std::cout << "Number of items is " << pds1->GetItemsNumber()
            << std::endl; // test number of items
  delete pds1;            // test destructor
  std::cin.get(c);
  //
  // Test 2
  //
  std::cout << std::endl
            << "Test default constructor and operator+=" << std::endl;
  DataStructure* pds2 = new DataStructure; // test default constructor
  for (int i = 0; i < 5; i++) {
    *pds2 += (T*)GetItem(iItem); // test operator+=
  }
  std::cout << *pds2 << std::endl;
  try {
    strcpy_s(buf, 20, "Taevakarva Sinine");
    *pds2 += (T*)GetItem(iItem, buf);
    *pds2 += (T*)GetItem(iItem, buf);
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << std::endl << *pds2 << std::endl;
  delete pds2;
  std::cin.get(c);
  //
  // Test 3
  //
  std::cout << std::endl << "Test copy constructor and operator==" << std::endl;
  DataStructure* pds3 = new DataStructure(5);
  DataStructure ds3 = *pds3; // test copy constructor
  std::cout << ds3 << std::endl;
  std::cout << ((*pds3 == ds3) ? "structures are identical"
                               : "structures are not identical")
            << std::endl; // test operator==
  *pds3 += (T*)GetItem(iItem);
  std::cout << ((*pds3 == ds3) ? "structures are identical"
                               : "structures are not identical")
            << std::endl;
  delete pds3;
  std::cin.get(c);
  //
  // Test 4
  //
  std::cout << std::endl << "Test operator=" << std::endl;
  DataStructure* pds4 = new DataStructure(5); // test operator=
  std::cout << *pds4 << std::endl;
  DataStructure* pds5 = new DataStructure(10);
  std::cout << *pds5 << std::endl;
  *pds5 = *pds4;
  std::cout << *pds5 << std::endl;
  std::cout << ((*pds4 == *pds5) ? "structures are identical"
                                 : "structures are not identical")
            << std::endl;
  std::cin.get(c);
  //
  // Test 5
  //
  std::cout << std::endl << "Test operator-=" << std::endl;
  DataStructure* pds6 = new DataStructure;
  for (int i = 0; i < 6; i++) {
    strcpy_s(buf, 20, colours[i]);
    ITEM5* p = (T*)GetItem(iItem, buf);
    *pds6 += (T*)GetItem(iItem, buf); // test operator+=
  }
  std::cout << *pds6 << std::endl;
  try {
    strcpy_s(buf, 20, "Pleekinud Pruun");
    *pds6 -= buf;
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  for (int i = 0; i < 6; i++) {
    strcpy_s(buf, 20, colours[i]);
    ITEM5* p = (T*)GetItem(iItem, buf);
    *pds6 -= buf; // test operator-=
  }
  std::cout << "Number of items is " << pds6->GetItemsNumber()
            << std::endl; // test number of items
  //
  // Test 6
  //
  std::cout << std::endl << "TestGetItem" << std::endl;
  DataStructure* pds7 = new DataStructure;
  for (int i = 0; i < 6; i++) {
    strcpy_s(buf, 20, colours[i]);
    ITEM5* p = (T*)GetItem(iItem, buf);
    *pds7 += (T*)GetItem(iItem, buf); // test operator+=
  }
  strcpy_s(buf, 20, "Heledam Roheline");
  std::cout << (pds7->GetItem(buf) ? "Item found" : "Item not found")
            << std::endl;
  strcpy_s(buf, 20, "Pleekinud Pruun");
  std::cout << (pds7->GetItem(buf) ? "Item found" : "Item not found")
            << std::endl;
  //
  // Test 7
  //
  std::cout << std::endl
            << "Test Write and constructor DataStructure(filename)"
            << std::endl;
  try {
    DataStructure* pds8 = new DataStructure(10);
    pds8->Write(filename); // test Write
    std::cout << "File stored" << std::endl;
    DataStructure* pds9 = new DataStructure(filename); // test constructor
    std::cout << "New structure created, the structures are ";
    std::cout << ((*pds8 == *pds9) ? "identical" : "not identical")
              << std::endl;
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}
