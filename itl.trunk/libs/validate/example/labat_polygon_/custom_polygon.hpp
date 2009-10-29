/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __custom_polygon_hpp_JOFA_090901__
#define __custom_polygon_hpp_JOFA_090901__

#include <boost/polygon/polygon.hpp>
#include "point_gentor.hpp"
#include "polygon_gentor.hpp"

typedef boost::itl::point<int> CPoint;

namespace boost{ namespace polygon{

  template <>
  struct geometry_concept<CPoint > { typedef point_concept type; };

  template <>
  struct point_traits<CPoint > {
    typedef int coordinate_type;

    static inline coordinate_type get(const CPoint & point,
                                      orientation_2d orient) {
      if(orient == HORIZONTAL)
        return point.x;
      return point.y;
    }
  };

  template <>
  struct point_mutable_traits<CPoint > {
    static inline void set(CPoint & point, orientation_2d orient, int value) {
      if(orient == HORIZONTAL)
        point.x = value;
      else
        point.y = value;
    }
    static inline CPoint  construct(int x_value, int y_value) {
      CPoint  retval;
      retval.x = x_value;
      retval.y = y_value;
      return retval;
    }
  };
}}

//the CPolygon example
typedef boost::itl::list<CPoint > CPolygon;

//we need to specialize our polygon concept mapping in gtl
namespace boost{ namespace polygon{
  //first register CPolygon as a polygon_concept type
  template <>
  struct geometry_concept<CPolygon>{ typedef polygon_concept type; };

  template <>
  struct polygon_traits<CPolygon> {
    typedef int coordinate_type;
    typedef CPolygon::const_iterator iterator_type;
    typedef CPoint point_type;

    // Get the begin iterator
    static inline iterator_type begin_points(const CPolygon& t) {
      return t.begin();
    }

    // Get the end iterator
    static inline iterator_type end_points(const CPolygon& t) {
      return t.end();
    }

    // Get the number of sides of the polygon
    static inline std::size_t size(const CPolygon& t) {
      return t.size();
    }

    // Get the winding direction of the polygon
    static inline winding_direction winding(const CPolygon& t) {
      return unknown_winding;
    }
  };

  template <>
  struct polygon_mutable_traits<CPolygon> {
    //expects stl style iterators
    template <typename iT>
    static inline CPolygon& set_points(CPolygon& t,
                      iT input_begin, iT input_end) {
      t.clear();
      while(input_begin != input_end) {
        t.push_back(CPoint ());
        gtl::assign(t.back(), *input_begin);
        ++input_begin;
      }
      return t;
    }

  };
}}

//OK, finally we get to declare our own polygon set type
typedef boost::itl::list<CPolygon> CPolygonSet;

//deque isn't automatically a polygon set in the library
//because it is a standard container there is a shortcut
//for mapping it to polygon set concept, but I'll do it
//the long way that you would use in the general case.
namespace boost{ namespace polygon{
  //first we register CPolygonSet as a polygon set
  template <>
  struct geometry_concept<CPolygonSet> { typedef polygon_set_concept type; };

  //next we map to the concept through traits
  template <>
  struct polygon_set_traits<CPolygonSet> {
    typedef int coordinate_type;
    typedef CPolygonSet::const_iterator iterator_type;
    typedef CPolygonSet operator_arg_type;

    static inline iterator_type begin(const CPolygonSet& polygon_set) {
      return polygon_set.begin();
    }

    static inline iterator_type end(const CPolygonSet& polygon_set) {
      return polygon_set.end();
    }

    //don't worry about these, just return false from them
    static inline bool clean(const CPolygonSet& polygon_set) { return false; }
    static inline bool sorted(const CPolygonSet& polygon_set) { return false; }
  };

  template <>
  struct polygon_set_mutable_traits<CPolygonSet> {
    template <typename input_iterator_type>
    static inline void set(CPolygonSet& polygon_set, input_iterator_type input_begin, input_iterator_type input_end) {
      polygon_set.clear();
      //this is kind of cheesy. I am copying the unknown input geometry
      //into my own polygon set and then calling get to populate the
      //deque
      polygon_set_data<int> ps;
      ps.insert(input_begin, input_end);
      ps.get(polygon_set);
      //if you had your own odd-ball polygon set you would probably have
      //to iterate through each polygon at this point and do something
      //extra
    }
  };
}}


#endif
