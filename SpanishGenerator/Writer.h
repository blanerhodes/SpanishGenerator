#pragma once

#include <fstream>
#include <vector>
#include <algorithm>
#include "DataStructures.h"

namespace Writer {

	void writeWordsToFile(std::ofstream& outFile, std::vector<Word>& words) {
		Word word;
		std::sort(words.begin(), words.end());
		for (int i = 0; i < words.size(); i++) {
			word = words.at(i);
			outFile << word.word << " " << word.data.repetitions << " " << word.data.easeFactor << " " << word.data.interval << " " << word.futureStudyDate << "\n";
		}
	}

	void backupFile(std::ifstream& inFile, std::ofstream& outFile) {
		std::string currLine;
		while (getline(inFile, currLine)) {
			outFile << currLine << "\n";
		}
		inFile.clear();
		inFile.seekg(0, std::ios::beg);
		outFile.close();
	}
}
