#include <iostream>
#include <string>
#include <vector>
#include <emscripten/bind.h>


std::vector<unsigned int> build_bracket_map(std::string& program) {
	std::vector<unsigned int> map(program.size(), 0);
	std::vector<unsigned int> stack;
	
	for (int i=0; i<program.size(); i++) {
		
		//cout << program[i] << "\n";
		
		if (program[i] == '[') {
			//cout << "L brack\n";
			//push onto stack
			stack.push_back(i);
			//for (auto x : stack) {
			//	cout << x << "\n";
			//}
		} else if (program[i] == ']') {
			//cout << "R back\n";
			//access last element
			if (stack.empty()) {
				std::cout << "unmatched ]\n";
			} else {
				unsigned int  open = stack.back();
				stack.pop_back();
				map[open] = i;
				map[i] = open;
			}
				
		} else {
			//nothing
		}
	}

	if (!stack.empty()) {
		std::cout << "unmatched [\n";
		abort();
	}

	return map;
}

std::string runInterperter(std::string input) {
    //std::string test = R"(>++++++++[<+++++++++>-]<.>++++[<+++++++>-]<+.+++++++..+++.>>++++++[<+++++++>-]<++.------------.>++++++[<+++++++++>-]<+.<.+++.------.--------.>>>++++[<++++++++>-]<+.)";
    std::string test = input;
    std::vector<unsigned char> tape(30000, 0);
    unsigned int dp = 0;
    unsigned int pc = 0;
    std::string output = "";

    std::vector<unsigned int> map = build_bracket_map(test);

    while (pc < test.size()) {
        //cout << test[pc] << "\n";
        switch (test[pc]) {
            case '>':
                dp++;
                break;
            case '<':
                dp--;
                break;
            case '+':
                tape[dp] = (tape[dp] += 1);
                break;
            case '-':
                tape[dp] = (tape[dp] -= 1);
                break;
            case '.':
                output = output + static_cast<char>(tape[dp]);
                break;
            case ',':
                //TODO: input
                break;
            case '[':
                if (tape[dp] == 0) {
                    pc = map[pc];
                }
                break;
            case ']':
                if (tape[dp] != 0) {
                pc = map[pc];
                }
                break;
            default:
                //nothing]
                break;
        }
        pc++;
    }
    output = output + "\n";
    std::cout << output;
    return output;
}

EMSCRIPTEN_BINDINGS(){
    emscripten::function("runInterperter", &runInterperter);
}


int main(int argc, char * argv[]) {
    return 0;
}
