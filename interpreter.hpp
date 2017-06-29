#ifndef interpreter_h
#define interpreter_h

#include "namespace.hpp"

namespace scott {
    class Interpreter {
    private:
        std::vector<char> const symbols = {'+', '-', '>', '<', ',', '.', '[', ']'};
        std::vector<char> cells;
        std::string const request;
        char_pos current_input_pos;
        std::string code;
        std::string input;
        std::string result;
        cell current_cell;
        int bracket_count;
        
        void m_more() { if(std::next(current_cell) != cells.end()) current_cell = std::next(current_cell); else cells.resize(cells.size() * 2); }
        void m_less() { if(current_cell != cells.begin()) current_cell = std::prev(current_cell); else { throw std::logic_error("Error: can't move farther left.\n"); } }
        void m_plus() { ++(*current_cell); }
        void m_minus() { --(*current_cell); }
        void m_comma() { if (current_input_pos != input.end()) *current_cell = *current_input_pos++;  else { throw std::runtime_error(""); } }
        void m_period() {
            if (*current_cell < 10) { result += std::to_string((int)*current_cell); }
            else { result += *current_cell; }
        }
        
        bool check_syntax();
        void parse();
        void recursion(char_pos);
         
    public:
        explicit Interpreter(std::string const & str): request(str), result(std::string{}) {
            cells.reserve(500);
            for (auto && c : cells) { c = 0; }
            parse();
            current_cell = cells.begin();
            current_input_pos = input.begin();
        }
        
        std::string start();
    };
}


#endif /* interpreter_h */
