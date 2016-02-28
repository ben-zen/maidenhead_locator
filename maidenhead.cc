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

  if (get_subsquare)
  {
    double subsquare_longitude = norm_longitude - ((longitude_degrees / 20) * 20)
      - ((longitude_square_degrees / 2) * 2);
    std::cout << "subsquare_longitude: " << subsquare_longitude << std::endl;
    m_subsquare[0] = 'a' + static_cast<char>(floor(24 * (subsquare_longitude / 2)));
    double subsquare_latitude = norm_latitude - ((latitude_degrees / 10) * 10)
      - latitude_square_degrees;
    std::cout << "subsquare_latitude: " << subsquare_latitude << std::endl;
    m_subsquare[1] = 'a' + static_cast<char>(floor(24 * subsquare_latitude));

    if (get_extendedSquare)
    {
      // Extended Square is going to require a bit of work. Like, a lot of work.
    }
  }
};

maidenhead_locator::maidenhead_locator(std::string locator)
{
  if ((locator.length() % 2) || (locator.length() < 4))
  {
    // throw an exception; maidenhead locators need at least a field and square.
  }

  m_field[0] = locator[0];
  m_field[1] = locator[1];

  m_square[0] = locator[2];
  m_square[1] = locator[3];

  if (locator.length() > 4)
  {
    m_subsquare[0] = locator[4];
    m_subsquare[1] = locator[5];

    if (locator.length() > 6)
    {
      m_extendedSquare[0] = locator[6];
      m_extendedSquare[1] = locator[7];
    }
  }
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
