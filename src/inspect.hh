#ifndef LIBPEPCXX11_INSPECT_HH
#define LIBPEPCXX11_INSPECT_HH



namespace pEp {
    namespace CXX {
        struct Inspect {
            // type ----------------------------------------
            // returns the type of c
            template<class T>
            static std::string type(T&);
            template<class T>
            static std::string type(T&&);

            // addr ----------------------------------------
            // returns the address of c
            template<class T>
            static std::string addr(T& c);
            template<class T>
            static std::string addr(T&& c);

            // val ----------------------------------------
            // returns the value of c (if T is pointer type, the address p is pointing to)
            template<class T>
            static std::string val(T& c, size_t val_len = 30);
            template<class T>
            static std::string val(T&& c, size_t val_len = 30);

            // all ----------------------------------------
            template<class T>
            static std::string all(T& c, size_t val_len = 30);
            template<class T>
            static std::string all(T&& c, size_t val_len = 30);
        };
    } // namespace CXX
} // namespace pEp

#include "inspect.hxx"
#endif
