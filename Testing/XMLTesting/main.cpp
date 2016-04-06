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
#include "math.h"

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
    
    std::vector<int> activeDimensions; //which dimensions (ie attributes) to include in calculations
    
    std::map<int, int> clusterAssignments; //each point by mapped id assigned to cluster
    int numberOfClusters; //how many clusters to use, should match centroids.size()
    std::vector<std::vector<float>> centroids; //vectors of each cluster centroid (list of vectors)
    
    Clusters(std::map<int, std::vector<float>> Data, int NumberOfClusters); //constructor
    float generateCentroid(std::map<int, std::vector<float>> data, std::vector<int> activeDimensions);
    float distance(std::vector<float> point, std::vector<float> centroid, std::vector<int> activeDimensions);
    void setupClusters(std::map<int, std::vector<float>> data, std::vector<std::vector<float>> centroids, int numberOfClusters);
    void iterateAssignmentStep(std::map<int, std::vector<float>> data, std::map<int, int> clusterAssignments, std::vector<std::vector<float>> centroids, std::vector<int> activeDimensions);
};

Clusters::Clusters(std::map<int, std::vector<float>> Data, int NumberOfClusters) {
    data = Data;
    numberOfClusters = NumberOfClusters;
}

float Clusters::generateCentroid(std::map<int, std::vector<float>> data, std::vector<int> activeDimensions) {
    return 2.3;
}

float Clusters::distance(std::vector<float> point, std::vector<float> centroid, std::vector<int> activeDimensions) {
    float distance = 0;
    for (int i = 0; i < point.size(); i++) {
        if (activeDimensions[i]) {
            distance = distance + pow(point[i]-centroid[i], 2); //add squared difference from point to centroid
        }
    }
    return sqrt(distance);
}

void Clusters::setupClusters(std::map<int, std::vector<float>> data, std::vector<std::vector<float>> centroids, int numberOfClusters) {
    //assign inital clusters based on first n data points [!! WILL CHANGE THIS]
    
    for (int i=0; i<numberOfClusters; i++) {
        centroids.push_back(data[i]);
    }
}

void Clusters::iterateAssignmentStep(std::map<int, std::vector<float>> data, std::map<int, int> clusterAssignments, std::vector<std::vector<float>> centroids, std::vector<int> activeDimensions) {
    //1: for each point calculate distance from centroid, determine which distance is smallest, assign to cluster with smaller distance
    int closestCluster;
    float minDistance = 1000000000000;
    float newDistance;
    //for each point
    for (int i=0; i<data.size(); i++) {
        closestCluster = 0;
        //for each cluster
        for (int j=0; j<centroids.size(); j++) {
            newDistance = distance(data[i], centroids[i], activeDimensions);
            if (newDistance < minDistance) {
                closestCluster = j;
                minDistance = newDistance;
            }
        }
    }//assigned cluster to each point
    std::vector<float> newCentroid;
    //2: calculate new centroid for each cluster
    //for each cluster
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



