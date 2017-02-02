#ifndef CELL_HPP_
#define CELL_HPP_

#include "dendrite.hpp"
#include "htmcla.hpp"

#include <vector>

using namespace std;

class Cell {
	private:
		// Cell's state on the previous and the current timestep
		bool activeState[2];
		bool predictiveState[2];
		bool learningState[2];
		// Distal dendrite segments
		vector<DendriteSegment> distalDendrites;

	public:
		// Setters
		inline void addDistalDendrite( DendriteSegment segment ) {
			distalDendrites.push_back(segment);
		}
		void setState( CellState state, int t, bool value );
		// Getters
		inline vector<DendriteSegment>& getDistalDendrites( ) {
			return distalDendrites;
		}
		bool getState( CellState state, int t ) const;
};

#endif /* CELL_HPP_ */
