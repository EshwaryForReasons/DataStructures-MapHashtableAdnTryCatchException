
#include "DiscussionList.h"
#include "Discussion.h"

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

static const string DISCUSSIONS_DATA = "discussions.dat";

InfoTypes DiscussionList::CurrentComparisionField = INFO_NONE;

static LinkedList<Discussion>* sorted_discussions = nullptr;

void DiscussionList::init_discussions()
{
	clear();
	hashmap.clear();

	ifstream in_file;
	in_file.exceptions(ifstream::badbit);

open_file:

	try
	{
		in_file.open(DISCUSSIONS_DATA);
	}
	catch (const ifstream::failure& error)
	{
		cout << "Failed to open " << DISCUSSIONS_DATA << " file!" << endl;
		cout << "Please make sure the file is placed in the correct directory and is given the correct name." << endl;
		cout << "Press any key to retry." << endl;

		string input;
		getline(cin, input);

		goto open_file;
	}

	Person person;
	string title;
	DateTime date_and_time;
	string content;

	//We use a rolling integer to keep track of which data we are collecting; 0 - name, 1 - title, 2 - date and time, 3 content
	int current_data_collected = 0;

	try
	{
	//Construct a Discussion and put it in the discussion_list for every discussion in the file
	string read_data;
	while (getline(in_file, read_data))
	{
		//Read next data type
		bool b_continue_to_next = true;
		switch (current_data_collected)
		{
		case 0:
		{
			stringstream name_stream(read_data);
			vector<string> name;
			string name_portion;
			while (getline(name_stream, name_portion, ' '))
				name.push_back(name_portion);
			//If vector has three things then we have a middle name
			person = Person(name[0], name.size() == 3 ? name[1] : "", name[name.size() == 3 ? 2 : 1]);
			break;
		}
		case 1:
		{
			title = read_data;
			break;
		}
		case 2:
		{
			date_and_time = DateTime(read_data);
			break;
		}
		case 3:
		{
			//If field contains \ then we continue reading the next line
			if (read_data.find("\\") != string::npos)
			{
				//Remove the slash
				read_data.erase(read_data.size() - 1);
				b_continue_to_next = false;
			}
			
			content += read_data;

			if (b_continue_to_next)
			{
				Discussion* discussion = new Discussion(person, title, date_and_time, content);
				insert(discussion);

				//After inserting to the list, we also want to hash it and store it in the hashmap
				hashmap.emplace(Discussion::generate_hash_code(discussion->get_person().get_first_name()), discussion);

				content = "";
			}

			break;
		}
		}

		if(b_continue_to_next)
			current_data_collected = ++current_data_collected % 4;
	}
	}
	catch (const ifstream::failure& error)
	{
		cout << "Failed to read data from " << DISCUSSIONS_DATA << " file!" << endl;
		cout << "Press any key to retry." << endl;

		string input;
		getline(cin, input);

		goto open_file;
	}

	in_file.close();
}

void DiscussionList::update_discussions_file()
{
	fstream file(DISCUSSIONS_DATA, ios::out);

	if (file.fail())
		cout << "Failed to open " << "discussions.dat" << " file!" << endl;

	for(const Node<Discussion>& node : *this)
		file << node.data->get_person().to_string() << "\n" << node.data->get_title() << "\n" << node.data->get_date_and_time().to_string() << "\n" << node.data->get_content() << endl;

	file.close();
}

void DiscussionList::print_list(InfoTypes _CurrentComparisionField, bool b_ascending)
{
	DiscussionList::CurrentComparisionField = _CurrentComparisionField;

	LinkedList<Discussion> sorted_discussions(*this);
	sorted_discussions.sort_list(b_ascending);

	//Print header
	cout << left;
	cout << setw(MEDIUM_COLUMN_WIDTH) << "First Name";
	cout << setw(SHORT_COLUMN_WIDTH) << "M";
	cout << setw(MEDIUM_COLUMN_WIDTH) << "Last Name";
	cout << setw(LONG_COLUMN_WIDTH) << "Date and Time";
	cout << setw(SUPER_LONG_COLUMN_WIDTH) << "Title";
	cout << endl << endl;

	for(const Node<Discussion>& node : sorted_discussions)
		cout << node.data->to_column_string() << "\n";

	cout << endl;
}

bool DiscussionList::view_discussion_details(const string& first_name)
{
	if(const Discussion* discussion = find_discussion(first_name))
	{
		cout << discussion->to_labeled_string() << endl;
		return true;
	}

	return false;
}

void DiscussionList::add_discussion(Discussion* new_discussion)
{
	insert(new_discussion);
	hashmap.emplace(Discussion::generate_hash_code(new_discussion->get_person().get_first_name()), new_discussion);
	update_discussions_file();
}

void DiscussionList::edit_discussion(const string& first_name, const Discussion& edited_discussion)
{
	Discussion* discussion = find_discussion(first_name);
	*discussion = edited_discussion;

	//Since we use first name for hash, if the first name was changed then we need to update the hash table
	if (discussion->get_person().get_first_name() != first_name)
	{
		hashmap.erase(Discussion::generate_hash_code(first_name));
		hashmap.emplace(Discussion::generate_hash_code(discussion->get_person().get_first_name()), discussion);
	}

	update_discussions_file();
}

bool DiscussionList::remove_discussion(const string& first_name)
{
	if (Discussion* discussion = find_discussion(first_name))
	{
		if (remove(discussion))
		{
			hashmap.erase(Discussion::generate_hash_code(first_name));
			update_discussions_file();
			return true;
		}
	}

	return false;
}

Discussion* DiscussionList::find_discussion(const string& first_name)
{
	//We can simply use the hashmap to find the discussion
	if(hashmap.find(Discussion::generate_hash_code(first_name)) != hashmap.end())
		return hashmap.at(Discussion::generate_hash_code(first_name));

	return nullptr;
}