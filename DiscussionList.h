
#pragma once

#include "LinkedList.h"
#include "Discussion.h"
#include "Utils.h"

#include <map>
#include <unordered_map>

using namespace std;

/**
	Class specifically designed to abstract LinkedList functionality behind easy to read and understand functions
*/
class DiscussionList : public LinkedList<Discussion>
{
public:

	DiscussionList() = default;

	static InfoTypes CurrentComparisionField;

	//Read discussions from file
	void init_discussions();

	//Update the file to match our list
	void update_discussions_file();

	//Prints list as first name, middle name, last name, date and time, title
	void print_list(InfoTypes _CurrentComparisionField, bool b_ascending);

	//Performs binary search for discussion by first and displays it
	bool view_discussion_details(const string& first_name);

	void add_discussion(Discussion* new_discussion);
	void edit_discussion(const string& first_name, const Discussion& edited_discussion);
	bool remove_discussion(const string& first_name);

	//Binary searchs for a discussion using first name; returns discussion
	Discussion* find_discussion(const string& first_name);

private:

	//This will serve as a hashmap for the linked list to make lookups quicker
	//The unique id will simply be assigned in order
	unordered_map<uint256_t, Discussion*> hashmap;
};