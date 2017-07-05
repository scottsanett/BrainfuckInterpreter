#include "interpreter.hpp"

namespace scott {
namespace interpreter {
bool interpreter::check_syntax() {
    if (code.length() == 0) { return false; }
    std::stack<char> char_stack;
    for (auto && i : code) {
        if (i == '[') { char_stack.push('['); }
        else if (i == ']') {
            if (char_stack.empty()) {
                result = "Error: unbalanced brackets.\n";
                return false;
            }
            else { char_stack.pop(); }
        }
        if (std::find(symbols.begin(), symbols.end(), i) == std::end(symbols)) {
            result = "Error: invalid symbols found.\n";
            return false; }
    }
    return true;
}

void interpreter::recursion(char_pos itr) {
    while (itr != code.end() && *itr != ']') {
        if (*itr == symbols[0]) m_plus();
        else if (*itr == symbols[1]) m_minus();
        else if (*itr == symbols[2]) m_more();
        else if (*itr == symbols[3]) m_less();
        else if (*itr == symbols[4]) m_comma();
        else if (*itr == symbols[5]) m_period();
        else if (*itr == symbols[6]) {
            itr = std::next(itr);
            while(*current_cell) { recursion(itr); }
            std::stack<int> int_stack;
            int_stack.push(0);
            while (true) {
                if (*itr == '[') { int_stack.push(0); }
                else if (*itr == ']') {
                    int_stack.pop();
                    if (int_stack.empty()) { break; }
                }
                itr = std::next(itr);
            }
        }
        itr = std::next(itr);
    }
}

void interpreter::reset() {
    for (int i = 0; i < 500; ++i) { cells[i] = 0; }
    current_cell = &cells[0];
    current_input_pos = input.begin();
    result.clear();
}

std::string interpreter::start(const std::string & str) {
    std::istringstream iss(str);
    iss >> code >> input;
    if (!check_syntax()) { return result; }
    try {
        reset();
        auto itr = code.begin();
        recursion(itr);
        return result + "\n";
    }
    catch (std::logic_error& e) {
        result = std::string(e.what()) + "\n";
        return result;
    }
    catch (std::runtime_error& e) {
        return "result: " + result + "\n";
        return result;
    }
    catch (...) {
        return "Unknown error occurred.\n";
    }
}
}
}
