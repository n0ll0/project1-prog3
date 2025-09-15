#include "DataSource.h"
#include "DataStructure.h"
#include "DateTime.h"
#include "Headers.h"
#include "Items.h"
#include "Test.h"
#include <exception>
#include <iostream>

int main() {
  try // uncomment you Struct
  {
    std::cout << "Hello world, why does this even work these days?"
              << std::endl;
    // HEADER_B* p1 = GetStruct1(5, 100);
    // HEADER_C* p2 = GetStruct2(5, 100);
    // HEADER_A** pp3 = GetStruct3(5, 100);
    // HEADER_D* p4 = GetStruct4(5, 100);
    // HEADER_E* p5 = GetStruct5(5, 100);

    EvaluationTest<ITEM5>(5, "out.md");

  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
