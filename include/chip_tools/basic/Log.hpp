#ifndef CHIP_TOOLS_LOG_HPP_
#define CHIP_TOOLS_LOG_HPP_

#include <chip_tools/basic/ErrorCodes.hpp>
#include <iostream>
#include <ctime>
#include <chrono>

/*! \file Log.hpp
 *  \brief Basic functionality for logging
 */

namespace chip_tools
{

/*! \addtogroup Common
 *  @{
 */

/*! \brief Class for calculating number of different messages types called during logging process
 */
class LogStatistics
{
public:
   /*! \brief Returns the number of the trace messages
    */
   size_t trace_msgs() const {return trace_messages_;}
   /*! \brief Returns the number of the debug messages
    */
   size_t debug_msgs() const {return debug_messages_;}
   /*! \brief Returns the number of the info messages
    */
   size_t info_msgs() const {return info_messages_;}
   /*! \brief Returns the number of the warning messages
    */
   size_t warning_msgs() const {return warning_messages_;}
   /*! \brief Returns the number of the error messages
    */
   size_t error_msgs() const {return error_messages_;}
   /*! \brief Returns the number of the fatal messages
    */
   size_t fatal_msgs() const {return fatal_messages_;}
   /*! \brief Reset all counters
    */
   void reset()
   {
      trace_messages_ = debug_messages_ = info_messages_ =
         warning_messages_ = error_messages_ = fatal_messages_ = 0;
   }

protected:
   size_t trace_inc() {return trace_messages_++;}
   size_t debug_inc() {return debug_messages_++;}
   size_t info_inc() {return info_messages_++;}
   size_t warning_inc() {return warning_messages_++;}
   size_t error_inc() {return error_messages_++;}
   size_t fatal_inc() {return fatal_messages_++;}

private:
   size_t trace_messages_ = 0;
   size_t debug_messages_ = 0;
   size_t info_messages_ = 0;
   size_t warning_messages_ = 0;
   size_t error_messages_ = 0;
   size_t fatal_messages_ = 0;
}; // class LogStatistics

/*! \brief Type of the timestamp function
 */
using Timestamp = std::string();

std::string timestamp()
{
   auto now_to_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
   const size_t timestamp_size = 64;
   char timestamp[timestamp_size];
   std::strftime(timestamp, timestamp_size, "[%c]", std::localtime(&now_to_time_t));
   return timestamp;
}

/*! \brief Basic logging engine
 *  \tparam Stream - output stream for messaging
 *  \tparam Statistics - class for statistics calculation
 *  \tparam gettime - function for representing timestamp in the string format.
 */
template<class Stream = std::ostream,
         class Statistics = LogStatistics,
         Timestamp gettime = timestamp>
class Log : public Statistics
{
public:
    /*! \brief The lowest level of displaying messages
     */
   enum class Level {trace, debug, info, warning, error, fatal};
   /*! \param[in] target - output stream for messages.
    */
   explicit Log(Stream &target = std::cout);
   /*! \brief Set the lowest log level
    */
   void set_log_level(Level level) {level_ = level;}
   /*! \brief Get the lowest log level
    */
   void get_log_level() const {return level_;}
   /*! \brief Put trace message
    */
   void trace(const std::string &text);
   /*! \brief Put debug message
    */
   void debug(const std::string &text);
   /*! \brief Put info message
    */
   void info(const std::string &text);
   /*! \brief Put warning message
    */
   void warning(const std::string &text);
   /*! \brief Put error message
    */
   void error(const std::string &text);
   /*! \brief Put fatal message
    */
   void fatal(const std::string &text);

private:
   Stream &stream_;
   Level level_;
}; // class Log

template<class Stream, class Statistics, Timestamp gettime>
Log<Stream, Statistics, gettime>::Log(Stream &target) :
   Statistics(), stream_(target), level_(Level::info)
{
}

template<class Stream, class Statistics, Timestamp gettime>
void Log<Stream, Statistics, gettime>::trace(const std::string &text)
{
   if (level_ <= Level::trace)
   {
      this->trace_inc();
      stream_ << gettime() << " trace: " << text << "\n";
   }
}

template<class Stream, class Statistics, Timestamp gettime>
void Log<Stream, Statistics, gettime>::debug(const std::string &text)
{
   if (level_ <= Level::debug)
   {
      this->debug_inc();
      stream_ << gettime() << " debug: " << text << "\n";
   }
}

template<class Stream, class Statistics, Timestamp gettime>
void Log<Stream, Statistics, gettime>::info(const std::string &text)
{
   if (level_ <= Level::info)
   {
      this->info_inc();
      stream_ << gettime() << " info: " << text << "\n";
   }
}

template<class Stream, class Statistics, Timestamp gettime>
void Log<Stream, Statistics, gettime>::warning(const std::string &text)
{
   if (level_ <= Level::warning)
   {
      this->warning_inc();
      stream_ << gettime() << " warning: " << text << "\n";
   }
}

template<class Stream, class Statistics, Timestamp gettime>
void Log<Stream, Statistics, gettime>::error(const std::string &text)
{
   if (level_ <= Level::error)
   {
      this->error_inc();
      stream_ << gettime() << " error: " << text << "\n";
   }
}

template<class Stream, class Statistics, Timestamp gettime>
void Log<Stream, Statistics, gettime>::fatal(const std::string &text)
{
   this->fatal_inc();
   stream_ << gettime() << " fatal: " << text << "\n";
}

using ConsoleLog = Log<>;
using LogFile = Log<std::ofstream>;

/** @{ */

} // namespace chip_tools 

#endif // #ifndef CHIP_TOOLS_LOG_HPP_
