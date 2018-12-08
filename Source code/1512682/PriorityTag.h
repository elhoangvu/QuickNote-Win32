#pragma once
#include "Tag.h"
#include <vector>

class PriorityTag
{
private:
	vector<Tag*> _tagList;
public:
	void addTag(Note* note);
	vector<Tag*>& getTagList();
	PriorityTag();
	~PriorityTag();
};

