#include "io.hpp"

#include <dirent.h>
#include <fstream>

#include "tools/logging/logging.hpp"

using namespace std;

vector<Folder> listFiles(std::string path, std::string ext) {
	std::string dirToOpen = path;
	DIR * dir = opendir(dirToOpen.c_str());
	struct dirent * entity = readdir(dir);
	vector<Folder> result;
	Folder currentFolder;

	while (entity != NULL) {
		if (entity->d_type == DT_DIR) {
			// Directory
			if (strcmp(entity->d_name, ".") && strcmp(entity->d_name, "..")) {
				string subdir = string(entity->d_name);
				vector<Folder> fns = listFiles(path + "/" + subdir, ext);

				for (size_t i = 0; i < fns.size(); i++) {
					if (fns[i].fileNames.size() > 0) {
						fns[i].name = subdir;
						std::sort(fns[i].fileNames.begin(), fns[i].fileNames.end());
						result.push_back(fns[i]);
					}
				}
			}
		} else if (entity->d_type == DT_REG) {
			// Regular file
			std::string fileName = std::string(entity->d_name);
			if (fileName.rfind(ext) == (fileName.size() - ext.size())) {
				currentFolder.fileNames.push_back(fileName);
			}
		}
		entity = readdir(dir);
	}
	closedir(dir);

	if (currentFolder.fileNames.size() > 0) {
		currentFolder.name = ".";
		result.push_back(currentFolder);
	}
	std::sort(result.begin(), result.end());
	return result;
}

void Dataset::loadDataset(std::string path, std::vector<Mat> &dataset) {
	std::string dirToOpen = path;
	DIR * dir = opendir(dirToOpen.c_str());
	struct dirent * entity = readdir(dir);

	while (entity != NULL) {
		if (entity->d_type == DT_DIR) {
			// Directory
			// Just skip it
		} else if (entity->d_type == DT_REG) {
			// Regular file
			std::string fileName = std::string(entity->d_name);
			Mat img = imread(path + "/" + fileName);
			cv::cvtColor(img, img, CV_BGR2GRAY);
			dataset.push_back(img);
		}
		entity = readdir(dir);
	}
	closedir(dir);
}

void Dataset::prepareTrainingSet(std::vector<Mat> &dataset, std::vector<Mat> &trainingSet, long batchSize, int patchSize) {
	int i, j, k, n;
	Mat img, patch;

	srand(time(NULL));
	for ( n = 0; n < batchSize; n++ ) {
		do {
			k = random() % dataset.size();
			img = dataset.at(k);
			i = random() % (img.rows - patchSize);
			j = random() % (img.cols - patchSize);
			patch = img(cv::Rect(j, i, patchSize, patchSize)).clone();
		} while ( !cv::sum(patch)[0] );
		trainingSet.push_back(patch);
	}
}

int Dataset::reverseInt(int i) {
	unsigned char ch1, ch2, ch3, ch4;

	ch1 = i & 255;
	ch2 = (i >> 8) & 255;
	ch3 = (i >> 16) & 255;
	ch4 = (i >> 24) & 255;
	return ((int) ch1 << 24) + ((int) ch2 << 16) + ((int) ch3 << 8) + ch4;
}

void Dataset::loadMNIST(string filename, vector<cv::Mat> &vec) {
	ifstream file(filename.c_str(), ios::binary);

	if (file.is_open()) {
		int magic_number = 0;
		int number_of_images = 0;
		int n_rows = 0;
		int n_cols = 0;

		file.read((char*) &magic_number, sizeof(magic_number));
		magic_number = reverseInt(magic_number);
		file.read((char*) &number_of_images, sizeof(number_of_images));
		number_of_images = reverseInt(number_of_images);
		file.read((char*) &n_rows, sizeof(n_rows));
		n_rows = reverseInt(n_rows);
		file.read((char*) &n_cols, sizeof(n_cols));
		n_cols = reverseInt(n_cols);

		for (int i = 0; i < number_of_images; i++) {
			cv::Mat tp = cv::Mat::zeros(n_rows, n_cols, CV_8UC1);
			for (int r = 0; r < n_rows; r++) {
				for (int c = 0; c < n_cols; c++) {
					unsigned char temp = 0;
					file.read((char*) &temp, sizeof(temp));
					tp.at<uchar>(r, c) = (int) temp;
				}
			}
			vec.push_back(tp);
		}
	}
}

void Dataset::loadMNISTLabels(string filename, vector<double> &vec) {
	ifstream file(filename.c_str(), ios::binary);

	if (file.is_open()) {
		int magic_number = 0;
		int number_of_images = 0;
		int n_rows = 0;
		int n_cols = 0;

		file.read((char*) &magic_number, sizeof(magic_number));
		magic_number = reverseInt(magic_number);
		file.read((char*) &number_of_images, sizeof(number_of_images));
		number_of_images = reverseInt(number_of_images);
		vec.resize(number_of_images);

		for (int i = 0; i < number_of_images; i++) {
			unsigned char temp = 0;
			file.read((char*) &temp, sizeof(temp));
			vec[i] = (double) temp;
		}
	}
}

long long BinaryFileHelper::readLL(std::fstream &f, int pos) {
	long long val;
	f.seekp(pos * sizeof(long long));
	f.read((char*) &val, sizeof(long long));
	return val;
}

int BinaryFileHelper::readInt(std::fstream &f, int pos) {
	int val;
	f.seekp(pos * sizeof(int));
	f.read((char*) &val, sizeof(int));
	return val;
}

void BinaryFileHelper::writeLL(std::fstream &f, int pos, long long val) {
	f.seekp(pos * sizeof(long long));
	f.write((char*) &val, sizeof(long long));
}

void BinaryFileHelper::writeInt(std::fstream &f, int pos, int val) {
	f.seekp(pos * sizeof(int));
	f.write((char*) &val, sizeof(int));
}

void showProgress(double p) {
	Mat img = Mat::zeros(40, 400, CV_8UC3) + Vec3b(255,255,255);
	rectangle(img, Point(0,0), Point(400*p,40), Scalar(0,0,255), -1, 8 );
	imshow("Progress Bar", img);
	waitKey(1);
}

Mat imagesc(Mat map) {
	double min;
	double max;

	cv::minMaxIdx(map, &min, &max);
	cv::Mat adjMap;
	// Histogram Equalization
	float scale = 255 / (max-min);
	map.convertTo(adjMap,CV_8UC1, scale, -min*scale);

	cv::Mat resultMap;
	applyColorMap(adjMap, resultMap, cv::COLORMAP_BONE);
	return resultMap;
}
