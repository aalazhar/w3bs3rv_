#include <iostream>
#include <vector>
#include <string>


std::vector<char> convertTocharVec(std::vector<std::string> cgiBuff){
	std::vector<std::string>::iterator it = cgiBuff.begin();
	std::vector<char> _new;
	size_t i = 0;
	while (it != cgiBuff.end()){
		i = 0;
		while (i < (*it).size()){
			_new.push_back((*it)[i]);
			i++;
		}
		it++;
	}
	return (_new);
}
int main(){


    std::vector<std::string> buff;
    std::vector<char> chars;
    buff.push_back("jhgekfhehfekhek");
    buff.push_back("jhgekfhehfekhek");
    buff.push_back("jhgekfhehfekhek");

    chars = convertTocharVec(buff);
    size_t i = 0;
    while (i < chars.size()){
        std::cout << chars[i];
        i++;
    }
    return (0);
}