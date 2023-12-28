

#include "include/address_book.h"

#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <iostream>


void AddressBook::add(const AddressBook::Entry& person)
{
	// Check if the entry has a first name and/or a last name
	if (person.first_name.empty() && person.last_name.empty()) {
		throw std::invalid_argument("Entry does not have a first and last name");
	}

	// Check if the entry already exists
	if (std::find(entries.begin(), entries.end(), person) != entries.end()) {
		throw std::invalid_argument("Entry already exists");
	}
	entries.push_back(person);
}


void AddressBook::remove(const AddressBook::Entry& person)
{
	// Try to find the entry
	std::vector<Entry>::iterator it = std::find(entries.begin(), entries.end(), person);

	// If the entry exists, remove it else throw an exception
	if (it != entries.end()) {
		entries.erase(it);
	} else {
		throw std::invalid_argument("Entry does not exist");
	}
}


std::vector<AddressBook::Entry> AddressBook::sortedByFirstName()
{
	// Sort the entries by first name and return the entries vector
	std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
		return a.first_name < b.first_name;
	});
	return entries;
}


std::vector<AddressBook::Entry> AddressBook::sortedByLastName()
{
	// Sort the entries by last name and return the entries vector
	std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
		return a.last_name < b.last_name;
	});
	return entries;
}


std::vector<AddressBook::Entry> AddressBook::find(const std::string& prefix)
{
	// Output vector
	std::vector<Entry> results;
	
	// Lower case the prefix (search term)
	std::string prefix_lower = prefix;
	std::transform(prefix_lower.begin(), prefix_lower.end(), prefix_lower.begin(), ::tolower);

	// Lambda function to check if the entry matches the prefix
	auto is_prefix_match = [prefix_lower](const Entry& person) {

		// Lowercase the the first and last names
		std::string first_name_lower = person.first_name;
		std::transform(first_name_lower.begin(), first_name_lower.end(), first_name_lower.begin(), ::tolower);

		std::string last_name_lower = person.last_name;
		std::transform(last_name_lower.begin(), last_name_lower.end(), last_name_lower.begin(), ::tolower);

		// Check if the first or last name starts with the prefix
		return first_name_lower.find(prefix_lower) == 0 || last_name_lower.find(prefix_lower) == 0;
	};

	// Copy all matching entries into the output vector
	std::copy_if(entries.begin(), entries.end(), std::back_inserter(results), is_prefix_match);

	return results;
}
