// Copyright (C) 2016 Ben Lewis KG7VOQ <benjf5+github@gmail.com>
// Licensed under the MIT License

#include <iostream>
#include <math.h>
#include "maidenhead.hh"

double normalize_latitude(double latitude)
{
  double norm_lat = latitude + 90;
  if ((norm_lat > 180) || (norm_lat < 0))
  {
    // throw an exception; invalid latitude?
  }
  return norm_lat;
};

double normalize_longitude(double longitude)
{
  double norm_long = longitude + 180;
  if ((norm_long > 360) || (norm_long < 0))
  {
    // throw an exception; invalid longitude
  }
  return norm_long;
};

maidenhead_locator::maidenhead_locator(double latitude, double longitude,
				       bool get_subsquare,
				       bool get_extendedSquare)
{
  std::cout << "Creating maidenhead_locator for ("
	    << latitude << ", " << longitude << ")." << std::endl;
  double norm_latitude = normalize_latitude(latitude);
  double norm_longitude = normalize_longitude(longitude);

  int latitude_degrees = static_cast<int>(floor(norm_latitude));
  int longitude_degrees = static_cast<int>(floor(norm_longitude));

  m_field[0] = 'A' + (longitude_degrees / 20);
  m_field[1] = 'A' + (latitude_degrees / 10);

  int latitude_square_degrees = latitude_degrees % 10;
  int longitude_square_degrees = longitude_degrees % 20;

  m_square[0] = '0' + (longitude_square_degrees / 2);
  std::cout << "longitude_square_degrees: " << longitude_square_degrees << std::endl;
  m_square[1] = '0' + latitude_square_degrees;
  std::cout << "latitude_square_degrees: " << latitude_square_degrees << std::endl;
};

std::string maidenhead_locator::to_string()
{
  std::string string_form;

  string_form += m_field[0];
  string_form += m_field[1];

  string_form += m_square[0];
  string_form += m_square[1];

  if (m_subsquare[0] != '\0')
  {
    string_form += m_subsquare[0];
    string_form += m_subsquare[1];

    if (m_extendedSquare[0] != '\0')
    {
      string_form += m_extendedSquare[0];
      string_form += m_extendedSquare[1];
    }
  }
  return string_form;
};
