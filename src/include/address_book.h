#pragma once

#include <string>
#include <vector>
#include <ostream>

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

public:

	// Default constructor
	AddressBook() {}

	// Copy constructor
	AddressBook(const AddressBook& ab) : entries(ab.entries) {}

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
	* Removes all entries in rhs from lhs ignoring entries that don't exist and returns the result
	* 
	* @param rhs The address book to subtract from this address book
	* @return AddressBook The result of subtracting rhs from this address 
	* 
	* This might be convenient if we want to remove entries from an address book that are in another address book
	* Like if we want to remove all the entries in a known spam address book from our address book
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
	* This method also does a linear search on the entries vector to check if the entry already exists.
	* It's going to get slower as the entries vector gets bigger. (Future improvement) Perhaps use a map? That will
	* make it faster but will take more memory.
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
	* does not exist.
	* It does that so we can know if an entry was removed or not.
	* Also does a linear search on the entries vector to check if the entry exists.
	*/
	void remove(const Entry& person);


	/*
	* @brief Return all entries sorted by first name
	* 
	* @return std::vector<AddressBook::Entry> The entries sorted by first name
	*
	* Note: This function mutates the entries vector and returns it
	* I figured it was ok to mutate the entries vector because if the function is called twice in a row,
	* the second call will not do anything because the entries vector is already sorted.
	*/
	std::vector<Entry> sortedByFirstName();


	/*
	* @brief Return all entries sorted by last name
	* 
	* @return std::vector<AddressBook::Entry> The entries sorted by last name
	*
	* Note: This function mutates the entries vector and returns it
	* Same reason as sortedByFirstName()
	*/
	std::vector<Entry> sortedByLastName();


	/*
	* @brief Return all entries that match the prefix (case insensitive)
	* 
	* Performs a case insensitive linear search on the entries vector and returns all entries that match the prefix
	* 
	* @param prefix The prefix to match
	* @return std::vector<AddressBook::Entry> The entries that match the prefix
	* 
	* Would be better to use a prefix tree for this but time constraints and potentially will take more memory as 
	* the tree will need to store the first and last name of each entry. (Future improvement)
	*/
	std::vector<Entry> find(const std::string & name);

};