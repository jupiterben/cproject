#pragma once
#include <iostream>
#include <vector>

std::string parseFilepath(const std::string &filepath);
std::wstring str2wstr(const std::string &str);
#define __DIR__ parseFilepath(__FILE__)

#include <chrono>
class MiniTimer
{
private:
    // Type aliases to make accessing nested type easier
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1>>;
    std::chrono::time_point<clock_t> m_beg;

public:
    MiniTimer() : m_beg(clock_t::now())
    {
    }
    void reset()
    {
        m_beg = clock_t::now();
    }
    double elapsed() const
    {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }
};

class DebugLogger
{
private:
    MiniTimer m_loggerTimer;
    MiniTimer m_innerTimer;
    std::string m_loggerName;
    std::string m_innerName;
    void printInnerStart()
    {
        std::cout << "============ " << m_innerName.c_str() << " ==========" << std::endl;
    }
    void printInnerEnd()
    {
        double t = m_innerTimer.elapsed();
        std::cout << m_innerName.c_str() << " use " << t << " seconds" << std::endl;
    }
    void printLoggerStart()
    {
        std::cout << "============ " << m_loggerName.c_str() << " ==========" << std::endl;
    }
    void printLoggerEnd()
    {
        double t = m_loggerTimer.elapsed();
        std::cout << m_loggerName.c_str() << " use " << t << " seconds" << std::endl;
        std::cout << std::endl;
    }

public:
    DebugLogger(const std::string &timerName) : m_loggerName(timerName)
    {
        printLoggerStart();
    }
    ~DebugLogger()
    {
        printInnerEnd();
        printLoggerEnd();
    }
    void LogTime(const std::string &name)
    {
        printInnerEnd();
        m_innerName = name;
        m_innerTimer.reset();
        printInnerStart();
    }
};
