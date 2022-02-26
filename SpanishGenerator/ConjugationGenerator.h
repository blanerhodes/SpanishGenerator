#pragma once

#include <vector>
#include <string>
#include <random>
#include <algorithm>

namespace ConjugationGenerator {
	struct ConjugationCombo {
		std::string mood;
		std::string tense;
		std::string pronoun;
	};

	enum Mood {
		INDICATIVE, SUBJUNCTIVE, IMPERATIVE, PROGRESSIVE, PERFECT, PERFECT_SUBJUNCTIVE
	};

	std::string moodToString(Mood mood) {
		switch (mood) {
		case INDICATIVE: return "Indicative";
		case SUBJUNCTIVE: return "Subjunctive";
		case IMPERATIVE: return "Imperative";
		case PROGRESSIVE: return "Progressive";
		case PERFECT: return "Perfect";
		case PERFECT_SUBJUNCTIVE: return "Perfect Subjunctive";
		}
	}

	std::vector<std::string> MOODS{"Indicative", "Subjunctive", "Imperative", "Progressive", "Perfect", "Perfect Subjunctive"}; 
	std::vector<std::string> TENSES{ "Present", "Imperfect", "Future", "Preterite", "Conditional", "Affirmative", "Negative" };
	std::vector<std::string> PRONOUNS{ "Yo", "Tu", "3PS", "Nosotros", "3PP" };

	ConjugationCombo genConjugationCombo(std::vector<Mood> wantedMoods) {
		std::vector<std::string> useableMoods;
		std::vector<std::string> useableTenses;
		std::vector<std::string> useablePronouns;
		std::random_device rd;
		std::mt19937 gen(rd());

		ConjugationCombo combo;

		for (int i = 0; i < wantedMoods.size(); i++) {
			useableMoods.push_back(*std::find(ConjugationGenerator::MOODS.begin(), ConjugationGenerator::MOODS.end(), moodToString(wantedMoods.at(i))));
		}
		std::uniform_int_distribution<> moodDist(0, useableMoods.size() - 1);
		std::uniform_int_distribution<> standardTenseDist(0, 4);
		std::uniform_int_distribution<> standardPronounDist(0, ConjugationGenerator::PRONOUNS.size() - 1);
		std::string currMood = useableMoods.at(moodDist(gen));
		std::string currTense;
		std::string currPronoun;

		if (currMood == "Imperative") {
			std::uniform_int_distribution<> tenseDist(ConjugationGenerator::TENSES.size() - 2, ConjugationGenerator::TENSES.size() - 1);
			std::uniform_int_distribution<> pronoundist(1, ConjugationGenerator::PRONOUNS.size() - 1);
			currTense = ConjugationGenerator::TENSES.at(tenseDist(gen));
			currPronoun = ConjugationGenerator::PRONOUNS.at(pronoundist(gen));

		}
		else if (currMood == "Subjunctive" || currMood == "Perfect Subjunctive") {
			std::uniform_int_distribution<> tenseDist(0, 2);
			currTense = ConjugationGenerator::TENSES.at(tenseDist(gen));
			currPronoun = ConjugationGenerator::PRONOUNS.at(standardPronounDist(gen));
		}
		else {
			currTense = ConjugationGenerator::TENSES.at(standardTenseDist(gen));
			currPronoun = ConjugationGenerator::PRONOUNS.at(standardPronounDist(gen));
		}
	
		std::uniform_int_distribution<> thirdPersonDist(0, 2);
		if (currPronoun == "3PS") {
			std::vector<std::string> vals{ "El", "Ella", "Usted" };
			currPronoun = vals[thirdPersonDist(gen)];
		}
		else if (currPronoun == "3PP") {
			std::vector<std::string> vals{ "Ellos", "Ellas", "Ustedes" };
			currPronoun = vals[thirdPersonDist(gen)];
		};
		
		combo.mood = currMood;
		combo.tense = currTense;
		combo.pronoun = currPronoun;
		return combo;
	}
}