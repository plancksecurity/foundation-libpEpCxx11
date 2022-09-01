#include <iostream>
#include <cctype>
#include <cassert>
#include "../src/inspect.hh"
#include <pEp/utils.hh>
#include <pEp/pEpLog.hh>

using pEp::CXX::Inspect;

int i{ 23 };
std::stringstream addr_i{};

std::stringstream addr_null{};

//template<typename T>
//T to_r_value(T& a)
//{
//    return a;
//}

//template<typename T>
//T* to_r_value_ref(T& a)
//{
//    return &a;
//}


int* to_r_value_ref(int& a)
{
    return &a;
}

int main()
{
    pEp::Adapter::pEpLog::set_enabled(true);
    addr_null << static_cast<void *>(nullptr);
    addr_i << static_cast<void *>(&i);

    pEpLogH2("Test int");
    {
        std::cout << Inspect::all(i) << std::endl;
        assert(Inspect::type(i) == "i");
        assert(Inspect::addr(i) == addr_i.str());
        assert(Inspect::val(i) == std::to_string(i));
    }

    pEpLogH3("Test int&");
    {
        int &ri = i;
        std::cout << Inspect::all(ri) << std::endl;
        assert(Inspect::type(ri) == "i");
        assert(Inspect::addr(ri) == addr_i.str());
        assert(Inspect::val(ri) == std::to_string(ri));
    }

    pEpLogH2("Test int*");
    {
        int *pi = &i;
        std::stringstream addr_pi{};
        addr_pi << static_cast<void *>(&pi);
        std::cout << Inspect::all(pi) << std::endl;
        assert(Inspect::type(pi) == "Pi");
        assert(Inspect::addr(pi) == addr_pi.str());
        assert(Inspect::val(pi) == addr_i.str());
    }
    pEpLogH2("Test int* == NULL");
    {
        int *pi = nullptr;
        std::stringstream addr_pi{};
        addr_pi << static_cast<void *>(&pi);
        std::cout << Inspect::all(pi) << std::endl;
        assert(Inspect::type(pi) == "Pi");
        assert(Inspect::addr(pi) == addr_pi.str());
        assert(Inspect::val(pi) == addr_null.str());
    }
    pEpLogH3("Test int*&");
    {
        int *pi = &i;
        std::stringstream addr_pi{};
        addr_pi << static_cast<void *>(&pi);
        int *&rpi = pi;
        std::cout << Inspect::all(rpi) << std::endl;
        assert(Inspect::type(rpi) == "Pi");
        assert(Inspect::addr(rpi) == addr_pi.str());
        assert(Inspect::val(rpi) == addr_i.str());
    }

    pEpLogH3("Test int*&&");
    {
        std::cout << Inspect::all(to_r_value_ref(i)) << std::endl;
        assert(Inspect::type(to_r_value_ref(i)) == "Pi");
        // The address of the rvalue-ref is not pre-determinable
        //        assert(Inspect::addr(to_r_value(i)()) == addr_i.str());
        assert(Inspect::val(to_r_value_ref(i)) == addr_i.str());
    }
}
