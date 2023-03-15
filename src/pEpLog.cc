// This file is under GNU General Public License 3.0
// see LICENSE.txt

#include "pEpLog.hh"
#include <iostream>
#include <sstream>
#include <mutex>
#include <atomic>
#include <cmath>
#include "std_utils.hh"

#ifdef ANDROID
    #include <android/log.h>
#endif

namespace pEp {
    namespace Adapter {
        namespace pEpLog {
            int line_width = 120;

            // NON CLASS
            std::mutex mtx;
            std::atomic_bool is_enabled{ false };

            void set_enabled(const bool& enabled)
            {
                if (enabled && !get_enabled()) {
                    is_enabled.store(true);
                    pEpLog("pEpLog enabled...");
                }
                if (!enabled && get_enabled()) {
                    pEpLog("pEpLog disabled...");
                    is_enabled.store(false);
                }
            }

            bool get_enabled()
            {
                return is_enabled.load();
            }

            // Common "print" function implementing the actual "backends"
            void _log(const std::string& msg, Utils::Color col = Utils::Color::WHITE)
            {
                std::lock_guard<std::mutex> l(mtx);
#ifdef ANDROID
                __android_log_print(ANDROID_LOG_DEBUG, "pEpDebugLog", "%s", msg.c_str());
#else
                std::cerr << Utils::to_termcol(col) << msg << Utils::to_termcol(Utils::Color::RESET)
                          << std::endl; //endl also flushes, but cerr is unbuffered anyways
#endif
            }

            void log(const std::string& msg, Utils::Color col)
            {
                _log(msg, col);
            }

            void logH1(const std::string& msg, Utils::Color col)
            {
                log(decorate_three_lines(msg, '='), col);
            }

            void logH2(const std::string& msg, Utils::Color col)
            {
                log("\n" + decorate_centered(msg, '='), col);
            }

            void logH3(const std::string& msg, Utils::Color col)
            {
                log(decorate_centered(msg, '-'), col);
            }

            std::string decorate_three_lines(const std::string& msg, char decoration)
            {
                std::stringstream tmp;
                tmp << std::string(line_width, decoration) << std::endl
                    << msg << std::endl
                    << std::string(line_width, decoration);
                return tmp.str();
            }

            std::string decorate_centered(const std::string& msg, char decoration)
            {
                std::stringstream tmp;
                size_t max_len = line_width - 10;
                // truncate msg
                std::string msg_truncated = msg;
                if (msg.length() >= max_len) {
                    msg_truncated = msg.substr(0, max_len);
                    msg_truncated += "...";
                }

                // define decolen
                int decolen = static_cast<int>(
                    floor((double(line_width - msg_truncated.length()))) / 2.0);

                tmp << std::string(decolen, decoration) << ' ' << msg_truncated << ' '
                    << std::string(decolen, decoration);
                return tmp.str();
            }

        } // namespace pEpLog
    }     // namespace Adapter
} // namespace pEp

namespace pEp {
    namespace Adapter {
        namespace pEpLog {
            // Class pEpLogger

            int pEpLogger::_auto_instance_nr = 0;
            pEpLogger::pEpLogger(const std::string& classname, const bool& enabled) :
                _is_enabled(enabled),
                _classname(classname)
            {
                _auto_instance_nr++;
                set_instancename(std::to_string(_auto_instance_nr));
            }

            pEpLogger::pEpLogger(const pEpLogger& rhs) :
                _is_enabled{ rhs._is_enabled },
                _classname{ rhs._classname },
                _instancename{ rhs._instancename }
            {
                _auto_instance_nr++;
                set_instancename(std::to_string(_auto_instance_nr));
            }

            pEpLogger& pEpLogger::operator=(const pEpLogger& rhs)
            {
                _is_enabled = rhs._is_enabled;
                _classname = rhs._classname;
                _auto_instance_nr++;
                set_instancename(std::to_string(_auto_instance_nr));
                return *this;
            }

            void pEpLogger::log(const std::string& msg, Utils::Color col) const
            {
                std::stringstream msg_;
                msg_ << "[" << getpid() << " " << std::this_thread::get_id() << "]";
                msg_ << " - ";
                msg_ << get_classname() << "[" << get_instancename() << "]";
                msg_ << " - " << msg;
                logRaw(msg_.str(), col);
            }

            void pEpLogger::logRaw(const std::string& msg, Utils::Color col) const
            {
                if (_is_enabled) {
                    _log(msg, col);
                }
            }

            void pEpLogger::set_enabled(const bool& enabled)
            {
                _is_enabled = enabled;
            }

            bool pEpLogger::get_enabled() const
            {
                return _is_enabled;
            }

            std::string pEpLogger::get_classname() const
            {
                return _classname;
            }

            void pEpLogger::set_instancename(const std::string& name)
            {
                _instancename = name;
            }

            std::string pEpLogger::get_instancename() const
            {
                return _instancename;
            }
        } // namespace pEpLog
    }     // namespace Adapter
} // namespace pEp
