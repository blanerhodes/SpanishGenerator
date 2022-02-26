#pragma once
#include <fstream>
#include <vector>
#include <algorithm>
#include <DataStructures.h>

namespace Reader {
	void readInNouns(std::ifstream& nounFile, std::vector<Word>& nouns) {
		Word noun;
		std::string article;
		std::string actualNoun;
		while (nounFile >> article) {
			nounFile >> actualNoun;
			noun.word = article + " " + actualNoun;
			nounFile >> noun.data.repetitions;
			nounFile >> noun.data.easeFactor;
			nounFile >> noun.data.interval;
			nounFile >> noun.futureStudyDate;
			nouns.push_back(noun);
		}
		std::sort(nouns.begin(), nouns.end());
	}

	void readInWords(std::ifstream& file, std::vector<Word>& words) {
		Word word;
		while (file >> word.word) {
			file >> word.data.repetitions;
			file >> word.data.easeFactor;
			file >> word.data.interval;
			file >> word.futureStudyDate;
			words.push_back(word);
		}
		std::sort(words.begin(), words.end());

	}
}
