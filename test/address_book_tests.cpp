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


/// Tests that it is not possible to add an invalid entry to the address book. (No first or last name)
TEST(AddressBookTests, AddInvalidPerson)
{
	AddressBook ab;
	// Try to Add an empty entry
	AddressBook::Entry entry = { "", "", "" };

	// Try to add the invalid entry
	EXPECT_THROW(ab.add(entry), std::invalid_argument) << "Expected invalid argument exception with empty entry";
}


/// Tests that it is not possible to add a duplicate person to the address book.
TEST(AddressBookTests, AddDuplicatePerson)
{
	AddressBook ab;
	// Add the first person to an empty address book
	AddressBook::Entry entry = { people[0][0], people[0][1], people[0][2] };
	ab.add(entry);

	// Ensure that the entry was added
	std::vector<AddressBook::Entry> results = ab.sortedByFirstName();
	EXPECT_EQ(results.size(), 1) << "Expected 1 entry in the address book " << entry;

	EXPECT_THROW(ab.add(entry), std::invalid_argument) << "Expected \"Invalid argument exception\" with duplicate entry " << entry;
}

/// Tests that it is possible to add person with the same first name but different last name to the address book.
TEST(AddressBookTests, AddPersonWithSameFirstName)
{
	AddressBook ab = AddTestPeople();

	AddressBook::Entry entry = { "Jacob", "Smith", "000000000" };
	ab.add(entry);
	AddressBook::Entry entry2 = { "Jacob", "Jones", "000000000" };
	ab.add(entry2);

	// Ensure that the entry was added
	std::vector<AddressBook::Entry> results = ab.sortedByFirstName();
	EXPECT_EQ(results.size(), 8) << "Expected 8 entries in the address book " << entry;

	// Find the entries we just added
	results = ab.find("Jacob");
	EXPECT_EQ(results.size(), 2) << "Expected 2 entries with first name \"Jacob\"";

	EXPECT_EQ(results[0].first_name, "Jacob");
	EXPECT_EQ(results[0].last_name, "Smith");
	EXPECT_EQ(results[0].phone_number, "000000000");

	EXPECT_EQ(results[1].first_name, "Jacob");
	EXPECT_EQ(results[1].last_name, "Jones");
	EXPECT_EQ(results[1].phone_number, "000000000");

	// Add another entry with the same last name but different first name
	AddressBook::Entry entry3 = { "Ingram", "Smith", "000000000" };
	ab.add(entry3);

	// Ensure that the entry was added
	results = ab.sortedByFirstName();
	EXPECT_EQ(results.size(), 9) << "Expected 9 entries in the address book " << entry;

	// Find the entries we just added
	results = ab.find("Smith");

	EXPECT_EQ(results.size(), 2) << "Expected 2 entries with last name \"Smith\"";
	EXPECT_EQ(results[0].first_name, "Jacob");
	EXPECT_EQ(results[0].last_name, "Smith");
	EXPECT_EQ(results[0].phone_number, "000000000");

	EXPECT_EQ(results[1].first_name, "Ingram");
	EXPECT_EQ(results[1].last_name, "Smith");
	EXPECT_EQ(results[1].phone_number, "000000000");
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
	EXPECT_EQ(results.size(), 6) << "Expected results size to be 6 got " << results.size();

	// Validate that all of the results exactly match the desired output
	for (size_t i=0; i < results.size(); i++)
	{
		auto personResult = results[i];
		auto personAnswer = people_sortedFirstNames[i];

		EXPECT_EQ(personResult.first_name, personAnswer[0]);
		EXPECT_EQ(personResult.last_name, personAnswer[1]);
		EXPECT_EQ(personResult.phone_number, personAnswer[2]);
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

	// Find a person whose name is, or starts with "Graham" (case insensitive)
	std::vector<AddressBook::Entry> results = ab.find("gRahAm");

	// There should only be exactly 1 entry in the results 
	EXPECT_EQ(results.size(), 1) << "Expected results size to be 1, got " << results.size() << " instead.";

	// Validate that the result is the entry we expected
	ASSERT_EQ(results[0].first_name, "Sally");
	ASSERT_EQ(results[0].last_name, "Graham");
	ASSERT_EQ(results[0].phone_number, "+44 7700 900297");

	// Find a person whose name is, or starts with "a"
	results = ab.find("a");

	// There should be 2 entries in the results
	ASSERT_EQ(results.size(), 2) << "Expected results size to be 2, got " << results.size() << " instead.";

	// Validate that the results are the entries we expected
	ASSERT_EQ(results[0].first_name, "Aaran");
	ASSERT_EQ(results[0].last_name, "Parks");
	ASSERT_EQ(results[0].phone_number, "");

	ASSERT_EQ(results[1].first_name, "Adriana");
	ASSERT_EQ(results[1].last_name, "Paul");
	ASSERT_EQ(results[1].phone_number, "(739) 391-4868");


	// Find a person whose name is, or starts with "x"
	results = ab.find("x");

	// There should be 0 entries in the results
	ASSERT_EQ(results.size(), 0);

	// Ensure that we are matching from the beginning of the name
	results = ab.find("ra");

	// There should be 0 entries in the results
	ASSERT_EQ(results.size(), 0);
}


/// Tests that we can pass in an empty string to get all entries
TEST(AddressBookTests, FindEmptyString)
{
	// Populate the address book
	AddressBook ab = AddTestPeople();

	// Find a person whose name is, or starts with ""
	std::vector<AddressBook::Entry> results = ab.find("");

	// There should be 6 entries in the results
	EXPECT_EQ(results.size(), 6);
}


// Tests that remove works
TEST(AddressBookTests, DeleteEntry) {

	AddressBook ab = AddTestPeople();

	// Find one random test person
	std::vector<AddressBook::Entry> results = ab.find("Jayden");

	// We should have found one person
	EXPECT_EQ(results.size(), 1);

	// Delete the person
	ab.remove(results[0]);

	// Find the person again
	results = ab.find("Jayden");

	// We should not have found the person
	EXPECT_EQ(results.size(), 0);
}


// Tests that if we remove and then sort, we get the correct sorted list
// added this because I had a bug where I was not updating the maps correctly
TEST(AddressBookTests, DeleteEntryAndSort) {

	AddressBook ab = AddTestPeople();

	// Find one random test person
	std::vector<AddressBook::Entry> results = ab.find("Jayden");

	// We should have found one person
	EXPECT_EQ(results.size(), 1) << "Failed to add entry before testing remove with entry: " << results[0];

	// Delete the person
	ab.remove(results[0]);

	// Sort the address book
	ab.sortedByFirstName();

	// Find the person again
	results = ab.find("Jayden");

	// We should not have found the person
	EXPECT_EQ(results.size(), 0) << "Entry still exists " << results[0];

	// The correctly sorted test data 
	const std::string people_sortedFirstNames2[][3] = {
		{"Aaran", "Parks", ""},
		{"Adriana", "Paul", "(739) 391-4868"},
		{"Hamza", "Bo", "+44 131 496 0571"},
		{"Phoenix", "Bond", "0161 496 0311"},
		{"Sally", "Graham", "+44 7700 900297"},
	};

	// Sort by first names
	results = ab.sortedByFirstName();

	// Check that the entries are still sorted correctly
	for (size_t i = 0; i < results.size(); i++)
	{
		auto personResult = results[i];
		auto personAnswer = people_sortedFirstNames2[i];

		EXPECT_EQ(personResult.first_name, personAnswer[0]);
		EXPECT_EQ(personResult.last_name, personAnswer[1]);
		EXPECT_EQ(personResult.phone_number, personAnswer[2]);
	}

}


// Tests that if we remove a non existant entry, we get an exception
TEST(AddressBookTests, DeleteNonExistantEntry) {

	AddressBook ab = AddTestPeople();

	// Create a non existant entry
	AddressBook::Entry entry = { "Non", "Existant", "000000000" };

	// Try to remove the entry
	EXPECT_THROW(ab.remove(entry), std::invalid_argument) << "Expected invalid argument exception with non existant entry " << entry;
}


// Test that the copy constructor works
TEST(AddressBookTests, CopyConstructor) {

	AddressBook ab = AddTestPeople();

	// Create a copy of the address book
	AddressBook ab_copy = AddressBook(ab);

	// Sort both address books by first name
	std::vector<AddressBook::Entry> results = ab.sortedByFirstName();
	std::vector<AddressBook::Entry> results_copy = ab_copy.sortedByFirstName();

	// Both address books should have the same number of entries
	ASSERT_EQ(results.size(), results_copy.size()) << "Expected sizes to be equal instead got: " << results.size() << " and " << results_copy.size();

	// Both address books should have the same entries
	for (size_t i = 0; i < results.size(); i++) {
		ASSERT_EQ(results[i], results_copy[i]);
	}
}


// Test that the + operator works
TEST(AddressBookTests, PlusOperator) {

	AddressBook ab = AddTestPeople();

	// Create a new address book
	AddressBook ab_new;

	// Add new people to the new address book
	AddressBook::Entry entry = { "Bandit", "Heeler", "832843234"};
	AddressBook::Entry entry2 = { "Radley", "Heeler", "953597223"};
	ab_new.add(entry);
	ab_new.add(entry2);

	// Add the new address book to the old address book
	ab = ab + ab_new;

	// Find the person we just added
	std::vector<AddressBook::Entry> results = ab.find("Bandit");

	// We should have found one person
	ASSERT_EQ(results.size(), 1) << "Failed to find entry " << entry << " (result size: " << results.size() << ")";

	// Validate that the person we found is the person we added
	ASSERT_EQ(results[0].first_name, "Bandit");
	ASSERT_EQ(results[0].last_name, "Heeler");
	ASSERT_EQ(results[0].phone_number, "832843234");

	// Find the other person we just added
	results = ab.find("Radley");

	// We should have found one person
	ASSERT_EQ(results.size(), 1);

	// Validate that the person we found is the person we added
	ASSERT_EQ(results[0].first_name, "Radley");
	ASSERT_EQ(results[0].last_name, "Heeler");
	ASSERT_EQ(results[0].phone_number, "953597223");
}	


// Test that the double sided + operator works
TEST(AddressBookTests, PlusOperatorDoubleSided) {
	AddressBook ab = AddTestPeople();
	AddressBook ab_new;
	AddressBook ab_empty;

	// Add new people to the new address book
	AddressBook::Entry entry = { "Bandit", "Heeler", "832843234" };
	AddressBook::Entry entry2 = { "Radley", "Heeler", "953597223" };
	ab_new.add(entry);
	ab_new.add(entry2);

	// Add the new address book to the old address book
	ab_empty = ab + ab_new;

	// Find the person we just added
	std::vector<AddressBook::Entry> results = ab_empty.find("Bandit");

	// We should have found one person
	ASSERT_EQ(results.size(), 1);

	// Validate that the person we found is the person we added
	ASSERT_EQ(results[0].first_name, "Bandit");
	ASSERT_EQ(results[0].last_name, "Heeler");
	ASSERT_EQ(results[0].phone_number, "832843234");

	// Find the other person we just added
	results = ab_empty.find("Radley");

	// We should have found one person
	ASSERT_EQ(results.size(), 1);

	// Validate that the person we found is the person we added
	ASSERT_EQ(results[0].first_name, "Radley");
	ASSERT_EQ(results[0].last_name, "Heeler");
	ASSERT_EQ(results[0].phone_number, "953597223");

	// Check that people got added from the ab address book
	for (auto person : people) {
		results = ab_empty.find(person[0]);
		ASSERT_EQ(results.size(), 1);
		ASSERT_EQ(results[0].first_name, person[0]);
		ASSERT_EQ(results[0].last_name, person[1]);
		ASSERT_EQ(results[0].phone_number, person[2]);
	}
}


// Test that the - operator works
TEST(AddressBookTests, MinusOperator) {

	AddressBook ab = AddTestPeople();

	// Create a new address book
	AddressBook ab_new;

	// Add new people from the people array to the new address book
	AddressBook::Entry new_entry1 = { "Adriana", "Paul", "(739) 391-4868" };
	AddressBook::Entry new_entry2 = { "Jayden", "Riddle", "+44 131 496 0609" };
	AddressBook::Entry non_existant_entry = { "Non", "Existant", "000000000" };

	ab_new.add(new_entry1);
	ab_new.add(new_entry2);
	ab_new.add(non_existant_entry);

	// Remove the new address book from the old address book
	ab = ab - ab_new;

	// Check the size of the address book (should be 4 because we removed 2 entries)
	std::vector<AddressBook::Entry> results = ab.sortedByFirstName();
	ASSERT_EQ(results.size(), 4);

	// Check that the entries we removed are not in the address book
	results = ab.find("Adriana");
	ASSERT_EQ(results.size(), 0);

	results = ab.find("Jayden");
	ASSERT_EQ(results.size(), 0);
}


// Test that the double sided - operator works
TEST(AddressBookTests, MinusOperatorDoubleSided) {
	AddressBook ab = AddTestPeople();
	AddressBook ab_new;
	AddressBook ab_empty;

	// Add new people from the people array to the new address book
	AddressBook::Entry new_entry1 = { "Adriana", "Paul", "(739) 391-4868" };
	AddressBook::Entry new_entry2 = { "Jayden", "Riddle", "+44 131 496 0609" };
	AddressBook::Entry non_existant_entry = { "Non", "Existant", "000000000" };

	ab_new.add(new_entry1);
	ab_new.add(new_entry2);
	ab_new.add(non_existant_entry);

	// Remove the new address book from the old address book
	ab_empty = ab - ab_new;

	// Check the size of the address book (should be 4 because we removed 2 entries)
	std::vector<AddressBook::Entry> results = ab_empty.sortedByFirstName();
	EXPECT_EQ(results.size(), 4) << "Expected 4 entries";

	// Check that the entries we removed are not in the address book
	results = ab_empty.find("Adriana");
	EXPECT_EQ(results.size(), 0) << "We still found adriana";

	results = ab_empty.find("Jayden");
	EXPECT_EQ(results.size(), 0);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
