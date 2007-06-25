#ifndef STREAMS_H
#define STREAMS_H
#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <list>
#include <vector>
#include <map>

class StreamSource; 
typedef boost::shared_ptr<StreamSource> streamSourcePtr_t; 
//typedef boost::weak_ptr<StreamSource> streamSourceWPtr_t; 


class StreamVis; 
typedef boost::shared_ptr<StreamVis> streamVisPtr_t; 
//typedef boost::weak_ptr<StreamVis> streamVisWPtr_t; 

typedef std::list<streamSourcePtr_t> sourcePtrList_t; 

typedef std::multimap<streamSourcePtr_t, streamVisPtr_t> visPtrMap_t; 


#endif

