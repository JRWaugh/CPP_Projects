#ifndef CACHE_H
#define CACHE_H
#include "MainMemory.h"
#include <memory>
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>

using namespace std;

enum class Policy { FIFO = 1, LRU, Random };

class Cache : public MainMemory {
	static const unsigned char mDirtyBit = 1;
	static const unsigned char mValidBit = 2;
	friend ostream& operator<<(ostream& os, const Cache& c);

private:
	/*****	 Cache Set Config	 *****/
	/* LRU | 0 | 1 | 2 |...| N | MRU */
	vector<vector<pair<unsigned char, unsigned int>>> mSets;

	/* Cache parameters. */
	uintc32_t mBlockSize, mSetSize, mTotalSize, mSetCount;
	Policy mPolicy;

	/* Cache statistics. */
	unsigned int mStoreHit, mStoreMiss, mLoadHit, mLoadMiss, mDirtyEvict;

	/* Pointer to the next cache level or main memory. */
	shared_ptr<MainMemory> mLowerMem;

	/* Random generator for Random Replacement strategy. */
	mt19937 gen;

public:
	Cache(uintc32_t blockSize, uintc32_t setSize, uintc32_t totalSize, Policy policy, uintc32_t accessTime, uintc32_t accessTimeLower);

	/* The read and write member functions are largely identical but have been kept separate anyway. */
	uintc32_t readAddress(uintc32_t address);
	uintc32_t writeAddress(uintc32_t address);

	void setLowerMem(const shared_ptr<MainMemory> lowerMem) {
		mLowerMem = lowerMem;
	}

	void invalidateCache() {
		/* Unsets the valid bits in each block in each set of the cache. Should be called when a new file is read. */
		for (auto& set : mSets)
			for (auto& block : set)
				block.first &= ~mValidBit;
				
	}
	void resetCacheStats() {
		/* Used for clearing out cache statistics without changing any of the parameters. */
		mDirtyEvict = mStoreHit = mStoreMiss = mLoadHit = mLoadMiss = 0;
	}

	const double getAMAT() const {
		return (double)mAccessTime + ( ((double)mLoadMiss + mStoreMiss) / ((double)mLoadMiss + mLoadHit + mStoreMiss + mStoreHit) ) * mLowerMem->getAMAT();
	}
};
#endif
