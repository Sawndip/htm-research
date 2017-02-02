#ifndef DENDRITE_HPP_
#define DENDRITE_HPP_

#include "common/types.hpp"
#include "htmcla.hpp"
#include "synapse.hpp"

#include <vector>

using namespace std;

class DendriteSegment {
	private:
		vector<Synapse> synapses;
		double activationThreshold;

	public:
		DendriteSegment( ) {
			this->activationThreshold = 0.0;
		}
		// Setters
		inline void addSynapse( Synapse s ) {
			this->synapses.push_back(s);
		}
		inline void setActivationThreshold( double activationThreshold ) {
			this->activationThreshold = activationThreshold;
		}
		// Getters
		inline vector<Synapse>* getSynapses( ) {
			return &(this->synapses);
		}
		inline double getActivationThreshold( ) const {
			return this->activationThreshold;
		}
		bool getActiveState( int t, CellState state ) const;
};

#endif /* DENDRITE_HPP_ */
