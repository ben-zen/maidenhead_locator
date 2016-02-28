// Copyright (C) 2016 Ben Lewis KG7VOQ <benjf5+github@gmail.com>
// Licensed under the MIT license.

#include <iostream>
#include "maidenhead.hh"

int main()
{
  maidenhead_locator first_location(0.0, 0.0);
  std::cout << first_location.to_string() << std::endl;

  maidenhead_locator seattle_location(47.59978, -122.3346, true);
  std::cout << "Expected Maidenhead coordinates for Seattle: CN87to. Actual: "
	    << seattle_location.to_string() << std::endl;

  std::cout << "Seattle coordinates used: (" << 47.59978 << ", " << -122.3346 << "). "
	    << "Maidenhead locator coordinates: (" << seattle_location.latitude() << ", "
	    << seattle_location.longitude() << ")." << std::endl;

  maidenhead_locator seattle_reconverted(seattle_location.latitude(),
					 seattle_location.longitude(),
					 true);

  std::cout << "Re-converted Seattle Maidenhead locator: " << seattle_reconverted.to_string()
	    << "." << std::endl;

  maidenhead_locator w1aw_location("FN31pr");
  std::cout << "Expected Maidehead locator for W1AW: FN31pr. Actual: "
	    << w1aw_location.to_string() << std::endl;
};
