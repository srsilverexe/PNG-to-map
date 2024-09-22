#include <opencv4/opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

struct colorDef {
    int r;
    int g;
    int b;
    std::string val;
};

void _help() {
    std::cout << "PngToMap" << "\n";
    std::cout << "________" << "\n";
    std::cout << "-h or --help"                         << "\t" << "Show every commands"                                << "\n";
    std::cout << "-tf <path> or --template_file <path>" << "\t" << "Pass the template file to consider. (Optional)"     << "\n";
    std::cout << "-mf <path> or --map_file <path>"      << "\t" << "Pass the map file to convert."                      << "\n";
    std::cout << "-o <path> or --output <path>"         << "\t" << "Pass the output file to export the file (Optional)" << "\n";
}

int main(int argc, char *argv[]) {
    cv::Mat image;
    std::vector<colorDef> colorDefs;
    std::filesystem::path templatePath = std::filesystem::current_path() / "template.map";
    std::filesystem::path outputPath   = std::filesystem::current_path() / "map.map";
    std::filesystem::path imagePath;

    if (argc < 2) {
        std::cerr << "Missing parameters" << "\n";
        _help();
        return -1;
    }

    for (int i = 1; i < argc; i++) {
        // Help command
        if (strncmp(argv[i], "-h", 2) == 0 || strncmp(argv[i], "--help", 6) == 0) {
            _help();
            return 0;
        } 
        // Template file
        else if (strncmp(argv[i], "-tf", 3) == 0 || strncmp(argv[i], "--template_file", 15) == 0) {
            if (i + 1 >= argc) {
                std::cerr << "Template file path missing after: " << argv[i] << "\n";
                return -1;
            }

            templatePath = argv[++i];
            if (!std::filesystem::exists(templatePath)) {
                std::cerr << "Template file not found: " << templatePath.string() << "\n";
                return -1;
            }
            std::cout << "Opening template file: " << templatePath.string() << "\n";
            std::ifstream templateFile(templatePath.string());
            
            std::cout << "Loading template file: " << templatePath.string() << "\n";
            std::string textLine;
            while (getline(templateFile, textLine)) {
                colorDef color;
                std::istringstream ss(textLine);
                std::string token;

                try {
                    getline(ss, token, ','); color.r = std::stoi(token);
                    getline(ss, token, ','); color.g = std::stoi(token);
                    getline(ss, token, ','); color.b = std::stoi(token);
                    getline(ss, token); color.val = token;

                    colorDefs.push_back(color);
                } catch (const std::invalid_argument&) {
                    std::cerr << "Invalid color definition in line: " << textLine << "\n";
                    return -1;
                }
            }

            templateFile.close();
            std::cout << "Template file loaded successfully: " << templatePath.string() << "\n";
        } 
        // Map file
        else if (strncmp(argv[i], "-mf", 3) == 0 || strncmp(argv[i], "--map_file", 10) == 0) {
            if (i + 1 >= argc) {
                std::cerr << "Map file path missing after: " << argv[i] << "\n";
                return -1;
            }

            imagePath = argv[++i];
            image = cv::imread(imagePath.string());
            std::cout << "Loading map file: " << imagePath.string() << "\n";

            if (image.empty()) {
                std::cerr << "Image not loaded: " << imagePath.string() << "\n";
                return -1;
            }
        } 
        // Output file
        else if (strncmp(argv[i], "-o", 2) == 0 || strncmp(argv[i], "--output", 8) == 0) {
            if (i + 1 >= argc) {
                std::cerr << "Output path missing after: " << argv[i] << "\n";
                return -1;
            }
            outputPath = argv[++i];
        } 
        // Invalid argument
        else {
            std::cerr << "Invalid argument: " << argv[i] << "\n";
            return -1;
        }
    }

    if (!imagePath.empty()) {
        std::ofstream exportFile(outputPath);

        if (!exportFile) {
            std::cerr << "Failed to create output file: " << outputPath << "\n";
            return -1;
        }
        
        // Add the number of rows and columns to the first two lines.
        exportFile << image.rows << "\n" << image.cols << "\n";

        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
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
                    exportFile << "Invalid ";
                }
            }
            exportFile << "\n";
        }

        exportFile.close();
        std::cout << "Map file created at " << outputPath << std::endl;
        return 0;
    } else {
        std::cerr << "No image path provided" << "\n";
        return -1;
    }
}
