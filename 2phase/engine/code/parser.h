#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
#include <string>
#include "vertex.h"
#include "group.h"

using namespace tinyxml2;

void parseGroup(XMLElement* element, Group* g);
Group* readXML(string f_path);

#endif