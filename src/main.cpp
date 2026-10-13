#include <iostream>
#include <string>
#include <stdexcept>
#include "LanguageModel.h"
#include "TextGenerator.h"

using namespace std;

int main(int argc, char* argv[]) {
    try {
        // we need either 4 or 6 arguments
        if (argc != 4 && argc != 6) {
            throw invalid_argument("slm: invalid number of arguments\n"
                                       "Usage: slm [-a alpha] <k> <file> <length>");
        }

        double alpha = 0.0;
        int arg_idx = 1;

        if (argc == 6) {
            // second argument has to correspond with option -a
            if (std::string(argv[1]) != "-a") {
                throw std::invalid_argument("slm: unknown option '" + string(argv[1]) + "'");
            }

            try {
                size_t l = 0;
                alpha = stod(argv[2], &l);
                // alpha has to be a positive number
                if (l != string(argv[2]).length() || alpha < 0.0) {
                    throw invalid_argument("slm: option -a requires a non-negative number");
                }
            } catch (const exception&) {
                throw invalid_argument("slm: option -a requires a non-negative number");
            }

            arg_idx = 3; 
        }

        int k = 0;
        try {
            size_t l = 0;
            k = stoi(argv[arg_idx], &l);
            // the size of the k-grams has to be a positive integer
            if (l != string(argv[arg_idx]).length() || k <= 0) {
                throw invalid_argument("slm: k must be a positive integer");
            }
        } catch (const exception&) {
            throw invalid_argument("slm: k must be a positive integer");
        }

        string filename = argv[arg_idx + 1];

        int length = 0;
        try {
            size_t l = 0;
            length = stoi(argv[arg_idx + 2], &l);
            // we need that the length of the produced text has to be an integer greater than the size of the k-grams
            if (l != string(argv[arg_idx + 2]).length() || length < k) {
                throw invalid_argument("slm: length must be an integer greater than or equal to k");
            }
        } catch (const exception&) {
            throw invalid_argument("slm: length must be an integer greater than or equal to k");
        }

        LanguageModel model(k, filename, alpha);
        TextGenerator generator(model);
        string writen_text = generator.write(length);
        cout << writen_text << endl;

        return 0;

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}