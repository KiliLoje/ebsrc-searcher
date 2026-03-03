#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
namespace fs = std::filesystem;

// write line by line all the string in data to a file at path.
// WARNING : DON'T USE THIS ON AN EXISTING FILE, IT WILL OVERWRITE IT
bool write_to_file(const std::string &path, std::vector<std::string> data) {
  // open the file
  std::ofstream file;
  file.open(path);
  if (!file.is_open()) {
    std::cerr << "Error : couldn't create file at " << path << "\n";
    return false;
  }

  // write data to it
  for (const std::string &line : data) {
    file << line << "\n";
  }
  std::cout << "Successfully written data at " << path << "\n";

  file.close();
  return true;
}

// construct the github link
std::string github_link_constructor(std::string listing_file, int line_index,
                                    int nb_of_instruction) {
  // remove line breaker from the file path
  if (!listing_file.empty() && listing_file.back() == '\r')
    listing_file.pop_back();

  std::string github_link =
      // repo link
      "https://github.com/Herringway/ebsrc/blob/main/src/" +
      // file path
      listing_file.substr(4) + "#L" + // file path
      // starting line index
      std::to_string(line_index - nb_of_instruction + 1);
  // ending line index
  if (nb_of_instruction > 1)
    github_link += std::to_string(line_index);

  return github_link;
}

/*
scan a file and return a set of ebsrc github links that points to the target
instrucion set
*/
std::vector<std::string>
scan_file(std::ifstream &file, const std::vector<std::string> &instruction_set,
          std::vector<std::string> ebscr_data) {

  // intitialize variables
  int instruction_index = 0;
  int line_index = 1;
  std::vector<std::string> addresses{};
  std::string listing_file = "";

  std::string line;

  std::vector<std::string> links{};

  // iterate through the file
  while (std::getline(file, line)) {
    // if every instructions are found
    if (instruction_index >= instruction_set.size()) {
      // construct the github link and add it to the list of links
      links.push_back(github_link_constructor(listing_file, line_index,
                                              instruction_set.size()));
      // print infos to the console
      std::cout << listing_file << "\n";
      for (const std::string &instruction : addresses) {
        std::cout << instruction << "\n";
        ebscr_data.push_back(instruction);
      }

      instruction_index = 0;
    }

    // ">>>" indicates a new file in ebsrc-to-linsting
    // this reset the line index and update the current file
    if (line.rfind(">>>") != std::string::npos) {
      listing_file = line;

      instruction_index = 0;
      line_index = 1;
      continue;
    }

    // check for instructions
    if (line.find(instruction_set[instruction_index]) != std::string::npos) {
      addresses.push_back(line);

      instruction_index += 1;
      continue;
    }

    addresses.clear();
    instruction_index = 0;
    line_index++;
  }
  return links;
}

// scan a directory, print the instruction set, and return a list of github
// links
std::vector<std::string>
scan_dir(const std::string &DIR_PATH,
         const std::vector<std::string> &instruction_set,
         std::vector<std::string> &ebscr_data) {

  // intitialize the list of links
  std::vector<std::string> links{};

  // iterate through each files
  for (const auto &entry : fs::recursive_directory_iterator(DIR_PATH)) {
    // skip non plain text files
    if (!entry.is_regular_file()) {
      return {};
    }

    // intitialize file
    std::ifstream file(entry.path());
    // scan the file
    std::vector<std::string> links_in_file =
        scan_file(file, instruction_set, ebscr_data);

    // append links generated from that file to the link list
    for (const std::string &link : links_in_file) {
      links.push_back(link);
    }
  }

  return links;
}

int main(int argc, char **argv) {

  // handle the help isntruction
  if (argc >= 2) {
    std::string arg1 = argv[1];
    if (arg1 == "-h" || arg1 == "help") {
      std::cout << "Usage : " << argv[0]
                << " path/to/ebsrc-to-listing path/to/instruction_set.txt\n";
      return 0;
    }
  }

  if (argc != 3) {
    std::cerr << "Error : Wrong number of argument\n";
    std::cerr << "Usage : " << argv[0]
              << " path/to/ebsrc-to-listing path/to/instruction_set.txt\n";
    return 1;
  }

  // intitialize paths and files
  const std::string DIR_PATH(argv[1]);
  const std::string INSTRUCTION_SET_FILE_PATH(argv[2]);
  std::ifstream instruction_set_file(INSTRUCTION_SET_FILE_PATH);

  if (!instruction_set_file) {
    std::cerr << "Error : cannot open instruction file\n";
    return 1;
  }

  // intitialize variables
  std::string instruction;
  std::vector<std::string> instruction_set{};
  std::vector<std::string> ebscr_data{};
  std::vector<std::string> links{};

  while (std::getline(instruction_set_file, instruction)) {
    instruction_set.push_back(instruction);
  }

  if (instruction_set.empty()) {
    std::cerr << "Error : empty instruction file\n";
    return 1;
  }

  // scan the directory
  links = scan_dir(DIR_PATH, instruction_set, ebscr_data);

  // print links to the console
  std::cout << "\n";
  for (const std::string &link : links)
    std::cout << link << "\n";
  std::cout << "\n";

  // write ebscr_data to ebscr_data.txt
  if (!write_to_file("ebscr_data.txt", ebscr_data))
    return 1;
  // write links to ebscr_links.txt
  if (!write_to_file("ebscr_links.txt", links))
    return 1;

  return 0;
}
