// This file is under GNU General Public License 3.0
// see LICENSE.txt

#include "std_utils.hh"
#include <iostream>
#include <iterator>
#include <fstream>
#include <cstdio>
#include <cerrno>
#include <cmath>
#include <algorithm>
#include <thread>
#include <random>
#include <cassert>
#include <cstring>
#include <iomanip>
#include "pEpLog.hh"

#ifndef WIN32
    #include <dirent.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

using namespace std;
using namespace pEp;

namespace pEp {
    namespace Utils {
        bool is_c_str_empty(const char *str)
        {
            if (str == nullptr) {
                return true;
            }
            string tmp{ str };
            if (tmp.empty()) {
                return true;
            }
            return false;
        }

        string nested_exception_to_string(const exception &e, int level, string src)
        {
            src += string(level, ' ') + "exception: " + e.what() + "\n";
            try {
                rethrow_if_nested(e);
            } catch (const exception &e) {
                src = nested_exception_to_string(e, level + 1, src);
            } catch (...) {
            }
            return src;
        }

        // File utils
        bool path_exists(const string &filename)
        {
            ifstream ifile(filename.c_str());
            return static_cast<bool>(ifile);
        }

        void path_delete(const string &filename)
        {
            int status = remove(filename.c_str());
            if (status) {
                runtime_error e{ string("path_delete(\"" + filename + "\") -  " + strerror(errno)) };
                throw(e);
            }
        }

#ifndef WIN32
        std::string dir_get_cwd()
        {
            std::string ret;
            char cwd[2048];
            char const *res = getcwd(cwd, sizeof(cwd));
            if (res == nullptr) {
                throw std::runtime_error(
                    "failed to get current working directory: " + std::string(strerror(errno)));
            }
            ret = std::string(res);
            return ret;
        }

        void dir_set_cwd(const std::string &dir)
        {
            int err = chdir(dir.c_str());
            if (err != 0) {
                throw std::runtime_error("dir_set_cwd(" + dir + ")" + std::string(strerror(errno)));
            }
        }

        bool path_is_dir(const string &path)
        {
            bool ret = false;
            struct stat statbuf;
            if (stat(path.c_str(), &statbuf) != 0) {
                runtime_error e{ "path_is_dir(\"" + path + "\") -  " + strerror(errno) };
                throw(e);
            }
            if (S_ISDIR(statbuf.st_mode)) {
                ret = true;
            }
            return ret;
        }

        void path_delete_all(const string &path)
        {
            try {
                if (!path_is_dir(path)) {
                    path_delete(path);
                } else {
                    vector<string> dirlist = dir_list_all(path);
                    if (dirlist.empty()) {
                        path_delete(path);
                    } else {
                        for (const string &filename : dirlist) {
                            string newpath = path + "/" + filename;
                            path_delete_all(newpath);
                        }
                        path_delete(path);
                    }
                }
            } catch (...) {
                runtime_error e{ "path_delete_all(\"" + path + "\")" };
                throw_with_nested(e);
            }
        }

        std::string path_dirname(std::string const &f)
        {
            if (f.empty())
                return f;
            if (f == "/")
                return "";

            auto len = f.size();
            // if the last character is / or \ ignore it
            if (f[len - 1] == '/' || f[len - 1] == '\\')
                --len;
            while (len > 0) {
                --len;
                if (f[len] == '/' || f[len] == '\\')
                    break;
            }

            if (f[len] == '/' || f[len] == '\\')
                ++len;
            return std::string(f.c_str(), len);
        }

        std::string path_get_abs(const std::string &path)
        {
            std::string ret{ path };
            if (path[0] != '/') {
                ret = dir_get_cwd() + "/" + path;
            }
            return ret;
        }

#endif

        ofstream file_create(const string &filename)
        {
            ofstream outfile{ filename };
            return outfile;
        }

        std::string file_read(const std::string &filename)
        {
            auto ss = ostringstream{};
            ifstream input_file(filename);
            if (!input_file.is_open()) {
                runtime_error e{ "Could not open the file: " + filename };
                exit(EXIT_FAILURE);
            }
            ss << input_file.rdbuf();
            return ss.str();
        }

#ifndef WIN32
        void path_ensure_not_existing(const string &path)
        {
            while (path_exists(path)) {
                path_delete_all(path);
            }
        }


        void dir_create(const string &dirname, const mode_t mode)
        {
            if (mkdir(dirname.c_str(), mode) != 0) {
                runtime_error e{ string("dir_create(\"" + dirname + "\") -  " + strerror(errno)) };
                throw(e);
            }
        }

        void dir_ensure(const std::string &path)
        {
            if (!Utils::path_exists(path)) {
                Utils::dir_create(path);
            }
        }

        void dir_recreate(const std::string &path)
        {
            Utils::path_ensure_not_existing(path);
            Utils::dir_ensure(path);
        }

        vector<string> dir_list_all(const std::string &path, const bool incl_dot_and_dotdot)
        {
            vector<string> ret;
            if (!path_exists(path)) {
                runtime_error e{ "dir_list_all(\"" + path + "\") -  Error: does not exist" };
                throw(e);
            }

            if (!path_is_dir(path)) {
                runtime_error e{ "dir_list_all(\"" + path + "\") -  Error: is not a directory" };
                throw(e);
            }

            DIR *dirp = opendir(path.c_str());
            if (dirp == nullptr) {
                runtime_error e{ "dir_list_all(\"" + path + "\") -  Error opening dir" };
                throw e;
            }

            struct dirent *dp;
            while ((dp = readdir(dirp)) != NULL) {
                ret.push_back(string(dp->d_name));
            }

            if (!incl_dot_and_dotdot) {
                ret.erase(
                    remove_if(
                        ret.begin(),
                        ret.end(),
                        [](string elem) { return (elem == "." || elem == ".."); }),
                    ret.end());
            }

            closedir(dirp);
            return ret;
        }

        vector<string> dir_list_dirs(const string &dirname, const bool incl_dot_and_dotdot)
        {
            vector<string> ret = dir_list_all(dirname, incl_dot_and_dotdot);
            ret.erase(
                remove_if(
                    ret.begin(),
                    ret.end(),
                    [dirname](string elem) { return !path_is_dir(dirname + "/" + elem); }),
                ret.end());

            return ret;
        }

        vector<string> dir_list_files(const string &dirname)
        {
            vector<string> ret = dir_list_all(dirname);
            ret.erase(
                remove_if(
                    ret.begin(),
                    ret.end(),
                    [dirname](string elem) { return path_is_dir(dirname + "/" + elem); }),
                ret.end());
            return ret;
        }
#endif

        // Attention, it pads left...
        string padTo(const string &str, const size_t num, const char paddingChar)
        {
            string ret{ str };
            if (num > ret.size()) {
                ret.insert(0, num - ret.size(), paddingChar);
            }
            return ret;
        }

        std::string clip(const std::string &str, const size_t len)
        {
            std::string ret{ str };
            if (str.length() > len) {
                ret = str.substr(0, len) + "...";
            }
            return ret;
        }

        // prints the beginning and the end of the string and clips out
        // content in the middle replaced by "... tldr  ..."
        std::string tldr(const std::string &str, const size_t len)
        {
            std::string decoration = "...";
            int trunclen = len - decoration.length();

            std::string ret{ str };
            if (str.length() > len) {
                ret = "\n" + str.substr(0, (int)floor(trunclen / 2.0)) + "\n... tldr ...\n";
                ret += str.substr(str.length() - (int)floor(trunclen / 2.0), str.length());
            }
            return ret;
        }

        std::string to_lower(const std::string &data)
        {
            std::string ret{ data };
            std::transform(ret.begin(), ret.end(), ret.begin(), [](unsigned char c) {
                return std::tolower(c);
            });
            return ret;
        }

        string to_termcol(const Color &col)
        {
            switch (col) {
                case Color::RESET:
                    return "\033[0m";
                case Color::BLACK:
                    return "\033[30m";
                case Color::RED:
                    return "\033[31m";
                case Color::GREEN:
                    return "\033[32m";
                case Color::YELLOW:
                    return "\033[33m";
                case Color::BLUE:
                    return "\033[34m";
                case Color::MAGENTA:
                    return "\033[35m";
                case Color::CYAN:
                    return "\033[36m";
                case Color::WHITE:
                    return "\033[37m";
                default:
                    return "\033[0m";
            }
        }

        void sleep_millis(int milis)
        {
            std::chrono::milliseconds timespan(milis);
            std::this_thread::sleep_for(timespan);
        }

        // Random --------------------------------------------------------------------------------
        unsigned g_seed = gen_seed();
        std::mt19937 gen{ g_seed };

        uint gen_seed()
        {
            uint ret{};
            if (std::random_device{}.entropy() > 0) {
                ret = std::random_device{}();
            } else {
                pEpLog("gen_seed(): No real random device present, falling back to epoch-time");
                ret = std::chrono::system_clock::now().time_since_epoch().count();
            }
            assert(ret != 0);
            return ret;
        }

        unsigned int random_fast(int max)
        {
            g_seed = (214013 * g_seed + 2531011);
            return ((g_seed >> 16) & 0x7FFF) % max + 1;
        }

        unsigned char random_char(unsigned char min, unsigned char max)
        {
            std::uniform_int_distribution<unsigned char> dis(min, max);
            return dis(gen);
        }

        std::string random_string(unsigned char min, unsigned char max, int len)
        {
            std::stringstream ret;
            for (int i = 0; i < len; i++) {
                ret << random_char(min, max);
            }
            return ret.str();
        }

        std::string random_string_fast(unsigned char min, unsigned char max, int len)
        {
            int range = std::max(max - min, 0);
            std::stringstream ret;
            for (int i = 0; i < len; i++) {
                ret << static_cast<unsigned char>(random_fast(range) + min);
            }
            return ret.str();
        }
    } // namespace Utils
} // namespace pEp
