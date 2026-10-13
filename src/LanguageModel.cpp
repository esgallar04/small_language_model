#include "LanguageModel.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

LanguageModel::LanguageModel(int k, const string& filename, double alpha)
    : k(k), alpha(alpha){
        train(filename);
    }

void LanguageModel::train(const string& filename){
    ifstream file(filename);
    // to check if the file has opened properly
    if (!file.is_open()){
        throw runtime_error("slm: can not open file '" + filename + "'");
    }

    ostringstream fs; // file to string
    fs << file.rdbuf();
    string text = fs.str();
    file.close();

    // to check if the training text is shorter than the k-grams
    if (text.length()<= static_cast<size_t>(k)){
        throw runtime_error("slm: training text is too short");
    }

    bool has_valid_transition = false;

    for (size_t i=0; i<= text.length()-k; ++i){
        string kgram= text.substr(i,k);

        for (char ch: kgram){
            alphabet.insert(ch);
        }

        if (i+k < text.length()){
            char next_char = text[i+k];
            alphabet.insert(next_char);

            // we count the frequencies of the k-grams and the characters that go after them
            kgram_counts[kgram]++;
            transitions[kgram][next_char]++;
            has_valid_transition = true;
        }
    }

    if (!has_valid_transition){
        throw runtime_error("slm: training text is too short");
    }
}

// to check whether a given k-gram is present or not
bool LanguageModel::has_kgram(const string& kgram) const {
    return kgram_counts.find(kgram) != kgram_counts.end();
}

// finds the characters that go after a k-gram and its frequency
const map<char, int>& LanguageModel::get_transitions(const string& kgram) const {
    static const map<char, int> empty_map;
    auto it = transitions.find(kgram);
    if (it != transitions.end()){
        return it -> second;
    }
    return empty_map;
}