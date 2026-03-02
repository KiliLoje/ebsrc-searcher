#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
namespace fs = std::filesystem;

// construct the github link
std::string github_link_constructor(std::string listing_file, int line_index,
                                    int nb_of_instruction) {
  std::cout << listing_file << "\n";

  if (!listing_file.empty() && listing_file.back() == '\r')
    listing_file.pop_back();

  std::string github_link =
      "https://github.com/Herringway/ebsrc/blob/main/src/";
  github_link = github_link + listing_file.substr(4) + "#L" +
                std::to_string(line_index - (nb_of_instruction - 1));
  if (nb_of_instruction > 1)
    github_link = github_link + "-L" + std::to_string(line_index);

  return github_link;
}

// scan a file and return a set of ebsrc github links that points to the target
// instrucion set
std::vector<std::string>
scan_file(std::ifstream &file,
          const std::vector<std::string> &instruction_set) {
  int instruction_index = 0;
  int line_index = 1;
  std::vector<std::string> addresses{};
  std::string listing_file = "";

  std::string line;

  std::vector<std::string> links{};

  while (std::getline(file, line)) {
    if (instruction_index >= instruction_set.size()) {
      links.push_back(github_link_constructor(listing_file, line_index,
                                              instruction_set.size()));

      for (const std::string &instruction : addresses) {
        std::cout << instruction << "\n";
      }

      instruction_index = 0;
    }

    if (line.rfind(">>>") != std::string::npos) {
      listing_file = line;

      instruction_index = 0;
      line_index = 1;
      continue;
    }

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
scan_dir(const fs::directory_entry &entry,
         const std::vector<std::string> &instruction_set) {
  if (!entry.is_regular_file()) {
    return {};
  }

  std::vector<std::string> links;
  std::ifstream file(entry.path());

  links = scan_file(file, instruction_set);

  return links;
}

int main(int argc, char **argv) {
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

  const std::string DIR_PATH(argv[1]);
  const std::string INSTRUCTION_SET_FILE_PATH(argv[2]);
  std::ifstream instruction_set_file(INSTRUCTION_SET_FILE_PATH);

  if (!instruction_set_file) {
    std::cerr << "Error : cannot open instruction file\n";
    return 1;
  }

  std::string instruction;
  std::vector<std::string> instruction_set{};

  while (std::getline(instruction_set_file, instruction)) {
    instruction_set.push_back(instruction);
  }

  if (instruction_set.empty()) {
    std::cerr << "Error : empty instruction file\n";
    return 1;
  }

  std::vector<std::string> links{};

  for (const auto &entry : fs::recursive_directory_iterator(DIR_PATH)) {
    std::vector<std::string> links_in_file = scan_dir(entry, instruction_set);
    for (const std::string &link : links_in_file) {
      links.push_back(link);
    }
  }
  std::ofstream file;
  file.open("ebsrc_link.txt");

  if (!file.is_open()) {
    std::cerr << "Error : couldn't create file ebsrc_link.txt\n";
    return 1;
  }

  for (const std::string &link : links) {
    file << link << "\n";
  }
  std::cout << "Successfully written links to ebsrc_link.txt\n";

  file.close();

  return 0;
}
