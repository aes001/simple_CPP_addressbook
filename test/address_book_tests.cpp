#include "address_book.h"

#include <gtest/gtest.h>
#include <string>

///  Sample test data
std::string people[][3] = {
		{"Sally", "Graham", "+44 7700 900297"},
		{"Phoenix", "Bond", "0161 496 0311"},
		{"Aaran", "Parks", ""},
		{"Jayden", "Riddle", "+44 131 496 0609"},
		{"Adriana", "Paul", "(739) 391-4868"},
		{"Hamza", "Bo", "+44 131 496 0571"}
	};

///  Sample test data
AddressBook AddTestPeople()
{
	AddressBook addressBook;
	// Add all of the test data to the address book
	for (auto person : people)
	{
		AddressBook::Entry entry = { person[0], person[1], person[2] };
		addressBook.add(entry);
	}
	return addressBook;
}


/// Tests that it is possible to add a person to the address book.
TEST(AddressBookTests, AddPerson)
{
	AddressBook ab;
	// Add just the first person to an empty address book
	AddressBook::Entry entry = { people[0][0], people[0][1], people[0][2] };
	ab.add(entry);

	// Get the contents of the address book
	std::vector<AddressBook::Entry> results = ab.sortedByFirstName();
	
	// There should only be 1 entry in the results
	ASSERT_EQ(results.size(), 1);

	// Validate that the name and phoen number is correct
	ASSERT_EQ(results[0].first_name, people[0][0]);
	ASSERT_EQ(results[0].last_name, people[0][1]);
	ASSERT_EQ(results[0].phone_number, people[0][2]);
}


/// Tests that entries are sorted by first name correctly.
TEST(AddressBookTests, SortedByFirstNames)
{
	// The correctly sorted test data 
	const std::string people_sortedFirstNames[][3] = {
		{"Aaran", "Parks", ""},
		{"Adriana", "Paul", "(739) 391-4868"},
		{"Hamza", "Bo", "+44 131 496 0571"},
		{"Jayden", "Riddle", "+44 131 496 0609"},
		{"Phoenix", "Bond", "0161 496 0311"},
		{"Sally", "Graham", "+44 7700 900297"},
	};

	// Populate the address book
	AddressBook ab = AddTestPeople();

	// Sort by first names
	std::vector<AddressBook::Entry> results = ab.sortedByFirstName();

	// There should only be 6 entries in the results 
	ASSERT_EQ(results.size(), 6);

	// Validate that all of the results exactly match the desired output
	for (size_t i=0; i < results.size(); i++)
	{
		auto personResult = results[i];
		auto personAnswer = people_sortedFirstNames[i];

		ASSERT_EQ(personResult.first_name, personAnswer[0]);
		ASSERT_EQ(personResult.last_name, personAnswer[1]);
		ASSERT_EQ(personResult.phone_number, personAnswer[2]);
	}
}


/// Tests that entries are sorted by last name correctly.
TEST(AddressBookTests, SortedByLastNames)
{
	const std::string people_sortedLastNames[][3] = {
		{"Hamza", "Bo", "+44 131 496 0571"},
		{"Phoenix", "Bond", "0161 496 0311"},
		{"Sally", "Graham", "+44 7700 900297"},
		{"Aaran", "Parks", ""},
		{"Adriana", "Paul", "(739) 391-4868"},
		{"Jayden", "Riddle", "+44 131 496 0609"},
	};

	// Populate the address book
	AddressBook ab = AddTestPeople();

	// Sort by last names
	std::vector<AddressBook::Entry> results = ab.sortedByLastName();

	// There should only be 6 entries in the results 
	ASSERT_EQ(results.size(), 6);

	// Validate that all of the results exactly match the desired output
	for (size_t i = 0; i < results.size(); i++)
	{
		auto personResult = results[i];
		auto personAnswer = people_sortedLastNames[i];

		ASSERT_EQ(personResult.first_name, personAnswer[0]);
		ASSERT_EQ(personResult.last_name, personAnswer[1]);
		ASSERT_EQ(personResult.phone_number, personAnswer[2]);
	}
}


/// Tests that an entry can be found in the address book.
TEST(AddressBookTests, FindPerson)
{
	// Populate the address book
	AddressBook ab = AddTestPeople();

	// Find a person whose name is, or starts with "Graham"
	std::vector<AddressBook::Entry> results = ab.find("Graham");

	// There should only be exactly 1 entry in the results 
	ASSERT_EQ(results.size(), 1);

	// Validate that the result is the entry we expected
	ASSERT_EQ(results[0].first_name, "Sally");
	ASSERT_EQ(results[0].last_name, "Graham");
	ASSERT_EQ(results[0].phone_number, "+44 7700 900297");
}

/// Add more tests here:
/// 
/// TEST(AddressBookTests, YOUR_TEST_NAME)
/// {
/// 	// Perform some test logic
/// }



int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}