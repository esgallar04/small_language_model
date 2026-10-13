#include "TextGenerator.h"
#include <vector>

TextGenerator::TextGenerator(const LanguageModel& model)
    :model(model){
        random_device rd; // random initial seed
        rng = mt19937(rd());
    }

string TextGenerator::choose_initial_kgram(){
    const auto& counts = model.get_kgram_counts();
    vector<string> kgrams;
    vector<int> frequencies;

    for (const auto& pair : counts){
        kgrams.push_back(pair.first);
        frequencies.push_back(pair.second);
    }
    // we create a random distribution with the obtained frequencies
    discrete_distribution<size_t> dist(frequencies.begin(), frequencies.end());
    size_t pos = dist(rng);
    
    return kgrams[pos];
}

char TextGenerator::choose_next_char(const string& current_kgram){
    const auto& alphabet = model.get_alphabet();
    vector<char> all_chars(alphabet.begin(), alphabet.end());
    vector<double> probabilities;
    double alpha = model.get_alpha();
    size_t alphabet_size = alphabet.size();

    bool kgram_exists = model.has_kgram(current_kgram);
    int total_amount = 0;
    map<char, int> transitions;

    if (kgram_exists){
        transitions = model.get_transitions(current_kgram);
        auto count_kgram = model.get_kgram_counts().find(current_kgram);
        if (count_kgram != model.get_kgram_counts().end()){
            total_amount = count_kgram->second;
        }
    }
    
    for (char ch : all_chars){
        int count_ch_kgram = 0; // how many times does a char appear after a kgram
        if (kgram_exists){
            auto it = transitions.find(ch);
            if (it != transitions.end()){
                count_ch_kgram = it->second;
            }
        }
        // calculation of probabilities given by the formula shown in the assignment
        double prob = 0.0;
        if (alpha > 0.0){
            prob = (count_ch_kgram + alpha)/(total_amount + alpha * alphabet_size);
        } else {
            if (total_amount >0){
                prob = static_cast<double>(count_ch_kgram)/total_amount;
            } else{
                prob = 0.0;
            }
        }
        probabilities.push_back(prob);
    }
    // with the computed probabilities we create a distribution to then pick the next char according to those probabilities
    discrete_distribution<size_t> dist(probabilities.begin(),probabilities.end());
    size_t random_index = dist(rng);

    return all_chars[random_index];
}

string TextGenerator::write(int length){
    string writen_text = choose_initial_kgram();

    while (static_cast<int>(writen_text.length()) < length){
        string current_kgram = writen_text.substr(writen_text.length()-model.get_k(), model.get_k());
        char new_char = choose_next_char(current_kgram);
        writen_text.push_back(new_char);
    }

    return writen_text;
}