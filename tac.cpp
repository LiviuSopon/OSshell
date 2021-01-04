#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include <getopt.h>

std::vector<std::string> file_tac(std::string filename, std::string sep) {
    std::vector<std::string> vctr;
    std::ifstream in(filename);

    if (!in.is_open()) {
        std::string err = ("tac: An error occurred while trying to open the file '" 
        + filename + "'. " +
        "Maybe it doesn't exist?\n");
        throw err;
    }

    std::string str;

    std::string cntn( (std::istreambuf_iterator<char>(in) ),
                       (std::istreambuf_iterator<char>()) );

    size_t pos = 0;
    while ((pos = cntn.find(sep)) != std::string::npos) {
        str = cntn.substr(0, pos);
        vctr.push_back(str);
        cntn.erase(0, pos+sep.length());
    }

    str = cntn.substr(0, pos);
    vctr.push_back(str);
    cntn.erase(0, pos+sep.length());

    in.close();
    return vctr;
}

std::vector<std::string> input_tac(std::string sep) {
    std::vector<std::string> vctr;
    std::string s, s1, str;

    s = "";
    while (std::getline(std::cin, s1)) {
        s += s1;
        s += '\n';
    }

    size_t pos = 0;
    while ((pos = s.find(sep.c_str())) != std::string::npos) {
        str = s.substr(0, pos);
        vctr.push_back(str);
        s.erase(0, pos+sep.length());
    }

    str = s.substr(0, pos);
    vctr.push_back(str);
    s.erase(0, pos+sep.length());

    return vctr;
}

void print_vector(std::vector<std::string> vec, bool bef, std::string sp) {
    std::vector<std::string> vct = vec;
    std::reverse(vct.begin(), vct.end());
    for(auto i : vct) {
        if(bef) printf("%s", sp.c_str());
        printf("%s", (char*)i.c_str());
        if(!bef) printf("%s", sp.c_str());
    }
}

int main(int argc, char* argv[]) {
    bool before;
    std::string separator;
    int c;

    before = false;
    separator = '\n';

    while((c = getopt(argc, argv, "bs:")) != -1) {
        switch (c) {
            case 'b': {
                before = true;
                break;
            }
            case 's': {
                separator = optarg;
                break;
            }
            default: {
		        exit(0);
            }
        }
    }

    std::vector<std::string> vect;

    if (optind >= argc) {
        vect = input_tac(separator);
        print_vector(vect, before, separator);
    }
    else {
        for (int i = optind; i < argc; i++)
            try {
                vect = file_tac(argv[i], separator);
                print_vector(vect, before, separator);
            }
            catch(std::string ex) {
                printf("%s", ex.c_str());
            }
    }

    return 0;

}