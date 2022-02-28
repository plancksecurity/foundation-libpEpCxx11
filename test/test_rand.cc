#include <pEp/std_utils.hh>
#include <pEp/pEpLog.hh>
#include <pEp/pitytest11/PityTest.hh>
#include <iostream>
#include <chrono>

using namespace pEp;


int test_random_string_fast()
{
    pEpLog("Test test_random_string_fast() correct length, min & max");
    for (int i = 0; i < 1000; i++) {
        uint len = Utils::random_fast(5000);
        std::string res = Utils::random_string_fast(0, 255, len);
        PITYASSERT(
            res.length() == len,
            "random_string_fast: wrong length: " +
                std::to_string(res.length()) + " is not " + std::to_string(len));
    }
    return 0;
}

int main()
{
    pEp::Adapter::pEpLog::set_enabled(true);

    test_random_string_fast();

    pEpLog("Random functions benchmark");
    int nr_mb = 10;
    int nr_bytes = 1024 * 1024 * nr_mb;
    pEpLog("generating " + std::to_string(nr_mb) + "MB's of random");

    {
        pEpLog("Utils::random_char()");
        auto time_start = std::chrono::steady_clock::now();
        for (int i = 0; i < nr_bytes; i++) {
            Utils::random_char(0, 255);
        }
        auto time_end = std::chrono::steady_clock::now();
        double duration_ms = (double)std::chrono::duration_cast<std::chrono::microseconds>(
                                 time_end - time_start)
                                 .count() /
                             1000;
        pEpLog("Time [ms/mbyte]:" + std::to_string(duration_ms / double(nr_mb)));
    }
    std::cout << "" << std::endl;
    {
        pEpLog("Utils::Fastrand()");
        auto time_start = std::chrono::steady_clock::now();
        for (int i = 0; i < nr_bytes; i++) {
            Utils::random_fast(255);
        }
        auto time_end = std::chrono::steady_clock::now();
        double duration_ms = (double)std::chrono::duration_cast<std::chrono::microseconds>(
                                 time_end - time_start)
                                 .count() /
                             1000;

        pEpLog("Time [ms/mbyte]:" + std::to_string(duration_ms / double(nr_mb)));
    }
    return 0;
}
