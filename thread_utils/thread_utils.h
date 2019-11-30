#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <iostream>
#include <queue>
#include <algorithm>

#include <sstream>

#include <type_traits>

namespace thread_utils
{
    constexpr uint8_t NUM_OF_THREADS = 10;
    inline std::atomic<uint8_t> numOfThreads = 0;

    inline std::mutex mtx_pathsVector;

    // For stdout
    inline std::mutex m_mtx_stdout;

    // For the output queue (called output)
    inline std::mutex m_mtx_output;
    inline std::unique_lock<std::mutex> m_unqLk_output(thread_utils::m_mtx_output);
    inline std::condition_variable m_outputReady;

    // Deprecated?
    inline std::atomic<bool> finished = false;

    // To keep the main thread alive
    inline std::mutex m_waitTillFinished;
    inline std::unique_lock<std::mutex> m_waitTillFinished_unqLk(thread_utils::m_waitTillFinished);
    inline std::condition_variable waitTillFinished;

//     is_string now checks whether the given type T is any kind of string:
//     Either char*, const char*, std::string or std::string_view are accepted.
    template <typename T> struct is_string : std::integral_constant<bool,
            std::is_same<char*, typename std::decay<T>::type>::value ||
            std::is_same<const char*, typename std::decay<T>::type>::value ||
            std::is_same<std::basic_string<char>, typename std::decay<T>::type>::value ||
            std::is_same<std::string_view, typename std::decay<T>::type>::value
    >
    {};
    template <> struct is_string<std::string> : std::true_type
    {};

    // m_queue struct which extends std::queue, only allows string types as templates and defaults to std::string.
    template <typename T = std::string, typename = typename std::enable_if<is_string<T>::value>::type>
    struct m_queue : public std::queue<T>
    {
    };

    inline m_queue output{};

    // Both T1 and T2 must be of type string (any string-like type)
    template <typename T1 = std::string, typename = typename std::enable_if<is_string<T1>::value>::type,
    typename T2 = std::string, typename = typename std::enable_if<is_string<T2>::value>::type>
    m_queue<T1>& operator<<(m_queue<T1>& q, const T2& item)
    {
        q.push(item);
        if(q == output)
            m_outputReady.notify_one();

        return q;
    }

    template <typename T = std::string, typename = typename std::enable_if<is_string<T>::value>::type>
    m_queue<T>& operator<<(m_queue<T>& q, const std::deque<T>& deq)
    {
        std::stringstream s;
        std::for_each(deq.begin(), deq.end() - 1, [&s](const std::string& page){
            s << page << " -> ";
        });
        s << deq.back() << '\n';

        return q << s.str();
    }

    struct m_cout_threadSafe{};
    inline m_cout_threadSafe coutThreadSafe;

    template <typename T> m_cout_threadSafe& operator<<(m_cout_threadSafe& s, const T& param)
    {
        m_mtx_stdout.lock();
        std::cout << param;
        m_mtx_stdout.unlock();

        return s;
    }

    template <typename T> m_cout_threadSafe& operator<<(m_cout_threadSafe& s, m_queue<T>& param)
    {
        m_mtx_stdout.lock();
        while(!param.empty())
        {
            std::cout << param.front();
            param.pop();
        }
        m_mtx_stdout.unlock();

        return s;
    }

    inline m_cout_threadSafe& operator<<(m_cout_threadSafe &s, std::ostream& (*f)(std::ostream &))
    {
        f(std::cout);
        return s;
    }

    inline m_cout_threadSafe& operator<<(m_cout_threadSafe &s, std::ostream& (*f)(std::ios &))
    {
        f(std::cout);
        return s;
    }

    inline m_cout_threadSafe& operator<<(m_cout_threadSafe &s, std::ostream& (*f)(std::ios_base &))
    {
        f(std::cout);
        return s;
    }

    inline void print()
    {
        while(!thread_utils::finished)
        {
            thread_utils::m_outputReady.wait(thread_utils::m_unqLk_output);
            while(!output.empty() && !thread_utils::finished)
            {
                thread_utils::coutThreadSafe << output;
            }
        }
    }
}
