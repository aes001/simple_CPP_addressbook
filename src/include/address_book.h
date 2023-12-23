#pragma once

#include <string>
#include <vector>

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
	};

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