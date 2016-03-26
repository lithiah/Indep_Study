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

//object to hold actor info in
class Actor {
public:
    std::string name;
    std::string gender;
    std::string race;
    int age;
    int firstYear;
    int yearRank[86] = {0}; //array of years where index in year starting 0 = 1924, and value is rank during that year, 0 if not ranked at all {issue with 0 being closer to 1 than 10}
    
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

int main() {
    std::vector<Actor*> actors; //vector to hold all actor objects
    
    //doc tree of XML & exit if it doesn't load
    pugi::xml_document doc;
    if (!doc.load_file("MovieStars1927to2013.xml")) return -1;
    pugi::xml_node root = doc.child("Root");
    
    //reuse vars for creating actor loop
    std::string name;
    std::string gender;
    std::string race;
    int age;
    int firstYear;
    
    for (pugi::xml_node row = root.first_child(); row; row = row.next_sibling()) {
        //create each actor
        for (pugi::xml_node star = row.first_child().first_child(); star; star = star.next_sibling()) {
            //pull values out from XML tree
            name = star.first_child().first_child().value();
            gender = star.first_child().next_sibling().first_child().value();
            race = star.first_child().next_sibling().next_sibling().first_child().value();
            age = atoi(star.first_child().next_sibling().next_sibling().next_sibling().first_child().value());
            firstYear = atoi(row.first_child().attribute("title").value());
            //push new actor to actors vector
            actors.push_back(new Actor(name, gender, race, age, firstYear));
        }
    }
    
    //print out all actors
    for (int i=0; i<actors.size(); i++) {
        std::cout << actors[i]->name << "...";
        std::cout << actors[i]->gender << "...";
        std::cout << actors[i]->race << "...";
        std::cout << actors[i]->age << "...";
        std::cout << actors[i]->firstYear << std::endl;
    }
    
    return 0;
}