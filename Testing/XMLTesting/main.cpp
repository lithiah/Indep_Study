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
#include <pthread.h>

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

void populateActiveDimensions(std::vector<int>* activeDimensions, int numberOfAttributes, std::vector<int> userInputDimensions) {
    for(std::vector<int>::iterator it = userInputDimensions.begin(); it != userInputDimensions.end(); ++it) {
        (*activeDimensions).push_back(*it);
    }
}

// ***************** CLUSTERING ***************** //
class Clusters {
public:
    //SETUP
    //constructor
    Clusters(std::map<int, std::vector<float>> Data, int NumberOfClusters, std::vector<int> activeDimensions);
    
    //initialize cluster centroids
    void setupClusters();
    
    //ALGORITHM
    void runSequential(int maxIterations);
    void runParallel(int maxIterations);
    
    //RESULTS
    void printClusterAssignments();
    
private:
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
    
    //HELPER FUNCTIONS
    //calculate distance between a point and centroid
    float distance(std::vector<float> point, std::vector<float> centroid);
    //complete one sequential iteration of assignment
    void seqAssignmentStep();
    //complete one parallel iteration of assignment
    static void* parAssignmentStep1(void* arg);
    static void* parAssignmentStep2(void* arg);
    //calculate centroids after assignments
    void calculateCentroids();
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
    //assign inital clusters based on first n data points [!! WILL CHANGE THIS??]
    
    for (int i=0; i<numberOfClusters; i++) {
        centroids.push_back(data[i]);
    }
}

void Clusters::printClusterAssignments() {
    typedef std::map<int, int>::iterator it_type;
    
    for(it_type iterator = clusterAssignments.begin(); iterator != clusterAssignments.end(); iterator++) {
        std::cout << iterator->first << ": ";
        std::cout << iterator->second << std::endl;
    }
}

void Clusters::runSequential(int maxIterations) {
    for (int i=0; i<maxIterations; i++) {
        seqAssignmentStep();
        calculateCentroids();
    }
}

void Clusters::runParallel(int maxIterations) {
    pthread_t thread1, thread2;
    
    for (int i=0; i<maxIterations; i++) {
        pthread_create(&thread1, 0, parAssignmentStep1, this);
        pthread_create(&thread2, 0, parAssignmentStep2, this);
        pthread_join(thread1, 0);
        pthread_join(thread2, 0);
        
        calculateCentroids();
    }
}

void* Clusters::parAssignmentStep1(void* arg) {
    Clusters* x = static_cast<Clusters*>(arg); //reference to current class object
    
    int endPoint = int(x->data.size())/2;
    
    //1: for each point calculate distance from centroid, determine which distance is smallest, assign to cluster with smaller distance
    int closestCluster;
    float minDistance;
    float newDistance;
    //for each point
    for (int i=0; i<endPoint; i++) {
        closestCluster = 0;
        minDistance = 1000000000;
        //for each cluster
        for (int j=0; j<x->centroids.size(); j++) {
            newDistance = x->distance(x->data[i], x->centroids[j]);
            
            if (newDistance < minDistance) {
                closestCluster = j;
                minDistance = newDistance;
            }
        }
        
        x->clusterAssignments[i] = closestCluster;
        
    }//assigned cluster to each point

    return 0;
}

void* Clusters::parAssignmentStep2(void* arg) {
    Clusters* x = static_cast<Clusters*>(arg); //reference to current class object
    
    int startPoint = int((x->data.size()/2));
    
    //1: for each point calculate distance from centroid, determine which distance is smallest, assign to cluster with smaller distance
    int closestCluster;
    float minDistance;
    float newDistance;
    //for each point
    for (int i=startPoint; i<x->data.size(); i++) {
        closestCluster = 0;
        minDistance = 1000000000;
        //for each cluster
        for (int j=0; j<x->centroids.size(); j++) {
            
            newDistance = x->distance(x->data[i], x->centroids[j]);
            
            if (newDistance < minDistance) {
                closestCluster = j;
                minDistance = newDistance;
            }
        }
        
        x->clusterAssignments[i] = closestCluster;
        
    }//assigned cluster to each point
    
    return 0;
}

void Clusters::seqAssignmentStep() {
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
            newDistance = distance(data[i], centroids[j]);
            
            if (newDistance < minDistance) {
                closestCluster = j;
                minDistance = newDistance;
            }
        }
        
        clusterAssignments[i] = closestCluster;
    }//assigned cluster to each point
}

void Clusters::calculateCentroids() {
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

// ************ MAIN ************* //
int main() {
    std::map<int, std::vector<float>> vectorsMap;
    int numberOfAttributes = 8;
    std::string fileName = "actorsData.xml";
    int maxIterations = 10;
    std::vector<int> activeDimensions;
    std::vector<int> userInputDimensions;
    
    //will be coming in from user ideally
    userInputDimensions.push_back(0);
    userInputDimensions.push_back(1);
    userInputDimensions.push_back(1);
    userInputDimensions.push_back(1);
    userInputDimensions.push_back(1);
    userInputDimensions.push_back(1);
    userInputDimensions.push_back(1);
    userInputDimensions.push_back(1);
    
    loadVectors(&vectorsMap, fileName, numberOfAttributes);
    populateActiveDimensions(&activeDimensions, numberOfAttributes, userInputDimensions); //use all
    
    Clusters testCase = Clusters(/*data*/ vectorsMap, /*number of clusters*/ 2, /*which dims to use */ activeDimensions);
    testCase.setupClusters();
    
    testCase.runSequential(maxIterations);
    testCase.printClusterAssignments();
    
//    testCase.runParallel(maxIterations);
//    testCase.printClusterAssignments();
    
    return 0;
}
