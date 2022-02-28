// This file is under GNU General Public License 3.0
// see LICENSE.txt

#include <iostream>
#include <thread>
#include <pEp/std_utils.hh>
#include "../src/Semaphore.hh"

using namespace std;
using namespace pEp;

int main()
{
    Semaphore semaphore;

    thread thread1([&]() {
        cout << "1: before stop\n";
        semaphore.stop();
        cout << "1: now on stop\n";
        semaphore.try_wait();
        cout << "1: and on go again\n";
        semaphore.try_wait();
        cout << "1: keep going\n";
    });

    Utils::sleep_millis(1000);

    thread thread2([&]() {
        cout << "2: setting go\n";
        semaphore.go();
    });

    thread1.join();
    thread2.join();

    return 0;
}
