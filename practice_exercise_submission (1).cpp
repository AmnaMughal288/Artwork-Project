#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

// Group 3 - Amna Mughal and Juliet Kern

//STEP ONE - CREATE ARTWORK CLASS
// PURPOSE: Represnets an instance of Artwork or one piece of Art.
class Artwork
{
	string artist_name, title;
	unsigned int year_made; // primitive data, so need to initialize.
public:
	// empty/default constructor
	Artwork() : year_made(0) {}

	// parametric constructor using implicit setters
	Artwork(string new_artist, string new_title, unsigned int new_year) : // unsigned = only positive ints
																		  artist_name(new_artist), title(new_title), year_made(new_year)
	{
	}
	// overloaded operator ==
	bool operator==(const Artwork &rhs) const
	{ //ensures that the ebject is not modified
		bool are_equal = true;
		are_equal = are_equal && title == rhs.title;
		are_equal = are_equal && year_made == rhs.year_made;
		return are_equal;
	}
};
//STEP TWO -CREATE CHILD CLASS OF ARTWORK - SOLDARTWORK
// PURPOSE: Represents a solf piece of Artwork
class SoldArtwork : public Artwork
{
	string cust_name, cust_addr;
	double sale_amount;

public:
	// empty/default constructor
	SoldArtwork() : sale_amount(0) {}

	// parametric constructor
	SoldArtwork(string new_cust_name, string new_cust_addr, double new_sale_amount,
				Artwork new_artwork) : cust_name(new_cust_name), cust_addr(new_cust_addr),
									   sale_amount(new_sale_amount), Artwork(new_artwork) {} // calling Artwork consrtuctor

	// overload operator ==
	bool operator==(const SoldArtwork rhs) const
	{
		bool are_equal = cust_name == rhs.cust_name;
		are_equal = are_equal && cust_addr == rhs.cust_addr;
		are_equal = are_equal && sale_amount == rhs.sale_amount;
		are_equal = are_equal && static_cast<Artwork>(rhs) == static_cast<Artwork>(*this); //function
		return are_equal;
	}
};

// STEP THREE -
// PURPOSE: Represents models artwork held by an art gallery
class ArtCollection
{
	vector<Artwork> my_artwork;			 //private attribute; a collection of Artwork instances
	vector<SoldArtwork> my_sold_artwork; // private attribute; a collection of Sold Artwork instances
public:
	// method signature; specifies name a parametry only
	// PURPOSE: Inserts an artowkr instance into my_Artwork vector; duplicates are rejected
	bool insert_artwork(const Artwork &artwork_info)
	{ // method signature; specifies name and parametry only
		// use a boolean to keep track of suplicate detection
		bool has_duplicate = false;

		// iterate through my_artwork to check for duplicate values
		for (int entry = 0; entry < my_artwork.size() && !has_duplicate; ++entry)
		{
			if (my_artwork[entry] == artwork_info)
			{
				has_duplicate = true;
			}
		}

		// insert the given artwork into the Artwork vector
		if (has_duplicate == false)
		{ // if (!has_duplicate)
			my_artwork.push_back(artwork_info);
		}

		//return true if insertion worked; else, return false
		return !has_duplicate;
	}

	// PURPOSE: Facilitates a sale of artwork and placement of soldartwork instance into the
	// corresponding vector
	bool sell_artwork(const SoldArtwork &artwork_info)
	{
		// use a boolean to keep track of successful sale
		bool have_sold = false;
		// iterate through my_Artowrk to find the artwork that matches artwork_info
		for (int entry = 0; entry < my_artwork.size() && !have_sold; ++entry)
		{
			// use operator== to compare vector element against the element that was passed in
			if (my_artwork[entry] == static_cast<Artwork>(artwork_info))
			{ // finding the artwork

				// swap the current entry with the last entry at my_artwork.size() - 1
				swap(my_artwork[entry], my_artwork[my_artwork.size() - 1]); // swap wanted art with last one in array (so u can popback)
																			// remove the last entry
				my_artwork.pop_back();
				// insert sold_artwork into the my_sold_artwork vector
				my_sold_artwork.push_back(artwork_info);
				// indicate
				have_sold = true;
			}
		}
		// reuturn true if the sale was successful
		return have_sold;
	}

	// STEP 4
	bool operator==(const ArtCollection &rhs) const
	{
		bool are_equal = true;

		if (my_artwork.size() != rhs.my_artwork.size() || my_sold_artwork.size() != rhs.my_sold_artwork.size())
			are_equal = false;

		//sort(my_artwork.begin(), my_artwork.end());
		//sort(rhs.my_artwork.begin(), rhs.my_artwork.end());
		for (unsigned int index = 0; index < my_artwork.size() && are_equal; ++index)
		{
			if (!(my_artwork[index] == rhs.my_artwork[index]))
				are_equal = false;
		}

		for (unsigned int index = 0; index < my_artwork.size() && are_equal; ++index)
		{
			if (!(my_sold_artwork[index] == rhs.my_sold_artwork[index]))
				are_equal = false;
		}

		return are_equal;
	}
	// external function which will have direct access to private members of ArtCollection

	friend ArtCollection operator+(const ArtCollection &lhs, const ArtCollection &rhs);
	friend class ArtCollectionTest;
};

// PURPOSE: Combines the two collections into one and returns a new ArtCollection instance with the Artwork and SoldArtwork included
ArtCollection operator+(const ArtCollection &lhs, const ArtCollection &rhs)
{
	ArtCollection target(lhs); //copy constructor
	for (unsigned int index = 0; index < rhs.my_artwork.size(); ++index)
	{
		target.my_artwork.push_back(rhs.my_artwork[index]);
	}

	for (unsigned int index = 0; index < rhs.my_sold_artwork.size(); ++index)
	{
		target.my_sold_artwork.push_back(rhs.my_sold_artwork[index]);
	}
	return target;
}

// STEP 5 - Test (Driver) Program
class ArtCollectionTest
{
	vector<Artwork> test_values;
	vector<SoldArtwork> test_soldArtwork;

public:
	void setup()
	{
		Artwork a1("Banksy", "Girl with balloon", 2010), a2("Amna", "The artist", 2020), a3("painting", "artist", 2000);
		SoldArtwork s1("juliet", "Cmh adress", 100.00, a1), s2("kern", "uwp address", 500.00, a2);

		test_values.push_back(a1);
		test_values.push_back(a2);

		test_soldArtwork.push_back(s1);
		test_soldArtwork.push_back(s2);
	}
	void test_insert_artwork()
	{
		ArtCollection test_collection;

		cout << "----- Test for insert_artwork ----- " << endl;

		//encode a test into a ternary operator to provde immediate feedback
		cout << (test_collection.insert_artwork(test_values[0]) ? "(TEST 1) test_collection.insert_artwork(a1) passed" : " (TEST 1) test_collection.insert_artwork(a1) failed") << endl;

		// checks when you insert first artpiece if my_artwork size is one
		assert(test_collection.my_artwork.size() == 1);
		cout << "(TEST 2) for my_artwork.size() passed" << endl;

		// checks if able to insert a second artpiece
		assert(test_collection.insert_artwork(test_values[1]));
		cout << "(TEST 3) for test_collection.insert_artwork(a2) passed" << endl;

		// checks if the size of my_artwork is two when both artpieces are inserted
		assert(test_collection.my_artwork.size() == 2);
		cout << "(TEST 4) for my_artwork.size() passed" << endl;

		// checks to make sure user is unable to insert artpiece that is not in vector
		assert(!(test_collection.insert_artwork(test_values[2])));
		cout << "(TEST 5) for test_collection.insert_artwork(a2) passed" << endl;

		// checks to make sure you can not insert duplicate
		assert(!(test_collection.insert_artwork(test_values[1])));
		cout << "(TEST 6)for test_collection.insert_artwork(a2) passed" << endl;

		// checks to verify my_artwork size is still two
		assert(test_collection.my_artwork.size() == 2);
		cout << "(TEST 7) for my_artwork.size() passed " << endl;
	}

	void test_sell_artwork()
	{
		ArtCollection test_collection;

		test_collection.insert_artwork(test_soldArtwork[0]);
		test_collection.insert_artwork(test_soldArtwork[1]);

		cout << "----- Test for sold_artwork ----- " << endl;
		//encode a test into a ternary operator to provde immediate feedback
		cout
			<< (test_collection.sell_artwork(test_soldArtwork[0]) ? "(TEST 1) test_collection.sell_artwork(s1) passed" : "(TEST 1) test_collection.sell_artwork failed(s1)") << endl;

		// checks to verify the size of my_artwork has gone up one (to equal 1)
		// checks to verify the size of my_sold_artwork has gone down one (to equal 1)
		assert(test_collection.my_sold_artwork.size() == 1 && test_collection.my_artwork.size() == 1);
		cout << "(TEST 2) for my_sold_artwork.size() passed" << endl;

		// checks to verify if able to sell artpiece
		assert(test_collection.sell_artwork(test_soldArtwork[1]));
		cout << "(TEST 3) for test_collection.sell_artwork(s2) passed" << endl;

		// checks to verify the size of my_artwork has gone up one (to equal 2)
		// checks to verify the size of my_sold_artwork has gone down one (to equal 0)
		assert(test_collection.my_sold_artwork.size() == 2 && test_collection.my_artwork.size() == 0);
		cout << "(TEST 4) for my_sold_artwork.size() passed" << endl;

		// checks to verify user is not able to re-sell the same artpiece
		assert(!(test_collection.sell_artwork(test_soldArtwork[1])));
		cout << "(TEST 5) for test_collection.sell_artwork(a2) passed" << endl;
	}

	void run()
	{
		setup();
		test_insert_artwork();
		test_sell_artwork();
	}
};

int main()
{
	ArtCollectionTest test;
	test.run();
}