// This file is under GNU General Public License 3.0
// see LICENSE.txt

#ifndef LIBPEPCXX11_CONSTANT_TIME_ALGO_HH
#define LIBPEPCXX11_CONSTANT_TIME_ALGO_HH

#include <string>

namespace pEp {
    // Returns false if a.size() != b.size().
    // Compares always _all_ characters of 'a' and 'b' so runtime does not
    // depends on the character position where the strings differ.
    // Use this function instead of operator== if timing sidechannel attack
    // might be a security problem.
    bool constant_time_equal(const std::string &a, const std::string &b);

} // end of namespace pEp

#endif // LIBPEPCXX11_CONSTANT_TIME_ALGO_HH
