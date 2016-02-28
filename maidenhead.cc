// Copyright (C) 2016 Ben Lewis KG7VOQ <benjf5+github@gmail.com>
// Licensed under the MIT License

#include <iostream>
#include <cctype>
#include <math.h>
#include <stdexcept>
#include "maidenhead.hh"

double normalize_latitude(double latitude)
{
  double norm_lat = latitude + 90;
  if ((norm_lat > 180) || (norm_lat < 0))
  {
    throw std::invalid_argument("normalize_latitude::latitude");
  }
  return norm_lat;
};

double normalize_longitude(double longitude)
{
  double norm_long = longitude + 180;
  if ((norm_long > 360) || (norm_long < 0))
  {
    throw std::invalid_argument("normalize_longitude::longitude");
  }
  return norm_long;
};

char normalize_field(char field_letter)
{
  char norm_letter = '\0';
  if (isalpha(field_letter))
  {
    norm_letter = toupper(field_letter);
    if (!('R' >= norm_letter))
    {
      throw std::invalid_argument("normalize_field::field_letter");
    }
  }
  else
  {
    throw std::invalid_argument("normalize_field::field_letter");
  }
  return norm_letter;
};

// Yes, I realize this is a wrapped call to `isdigit`.
char normalize_square(char square_digit)
{
  if (!isdigit(square_digit))
  {
    throw std::invalid_argument("normalize_square::square_digit");
  }
  return square_digit;
};

char normalize_subsquare(char subsquare_letter)
{
  char norm_letter = '\0';
  if (isalpha(subsquare_letter))
  {
    norm_letter = tolower(subsquare_letter);
    if (!('x' >= norm_letter))
    {
      throw std::invalid_argument("normalize_subsquare::subsquare_letter");
    }
  }
  else
  {
    throw std::invalid_argument("normalize_subsquare::subsquare_letter");
  }
  return norm_letter;
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
    throw std::invalid_argument("maidenhead_locator::maidenhead_locator::locator");
  }

  m_field[0] = normalize_field(locator[0]);
  m_field[1] = normalize_field(locator[1]);

  m_square[0] = normalize_square(locator[2]);
  m_square[1] = normalize_square(locator[3]);

  if (locator.length() > 4)
  {
    m_subsquare[0] = normalize_subsquare(locator[4]);
    m_subsquare[1] = normalize_subsquare(locator[5]);

    if (locator.length() > 6)
    {
      m_extendedSquare[0] = normalize_square(locator[6]);
      m_extendedSquare[1] = normalize_square(locator[7]);
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

double maidenhead_locator::longitude()
{
  double longitude = (m_field[0] - 'A') * 20;
  longitude += (m_square[0] - '0') * 2;
  if (m_subsquare[0] != '\0')
  {
    double longitude_subsquare_measure = 1.0 / 12;
    longitude += (m_subsquare[0] - 'a') * longitude_subsquare_measure;
    if (m_extendedSquare[0] != '\0')
    {
      longitude += (m_extendedSquare[0] - '0') * (longitude_subsquare_measure / 10);
      longitude += (longitude_subsquare_measure / 20);
    }
    else
    {
      // We get the midpoint for this subsquare, since there's no more precision.
      longitude += (longitude_subsquare_measure / 2);
    }
  }
  else
  {
    // We get the midpoint for this square, since there's no higher precision to work with.
    longitude += 1;
  }
  longitude -= 180;
  return longitude;
}


double maidenhead_locator::latitude()
{
  double latitude = (m_field[1] - 'A') * 10;
  latitude += m_square[1] - '0';
  if (m_subsquare[1] != '\0')
  {
    double latitude_subsquare_measure = 1.0 / 24;
    latitude += (m_subsquare[1] - 'a') * latitude_subsquare_measure;
    if (m_extendedSquare[1] != '\0')
    {
      latitude += (m_extendedSquare[1] - '0') * (latitude_subsquare_measure / 10);
      latitude += (latitude_subsquare_measure / 20);
    }
    else
    {
      // We get the midpoint for this subsquare, since there's no greater precision here.
      latitude += (latitude_subsquare_measure / 2);
    }
  }
  else
  {
    // We get the midpoint for this square, since there's no higher precision to work with.
    latitude += 0.5;
  }
  latitude -= 90.0;
  return latitude;
}
