#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <opencv2/core.hpp>
#include <sstream>
#include <vector>

using namespace cv;
using namespace std;

// Prevent eclipse from writing about unknown keywords
#ifdef __CDT_PARSER__
	#define __global__
	#define __device__
	#define __host__
	#define __shared__
#endif

class DataSource {
	protected:
		size_t height, width;

	public:
		// Get size
		size_t getHeight( ) {
			return this->height;
		}
		size_t getWidth( ) {
			return this->width;
		}
		// Get value
		virtual double getValue( size_t i, size_t j, size_t k ) const = 0;

		virtual ~DataSource( ) {

		}
};

template<typename T>
class InputSource : public DataSource {
	private:
		T data;

	public:
		InputSource( );
		// Set/get data
		void setData( T data ) {
			this->data = data;
		}
		T& getData( ) {
			return this->data;
		}
		// Get value
		double getValue( size_t i, size_t j, size_t k = 0 ) const override {
			if ( is_same<T, Mat>::value ) {
				auto m = static_cast<Mat>(this->data);
				uchar val;

				if (m.channels() == 1) {
					// Grayscale value
					val = m.at<uchar>(i,j);
				} else {
					// BGR value
					Vec3b v = m.at<Vec3b>(i,j);
					val = v[0];
				}
				return val;
			} else if ( is_same<T, vector<Mat>>::value ) {
				auto m = static_cast<vector<Mat> >(this->data);
				return m[k].at<double>(i,j);
			}
			return 0.0;
		}
};

template<typename T>
class basevector : public std::vector<T> {
	public:
		basevector( size_t size, T value ) : vector<T>( size, value ) {

		}
};

class bitvector : public basevector<uchar> {
	public:
		bitvector( size_t size ) : basevector<uchar>( size / 8, 0 ) {

		}

		bool at( size_t bit ) {
			uchar val = this->data()[bit / 8];
			return (val & (0x01 << (bit % 8))) != 0;
		}

		size_t distance( bitvector& v ) {
			size_t res = 0;
			for ( size_t i = 0; i < this->size(); i++ ) {
				uchar val = this->data()[i] ^ v[i];
				res += val & 0x01;
				res += (val >> 1) & 0x01;
				res += (val >> 2) & 0x01;
				res += (val >> 3) & 0x01;
				res += (val >> 4) & 0x01;
				res += (val >> 5) & 0x01;
				res += (val >> 6) & 0x01;
				res += (val >> 7) & 0x01;
			}
			return res;
		}

		void normalOr( bitvector& v ) {
			for ( size_t i = 0; i < this->size(); i++ ) {
				this->data()[i] |= v[i];
			}
		}

		void exclusiveOr( bitvector& v ) {
			for ( size_t i = 0; i < this->size(); i++ ) {
				this->data()[i] ^= v[i];
			}
		}

		size_t count( ) {
			size_t res = 0;
			for ( size_t i = 0; i < this->size(); i++ ) {
				uchar val = this->data()[i];
				res += val & 0x01;
				res += (val >> 1) & 0x01;
				res += (val >> 2) & 0x01;
				res += (val >> 3) & 0x01;
				res += (val >> 4) & 0x01;
				res += (val >> 5) & 0x01;
				res += (val >> 6) & 0x01;
				res += (val >> 7) & 0x01;
			}
			return res;
		}

		string toString() {
			stringstream ss;
			for ( size_t i = 0; i < this->size(); i++ ) {
				uchar val = this->data()[i];
				ss << static_cast<int>(val & 0x01);
				ss << static_cast<int>((val >> 1) & 0x01);
				ss << static_cast<int>((val >> 2) & 0x01);
				ss << static_cast<int>((val >> 3) & 0x01);
				ss << static_cast<int>((val >> 4) & 0x01);
				ss << static_cast<int>((val >> 5) & 0x01);
				ss << static_cast<int>((val >> 6) & 0x01);
				ss << static_cast<int>((val >> 7) & 0x01);
			}
			return ss.str();
		}
};

class intvector : public basevector<int> {
	public:
		intvector( size_t size ) : basevector<int>( size, 0 ) {

		}

		size_t distance( intvector& v ) {
			size_t res = 0;
			for ( size_t i = 0; i < this->size(); i++ ) {
				res += abs(this->at(i) - v.at(i));
			}
			return res;
		}
};

#endif /* TYPES_HPP_ */
