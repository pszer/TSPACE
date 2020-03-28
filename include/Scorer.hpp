#pragma once

#include "Tetrion.hpp"

enum SCORE_SIZE { SCORE_NULL , SCORE_SMALL , SCORE_MEDIUM , SCORE_BIG , SCORE_MASSIVE };

struct Scorer {
	Scorer(long long& score): score_counter(score) { ; }

	// returns 'size' of score
	virtual SCORE_SIZE Check(Tetrion& tetrion, int * lines_cleared) = 0;

	int  CountFullLines(Tetrion& tetrion);
	void DropClearedLines(Tetrion& tetrion);

	long long& score_counter;
};

struct LeveledScorer : public Scorer {
	LeveledScorer(long long& score, int& level): Scorer(score), level_counter(level) { ; }

	SCORE_SIZE Check(Tetrion& tetrion, int * lines_cleared = nullptr);

	int& level_counter;
};
