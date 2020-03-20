#pragma once

#include <vector>
#include <random>

#include "Tetrion.hpp"

// Abstract class for a randomizer of shapes.
// Used to draw random shapes out of a pool.
class Randomizer {
public:
	Randomizer(const std::vector<Shape>& pool):
	  shape_pool(pool) { ; }
	Randomizer(const std::vector<Shape>& pool, uint64_t seed):
	  shape_pool(pool) { SetSeed(seed); }

	void SetSeed(uint64_t seed) { random.seed(seed); }
	virtual Shape Next(void) = 0;
protected:
	std::default_random_engine random;
	const std::vector<Shape>& shape_pool;
};

// Used to draw random tetriminoes out of a contaning pool
// Uses the permutation method.
// The set of tetriminoes get given a random permutation, then
// tetriminoes are drawn out of this permutation
class StandardRandomizer : public Randomizer {
public:
	StandardRandomizer(const std::vector<Shape>& pool): Randomizer(pool)
	  { GenerateNextPermutation(); }

	Shape Next(void);
private:
	void GenerateNextPermutation();

	std::vector<int> permutation;
	decltype(permutation.begin()) perm_iter;
};

// Naively pulls shapes out of a pool at random using a uniform
// distribution. Has no protection against long strings of the same
// pieces.
class NaiveRandomizer : public Randomizer {
public:
	NaiveRandomizer(const std::vector<Shape>& pool): Randomizer(pool) { ; }
	Shape Next(void);
};
