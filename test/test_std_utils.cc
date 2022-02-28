#include <pEp/std_utils.hh>
#include <pEp/pEpLog.hh>
#include <iostream>
#include <cassert>
#include <pEp/pitytest11/PityTest.hh>

using namespace pEp;


int test_random_string_fast()
{
    pEpLog("Test test_random_string_fast() correct length, min & max");
    for (int i = 0; i < 1000; i++) {
        uint len = Utils::random_fast(5000);
        std::string res = Utils::random_string_fast(0, 255, len);
        PITYASSERT(
            res.length() == len,
            "random_string_fast: wrong length: " + std::to_string(res.length()) + " is not " +
                std::to_string(len));
    }
    return 0;
}

int test_rand()
{
    pEpLogH1("Test random functions");
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

int test_file_rw_bin()
{
    pEpLogH1("Test file_read_bin() / file_write_bin()");
    std::string filename = "test_rw.bin";

    std::vector<char> v_out{};
    for (int i = 0; i < 1000; i++) {
        v_out.push_back(pEp::Utils::random_char(0, 255));
    }

    pEp::Utils::file_write_bin(filename, v_out);

    std::vector<char> v_in = pEp::Utils::file_read_bin(filename);

    PITYASSERT(v_in == v_out, "in is not equal out");
    return 0;
}

int test_hex2bin() {
    // Test hex2bin
    pEpLogH1("Test hex2bin");
    {
        pEpLogH2("Test Valid hex string");
        std::string str_in{ "FFABCD00EF123200" };
        std::vector<unsigned char> bin = Utils::hex2bin<unsigned char>(str_in);
        PITYASSERT(str_in.length() == bin.size() * 2, "Size error");

        std::string str_out = pEp::Utils::bin2hex(bin);
        pEpLog("Hex_IN  : '" + Utils::to_lower(str_in) + "'");
        pEpLog("Hex_OUT : '" + Utils::to_lower(str_out) + "'");

        PITYASSERT(Utils::to_lower(str_in) == Utils::to_lower(str_out), "roundtrip failed");
    }

    {
        pEpLogH2("Test Uneven string throws");
        std::string str_in{ "FFA" };
        PITYASSERT_THROWS(Utils::hex2bin<char>(str_in), "Uneven string should throw");
    }

    {
        pEpLogH2("Test Non-hex chars throws");
        std::string str_in{ "pEp!" };
        PITYASSERT_THROWS(Utils::hex2bin<char>(str_in), "Invalid hex chars should throw");
    }
    return 0;
}

int main()
{
    pEp::Adapter::pEpLog::set_enabled(true);

    test_file_rw_bin();
    test_hex2bin();
    test_rand();

    pEpLog("All tests passsed");
}
