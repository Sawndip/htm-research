#ifndef IO_HPP_
#define IO_HPP_

#include <dirent.h>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

// Folder class
class Folder {
	public:
		string name;
		vector<string> fileNames;
		bool operator<(const Folder &rhs) const {
			return name < rhs.name;
		}
};

// List all files given the path and the required file extension
vector<Folder> listFiles(std::string path, std::string ext = string(".jpg"));

// Dataset class is used to load and prepare data for the further usage
class Dataset {
	private:
		static int reverseInt(int i);

	public:
		static void loadDataset(std::string path, std::vector<Mat> &dataset);
		static void prepareTrainingSet(
			std::vector<Mat> &dataset, std::vector<Mat> &trainingSet, long batchSize, int patchSize);
		static void loadMNIST(string filename, vector<cv::Mat> &vec);
		static void loadMNISTLabels(string filename, vector<double> &vec);
};

// Binary read/write operations
class BinaryFileHelper {
	public:
		static long long readLL(std::fstream &f, int pos);
		static int readInt(std::fstream &f, int pos);
		static void writeLL(std::fstream &f, int pos, long long val);
		static void writeInt(std::fstream &f, int pos, int val);
};

// Show progress bar
void showProgress(double p);

// Display matrix as an image using grayscale color map
Mat imagesc(Mat map);

#endif /* IO_HPP_ */
