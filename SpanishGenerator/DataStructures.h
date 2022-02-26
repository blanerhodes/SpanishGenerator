#pragma once
#include <string>
#include <queue>
struct SM2Data {
	int repetitions;
	float easeFactor;
	int interval;
};

struct Word {
	std::string word;
	SM2Data data;
	double futureStudyDate;

	bool operator<(const Word& word) const {
		return futureStudyDate < word.futureStudyDate;
	}
};
