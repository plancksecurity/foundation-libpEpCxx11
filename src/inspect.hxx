#ifndef LIBPEPCXX11_INSPECT_HXX
#define LIBPEPCXX11_INSPECT_HXX

#include <iostream>
#include <cctype>
#include <pEp/utils.hh>
#include <sstream>
#include <type_traits>

namespace pEp {
    namespace CXX {
        // type ----------------------------------------
        // same for pointer and value typee
        template<class T>
        std::string Inspect::type(T&)
        {
            std::stringstream ss_type;
            ss_type << typeid(T).name();
            return ss_type.str();
        }


        // addr ----------------------------------------
        //
        template<class T>
        std::string Inspect::addr(T& c)
        {
            std::stringstream ss_addr{};
            ss_addr << &c;
            return ss_addr.str();
        }

        // val ----------------------------------------
        template<class T>
        struct val_str_helper {
            static std::string str(T c, size_t val_len)
            {
                std::stringstream ss_val{};
                ss_val << c;
                return pEp::Utils::clip(ss_val.str(), val_len);
            }
        };

        template<class T>
        struct val_str_helper<T*> {
            static std::string str(T* c, size_t val_len)
            {
                std::stringstream ss_val{};
                if (c != nullptr) {
                    ss_val << c;
                } else {
                    ss_val << "<NULL>";
                }
                return pEp::Utils::clip(ss_val.str(), val_len);
            }
        };

        template<class T>
        std::string Inspect::val(T c, size_t val_len)
        {
            return val_str_helper<T>::str(c, val_len);
        }

        // all ----------------------------------------
        template<class T>
        std::string Inspect::all(T& c, size_t val_len)
        {
            std::stringstream ret{};
            ret << "{ " << type(c) << " | " << addr(c) << " | " << val(c, val_len) << " }";
            return ret.str();
        }
    } // namespace CXX
} // namespace pEp


#endif