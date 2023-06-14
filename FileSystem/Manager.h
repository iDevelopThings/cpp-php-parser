//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_MANAGER_H
#define INTERPRETER_MANAGER_H

#include <string>
#include <filesystem>

namespace FileSystem {

    struct File {
        std::filesystem::path path;

    private:
        bool        loaded_contents = false;
        std::string content;
    public:

        explicit File(std::filesystem::path path) : path(std::move(path)), loaded_contents(false) {}

        File(std::filesystem::path path, std::string content) : path(std::move(path)), content(std::move(content)), loaded_contents(true) {}

        std::string get_content() {
            if(!loaded_contents) {
                std::ifstream file(path);

                // Read the file into a string
                content = std::string((std::istreambuf_iterator<char>(file)),
                                      (std::istreambuf_iterator<char>()));

                // Close the file
                file.close();

                loaded_contents = true;
            }

            return content;
        }

    };


    class Manager {

    public:
        std::map<std::filesystem::path, File> files;

        Manager();

        static Manager* load(const std::string &directory, bool load_file_contents = false);

        void load_directory(const std::string &directory, bool load_file_contents = false);
    };


} // FileSystem

#endif //INTERPRETER_MANAGER_H
