// Copyright (C) 2016 Ben Lewis KG7VOQ <benjf5+github@gmail.com>
// Licensed under the MIT license

// The Maidenhead Locator system encodes the latitude and longitude of a point
// into a grid system, with a variable level of precision depending on how many
// elements of the locator are provided; the information is then encoded into a
// string of the form "FN31pr" (the approximate location of the W1AW station).
// That locator has Field (FN), Square (31), and Subsquare (pr) components, but
// lacks an Extended Square field (which would be represented by two digits).

// The first character of each component provides longitude, while the second
// provides latitude; they are not quite square, even though we refer to these
// coordinates as "grid squares"; in reality, they're rectangles. To determine a
// location, we normalize all points to be non-negative, so the Prime Meridian
// is 180 degrees instead of 0 in this system, and the equator is 90 degrees.

// A field is a convergence of a specific zone of longitude and a specific zone
// of latitude; there are 18 of each, specified from A to R ascending. As such,
// each longitudinal zone covers 20 degrees of longitude, and each latitudinal
// zone covers 10 degrees of latitude.

// Inside each field, there are ten zones each longitudinally and latitudinally,
// and the intersection of two of those zones is known as a square. Each square
// covers 2 degrees of longitude and 1 degree of latitude.

// For increased precision, it is possible to get a subsquare, for which there
// are 24 zones each of longitude and latitude, and are marked with letters 'a'
// through 'x'. Each subsquare is 2.5' of latitude by 5' of longitude.

// To find anything more precise, there's the common concept of dividing each
// subsquare into Extended Squares, again using base 10 for both latitude and
// longitude.

#include <string>

class maidenhead_locator
{
public:
  maidenhead_locator(double latitude, double longitude,
		     bool get_subsquare = false,
		     bool get_extendedSquare = false);
  maidenhead_locator(std::string locator_string);

  std::string to_string();

  // Note that these operations are lossy, and will return the midpoint of the
  // locator at its provided precision; there are many points that can return
  // any given longitude or latitude, and converting one maidenhead locator into
  // another maidenhead locator can result in a loss of precision. These values
  // are restored to their non-normalized form.
  double longitude();
  double latitude();

private:
  char m_field[2] = {};
  char m_square[2] = {};
  char m_subsquare[2] = {};
  char m_extendedSquare[2] = {};
};
