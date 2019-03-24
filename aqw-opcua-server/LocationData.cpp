#include "LocationData.h"

using namespace weathersvr;

const utility::string_t LocationData::KEY_NAME = U("location");
const utility::string_t LocationData::KEY_CITY_NAME = U("city");
const utility::string_t LocationData::KEY_COUNTRY_CODE = U("country");
const utility::string_t LocationData::KEY_COORDINATES = U("coordinates");
const utility::string_t LocationData::KEY_LATITUDE = U("latitude");
const utility::string_t LocationData::KEY_LONGITUDE = U("longitude");
const short LocationData::INVALID_LATITUDE = 91;
const short LocationData::INVALID_LONGITUDE = 181;

weathersvr::LocationData::LocationData(std::string name, std::string city, std::string countryCode,
	double latitude, double longitude)
	: name {name}, city {city}, countryCode {countryCode}, latitude {latitude}, longitude {longitude}
{}

LocationData LocationData::parseJson(web::json::value &json) {
	// Converts from wstring to string
	std::string lName = utility::conversions::to_utf8string(json.at(KEY_NAME).as_string());
	std::string lCity = utility::conversions::to_utf8string(json.at(KEY_CITY_NAME).as_string());
	std::string lCountryCode = utility::conversions::to_utf8string(json.at(KEY_COUNTRY_CODE).as_string());

	double lLatitude = INVALID_LATITUDE;
	double lLongittude = INVALID_LONGITUDE;
	try {
		auto coordinates = json.at(KEY_COORDINATES);
		lLatitude = coordinates.at(KEY_LATITUDE).as_double();
		lLongittude = coordinates.at(KEY_LONGITUDE).as_double();
	} catch (const web::json::json_exception&) {
		/* Some locations does not provide coordinators. For example Australia and Brazil
		have locations with no coordinates object in the Json array. In this case we use an
		invalid latitude and longitude represented by the constants when a exception is caught. */
	}
	return LocationData(lName, lCity, lCountryCode, lLatitude, lLongittude);
}

std::vector<LocationData> LocationData::parseJsonArray(web::json::value &jsonArray) {
	std::vector<LocationData> vectorAllLocations;
	if (jsonArray.is_array()) {
		for (size_t i {0}; i < jsonArray.size(); i++) {
			auto location = jsonArray[i];
			LocationData locationData = LocationData::parseJson(location);
			vectorAllLocations.push_back(locationData);
		}
	}

	return vectorAllLocations;
}
