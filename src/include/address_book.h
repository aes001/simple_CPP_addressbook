#pragma once

#include <string>
#include <vector>
#include <ostream>

/// The main Address Book implementation. Extend as required.
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
		bool operator==(const Entry& rhs) {
			return this->first_name == rhs.first_name && this->last_name == rhs.last_name && this->phone_number == rhs.phone_number;
		}

		// Overload the output operator so we can print an entry
		friend std::ostream& operator<<(std::ostream& os, const Entry& e) {
			os << e.first_name << " " << e.last_name << " " << e.phone_number;
			return os;
		}
	};

private:
	// Vector to store all the entries
	std::vector<Entry> entries;

public:

	// Might have to create the copy constructor

	// Where is the constructor for this?
		// Need to think of how to store all the data address data...
		// Could do it with std::vector...

	// Overloading the add opeartor would be nice

	// Overloading the minus operator would be nice too

	// If I'm using std::map then I need to overload the [] operator
	// to prevent adding empty entries



	/// Add an entry. Implement in address_book.cpp.
	void add(Entry person);

	/// Remove an entry. Implement in address_book.cpp.
	void remove(Entry person);

	/// Return all entries sorted by first names. Implement in address_book.cpp.
	std::vector<Entry> sortedByFirstName();

	/// Return all entries sorted by last names. Implement in address_book.cpp.
	std::vector<Entry> sortedByLastName();

	/// Return all matching entries. Implement in address_book.cpp.
	std::vector<Entry> find(const std::string & name);

};