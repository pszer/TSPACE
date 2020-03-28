#include "Randomizer.hpp"

void StandardRandomizer::GenerateNextPermutation() {
	permutation = std::vector<int>(shape_pool->size(), -1);

	std::uniform_int_distribution<int> distribution(0, shape_pool->size() - 1);
	distribution(random);

	int count = 0;
	while (count < shape_pool->size()) {
		std::size_t i = distribution(random);

		if (permutation.at(i) == -1) {
			permutation.at(i) = count;
			++count;
		}
	}

	perm_iter = permutation.begin();
}

Shape StandardRandomizer::Next(void) {
	if (perm_iter == permutation.end()) GenerateNextPermutation();
	Shape s = shape_pool->at(*perm_iter);
	++perm_iter;
	return s;
}

Shape NaiveRandomizer::Next(void) {
	std::uniform_int_distribution<int> distribution(0, shape_pool->size() - 1);
	return shape_pool->at(distribution(random));
}

