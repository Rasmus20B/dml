
import dml;

#include <iostream>
#include <fstream>
#include <memory>

int main() {

  std::shared_ptr<enm> e = std::make_shared<enm>(); 

  std::string level = "../test/Examples/bytecode/adder.dml";
  std::ifstream instream(level, std::ios::in | std::ios::binary);
  std::vector<uint8_t> s((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
  VM vm(s, (e));
  vm.run();

}
