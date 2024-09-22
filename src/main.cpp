#include <opencv4/opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

struct colorDef {
	int r;
	int g;
	int b;

	std::string val;
};

int main(int argc, char *argv[]) {
	cv::Mat image;
	std::vector<colorDef> colorDefs;

	if (argc < 3) {
		std::cerr << "Missing parameters\n";
		return 0;
	}

	for (int i = 1; i < argc; i ++) {
		if (strncmp(argv[i], "--template-file=", 16) == 0) {
			char* path = strchr(argv[i], '=') + 1; 
			std::ifstream templateFile(path);

			if (!templateFile) {
				std::cerr << "file not found\n";
				return 0;
			}

			std::string textLine;
			while (getline(templateFile, textLine)) {
   				colorDef color;
   				std::istringstream ss(textLine);
   				std::string token;

   				try {
	   				getline(ss, token, ','); color.r = stoi(token);
				    getline(ss, token, ','); color.g = stoi(token);
				    getline(ss, token, ','); color.b = stoi(token);
				    getline(ss, token); color.val = token;

					colorDefs.push_back(color);
				} catch (const std::invalid_argument&) {
					std::cerr << "Invalid color definition in line: " << textLine << "\n";
				}
			}
			templateFile.close();

		} else if (strncmp(argv[i], "1", 12) == 0) {
			char* path = strchr(argv[i], '=') + 1; 
   			image = cv::imread(path);

   			if (image.empty()) {
   				std::cerr << "image not load\n";
   				return 0;
   			}

   		} else {
			std::cerr << "invalid argument\n";
			return 0;
		}
	}

	std::ofstream exportFile("map.map");
	exportFile << image.rows << "\n";
	exportFile << image.cols << "\n";

	for (int y = 0; y < image.rows; y ++) {
		for (int x = 0; x < image.cols; x ++) {
			cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
			int r = static_cast<int>(pixel[2]);
			int g = static_cast<int>(pixel[1]);
			int b = static_cast<int>(pixel[0]);

			bool match = false;

			for (const auto& it : colorDefs) {
				if (it.r == r && it.g == g && it.b == b) {
					exportFile << it.val << " ";
					match = true;
					break;
				}
			}
			if (!match) {
				exportFile << "00 ";
			}
		}
		exportFile << "\n";
	}

	exportFile.close();
	return 0;
}