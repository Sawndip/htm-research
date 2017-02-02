#ifndef SYNAPSE_HPP_
#define SYNAPSE_HPP_

#include "common/types.hpp"

#include <cuda_runtime.h>
#include <exception>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

enum class SynapseType {
	EXCITATORY,
	INHIBITORY
};

class IllegalPermanenceException : public exception {
	virtual const char* what() const throw() {
		return "Illegal permanence exception happened";
	}
};

class Synapse {
	private:
		SynapseType type;
		size_t i, j, k;
		float permanence;
		DataSource *dataSource;

	public:
		Synapse( size_t i, size_t j, size_t k, DataSource *dataSource, SynapseType type = SynapseType::EXCITATORY ) {
			this->i = i;
			this->j = j;
			this->k = k;
			this->dataSource = dataSource;
			this->type = type;
			this->permanence = 0.0;
		}

		// Update permanence
		inline void setPermanence( float permanence ) {
			if ( permanence < 0.0 || permanence > 1.0 ) {
				throw IllegalPermanenceException();
			}
			this->permanence = permanence;
		}
		inline void increasePermanence( float amount, float limit = 1.0 ) {
			this->permanence = std::min(this->permanence + amount, limit);
		}
		inline void decreasePermanence( float amount, float limit = 0.0 ) {
			this->permanence = std::max(this->permanence - amount, limit);
		}
		// Getters
		inline __host__ __device__ size_t getI( ) const {
			return this->i;
		}
		inline __host__ __device__ size_t getJ( ) const {
			return this->j;
		}
		inline __host__ __device__ size_t getK( ) const {
			return this->k;
		}
		inline __host__ __device__ float getPermanence( ) const {
			return this->permanence;
		}
		inline __host__ __device__ SynapseType getType( ) const {
			return this->type;
		}
		inline bool isExcitatory( ) const {
			return (this->type == SynapseType::EXCITATORY);
		}
		inline bool isInhibitory( ) const {
			return (this->type == SynapseType::INHIBITORY);
		}
		inline bool isConnected( ) const {
			return (this->permanence >= connectThreshold);
		}
		inline double getInputValue( ) const {
			return this->dataSource->getValue(this->i,this->j, this->k);
		}
};

#endif /* SYNAPSE_HPP_ */
