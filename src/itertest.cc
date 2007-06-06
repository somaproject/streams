#include <iostream>
#include <list>

using namespace std;
int main(void)
{
  std::list<int> mylist; 
  if (mylist.begin() == mylist.end() )
    {
      std::cout << "when list is empty, mylist.begin() == mylist.end()"
		<< std::endl; 
    }

}
