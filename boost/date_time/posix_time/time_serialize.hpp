#ifndef POSIX_TIME_SERIALIZE_HPP___
#define POSIX_TIME_SERIALIZE_HPP___

/* Copyright (c) 2004 CrystalClear Software, Inc.
 * Use, modification and distribution is subject to the 
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)
 * Author: Jeff Garland, Bart Garst
 * $Date$
 */

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/greg_serialize.hpp"
#include "boost/serialization/split_free.hpp"

// macros to split serialize functions into save & load functions
BOOST_SERIALIZATION_SPLIT_FREE(boost::posix_time::ptime)
BOOST_SERIALIZATION_SPLIT_FREE(boost::posix_time::time_duration)
BOOST_SERIALIZATION_SPLIT_FREE(boost::posix_time::time_period)

namespace boost {
namespace serialization {

/*** time_duration ***/

//! Function to save posix_time::time_duration objects using serialization lib
/*! time_duration objects are broken down into 4 parts for serialization:
 * types are hour_type, min_type, sec_type, and fractional_seconds_type
 * as defined in the time_duration class
 */
template<class Archive>
void save(Archive & ar, 
          const posix_time::time_duration& td, 
          unsigned int version)
{
  typename posix_time::time_duration::hour_type h = td.hours();
  typename posix_time::time_duration::min_type m = td.minutes();
  typename posix_time::time_duration::sec_type s = td.seconds();
  typename posix_time::time_duration::fractional_seconds_type fs = td.fractional_seconds();
  ar & make_nvp("time_duration: hours", h);
  ar & make_nvp("time_duration: minutes", m);
  ar & make_nvp("time_duration: seconds", s);
  ar & make_nvp("time_duration: fractional seconds", fs);
}

//! Function to load posix_time::time_duration objects using serialization lib
/*! time_duration objects are broken down into 4 parts for serialization:
 * types are hour_type, min_type, sec_type, and fractional_seconds_type
 * as defined in the time_duration class
 */
template<class Archive>
void load(Archive & ar, 
          posix_time::time_duration & td, 
          unsigned int version)
{
  typename posix_time::time_duration::hour_type h(0);
  typename posix_time::time_duration::min_type m(0);
  typename posix_time::time_duration::sec_type s(0);
  typename posix_time::time_duration::fractional_seconds_type fs(0);
  ar & make_nvp("time_duration: hours", h);
  ar & make_nvp("time_duration: minutes", m);
  ar & make_nvp("time_duration: seconds", s);
  ar & make_nvp("time_duration: fractional seconds", fs);
  td = posix_time::time_duration(h,m,s,fs);
}

// no load_construct_data function provided as time_duration provides a
// default constructor

/*** ptime ***/

//! Function to save posix_time::ptime objects using serialization lib
/*! ptime objects are broken down into 2 parts for serialization:
 * a date object and a time_duration onject
 */
template<class Archive>
void save(Archive & ar, 
          const posix_time::ptime& pt, 
          unsigned int version)
{
  // from_iso_string does not include fractional seconds
  // therefore date and time_duration are used
  typename posix_time::ptime::date_type d = pt.date();
  typename posix_time::ptime::time_duration_type td = pt.time_of_day();
  ar & make_nvp("ptime: date", d);
  ar & make_nvp("ptime: time_duration", td);
}

//! Function to load posix_time::ptime objects using serialization lib
/*! ptime objects are broken down into 2 parts for serialization:
 * a date object and a time_duration onject
 */
template<class Archive>
void load(Archive & ar, 
          posix_time::ptime & pt, 
          unsigned int version)
{
  // from_iso_string does not include fractional seconds
  // therefore date and time_duration are used
  typename posix_time::ptime::date_type d(posix_time::not_a_date_time);
  typename posix_time::ptime::time_duration_type td;
  ar & make_nvp("ptime: date", d);
  ar & make_nvp("ptime: time_duration", td);
  pt = boost::posix_time::ptime(d,td);
}

//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, 
                                posix_time::ptime* pt, 
                                const unsigned int file_version)
{
  // retrieve data from archive required to construct new 
  // invoke inplace constructor to initialize instance of date
  new(pt) boost::posix_time::ptime(boost::posix_time::not_a_date_time);
}

/*** time_period ***/

//! Function to save posix_time::time_period objects using serialization lib
/*! time_period objects are broken down into 2 parts for serialization:
 * a begining ptime object and an ending ptime object
 */
template<class Archive>
void save(Archive & ar, 
          const posix_time::time_period& tp, 
          unsigned int version)
{
  typename posix_time::ptime beg(tp.begin().date(), tp.begin().time_of_day());
  typename posix_time::ptime end(tp.end().date(), tp.end().time_of_day());
  ar & make_nvp("time_period: begin", beg);
  ar & make_nvp("time_period: end", end);
}

//! Function to load posix_time::time_period objects using serialization lib
/*! time_period objects are broken down into 2 parts for serialization:
 * a begining ptime object and an ending ptime object
 */
template<class Archive>
void load(Archive & ar, 
          boost::posix_time::time_period & tp, 
          unsigned int version)
{
  typename posix_time::time_duration td(1,0,0);
  typename gregorian::date d(gregorian::not_a_date_time);
  typename posix_time::ptime beg(d,td);
  typename posix_time::ptime end(d,td);
  ar & make_nvp("time_period: begin", beg);
  ar & make_nvp("time_period: end", end);
  tp = boost::posix_time::time_period(beg, end);
}

//!override needed b/c no default constructor
template<class Archive>
inline void load_construct_data(Archive & ar, 
                                boost::posix_time::time_period* tp, 
                                const unsigned int file_version)
{
  typename posix_time::time_duration td(1,0,0);
  typename gregorian::date d(gregorian::not_a_date_time);
  typename posix_time::ptime beg(d,td);
  typename posix_time::ptime end(d,td);
  new(tp) boost::posix_time::time_period(beg,end);
}

} // namespace serialization
} // namespace boost

#endif
