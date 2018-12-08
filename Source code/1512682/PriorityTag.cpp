#include "stdafx.h"
#include "PriorityTag.h"


void PriorityTag::addTag(Note * note)
{
	auto tagList = note->getTagList();
	bool isExistTag;
	int i;
	for (auto iTag : tagList)
	{
		i = 0;
		isExistTag = false;
		for (auto jTag : _tagList)
		{
			if (jTag->name == iTag)
			{
				isExistTag = true;
				jTag->onAddTag(note);

				if (_tagList[0]->getCount() <= jTag->getCount())
				{
					if (_tagList[0] == jTag)
					{
						break;
					}
					_tagList.erase(_tagList.begin() + i);
					_tagList.insert(_tagList.begin(), jTag);
					break;
				}
				else
				{
					_tagList.erase(_tagList.begin() + i);
					bool isBreak = false;
					for (int j = i - 1; j >= 0; j--)
					{
						if (_tagList[j]->getCount() >= jTag->getCount() + 1)
						{
							_tagList.insert(_tagList.begin() + j + 1, jTag);
							isBreak = true;
							break;
						}		
					}
					if (isBreak)
					{
						break;
					}
				}
		
			}
			i++;
		}
		if (!isExistTag)
		{
			auto tag = new Tag();
			tag->name = iTag;
			tag->onAddTag(note);
			if (_tagList.size() > 0)
			{
				if (_tagList[0]->getCount() == 1)
				{
					_tagList.insert(_tagList.begin(), tag);
				}
				else
				{
					for (int j = _tagList.size() - 1; j >= 0; j--)
					{
						if (_tagList[j]->getCount() >= 2)
						{
							_tagList.insert(_tagList.begin() + j + 1, tag);
							break;
						}
					}
				}
			}
			else
			{
				_tagList.push_back(tag);
			}
		}
	}
}

vector<Tag*>& PriorityTag::getTagList()
{
	return _tagList;
}

PriorityTag::PriorityTag()
{
}


PriorityTag::~PriorityTag()
{
}
