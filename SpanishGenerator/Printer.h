#pragma once

#include <vector>
#include <iostream>
#include <sstream>
#include "ConjugationGenerator.h"

namespace Printer {

	struct Qualities {
		unsigned int verbQuality;
		unsigned int nounQuality;
		unsigned int adjectiveQuality;
	};

	void printDesiredMoodsPrompt(std::vector<ConjugationGenerator::Mood>& moods) {
		std::cout << "Cual tiempos estudiaria?" << std::endl;
		std::cout << "[1] Indicative\n";
		std::cout << "[2] Subjunctive\n";
		std::cout << "[3] Imperative\n";
		std::cout << "[4] Progressive\n";
		std::cout << "[5] Perfect \n";
		std::cout << "[6] Perfect Subjunctive\n";
		std::cout << "Moods: ";
		
		int mood;
		std::string moodInts;
		std::getline(std::cin, moodInts);
		std::stringstream moodStream(moodInts);
		while(moodStream >> mood){
			if (mood < 1 || mood > 6) {
				std::cout << "INVALID MOOD VALUE\n";
						break;
			}
			switch (mood) {
			case 1: moods.push_back(ConjugationGenerator::Mood::INDICATIVE); break;
			case 2: moods.push_back(ConjugationGenerator::Mood::SUBJUNCTIVE); break;
			case 3: moods.push_back(ConjugationGenerator::Mood::IMPERATIVE); break;
			case 4: moods.push_back(ConjugationGenerator::Mood::PROGRESSIVE); break;
			case 5: moods.push_back(ConjugationGenerator::Mood::PERFECT); break;
			case 6: moods.push_back(ConjugationGenerator::Mood::PERFECT_SUBJUNCTIVE); break;
			default: std::cout << "INVALID MOOD VALUE\n"; break;
			}
		}
		std::cout << std::endl;
	}

	Qualities printQualityQuestions() {
		std::cout << "--Los cualidades de su comprension--\n";
		std::cout << "--------------------\n";
		std::cout << "5: Respuesta perfecta\n";
		std::cout << "4: Respuesta correcta pero con vacilacion\n";
		std::cout << "3: Respuesta correcta pero con mucho dificultad\n";
		std::cout << "2: Respuesta incorrecta mientras la respuesta correcta parecio facil de recordar\n";
		std::cout << "1: Respuesta incorrecta pero recordo' la respuesta correcta\n";
		std::cout << "0: No pudo recordar la respuesta correcta\n";
		std::cout << "--------------------\n\n";

		Qualities qualities;
		std::cout << "Cual es la cualidad del verbo?: ";
		std::cin >> qualities.verbQuality;
		std::cout << std::endl;
		std::cout << "Cual es la cualidad del sustantivo?: ";
		std::cin >> qualities.nounQuality;
		std::cout << std::endl;
		std::cout << "Cual es la cualidad del adjetivo?: ";
		std::cin >> qualities.adjectiveQuality;
		std::cout << std::endl;
		
		return qualities;
	}

	void generateSentence(ConjugationGenerator::ConjugationCombo combo, std::string verb, std::string noun, std::string adjective) {
		std::cout << "Pronoun: " << combo.pronoun << std::endl;
		std::cout << "Noun: " << noun << std::endl;
		std::cout << "Adjective: " << adjective << std::endl;
		std::cout << "Verb: " << verb << std::endl;
		std::cout << "Tense: " << combo.tense << std::endl;
		std::cout << "Mood: " << combo.mood << std::endl;
		std::cout << std::endl;
	}
}
