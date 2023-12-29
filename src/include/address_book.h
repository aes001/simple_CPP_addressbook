#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <map>

/*
* @brief A class to store address book data
* 
* This class stores address book data and provides methods to add, remove, and find entries
*/
class AddressBook
{
public:
	/// A container for address book data
	struct Entry
	{
		std::string first_name;
		std::string last_name;
		std::string phone_number;

		// Overload the equality operator so we can compare two entries
		// This is useful for functions like std::find and std::remove
		bool operator==(const Entry& rhs);

		friend bool operator==(const Entry& lhs, const Entry& rhs);

		// Overload the inequality operator so we can compare two entries
		// Added for completeness
		friend bool operator!=(const Entry& lhs, const Entry& rhs);

		// Overload the output operator so we can print an entry
		// This is useful for debugging
		friend std::ostream& operator<<(std::ostream& os, const Entry& e);
	};

private:
	// Vector to store all the entries
	std::vector<Entry> entries;

	// Maps to map first and last names to entries
	// This is useful for sorting, and finding entries by first and last name
	// Keys are first for the first_name_map and last names for the last_name_map
	// Values are a vector of indices to entries in the entries vector
	std::map<std::string, std::vector<size_t>> first_name_map;
	std::map<std::string, std::vector<size_t>> last_name_map;

	/*
	* Method to rebuild the maps
	* 
	* This method is called everytime an entry is removed from the address book
	* It rebuilds the maps so that the indices are correct. Indices change everytime an entry is removed so
	* we have to rebuild the maps to keep them consistent.
	* 
	* Future improvement: Perhaps we can use a map of pointers to entries instead of indices to entries
	* That way we might be able to avoid having to rebuild the maps everytime an entry is removed
	*/
	void rebuildMaps();

public:

	// Default constructor
	AddressBook() {}

	// Copy constructor
	AddressBook(const AddressBook& ab) : entries(ab.entries), first_name_map(ab.first_name_map), last_name_map(ab.last_name_map) {};

	// Copy assignment operator
	AddressBook& operator=(const AddressBook& ab);

	// Move assignment operator
	// Convenient for "AddressBook arithmetic"
	AddressBook& operator=(AddressBook&& ab) noexcept;


	/*
	* @brief Overload the plus operator so we can combine two address books together (Ignoring duplicate entries)
	* 
	* Adds all entries in rhs to lhs ignoring duplicate entries and returns the result
	* 
	* @param rhs The address book to add to this address book
	* @return AddressBook The result of adding rhs to this address 
	* 
	* This might be convenient if we want to combine two address books together rather than having to add each entry
	* in a for loop
	*/
	AddressBook operator+(const AddressBook& rhs);
	friend AddressBook operator+(const AddressBook& lhs, const AddressBook& rhs);


	/*
	* @brief Overload the minus operator so we can subtract two address books (Ignoring entries that don't exist)
	* 
	* Removes all entries in rhs from lhs ignoring entries that don't exist and returns the result. Slightly more efficient
	* than calling remove on each entry in rhs as we only rebuild the maps once rather than everytime an entry is removed.
	* 
	* @param rhs The address book to subtract from this address book
	* @return AddressBook The result of subtracting rhs from this address 
	* 
	* Note: We are not using the remove method here because we don't want to rebuild the maps everytime an entry is removed
	* that way we save some time and memory. We only rebuild the maps once at the end.
	* But we are also not using the maps to find the entries to remove. Instead we are using std::remove_if and looping through
	* the rhs entries vector to find the entries to remove. This is quite inefficient making this method quite expensive to call.
	*/
	AddressBook operator-(const AddressBook& rhs);
	friend AddressBook operator-(const AddressBook& lhs, const AddressBook& rhs);


	/*
	* @brief Add a person to the address book
	* 
	* @param person The person to add
	* @throws std::invalid_argument if the entry does not have a first or last name
	* @throws std::invalid_argument if the entry already exists
	* @return void
	* 
	* Note: It's probably a good idea to call this method in a try catch block as it throws an exception if the entry
	* does not have a first or last name or if the entry already exists in the address book.
	* Also checks if the entry already exists by using the first and last name maps.
	* If the entry already exists, it throws an exception.
	*/
	void add(const Entry& person);


	/*
	* @brief Remove a person from the address book
	* 
	* @param person The person to remove
	* @throws std::invalid_argument if the entry does not exist
	* @return void
	* 
	* Note: Probably also a good idea to call this method in a try catch block as it throws an exception if the entry 
	* does not exist
	* It does that so we can know if an entry was removed or not
	* This is also an expensive method to call as we have to rebuild the maps everytime an entry is removed
	*/
	void remove(const Entry& person);


	/*
	* @brief Return all entries sorted by first name
	* 
	* Iterates through the first name map and returns all entries in the order they appear in the map since the map is
	* already sorted by first name
	* 
	* @return std::vector<AddressBook::Entry> The entries sorted by first name
	*
	*/
	std::vector<Entry> sortedByFirstName();


	/*
	* @brief Return all entries sorted by last name
	* 
	* Iterates through the last name map and returns all entries in the order they appear in the map since the map is
	* already sorted by last name
	* 
	* @return std::vector<AddressBook::Entry> The entries sorted by last name
	*/
	std::vector<Entry> sortedByLastName();


	/*
	* @brief Return all entries that match the prefix (case insensitive)
	* 
	* Finds all entries that match the prefix (case insensitive) and returns them in a vector. Uses the first and last
	* name maps to find entries that match the prefix.
	* 
	* @param prefix The prefix to match
	* @return std::vector<AddressBook::Entry> The entries that match the prefix
	* 
	* Note: Might be a good idea to use a prefix tree for this but potentially will take more memory as the tree will need to 
	* store the first and last name of each entry. (Future improvement)
	*/
	std::vector<Entry> find(const std::string & name);

};