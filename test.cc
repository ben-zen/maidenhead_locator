#include <iostream>
#include "maidenhead.hh"

int main()
{
  maidenhead_locator first_location(0.0, 0.0);
  std::cout << first_location.to_string() << std::endl;

  maidenhead_locator seattle_location(47.59978, -122.3346);
  std::cout << "Expected Maidenhead coordinates for Seattle: CN87. Actual: "
	    << seattle_location.to_string() << std::endl;
};
