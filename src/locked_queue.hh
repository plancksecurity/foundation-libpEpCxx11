// This file is under GNU General Public License 3.0
// see LICENSE.txt

#ifndef LIBPEPCXX11_LOCKED_QUEUE_HH
#define LIBPEPCXX11_LOCKED_QUEUE_HH

#include <deque>
#include <condition_variable>
#include <mutex>

namespace utility {
    template<class T, void (*Deleter)(T) = nullptr>
    class locked_queue {
        typedef std::recursive_mutex Mutex;
        typedef std::unique_lock<Mutex> Lock;

        int _waiting = 0;
        Mutex _mtx;
        std::condition_variable_any _cv;
        std::deque<T> _q;

    public:
        ~locked_queue()
        {
            clear();
        }

        void clear()
        {
            Lock L(_mtx);
            if (Deleter != nullptr) {
                for (auto q : _q) {
                    Deleter(q);
                }
            }
            _q.clear();
        }

        // defined behavior when queue empty
        T back()
        {
            Lock lg(_mtx);
            if (_q.empty())
                throw std::underflow_error("queue empty");
            return _q.back();
        }

        // defined behavior when queue empty
        T front()
        {
            Lock lg(_mtx);
            if (_q.empty())
                throw std::underflow_error("queue empty");
            return _q.front();
        }

        // returns a copy of the last element.
        // blocks when queue is empty
        T pop_back()
        {
            Lock L(_mtx);
            _cv.wait(L, [&] { return !_q.empty(); });
            T r{std::move(_q.back())};
            _q.pop_back();
            return r;
        }

        // returns a copy of the first element.
        // blocks when queue is empty
        T pop_front()
        {
            Lock L(_mtx);
            _cv.wait(L, [&] { return !_q.empty(); });
            T r{std::move(_q.front())};
            _q.pop_front();
            return r;
        }

        // returns true and set a copy of the last element and pop it from queue if there is any
        // returns false and leaves 'out' untouched if queue is empty even after 'end_time'
        bool try_pop_back(T& out, std::chrono::steady_clock::time_point end_time)
        {
            Lock L(_mtx);
            ++_waiting;
            if (!_cv.wait_until(L, end_time, [this] { return !_q.empty(); })) {
                --_waiting;
                return false;
            }

            --_waiting;
            out = std::move(_q.back());
            _q.pop_back();
            return true;
        }

        // returns true and set a copy of the first element and pop it from queue if there is any
        // returns false and leaves 'out' untouched if queue is empty even after 'end_time'
        bool try_pop_front(T& out, std::chrono::steady_clock::time_point end_time)
        {
            Lock L(_mtx);
            ++_waiting;
            if (!_cv.wait_until(L, end_time, [this] { return !_q.empty(); })) {
                --_waiting;
                return false;
            }

            --_waiting;
            out = std::move(_q.front());
            _q.pop_front();
            return true;
        }

        // returns true and set a copy of the first element and pop it from queue if there is any
        // returns false and leaves 'out' untouched if queue is empty even after 'duration'
        bool try_pop_front(T& out, std::chrono::seconds duration)
        {
            Lock L(_mtx);
            ++_waiting;
            if (!_cv.wait_for(L, duration, [this] { return !_q.empty(); })) {
                --_waiting;
                return false;
            }

            --_waiting;
            out = std::move(_q.front());
            _q.pop_front();
            return true;
        }


        void push_back(const T& data)
        {
            {
                Lock L(_mtx);
                _q.push_back(data);
            }
            _cv.notify_one();
        }

        void emplace_back(const T& data)
        {
            {
                Lock L(_mtx);
                _q.emplace_back(data);
            }
            _cv.notify_one();
        }

        void push_front(const T& data)
        {
            {
                Lock L(_mtx);
                _q.push_front(data);
            }
            _cv.notify_one();
        }

        size_t size()
        {
            Lock lg(_mtx);
            return _q.size();
        }

        bool empty()
        {
            Lock lg(_mtx);
            return _q.empty();
        }

        // returns the number of threads that are waiting in pop_...() or try_pop_...()
        int waiting()
        {
            Lock L(_mtx);
            return _waiting;
        }
    };

} // end of namespace utility

#endif // LIBPEPCXX11_LOCKED_QUEUE_HH
