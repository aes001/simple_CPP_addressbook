

#include "include/address_book.h"

#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <unordered_map>


bool AddressBook::Entry::operator==(const AddressBook::Entry& rhs)
{
	return first_name == rhs.first_name && last_name == rhs.last_name && phone_number == rhs.phone_number;
}


bool operator==(const AddressBook::Entry& lhs, const AddressBook::Entry& rhs)
{
	return lhs.first_name == rhs.first_name && lhs.last_name == rhs.last_name && lhs.phone_number == rhs.phone_number;
}


bool operator!=(const AddressBook::Entry& lhs, const AddressBook::Entry& rhs)
{
	return !(lhs == rhs);
}


std::ostream& operator<<(std::ostream& os, const AddressBook::Entry& e)
{
	os << e.first_name << " " << e.last_name << " " << e.phone_number;
	return os;
}


// Hash function for AddressBook::Entry
// This is used when we store entries in an unordered map in the find method
namespace std
{
	template<>
	struct hash<AddressBook::Entry>
	{
		size_t operator()(const AddressBook::Entry& e) const
		{
			// I added a comment here please remove later
			// Please remove later I'm trying to get a merge conflict
			return hash<string>()(e.last_name + e.first_name + e.phone_number);
		}
	};
}


AddressBook& AddressBook::operator=(const AddressBook& ab)
{
	entries = ab.entries;
	return *this;
}


// Move assignment operator
AddressBook& AddressBook::operator=(AddressBook&& ab) noexcept
{
	entries = std::move(ab.entries);
	first_name_map = std::move(ab.first_name_map);
	last_name_map = std::move(ab.last_name_map);
	return *this;
}


AddressBook AddressBook::operator+(const AddressBook& rhs)
{
	AddressBook ab = AddressBook(*this);
	for (Entry entry : rhs.entries) {
		try {
			ab.add(entry);
		}
		catch (std::invalid_argument& e) {} // Ignore duplicate or empty entries
	}
	return ab;
}


AddressBook operator+(const AddressBook& lhs, const AddressBook& rhs)
{
	return lhs + rhs;
}


AddressBook AddressBook::operator-(const AddressBook& rhs)
{
	// Remove all entries that are in rhs from this
	auto remove_it = std::remove_if(entries.begin(), entries.end(), [&rhs](const Entry& entry) {
		for (Entry rhs_entry : rhs.entries) {
			if (entry == rhs_entry) {
				return true;
			}
		}
		return false;
	});

	// Delete the entries from the entries vector (remove-erase idiom)
	entries.erase(remove_it, entries.end());

	this->rebuildMaps();
	return *this;
}


AddressBook operator-(const AddressBook& lhs, const AddressBook& rhs)
{
	return lhs - rhs;
}


void AddressBook::add(const AddressBook::Entry& person)
{
	// Check if the entry has a first name and/or a last name
	if (person.first_name.empty() && person.last_name.empty()) {
		throw std::invalid_argument("Entry does not have a first and last name");
	}

	// Lower case the first and last names for the maps (We store the lower case versions of the names)
	std::string first_name_lower = person.first_name;
	std::transform(first_name_lower.begin(), first_name_lower.end(), first_name_lower.begin(), ::tolower);

	std::string last_name_lower = person.last_name;
	std::transform(last_name_lower.begin(), last_name_lower.end(), last_name_lower.begin(), ::tolower);

	// Check if the entry already exists for the first name
	try {
		// Get the vector of indices for the first name
		std::vector<size_t> get_first_name_temp = first_name_map.at(first_name_lower);
		// Loop through the indices and check if the entry already exists
		for (size_t index : get_first_name_temp) {
			if (entries.at(index) == person) {
				throw std::invalid_argument("Entry already exists");
			}
		}
	} catch (std::out_of_range& e) {
		// Entry does not exist (good)
		// This gets thrown if we try to access an entry that does not exist in the map
	}

	// Check if the entry already exists for the last name
	try {
		// Get the vector of indices for the last name
		std::vector<size_t> get_last_name_temp = last_name_map.at(last_name_lower);

		// Loop through the indices and check if the entry already exists
		for (size_t index: get_last_name_temp) {
			if (entries.at(index) == person) {
				throw std::invalid_argument("Entry already exists");
			}
		}
	}
	catch (std::out_of_range& e) {
		// Entry does not exist (good)
		// This gets throwm if we try to access an entry that does not exist in the map
	}

	// If we get here, the entry does not exist in the address book
	// Add the entry to the entries vector
	entries.push_back(person);

	// Add the entry to the first name map
	first_name_map[first_name_lower].push_back(entries.size() - 1);
	last_name_map[last_name_lower].push_back(entries.size() - 1);

}


void AddressBook::remove(const AddressBook::Entry& person)
{
	// Lower case the first and last names for the maps
	std::string first_name_lower = person.first_name;
	std::transform(first_name_lower.begin(), first_name_lower.end(), first_name_lower.begin(), ::tolower);

	std::string last_name_lower = person.last_name;
	std::transform(last_name_lower.begin(), last_name_lower.end(), last_name_lower.begin(), ::tolower);

	// Index of the entry we want to remove (-1 means it does not exist)
	size_t match_index = -1;

	// Check if the entry exists in the first name map
	try {
		// Get the vector of indices for the first name
		std::vector<size_t>& first_name_matched_indices = first_name_map.at(first_name_lower);

		// Loop through the indices and check if the entry exists
		for (int i = 0; i < first_name_matched_indices.size(); i++) {

			// If the entry exists, set the match index and break out of the loop
			// We've found the index of the entry we want to remove
			if (entries.at(first_name_matched_indices.at(i)) == person) {
				match_index = first_name_matched_indices.at(i);
				break;
			}
		}
	}
	catch (std::out_of_range& e) {
		throw std::invalid_argument("Entry does not exist");
	}

	// If we didn't find the entry using the first name map, check in the last name map
	if (match_index == -1)
	{
		// Check if the entry exists in the last name map
		try {
			// Get the vector of indices for the last name
			std::vector<size_t>& last_name_matched_indices = last_name_map.at(last_name_lower);

			// Loop through the indices and check if the entry exists
			for (int i = 0; i < last_name_matched_indices.size(); i++) {

				// If the entry exists, set the match index and break out of the loop
				// We've found the index of the entry we want to remove
				if (entries.at(last_name_matched_indices.at(i)) == person) {
					match_index = last_name_matched_indices.at(i);
					break;
				}
			}
		}
		catch (std::out_of_range& e) {
			throw std::invalid_argument("Entry does not exist");
		}
	}

	// If we get here and the match index is still -1, the entry does not exist
	if (match_index == -1) {
		throw std::invalid_argument("Entry does not exist");
	}

	// Fast remove the entry from the first name map, we can do this because we don't care about the order of the indices
	if (match_index != entries.size() - 1) {
		// Swap the entry we want to remove with the last entry in the entries vector
		std::swap(entries.at(match_index), entries.at(entries.size() - 1));
	}
	// Remove the last entry in the entries vector
	entries.pop_back();

	// Rebuild the maps
	rebuildMaps();
}


void AddressBook::rebuildMaps() {
	// Clear the maps
	first_name_map.clear();
	last_name_map.clear();

	// Rebuild the maps
	for (size_t i = 0; i < entries.size(); i++) {
		// Lower case the first and last names for the maps
		std::string first_name_lower = entries.at(i).first_name;
		std::transform(first_name_lower.begin(), first_name_lower.end(), first_name_lower.begin(), ::tolower);

		std::string last_name_lower = entries.at(i).last_name;
		std::transform(last_name_lower.begin(), last_name_lower.end(), last_name_lower.begin(), ::tolower);

		// Add the entry to the maps
		first_name_map[first_name_lower].push_back(i);
		last_name_map[last_name_lower].push_back(i);
	}
}


std::vector<AddressBook::Entry> AddressBook::sortedByFirstName()
{
	// Output vector
	std::vector<Entry> results;

	// Iterate through the first name map and add all the entries to the output vector
	// We can do this because the first name map is already sorted by first name (std::map)
	for (auto it = first_name_map.begin(); it != first_name_map.end(); it++) {
		for (size_t index : it->second) {
			results.push_back(entries.at(index));
		}
	}

	return results;
}


std::vector<AddressBook::Entry> AddressBook::sortedByLastName()
{
	// Output vector
	std::vector<Entry> results;

	// Iterate through the last name map and add all the entries to the output vector
	// We can do this because the last name map is already sorted by last name (std::map)
	for (auto it = last_name_map.begin(); it != last_name_map.end(); it++) {
		for (size_t index : it->second) {
			results.push_back(entries.at(index));
		}
	}

	return results;
}


std::vector<AddressBook::Entry> AddressBook::find(const std::string& prefix)
{
	// Output vector
	std::vector<Entry> results;
	
	// Lower case the prefix (search term)
	std::string prefix_lower = prefix;
	std::transform(prefix_lower.begin(), prefix_lower.end(), prefix_lower.begin(), ::tolower);

	// Get the lower bound iterators for the first and last name maps
	// This is the first entry in the map that is >= the prefix
	auto lower_bound_it_f_name = first_name_map.lower_bound(prefix_lower);
	auto lower_bound_it_l_name = last_name_map.lower_bound(prefix_lower);

	// Unordered map to keep track of found entries
	// This way we can avoid adding duplicate entries to the output vector
	std::unordered_map<Entry, bool> found_entry_map;

	if (lower_bound_it_f_name != first_name_map.end()) { // Check if the lower bound iterator is valid (if the map found something)
		// Iterate through the first name map starting at the lower bound iterator and stop when we reach the end of the map 
		// or the prefix is no longer a prefix of the first name
		for (auto& it = lower_bound_it_f_name; it != first_name_map.end() && it->first.find(prefix_lower) == 0; it++) {
			// Iterate through the indices returned by the map
			for (size_t index : it->second) {
				// Add the entry to the output vector
				results.push_back(entries.at(index));
				// Add the entry to the found entry map
				found_entry_map[entries.at(index)] = true;
			}
		}
	}

	if (lower_bound_it_l_name != last_name_map.end()) { // Check if the lower bound iterator is valid (if the map found something)
	// Iterate through the last name map starting at the lower bound iterator and stop when we reach the end of the map
	// or the prefix is no longer a prefix of the last name too
		for (auto& it = lower_bound_it_l_name; it != last_name_map.end() && it->first.find(prefix_lower) == 0; it++) {
			// Iterate through the indices returned by the map
			for (size_t index : it->second) {
				// Check if the entry is already in the output vector (to avoid adding the same entry twice)
				if (found_entry_map.find(entries.at(index)) == found_entry_map.end()) {
					// If the entry is not in the output vector, add it
					results.push_back(entries.at(index));
				}
			}
		}
	}

	return results;
}
