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

// ***************** LOAD AND READ DATA ***************** //
void loadVectors(std::map<int, std::vector<float>>* vectorsMap, std::string fileName, int numberOfAttributes) {
    pugi::xml_document doc;
    doc.load_file(&fileName[0]);
    pugi::xml_node root = doc.first_child();
    
    int j = 0;
    pugi::xml_node attr;
    std::vector<float>* currentVector;
    
    for (pugi::xml_node title = root.first_child(); title; title = title.next_sibling()) {
        
        attr = title.first_child();
        currentVector = new std::vector<float>;
        
        for (int i=0; i< numberOfAttributes; i++) {
            
            currentVector->push_back(atoi(attr.first_child().value()));
            
            attr = attr.next_sibling();
        }
        
        (*vectorsMap)[j] = *currentVector;
        j = j+1;
    }
}

void printVectors(std::map<int, std::vector<float>>* vectorsMap, int numberOfAttributes) {
    typedef std::map<int, std::vector<float>>::iterator it_type;
    for(it_type iterator = vectorsMap->begin(); iterator != vectorsMap->end(); iterator++) {
        std::cout << iterator->first << ": ";
        for (int i=0; i<numberOfAttributes; i++) {
            std::cout << iterator->second[i] << ", ";
        }
        std::cout << std::endl;
    }
}

void populateActiveDimensions(std::vector<int>* activeDimensions, int numberOfAttributes) {
    (*activeDimensions).push_back(0);
    for (int i = 0; i < numberOfAttributes; i++) {
        (*activeDimensions).push_back(1);
    }
}

// ***************** CLUSTERING ***************** //
class Clusters {
public:
    std::map<int, std::vector<float>> data;
    
    int numberOfClusters; //how many clusters to use, should match centroids.size()
    std::vector<int> centroids; //vector containing id's of points to use as cluster centroid
    std::vector<int> means; //vector in same order as centroids containing their means
    
    Clusters(std::map<int, std::vector<float>> Data, int NumberOfClusters); //constructor
    float generateMean(std::vector<float> point, std::vector<int> activeDimensions);
    float sumOfSquares(std::vector<float> point, float mean, std::vector<int> activeDimensions);
};

Clusters::Clusters(std::map<int, std::vector<float>> Data, int NumberOfClusters) {
    data = Data;
    numberOfClusters = NumberOfClusters;
}

float Clusters::generateMean(std::vector<float> point, std::vector<int> activeDimensions) {
    float mean = 0;
    float n = 0;
    for (int i = 0; i < point.size(); i++) {
        if (activeDimensions[i]) {
            mean = mean + point[i];
            n = n + 1;
        }
    }
    mean = mean / n;
    return mean;
}

float Clusters::sumOfSquares(std::vector<float> point, float mean, std::vector<int> activeDimensions) {
    float sumOfSquares = 0;
    for (int i = 0; i < point.size(); i++) {
        if (activeDimensions[i]) {
            sumOfSquares = sumOfSquares + (sumOfSquares + (point[i] - mean)) * (sumOfSquares + (point[i] - mean));
        }
    }
    return sumOfSquares;
}


// ************ MAIN ************* //
int main() {
    std::map<int, std::vector<float>> vectorsMap;
    int numberOfAttributes = 8;
    std::string fileName = "actorsData.xml";
    std::vector<int> activeDimensions;
    
    loadVectors(&vectorsMap, fileName, numberOfAttributes);
    printVectors(&vectorsMap, numberOfAttributes);
    populateActiveDimensions(&activeDimensions, numberOfAttributes);
    
    return 0;
}



