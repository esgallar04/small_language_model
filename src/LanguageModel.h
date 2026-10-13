#ifndef LANGUAGE_MODEL_H
#define LANGUAGE_MODEL_H

#include <string>
#include <map>
#include <set>
#include <vector>

using namespace std;

class LanguageModel {
    private:
        int k;
        double alpha;
        set<char> alphabet; // the set of characters that appear in the training text
        map<string,int> kgram_counts; // to count the number of k-grams of a given type
        map<string, map<char,int>> transitions; // to count how many times a given char goes after a kgram

        void train(const string& filename);

    public:
        //constructor
        LanguageModel(int k, const string& filename, double alpha = 0.0);

        int get_k() const {return k;}
        int get_alpha() const {return alpha;}
        const set<char>& get_alphabet() const {return alphabet;}
        const map<string, int>& get_kgram_counts() const {return kgram_counts;}
        const map<char,int>& get_transitions(const string& ngram) const;
        bool has_kgram(const string& kgram) const;

};

#endif // LANGUAGE_MODEL_H