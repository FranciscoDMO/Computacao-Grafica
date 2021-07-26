#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
#include <string>
#include "vertex.h"
#include "group.h"

using namespace tinyxml2;

void parseGroup(XMLElement* element, Group* g, std::vector<Translate*>* orbits);
Group* readXML(string f_path, std::vector<Translate*>* orbits);
Model* parseFile(string fname, Model* m);

#endif