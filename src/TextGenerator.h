#ifndef TEXT_GENERATOR_H
#define TEXT_GENERATOR_H

#include "LanguageModel.h"
#include <string>
#include <random>

class TextGenerator {
    private:
        const LanguageModel& model;
        mt19937 rng; // random number generator engine
        string choose_initial_kgram();
        char choose_next_char(const string& current_kgram);

    public:
        explicit TextGenerator(const LanguageModel& model);
        string write(int length);
};

#endif // TEXT_GENERATOR_H