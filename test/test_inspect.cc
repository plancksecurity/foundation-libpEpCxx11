#include <iostream>
#include <cctype>
#include "../src/inspect.hh"
#include <pEp/utils.hh>
#include <pEp/pEpLog.hh>

using pEp::CXX::Inspect;

int main()
{
    int i{ 23 };
    std::cout << pEp::CXX::Inspect::all(i) << std::endl;
    std::stringstream ss_i{};
    ss_i << static_cast<void *>(&i);
    assert(Inspect::type(i) == "i");
    assert(Inspect::addr(i) == ss_i.str());
    assert(Inspect::val(i) == std::to_string(i));

    int *pi = &i;
    std::stringstream ss_pi{};
    ss_pi << static_cast<void *>(&pi);
    std::cout << pEp::CXX::Inspect::all(pi) << std::endl;
    assert(Inspect::type(pi) == "Pi");
    assert(Inspect::addr(pi) == ss_pi.str());
    assert(Inspect::val(pi) == ss_i.str());
}
