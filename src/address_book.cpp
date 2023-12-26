

#include "include/address_book.h"

#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <iostream>

void AddressBook::add(AddressBook::Entry person)
{
	entries.push_back(person);
}

void AddressBook::remove(AddressBook::Entry person)
{
	std::vector<Entry>::iterator it = std::find(entries.begin(), entries.end(), person);

	if (it != entries.end()) {
		entries.erase(it);
	}
}

std::vector<AddressBook::Entry> AddressBook::sortedByFirstName()
{
	std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
		return a.first_name < b.first_name;
		});
	return entries;
}

#include <algorithm>
#include <cctype> // Add this line

std::vector<AddressBook::Entry> AddressBook::sortedByLastName()
{
	std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
		return a.last_name < b.last_name;
		});
	return entries;
}

std::vector<AddressBook::Entry> AddressBook::find(const std::string& name)
{
	// Output vector
	std::vector<Entry> results;

	// Lambda function to check if two chars are the same regardless of case (this gets passed into
	// the lambda function bellow)
	auto is_same_char = [](char a, char b) { return std::tolower(a) == std::tolower(b); };

	// Lambda function to check if a prefix matches an entry's first or last name
	auto copy_condition = [is_same_char, name](Entry e) {
		// Check if first name matches
		if (name.size() <= e.first_name.size()){ // Check if the first name is long enough to match the prefix
			if (std::equal(name.begin(), name.end(), e.first_name.begin(), is_same_char)) {
				return true;
			}
		}

		// Check if the last name matches
		if (name.size() <= e.last_name.size()){ // Check if the last name is long enough to match the prefix
			if (std::equal(name.begin(), name.end(), e.last_name.begin(), is_same_char)) {
				return true;
			}
		}

		return false;
	};

	// Copy all matching entries into the output vector
	std::copy_if(entries.begin(), entries.end(), std::back_inserter(results), copy_condition);

	return results;
}
