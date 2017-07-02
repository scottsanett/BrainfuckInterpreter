#ifndef interpreter_h
#define interpreter_h

#include "namespace.hpp"

namespace scott {
    class Interpreter {
    private:
        std::array<char, 8> const symbols = {'+', '-', '>', '<', ',', '.', '[', ']'};
        char cells[0x100000];
        std::string const request;
        char_pos current_input_pos;
        std::string code;
        std::string input;
        std::string result;
        char * current_cell;
        int bracket_count;
        
        void m_more() { ++current_cell; }
        void m_less() { --current_cell; }
        void m_plus() { ++(*current_cell); }
        void m_minus() { --(*current_cell); }
        void m_comma() {
            if (current_input_pos != input.end()) { *current_cell = *current_input_pos++; }
            else { throw std::runtime_error(""); }
        }
        void m_period() { result.append(1, *current_cell); }
        
        bool check_syntax();
        void recursion(char_pos);
        void reset();
         
    public:
        explicit Interpreter() = default;
        std::string start(std::string const & str);
    };
}


#endif /* interpreter_h */
