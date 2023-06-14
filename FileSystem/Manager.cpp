//
// Created by Sam on 10/06/2023.
//

#include <iostream>
#include <map>
#include <filesystem>
#include <fstream>
#include <utility>
#include "Manager.h"
#include "../timer.h"

namespace FileSystem {

    Manager::Manager() {
        files = std::map<std::filesystem::path, File>();
    }

    Manager* Manager::load(const std::string &directory, bool load_file_contents) {
        auto manager = new Manager();
        manager->load_directory(directory, load_file_contents);

        return manager;
    }

    void Manager::load_directory(const std::string &directory, bool load_file_contents) {
        auto file_timer = time("File loading");

        std::cout << "Loading files from " << directory << std::endl;

        int count = 0;

        // We need to recursively iterate over the directory
        for (const auto &entry: std::filesystem::recursive_directory_iterator(directory)) {
            // We only want to load files with the .php extension
            if (entry.path().extension() != ".php") {
                continue;
            }

            File file(entry.path());

            if(load_file_contents) {
                file.get_content();
            }

            files.emplace(entry.path(), file);

            count++;

            /*
            // Create a new script object
            auto script = new AST::Script(entry.path());

            // Parse the file
            script->parse(content);

            // Print the AST
            script->print_info(std::cout, 0);
            */
        }

        std::cout << "Loaded " << count << " files" << std::endl;

        file_timer();
    }

} // FileSystem
