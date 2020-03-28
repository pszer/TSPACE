#include "Scorer.hpp"

int Scorer::CountFullLines(Tetrion& tetrion) {
	int count = 0;
	for (int y = 0; y < tetrion.height; ++y) {
		int x;
		for (x = 0; x < tetrion.width; ++x)
			if (!tetrion.Get(x,y).type) break;

		if (x == tetrion.width) ++count;
	}

	return count;
}

void Scorer::DropClearedLines(Tetrion& tetrion) {
	for (int y = 0; y < tetrion.height; ++y) {
		int x;
		for (x = 0; x < tetrion.width; ++x)
			if (!tetrion.Get(x,y).type) break;

		if (x == tetrion.width) {
			int yi,xi;
			for (yi = y; yi >= 0; --yi) {
				for (xi = 0; xi < tetrion.width; ++xi) {
					if (yi)
						*tetrion.At(xi, yi) = tetrion.Get(xi, yi-1);
					else
						*tetrion.At(xi, yi) = Mino(0,0);
				}
			}

			DropClearedLines(tetrion);
		}
	}
}

SCORE_SIZE LeveledScorer::Check(Tetrion& tetrion, int * lines_cleared) {
	int count = CountFullLines(tetrion);
	if (lines_cleared) *lines_cleared = count;
	if (count) DropClearedLines(tetrion);

	int l = level_counter + 1;

	switch (count) {
	case 0:
		return SCORE_NULL;
	case 1:
		score_counter += 100 * l;
		return SCORE_SMALL;
	case 2:
		score_counter += 300 * l;
		return SCORE_MEDIUM;
	case 3:
		score_counter += 500 * l;
		return SCORE_BIG;
	case 4:
		score_counter += 800 * l;
		return SCORE_MASSIVE;
	default:
		return SCORE_NULL;
	}
}
