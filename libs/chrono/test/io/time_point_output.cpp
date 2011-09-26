//  Copyright 2011 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono/chrono_io.hpp>
#include <sstream>
#include <boost/detail/lightweight_test.hpp>

template<typename Clock, typename D>
void test_good_prefix(const char* str, D d)
{
  std::ostringstream out;
  boost::chrono::time_point<Clock,D> tp(d);
  out << tp;
  BOOST_TEST(out.good());
  BOOST_TEST((out.str() == std::string(str)+boost::chrono::clock_string<Clock,char>::since()));
}

template<typename Clock, typename D>
void test_good_symbol(const char* str, D d)
{
  std::ostringstream out;
  boost::chrono::time_point<Clock,D> tp(d);
  out << boost::chrono::duration_short << tp;
  BOOST_TEST(out.good());
  BOOST_TEST((out.str() == std::string(str)+boost::chrono::clock_string<Clock,char>::since()));
}

template<typename Clock, typename D>
void test_good(const char* str, D d, boost::chrono::duration_style::type style)
{
  std::ostringstream out;
  boost::chrono::time_point<Clock,D> tp(d);
  out << boost::chrono::duration_fmt(style) << tp;
  BOOST_TEST(out.good());
  BOOST_TEST((out.str() == std::string(str)+boost::chrono::clock_string<Clock,char>::since()));
}

template<typename Clock>
void check_all()
{
  using namespace boost::chrono;
  using namespace boost;

  test_good<Clock>("2 hours", hours(2), duration_style::prefix_text);
  test_good<Clock>("2 h", hours(2), duration_style::symbol);

  test_good_prefix<Clock>("2 hours", hours(2));
  test_good_prefix<Clock>("2 minutes", minutes(2));
  test_good_prefix<Clock>("2 seconds", seconds(2));
  test_good_prefix<Clock>("1 seconds", seconds(1));
  test_good_prefix<Clock>("-1 seconds", seconds(-1));
  test_good_prefix<Clock>("2 milliseconds", milliseconds(2));
  test_good_prefix<Clock>("2 microseconds", microseconds(2));
  test_good_prefix<Clock>("2 nanoseconds", nanoseconds(2));
  test_good_prefix<Clock>("2 deciseconds", duration<int_least64_t, deci> (2));
  test_good_prefix<Clock>("2 [1/30]seconds", duration<int_least64_t, ratio<1, 30> > (2));

  test_good_symbol<Clock>("2 h", hours(2));
  test_good_symbol<Clock>("2 m", minutes(2));
  test_good_symbol<Clock>("2 s", seconds(2));
  test_good_symbol<Clock>("2 ms", milliseconds(2));
  test_good_symbol<Clock>("2 ns", nanoseconds(2));
  test_good_symbol<Clock>("2 ds", duration<int_least64_t, deci> (2));
  test_good_symbol<Clock>("2 [1/30]s", duration<int_least64_t, ratio<1, 30> > (2));
}

int main()
{

  std::cout << "high_resolution_clock=";
  check_all<boost::chrono::high_resolution_clock>();
#ifdef BOOST_CHRONO_HAS_CLOCK_STEADY
  std::cout << "steady_clock=";
  check_all<boost::chrono::steady_clock>();
#endif
  //std::cout << "system_clock=";
  //check_all<boost::chrono::system_clock>();

#if defined(BOOST_CHRONO_HAS_THREAD_CLOCK)
  std::cout << "thread_clock=";
  check_all<boost::chrono::thread_clock>();
#endif

#if defined(BOOST_CHRONO_HAS_PROCESS_CLOCKS)
  std::cout << "process_real_cpu_clock=";
  check_all<boost::chrono::process_real_cpu_clock>();
  std::cout << "process_user_cpu_clock=";
  check_all<boost::chrono::process_user_cpu_clock>();
  std::cout << "process_system_cpu_clock=";
  check_all<boost::chrono::process_system_cpu_clock>();
  std::cout << "process_cpu_clock=";
  check_all<boost::chrono::process_cpu_clock>();
#endif
}
