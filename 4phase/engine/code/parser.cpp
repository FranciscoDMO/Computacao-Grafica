#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "tinyxml/tinyxml2.h"
#include "parser.h"

using namespace tinyxml2;
using namespace std;
using std::vector;

Model* parseFile(string m_file) {
    string line;
    vector<Vertex*> v_list, n_list, t_list;
    vector<float> i_list;
    Model* m = new Model();

    ifstream file(m_file);

    if (file.fail()) {
        cout << "Couldn't find file: " << m_file << "." << endl;
    }
    else {
        while (getline(file, line)) {

            if (line[0] == 'V') {
                line.erase(0, 2);
                Vertex* v = new Vertex(line);
                v_list.push_back(v);
                
            }

            if (line[0] == 'N') {
                line.erase(0, 2);
                Vertex* n = new Vertex(line);
                n_list.push_back(n);
               
            }

            if (line[0] == 'T') {
                line.erase(0, 2);
                Vertex* t = new Vertex(line);
                t_list.push_back(t);
                
            }

                if (line[0] == 'T') {
                    line.erase(0, 2);
                    Vertex* t = new Vertex(line);
                    t_list.push_back(t);
                    
                }

                if (line[0] == 'I') {
                    line.erase(0, 2);
                    float ind = std::stof(line);
                    i_list.push_back(ind);
                }
            }

            m->setUpVBO(v_list, n_list, t_list, i_list);
            file.close();

            return m;
        }
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
            string m_path, t_path;

			for(elementChild = element->FirstChildElement(); elementChild; elementChild = elementChild->NextSiblingElement()){
				string ficheiro = elementChild->Attribute("file");
                Model* m = new Model();

                m = parseFile(ficheiro);
                
                if(elementChild->Attribute("texture")){
                    t_path = elementChild->Attribute("texture");
                    m->setUpTexture(t_path);
                }


                float diff[4] = {0.8, 0.8, 0.8, 1};
                float spec[4] = {0, 0, 0, 1};
                float emi[4] = {0, 0, 0, 1};
                float amb[4] = {0.2, 0.2, 0.2, 1};
                float shini = 1.0f;

                char* diffR = (char*)element->Attribute("diffR");
                if(diffR != NULL) diff[0] = atof(diffR);

                char* diffG = (char*)element->Attribute("diffG");
                if(diffG != NULL) diff[1] = atof(diffG);

                char* diffB = (char*)element->Attribute("diffB");
                if(diffB != NULL) diff[2] = atof(diffB);

                char* specR = (char*)element->Attribute("specR");
                if(specR != NULL) spec[0] = atof(specR);

                char* specG = (char*)element->Attribute("specG");
                if(specG != NULL) spec[1] = atof(specG);

                char* specB = (char*)element->Attribute("specB");
                if(specB != NULL) spec[2] = atof(specB);

                char* emiR = (char*)element->Attribute("emiR");
                if(emiR != NULL) emi[0] = atof(emiR);

                char* emiG = (char*)element->Attribute("emiG");
                if(emiG != NULL) emi[1] = atof(emiG);

                char* emiB = (char*)element->Attribute("emiB");
                if(emiB != NULL) emi[2] = atof(emiB);

                char* ambR = (char*)element->Attribute("ambR");
                if(ambR != NULL) amb[0] = atof(ambR);

                char* ambG = (char*)element->Attribute("ambG");
                if(ambG != NULL) amb[1] = atof(ambG);

                char* ambB = (char*)element->Attribute("ambB");
                if(ambB != NULL) amb[2] = atof(ambB);

                char* sh = (char*)element->Attribute("shini");
                if(sh != NULL) shini = atof(sh);

                m->pushMaterials(diff,spec,emi,amb, shini);

            	g->pushModel(m);
			}
		}

        if(strcmp(element->Value(), "lights")==0){
            XMLElement *elementChild = element->FirstChildElement();
            
            float p[4];
            
            char* posX = (char*)elementChild->Attribute("posX");
            if(posX != NULL) p[0] = atof(posX);

            char* posY = (char*)elementChild->Attribute("posY");
            if(posY != NULL) p[1] = atof(posY);

            char* posZ = (char*)elementChild->Attribute("posZ");
            if(posZ != NULL) p[2] = atof(posZ);

            if(!strcmp(elementChild->Attribute("type"),"POINT")){
                p[3] = 1.0f;
            }
        
            
            Light* l = new Light(p);

            g->pushLight(l);

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