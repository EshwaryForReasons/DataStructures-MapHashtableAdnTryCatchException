/******************************
*
* Program Name: Map Hashtable and Try Catch Exception
* Author: Eshwary Mishra
* Date: 18 November 2023
* Description: This program manages discussions provided by a file. It can update the file as discussion information changes.
* Changes From Lab 4: Try catch exceptions have been added around opening files and converting strings to integers. They allow
* the user to retry upon failure. Additionally, a map<uint265_t, Discussion*> has been added to make lookups quicker. The linked
* list is still used for navigation. A custom uint256_t has been added to handle the massive hash codes generated from the hashing
* function. First names from the data are used to generate a hash code.
*
*******************************/

#include "DiscussionMenu.h"

int main()
{
	DiscussionMenu menu = DiscussionMenu();
	menu.begin();
	return 0;
}