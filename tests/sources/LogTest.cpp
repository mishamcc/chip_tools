#include <chip_tools/basic/Log.hpp>

#define BOOST_TEST_MODULE chip_tools_log_test
#include <boost/test/included/unit_test.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace chip_tools;

BOOST_AUTO_TEST_SUITE(LogTest)

size_t trace_msgs = 6;
size_t debug_msgs = 5;
size_t info_msgs = 4;
size_t warning_msgs = 3;
size_t error_msgs = 2;
size_t fatal_msgs = 1;

template<typename Log>
void test_log(Log &log, size_t trace_msgs, size_t debug_msgs, size_t info_msgs,
   size_t warning_msgs, size_t error_msgs, size_t fatal_msgs)
{
   for (decltype(trace_msgs) i = 0; i < trace_msgs; ++i)
      log.trace("trace");
   for (decltype(trace_msgs) i = 0; i < debug_msgs; ++i)
      log.debug("debug");
   for (decltype(trace_msgs) i = 0; i < info_msgs; ++i)
      log.info("info");
   for (decltype(trace_msgs) i = 0; i < warning_msgs; ++i)
      log.warning("warning");
   for (decltype(trace_msgs) i = 0; i < error_msgs; ++i)
      log.error("error");
   for (decltype(trace_msgs) i = 0; i < fatal_msgs; ++i)
      log.fatal("fatal");
}

BOOST_AUTO_TEST_CASE(ConsoleLogTest)
{
   ConsoleLog log;
   // with default log level
   int times = 1;
   test_log(log, trace_msgs, debug_msgs, info_msgs, warning_msgs, error_msgs, fatal_msgs);
   BOOST_CHECK_EQUAL(log.trace_msgs(), (times - 1) * trace_msgs);
   BOOST_CHECK_EQUAL(log.debug_msgs(), (times - 1) * debug_msgs);
   BOOST_CHECK_EQUAL(log.info_msgs(), times * info_msgs);
   BOOST_CHECK_EQUAL(log.warning_msgs(), times * warning_msgs);
   BOOST_CHECK_EQUAL(log.error_msgs(), times * error_msgs);
   BOOST_CHECK_EQUAL(log.fatal_msgs(), times * fatal_msgs);

   // level = trace
   log.set_log_level(ConsoleLog::Level::trace);
   ++times;
   test_log(log, trace_msgs, debug_msgs, info_msgs, warning_msgs, error_msgs, fatal_msgs);
   BOOST_CHECK_EQUAL(log.trace_msgs(), (times - 1) * trace_msgs);
   BOOST_CHECK_EQUAL(log.debug_msgs(), (times - 1) * debug_msgs);
   BOOST_CHECK_EQUAL(log.info_msgs(), times * info_msgs);
   BOOST_CHECK_EQUAL(log.warning_msgs(), times * warning_msgs);
   BOOST_CHECK_EQUAL(log.error_msgs(), times * error_msgs);
   BOOST_CHECK_EQUAL(log.fatal_msgs(), times * fatal_msgs);

   // level = warning
   log.set_log_level(ConsoleLog::Level::warning);
   ++times;
   test_log(log, trace_msgs, debug_msgs, info_msgs, warning_msgs, error_msgs, fatal_msgs);
   BOOST_CHECK_EQUAL(log.trace_msgs(), (times - 2) * trace_msgs);
   BOOST_CHECK_EQUAL(log.debug_msgs(), (times - 2) * debug_msgs);
   BOOST_CHECK_EQUAL(log.info_msgs(), (times - 1) * info_msgs);
   BOOST_CHECK_EQUAL(log.warning_msgs(), times * warning_msgs);
   BOOST_CHECK_EQUAL(log.error_msgs(), times * error_msgs);
   BOOST_CHECK_EQUAL(log.fatal_msgs(), times * fatal_msgs);

   // level = error
   log.set_log_level(ConsoleLog::Level::error);
   ++times;
   test_log(log, trace_msgs, debug_msgs, info_msgs, warning_msgs, error_msgs, fatal_msgs);
   BOOST_CHECK_EQUAL(log.trace_msgs(), (times - 3) * trace_msgs);
   BOOST_CHECK_EQUAL(log.debug_msgs(), (times - 3) * debug_msgs);
   BOOST_CHECK_EQUAL(log.info_msgs(), (times - 2) * info_msgs);
   BOOST_CHECK_EQUAL(log.warning_msgs(), (times - 1) * warning_msgs);
   BOOST_CHECK_EQUAL(log.error_msgs(), times * error_msgs);
   BOOST_CHECK_EQUAL(log.fatal_msgs(), times * fatal_msgs);

   // level = fatal
   log.set_log_level(ConsoleLog::Level::fatal);
   ++times;
   log.reset();
   test_log(log, trace_msgs, debug_msgs, info_msgs, warning_msgs, error_msgs, fatal_msgs);
   BOOST_CHECK_EQUAL(log.trace_msgs(), 0);
   BOOST_CHECK_EQUAL(log.debug_msgs(), 0);
   BOOST_CHECK_EQUAL(log.info_msgs(), 0);
   BOOST_CHECK_EQUAL(log.warning_msgs(), 0);
   BOOST_CHECK_EQUAL(log.error_msgs(), 0);
   BOOST_CHECK_EQUAL(log.fatal_msgs(), 1);
}

BOOST_AUTO_TEST_SUITE_END() // LogTest
