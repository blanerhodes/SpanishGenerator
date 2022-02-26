#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <ctime>

#include "Reader.h"
#include "ConjugationGenerator.h"
#include "Writer.h"
#include "Printer.h"

#define ASSERTFAIL __debugbreak();

void updateSM2Data(Word& word, int difficulty);

static const int MAX_NEW_WORDS = 5;
static const int SECONDS_IN_DAY = 86400;
static const time_t timeNow = time(0);
static const std::string verbFileLocation = "../../verbs";
static const std::string nounFileLocation = "../../nouns";
static const std::string adjectiveFileLocation = "../../adjectives";
static const std::string verbBackupLocation = "../../verbs_backup";
static const std::string nounBackupLocation = "../../nouns_backup";
static const std::string adjectiveBackupLocation = "../../adjectives_backup";

bool isReviewable(const Word& word) {
	return word.futureStudyDate <= timeNow && word.data.interval != 0;
}

bool foundWord(const Word& word1, const Word& word2) {
	return word1.word == word2.word;
}

void reflectChanges(std::vector<Word>& allWords, std::vector<Word>& todaysWords) {
	for (int i = 0; i < todaysWords.size(); i++) {
		Word* todayWord = &todaysWords.at(i);
		for (int j = 0; j < allWords.size(); j++) {
			Word* allWord = &allWords.at(j);
			if (todayWord->word == allWord->word) {
				allWord->data.easeFactor = todayWord->data.easeFactor;
				allWord->data.interval = todayWord->data.interval;
				allWord->data.repetitions = todayWord->data.repetitions;
				allWord->futureStudyDate = todayWord->futureStudyDate;
			}
		}
	}
}

void loadTodaysWords(std::vector<Word>& allWords, std::vector<Word>& todaysWords) {
	for(int i = 0; i < MAX_NEW_WORDS; i++){
		if (allWords.at(i).data.interval == 0) {
			todaysWords.push_back(allWords.at(i));
		}
	}
	auto it = std::find_if(allWords.begin(), allWords.end(), isReviewable);
	for (; it != allWords.end() && isReviewable(*it); it++) {
		todaysWords.push_back(*it);
	}

}
	

int main(int argv, char* argc[]) {

	std::ifstream verbFile(verbFileLocation);
	std::ifstream nounFile(nounFileLocation);
	std::ifstream adjectiveFile(adjectiveFileLocation);
	std::ofstream verbFileBackup(verbBackupLocation);
	std::ofstream nounFileBackup(nounBackupLocation);
	std::ofstream adjectiveFileBackup(adjectiveBackupLocation);

	Writer::backupFile(verbFile, verbFileBackup);
	Writer::backupFile(nounFile, nounFileBackup);
	Writer::backupFile(adjectiveFile, adjectiveFileBackup);
	//if (verbFile.is_open()) {
	//	std::cout << "we're open" << std::endl;
	//}
	//if (nounFile.is_open()) {
	//	std::cout << "we're open noun" << std::endl;
	//}
	//if (adjectiveFile.is_open()) {
	//	std::cout << "we're open adjective" << std::endl;
	//}


	std::vector<Word> allVerbs;
	std::vector<Word> allAdjectives;
	std::vector<Word> allNouns;
	Reader::readInWords(verbFile, allVerbs);
	Reader::readInNouns(nounFile, allNouns);
	Reader::readInWords(adjectiveFile, allAdjectives);

	std::vector<Word> todaysVerbs;
	std::vector<Word> todaysNouns;
	std::vector<Word> todaysAdjectives;
	loadTodaysWords(allVerbs, todaysVerbs);
	loadTodaysWords(allNouns, todaysNouns);
	loadTodaysWords(allAdjectives, todaysAdjectives);

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(todaysVerbs.begin(), todaysVerbs.end(), g);
	std::shuffle(todaysNouns.begin(), todaysNouns.end(), g);
	std::shuffle(todaysAdjectives.begin(), todaysAdjectives.end(), g);

	std::vector<ConjugationGenerator::Mood> desiredMoods;
	Printer::printDesiredMoodsPrompt(desiredMoods);

	char input = ' ';
	int verbIndex = 0;
	int nounIndex = 0;
	int adjIndex = 0;
	Word* currVerb;
	Word* currNoun;
	Word* currAdj;
	std::uniform_int_distribution<> verbDist(0, todaysVerbs.size() - 1);
	std::uniform_int_distribution<> nounDist(0, todaysNouns.size() - 1);
	std::uniform_int_distribution<> adjDist(0, todaysAdjectives.size() - 1);

	while (input != 'n' && input != 'N') {
		Printer::Qualities qualities;
		ConjugationGenerator::ConjugationCombo combo = ConjugationGenerator::genConjugationCombo(desiredMoods);
		bool verbRepeated = false;
		bool nounRepeated = false;
		bool adjectiveRepeated = false;

		if (verbIndex > todaysVerbs.size() - 1) {
			currVerb = &todaysVerbs.at(verbDist(g));
			verbRepeated = true;
		}
		else {
			currVerb = &todaysVerbs.at(verbIndex);
		}
		
		if (nounIndex > todaysNouns.size() - 1) {
			currNoun = &todaysNouns.at(nounDist(g));
			nounRepeated = true;
		}
		else {
			currNoun = &todaysNouns.at(nounIndex);
		}

		if (adjIndex > todaysAdjectives.size() - 1) {
			currAdj = &todaysAdjectives.at(adjDist(g));
			adjectiveRepeated = true;
		}
		else {
			currAdj = &todaysAdjectives.at(adjIndex);
		}
		
		Printer::generateSentence(combo, currVerb->word, currNoun->word, currAdj->word);
		qualities = Printer::printQualityQuestions();

		if (!verbRepeated) {
			updateSM2Data(*currVerb, qualities.verbQuality);
		}
		if (!nounRepeated) {
			updateSM2Data(*currNoun, qualities.nounQuality);
		}
		if (!adjectiveRepeated) {
			updateSM2Data(*currAdj, qualities.adjectiveQuality);
		}

		verbIndex++;
		nounIndex++;
		adjIndex++;

		std::cout << "Genere otra frase? (Oprima 'n' si no quiere continuar o cualquiera tecla si quiere): ";
		std::cin >> input;
		std::cout << std::endl;
	}

	verbFile.close();
	nounFile.close();
	adjectiveFile.close();

	std::ofstream verbFileOut(verbFileLocation);
	std::ofstream nounFileOut(nounFileLocation);
	std::ofstream adjectiveFileOut(adjectiveFileLocation);

	reflectChanges(allVerbs, todaysVerbs);
	reflectChanges(allNouns, todaysNouns);
	reflectChanges(allAdjectives, todaysAdjectives);

	Writer::writeWordsToFile(verbFileOut, allVerbs);
	Writer::writeWordsToFile(nounFileOut, allNouns);
	Writer::writeWordsToFile(adjectiveFileOut, allAdjectives);

	
	std::cout << "Adios!";

	return 0;
}



void updateSM2Data(Word& word, int quality) {
	if (quality > 2) {
		if (word.data.repetitions == 0) {
			word.data.interval = 1;
		}
		else if (word.data.repetitions == 1) {
			word.data.interval = 6;
		}
		else if (word.data.repetitions > 1) {
			word.data.interval = word.data.interval * word.data.easeFactor;
		}
		else {
			std::cout << "repetitions is negative somehow" << std::endl;
			ASSERTFAIL
		}

		word.data.interval = std::ceil(word.data.interval);
		word.data.repetitions++;
		word.data.easeFactor = word.data.easeFactor + (0.1f - (5 - quality) * (0.08f + (5 - quality) * 0.02));
	}
	else {
		word.data.repetitions = 0;
		word.data.interval = 1;
	}
	if (word.data.easeFactor < 1.3f) {
		word.data.easeFactor = 1.3f;
	}
	word.futureStudyDate = timeNow + (double)word.data.interval * SECONDS_IN_DAY;
}

//perezoso, trabajador, barato, apretado, intelligente

//blusa, avion, refrigerador, silla, alfombra


/*
yo tu 3ps nosostros 3pp
present preterite imperfect conditional future

5 bits pronouns
7 bits tense
6 bits mood

16 8  4   2        1
yo tu 3ps nosotros 3pp

32  16   8		    4    2       1
ind subj imperative prog perfect perfsubj

64   32   16     8    4   2      1
pres pret imperf cond fut affirm neg

be able to toggle what tense/mood to focus, maybe break moods into different files for this
*/


