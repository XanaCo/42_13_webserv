#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h> // POSIX directory API

std::string generateDirectoryListingHTML(const std::string& directoryPath) {
    DIR *dir;
    struct dirent *ent;
    std::string html = "<html><head><title>Directory Listing</title></head><body>";
    html += "<h1>Directory Listing of " + directoryPath + "</h1><ul>";

    if ((dir = opendir(directoryPath.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string fileOrDirName(ent->d_name);
            
            if (fileOrDirName == "." || fileOrDirName == "..") {
                continue;
            }

            html += "<li><a href='" + fileOrDirName + "'>" + fileOrDirName + "</a></li>";
        }
        closedir(dir);
    } else {
        html += "<p>Error: Could not open directory.</p>";
    }

    html += "</ul></body></html>";
    return html;
}


int main(int argc, char **argv) 
{
    if (argc != 2)
        return 1;
    std::string directoryPath = argv[1]; // Replace with your directory path
    std::ofstream file;
    std::string htmlContent = generateDirectoryListingHTML(directoryPath);
    file.open("index.html", std::fstream::out);
    if (!file.is_open())
        return 1;
    file << htmlContent << std::endl;
    return 0;
}
