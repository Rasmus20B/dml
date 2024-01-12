#include "data_structures/sparse_set.h"
#include <__expected/expected.h>
#include <cstdint>
#include <cstring>
#include <memory>
#include <type_traits>
#include <variant>
#include <iostream>
#include <array>

#include "opcodes.h"



namespace dml {

  struct enm {
    int i;
  };

  enum DataType {
    Integer,
    Float,
  };

  enum VMErr {
    STACK_UNDERFLOW,
  };

  struct StackSlot {
    void print() {
      if(type == dml::Integer) {
        int val = std::get<int>(data);
        std::cout << std::dec << val << " ";
      } else if(type == dml::Float) {
        float val = std::get<float>(data);
        std::cout << std::dec << val << " ";
      }
    }
    std::variant<int, float> data;
    DataType type;
  };

  class VM {
    public:
      VM(std::vector<uint8_t> code, std::shared_ptr<enm> e) : m_code(code), gamestate(e) {
        stack.resize(1024);
      }

      [[nodiscard]]
      int get_int_operand() noexcept {
        int num = 0;
        num += m_code[pc+1] & 0x000000FF;
        num += (m_code[pc+2] << 8) & 0x0000FF00;
        num += (m_code[pc+3] << 16) & 0x00FF0000;
        num += (m_code[pc+4] << 24) & 0xFF000000;
        return num;
      }

      [[nodiscard]]
      float get_float_operand() noexcept {
        std::vector<uint8_t> num_str(m_code.begin() + pc + 1, m_code.begin() + pc + 5);
        float res = 0;
        memcpy(&res, num_str.data(), 4);
        return res;
      }

      void run() { 
        constexpr std::array magic = { 0x7f, 0x44, 0x4d, 0x4c };
        for(int i = 0; i < 4; ++i) {
          if(magic[i] != m_code[i]) {
            std::cerr << "unknown file type\n";
            break;
          }
        }
        // Set pc to the entry point found in header file
        pc += (m_code[4]) & 0x000000FF;
        pc += (m_code[5] << 8) & 0x0000FF00;
        pc += (m_code[6] << 16) & 0x00FF0000;
        pc += (m_code[7] << 24) & 0xFF000000;
        m_code.erase(m_code.begin(), m_code.begin() + 8);

        while(pc < m_code.size()) {
          int op = m_code[pc];
          op = op << 8;
          pc++;
          op |= m_code[pc];
          auto opcode = static_cast<OpCode>(op);
          switch(opcode) {
            case OpCode::CALL:
              call_stack.push_back(pc+5);
              pc = get_int_operand();
              break;
            case OpCode::RETURN: {
              if(call_stack.empty()) [[unlikely]] return;
              size_t addr = call_stack.back();
              call_stack.pop_back();
              pc = addr;
              break;
            }
            case OpCode::PUSHI: {
              auto val = get_int_operand();
              push(val);
              pc += 5;
              break;
            }
            case OpCode::PUSHF: {
              auto res = get_float_operand();
              push(res);
              pc += 5;
              break;
            }
            case OpCode::SET: {
              auto res = pop();
              if(res.has_value()) {
                registers.push_back(res.value());
              } else {
                std::cerr << "Error: " << res.error() << "\n";
              }
              pc += 5;
            }
            break;
            case OpCode::ADDI: {
              int total = 0;
              while(sp > 0) {
                auto e = pop().value();
                if(e.type == Integer) {
                  total += std::get<int>(e.data);
                } else if(e.type == Float) {
                  total += std::get<float>(e.data);
                }
              }
              push(total);
              pc++;
            }
            break;
            case OpCode::PRINT:
              std::cout << "Stack: "; 
              for(int i = 0; i < sp; ++i) {
                stack[i].print();
              }
              std::cout << "\n";
              pc++;
              break;
            default:
              std::cerr << "Error: Unknown Opcode: " << std::hex << op << "\n";
              return;
          }
        }
      }

      template<typename T>
      requires(std::is_same_v<T, int> || std::is_same_v<T, float>)
      void push(T val) {
        if constexpr(std::is_same_v<T, int>) {
          stack[sp] = {val, Integer};
        } else if constexpr(std::is_same_v<T, float>) {
          stack[sp] = {val, Float};
        }
        sp++;
      }

      std::expected<StackSlot, VMErr> pop() {
        if (sp==0) [[unlikely]] return std::unexpected{VMErr::STACK_UNDERFLOW};
        sp--;
        return stack[sp];
      }
    
    private:
      std::vector<uint8_t> m_code;
      int pc = 0;
      std::vector<StackSlot> stack;
      std::vector<size_t> call_stack;
      int sp = 0;
      std::shared_ptr<enm> gamestate;
      std::vector<StackSlot> registers;
  };

}
