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
    if (!file.is_open()){
        throw runtime_error("slm: can not open file '" + filename + "'");
    }

    ostringstream fs;
    fs << file.rdbuf();
    string text = fs.str();
    file.close();

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

            kgram_counts[kgram]++;
            transitions[kgram][next_char]++;
            has_valid_transition = true;
        }
    }

    if (!has_valid_transition){
        throw runtime_error("slm: training text is too short");
    }
}

bool LanguageModel::has_kgram(const string& kgram) const {
    return kgram_counts.find(kgram) != kgram_counts.end();
}

const map<char, int>& LanguageModel::get_transitions(const string& kgram) const {
    static const map<char, int> empty_map;
    auto it = transitions.find(kgram);
    if (it != transitions.end()){
        return it -> second;
    }
    return empty_map;
}