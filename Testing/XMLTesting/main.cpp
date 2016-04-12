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
    //VARIABLES
    
    //the raw vector data from XML
    std::map<int, std::vector<float>> data;
    
    //which dimensions to include in calculations
    std::vector<int> activeDimensions;
    
    //each point id mapped to it's cluster
    std::map<int, int> clusterAssignments;
    
    //vector of each cluster's size
    std::map<int, int> clusterSizes;
    
    //how many clusters to use
    int numberOfClusters;
    
    //vectors of each cluster centroid
    std::vector<std::vector<float>> centroids;
    
    //SETUP
    //constructor
    Clusters(std::map<int, std::vector<float>> Data, int NumberOfClusters, std::vector<int> activeDimensions);
    
    //initialize cluster centroids
    void setupClusters();
    
    //ALGORITHM
    //complete one iteration of assignment
    void iterateAssignmentStep();
    
    //RESULTS
    void printClusterAssignments();
    
private:
    //HELPER FUNCTIONS
    //calculate distance between a point and centroid
    float distance(std::vector<float> point, std::vector<float> centroid);
    
};

Clusters::Clusters(std::map<int, std::vector<float>> Data, int NumberOfClusters, std::vector<int> ActiveDimensions) {
    data = Data;
    numberOfClusters = NumberOfClusters;
    activeDimensions = ActiveDimensions;
}

float Clusters::distance(std::vector<float> point, std::vector<float> centroid) {
    float distance = 0;
    for (int i = 0; i < point.size(); i++) {
        if (activeDimensions[i]) {
            distance = distance + pow(point[i]-centroid[i], 2); //add squared difference from point to centroid
        }
    }
    return sqrt(distance);
}

void Clusters::setupClusters() {
    //assign inital clusters based on first n data points [!! WILL CHANGE THIS]
    
    for (int i=0; i<numberOfClusters; i++) {
        centroids.push_back(data[i]);
    }
}

void Clusters::iterateAssignmentStep() {
    //1: for each point calculate distance from centroid, determine which distance is smallest, assign to cluster with smaller distance
    int closestCluster;
    float minDistance;
    float newDistance;
    //for each point
    for (int i=0; i<data.size(); i++) {
        closestCluster = 0;
        minDistance = 1000000000;
        //for each cluster
        for (int j=0; j<centroids.size(); j++) {
            
            std::cout << "data[i] (x,y): " << data[i][0] << ", " << data[i][1] << std::endl;
            std::cout << "centroids[j] (x,y): " << centroids[j][0] << ", " << centroids[j][1] << std::endl;
            
            newDistance = distance(data[i], centroids[j]);
            
            std::cout << "newDistance: " << newDistance << std::endl;
            std::cout << "minDistance: " << minDistance << std::endl;
            
            if (newDistance < minDistance) {
                std::cout << "Reassigning to newDistance" << std::endl;
                closestCluster = j;
                minDistance = newDistance;
            }

            std::cout << std::endl << "---------------------" << std::endl << std::endl;
        }
        
        clusterAssignments[i] = closestCluster;
        
    }//assigned cluster to each point
    
    //2: calculate new centroid for each cluster
    //for each point add dimension values to centroid
    
    //first clear centroids
    for (int i=0; i<clusterSizes.size(); i++) {
        clusterSizes[i] = 0;
    }
    for (int i=0; i<centroids.size(); i++) {
        for (int j=0; j<centroids[0].size(); j++) {
            centroids[i][j] = 0;
        }
    }

    
    //add each point to corresponding centroid
    for (int i=0; i<data.size(); i++) {
        clusterSizes[clusterAssignments[i]] = clusterSizes[clusterAssignments[i]] + 1;
        
        //for each dimension
        for (int j=0; j<data[i].size(); j++){
            
            if (activeDimensions[j]) {
                centroids[clusterAssignments[i]][j] = centroids[clusterAssignments[i]][j] + data[i][j];
            }
        }
    }
    for (int i=0; i<centroids.size(); i++) {
        for (int j=0; j<centroids[0].size(); j++) {
            if (clusterSizes[i] > 0) {
                centroids[i][j] = centroids[i][j] / clusterSizes[i];
            }
            else {
                centroids[i][j] = 0;
            }
        }
    }
}

void Clusters::printClusterAssignments() {
    typedef std::map<int, int>::iterator it_type;
    
    for(it_type iterator = clusterAssignments.begin(); iterator != clusterAssignments.end(); iterator++) {
        std::cout << iterator->first << ": ";
        std::cout << iterator->second << std::endl;
    }
}


// ************ MAIN ************* //
int main() {
    std::map<int, std::vector<float>> vectorsMap;
    int numberOfAttributes = 3;
    std::string fileName = "testFile.xml";
    std::vector<int> activeDimensions;
    
    activeDimensions.push_back(0);
    activeDimensions.push_back(1);
    activeDimensions.push_back(0);
    
    loadVectors(&vectorsMap, fileName, numberOfAttributes);
//    printVectors(&vectorsMap, numberOfAttributes);
    populateActiveDimensions(&activeDimensions, numberOfAttributes);
    
    Clusters testCase = Clusters(/*data*/ vectorsMap, /*number of clusters*/ 2, /*which dims to use */ activeDimensions);
    
    testCase.setupClusters();
    testCase.iterateAssignmentStep();
    testCase.printClusterAssignments();
    testCase.iterateAssignmentStep();
    testCase.printClusterAssignments();
    
    return 0;
}



