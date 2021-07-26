#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "tinyxml/tinyxml2.h"
#include "parser.h"

using namespace tinyxml2;
using namespace std;
using std::vector;

vector<Vertex*> parseFile(string fname){
    string line;
    ifstream file(fname);
    vector<Vertex*> vertices;

    if (file.fail()) {
        throw std::ios_base::failure(string("Couldn't find file: ") + fname);
    }

    else {
        while (getline(file, line)) {
            size_t pos;

            float x,y,z;

            x = std::stof(line,&pos);
            line.erase(0,pos+1);
            y = std::stof(line,&pos);
            line.erase(0,pos+1);
            z = std::stof(line,&pos);

            Vertex *v = new Vertex(x,y,z);
            vertices.push_back(v);
        }
    }

    file.close();
    return vertices;
}

Group* parseGroup(XMLElement* element, std::vector<Translate*>* orbits){
	Group* g = new Group();
	
	for(element = element->FirstChildElement(); element; element = element->NextSiblingElement()){
		if(strcmp(element->Value(), "translate")==0){
            float x=0, y=0, z=0, time=0;

            char* name = (char*)element->Value();

            char* translateTime = (char*)element->Attribute("time");
            if(translateTime != NULL) time = atof(translateTime);

            Translate* t = new Translate(strdup(name),x,y,z,time);

            XMLElement* translatePoints = element->FirstChildElement();

            for(; translatePoints; translatePoints = translatePoints->NextSiblingElement()){
    			float px=0, py=0, pz=0;

                char* child_name = (char*)translatePoints->Value();


    			char* translateX = (char*)translatePoints->Attribute("X");
                if (translateX != NULL) px = atof(translateX);

                char* translateY = (char*)translatePoints->Attribute("Y");
                if (translateY != NULL) py = atof(translateY);

                char* translateZ = (char*)translatePoints->Attribute("Z");
                if (translateZ != NULL) pz = atof(translateZ);

                Vertex* p =  new Vertex(px,py,pz);

                t->addPoint(p);
            }

            if(t->getCurveSize() != 0){
                t->generateCurve(); 
                orbits->push_back(t);
            }

            g->pushOperation(t);
		}

		if(strcmp(element->Value(), "rotate")==0){
			char* name = (char*)element->Value();

			float x=0, y=0, z=0, angle=0, time=0;

			char* rotateAngle = (char*)element->Attribute("angle");
            if(rotateAngle != NULL) angle = atof(rotateAngle);

			char* rotateX = (char*)element->Attribute("axisX");
            if (rotateX != NULL) x = atof(rotateX);

            char* rotateY = (char*)element->Attribute("axisY");
            if (rotateY != NULL) y = atof(rotateY);

            char* rotateZ = (char*)element->Attribute("axisZ");
            if (rotateZ != NULL) z = atof(rotateZ);

            char* rotateTime = (char*)element->Attribute("time");
            if(rotateTime != NULL) time = atof(rotateTime);

            Rotate* r = new Rotate(strdup(name),x,y,z,angle,time);

            g->pushOperation(r);
		}

		if(strcmp(element->Value(), "scale")==0){
			char* name = (char*)element->Value();

			float x=0, y=0, z=0;

			char* scaleX = (char*)element->Attribute("X");
			if(scaleX != NULL) x = atof(scaleX);

			char* scaleY = (char*)element->Attribute("Y");
			if(scaleY != NULL) y = atof(scaleY);

			char* scaleZ = (char*)element->Attribute("Z");
			if(scaleZ != NULL) z = atof(scaleZ);

			Oper* op = new Oper(strdup(name),x,y,z);
            
			g->pushOperation(op);
		}

		if(strcmp(element->Value(), "models")==0){
			XMLElement *elementChild;
			for(elementChild = element->FirstChildElement(); elementChild; elementChild = elementChild->NextSiblingElement()){
				string ficheiro = elementChild->Attribute("file");
				Model* m = new Model(parseFile(ficheiro));

            	g->pushModel(m);
			}
		}

		if(strcmp(element->Value(), "group")==0){
			Group* child = parseGroup(element,orbits);
			g->pushChild(child);
		}
	}
	return g;
}

Group* readXML(string f_path, std::vector<Translate*>* orbits){
    XMLDocument xmlDoc;
    XMLElement *element;
    Group* g = new Group;
    Group* scene = new Group;

    auto result = (xmlDoc.LoadFile(f_path.c_str()));
    if (!result){
        element = xmlDoc.FirstChildElement("scene");
        for (element = element->FirstChildElement(); element; element = element->NextSiblingElement()){
        	g = parseGroup(element,orbits);
        	scene->pushChild(g);
        }
    }
    else {
        printf("XML File %s could not be found, result: %d",f_path.c_str(), result);
    }

    return scene;
}