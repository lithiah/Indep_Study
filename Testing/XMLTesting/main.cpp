//
//  main.cpp
//  XMLTesting
//
//  Created by Lithia Helmreich on 3/24/16.
//  Copyright © 2016 Lithia Helmreich. All rights reserved.
//

#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>

// *************** ACTOR XML STUFF ************** //
//object to hold actor info in
class Actor {
public:
    std::string name;
    std::string gender;
    std::string race;
    int age;
    int firstYear;
    int yearRank[85] = {0}; //array of years where index in year starting 0 = 1924, and value is rank during that year, 0 if not ranked at all {issue with 0 being closer to 1 than 10}
    
    Actor(std::string Name, std::string Gender, std::string Race, int Age, int firstYear); //constructor
};

//constructor
Actor::Actor(std::string Name, std::string Gender, std::string Race, int Age, int FirstYear)
{
    name = Name;
    gender = Gender;
    race = Race;
    age = Age;
    firstYear = FirstYear;
}

//print out each actor and corresponding key
void printActors(std::map<int, Actor*>* actorsMap) {
    typedef std::map<int, Actor*>::iterator it_type;
    for(it_type iterator = actorsMap->begin(); iterator != actorsMap->end(); iterator++) {
        std::cout << iterator->first << ": " << iterator->second->name << std::endl;
    }
}

//get actor details and key, map together
int mapActorsToKey(std::map<int, Actor*>* actorsMap) {
    //doc tree of actors XML & exit if it doesn't load
    pugi::xml_document doc;
    if (!doc.load_file("MovieStars1927to2013.xml")) return 0;
    pugi::xml_node root = doc.child("Root");
    //doc tree of keys
    pugi::xml_document doc2;
    if (!doc2.load_file("numatt.xml")) return 0;
    pugi::xpath_node keySearch; //xpath object to find key
    std::string keyQuery; //query for xpath containing actor name we want
    int keyVal; //value of key associated with that actor
    
    //reuse vars for creating actor loop
    std::string name;
    std::string gender;
    std::string race;
    int age;
    int firstYear;
    
    for (pugi::xml_node row = root.first_child(); row; row = row.next_sibling()) {
        //create each actor
        for (pugi::xml_node star = row.first_child().first_child(); star; star = star.next_sibling()) {
            //pull values out from doc1 XML tree
            name = star.first_child().first_child().value();
            
            gender = star.first_child().next_sibling().first_child().value();
            race = star.first_child().next_sibling().next_sibling().first_child().value();
            age = atoi(star.first_child().next_sibling().next_sibling().next_sibling().first_child().value());
            firstYear = atoi(row.first_child().attribute("title").value());
            //push new actor to actors vector
            
            //generate keySearch string
            keyQuery = "//TITLE[NAME='";
            keyQuery.append(name); //name of actor
            keyQuery.append("']");
    
            //get key for actor name and turn to int
            keySearch = doc2.select_node(&keyQuery[0]); //finds <NAME>actor</NAME> in doc2
            keyVal = atoi(keySearch.node().first_child().next_sibling().first_child().value()); //gets key
            
            actorsMap->insert(std::pair<int,Actor*>(keyVal, new Actor(name, gender, race, age, firstYear)));
        }
    }
    return 1;
}

//load years Actor is ranked into actor objects
int loadYears(std::map<int, Actor*>* actorsMap) {
    pugi::xml_document doc;
    if (!doc.load_file("masterkey.xml")) return 0;
    pugi::xml_node root = doc.child("Root");
    
    pugi::xml_node rank;
    int actorKey;
    int year = 0;
    
    for (pugi::xml_node title = root.first_child(); title; title = title.next_sibling()) {
        rank = title.first_child().next_sibling();
        
        for (int i = 1; i < 11; i++) {
            actorKey = atoi(rank.first_child().value());
            (*actorsMap)[actorKey]->yearRank[year] = i;
            
            rank = rank.next_sibling();
        }
        year = year + 1;
    }
    
    
    return 1;
}

void constructVectors(std::map<int, Actor*>* actorsMap, std::map<int, std::vector<int> >* vectorsMap) {
    typedef std::map<int, Actor*>::iterator it_type;
    
//    vars for looping
//    int actorKey;
    
    for(it_type iterator = actorsMap->begin(); iterator != actorsMap->end(); iterator++) {
        std::cout << iterator->first << ": " << iterator->second->name << std::endl;
        vectorsMap->insert(std::pair<int, std::vector<int>>(iterator->first, *new std::vector<int>));
        //add
        
        (*vectorsMap)[iterator->first].push_back(iterator->second->age);
        (*vectorsMap)[iterator->first].push_back(iterator->second->firstYear);
        
        for (int i=0; i<86; i++) {
            (*vectorsMap)[iterator->first].push_back(iterator->second->yearRank[i]);
        }

        
    }
}

// *************** END ACTOR XML STUFF ************** //

// *************** GENERALIZED XML LOADING ************** //
void loadVectors(std::map<int, std::vector<int>>* vectorsMap, int numberOfAttributes) {
    pugi::xml_document doc;
    doc.load_file("masterkey.xml");
    pugi::xml_node root = doc.child("Root");
    
    int j = 0;
    pugi::xml_node attr;
    std::vector<int>* currentVector;
    
    for (pugi::xml_node title = root.first_child(); title; title = title.next_sibling()) {
        
        attr = title.first_child();
        currentVector = new std::vector<int>;
        
        for (int i=0; i< numberOfAttributes; i++) {
            
            
            currentVector->push_back(atoi(attr.first_child().value()));
            
            attr = attr.next_sibling();
        }
        
        (*vectorsMap)[j] = *currentVector;
        j = j+1;
    }
}

void printVectors(std::map<int, std::vector<int>>* vectorsMap, int numberOfAttributes) {
    typedef std::map<int, std::vector<int>>::iterator it_type;
    for(it_type iterator = vectorsMap->begin(); iterator != vectorsMap->end(); iterator++) {
        std::cout << iterator->first << ": ";
        for (int i=0; i<numberOfAttributes; i++) {
            std::cout << iterator->second[i] << ", ";
        }
        std::cout << std::endl;
    }
}

int main() {
//    std::map <int, Actor*> actorsMap;
//    std::map <int, std::vector<int>> vectorsMap;
//    
//    //load up data from XML
//    if (!(mapActorsToKey(&actorsMap))) return -1; //load actors and keys into map, exit if XML loading errors
////    printActors(&actorsMap); //print out key:actor pairs
//    if (!(loadYears(&actorsMap))) return -1; //load year rankings into actor objects, exit if XML loading errors
//    
//    //generate vectors for clusters
//    constructVectors(&actorsMap, &vectorsMap);
////    printVectors(&vectorsMap);
//    
//    std::cout << (vectorsMap[4])[0];

    std::map<int, std::vector<int>> vectorsMap;
    int numberOfAttributes = 11;
    std::string name;
    
    loadVectors(&vectorsMap, numberOfAttributes);
    printVectors(&vectorsMap, numberOfAttributes);
    
    return 0;
}