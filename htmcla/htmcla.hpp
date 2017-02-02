#ifndef HTMCLA_HPP_
#define HTMCLA_HPP_

const double pConnect = 0.15;
const double initialPerm = 0.01;
const double connectThreshold = 0.2;
const double pInc = 0.01;
const double pDec = 0.01;
const double bInc = 0.005;
const double bMax = 4;
const double minActivityThreshold = 0.01;
const double EMA_ALPHA = 0.001;

/* This one defines the initialization strategy
   of the synaptic permanence values
   used by the augmented spatial pooler */
enum class ASPType {
	ASP_TYPE_DEFAULT,
    ASP_TYPE_M,
    ASP_TYPE_G
};

// Augmented spatial pooler is defined by its type (see above), dictionary size and
// image patch size used for the dictionary learning
using ASPDetails = struct {
	ASPType type;
	int dictSize;
	int patchSize;
};

// Stability criteria
enum class StabilityCriteria {
	PERMANENCE_STABILITY,
	CONNECTION_STABILITY
};

// Cell's state could be active, predictive or learning
enum class CellState {
	ACTIVE_STATE,
	PREDICTIVE_STATE,
	LEARN_STATE
};

#endif /* HTMCLA_HPP_ */
