# PNG-to-map

## Description
PNG-to-map is a command-line utility that converts PNG images into map files. It uses OpenCV to process the image and generate a corresponding map file based on predefined color definitions.

## Dependencies
- CMake 3.10 or higher
- OpenCV library

## Requirements
- CMake 3.10 or higher must be installed.
- OpenCV library must be installed and accessible on your system.

## Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/srsilverexe/PNG-to-map.git
   cd PNG-to-map
   ```

2. Build the project using CMake:
   ```sh
   cmake .
   make
   ```

## Usage
To run the PNG-to-map utility, use the following command structure:
```sh
./pngToMap -mf <path_to_png> [-tf <path_to_template_file>] [-o <path_to_output_file>]
```
### Options:
- `-h` or `--help`: Show available commands.
- `-tf <path>` or `--template_file <path>`: Specify the template file to consider (optional).
- `-mf <path>` or `--map_file <path>`: Specify the map file to convert.
- `-o <path>` or `--output <path>`: Specify the output file to save the results (optional).

## Example
```sh
./pngToMap -mf input.png -tf template.map -o output.map
```

## License
This project is licensed under the GNU General Public License v3.0. See the [LICENSE](https://github.com/srsilverexe/PNG-to-map/blob/main/LICENSE) file for details.

### Notes:
- Ensure that the paths provided for the PNG file and the optional template file exist.
- If no output path is specified, the resulting map will be saved to `map.map` in the current working directory.
