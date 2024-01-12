#include "../../src/dml.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

int main() {

  std::shared_ptr<dml::enm> e = std::make_shared<dml::enm>(); 

  std::string level = "../test/Examples/bytecode/push_pop.dml";
  std::ifstream instream(level, std::ios::in | std::ios::binary);
  std::vector<uint8_t> s((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
  dml::VM vm(s, (e));
  vm.run();
}
