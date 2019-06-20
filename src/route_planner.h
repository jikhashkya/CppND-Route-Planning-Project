#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "route_model.h"


class RoutePlanner {
  public:
    RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y);
    // Add public variables or methods declarations here.
    float GetDistance(){return distance;}; // getter method that returns distance between two nodes
  	void AStarSearch(); // performs AStarSearch between two nodes(start and goal)

  private:
    // Add private variables or methods declarations here.
    RouteModel &m_Model;
  	RouteModel::Node* start_node;
  	RouteModel::Node* end_node;
  	float distance;
  	std::vector<RouteModel::Node *> open_list;//list of un-explored node pointers
    
    //construct the final path from final/goal node to the start node
  	std::vector<RouteModel::Node> ConstructFinalPath(RouteModel::Node * current_node);
    
    //calculate HValue which just calculates euclidean distance using
    //distance() function from RouteModel
    float CalculateHValue(RouteModel::Node * const node_ptr);
    
    //finds the next node with the lowest F-value
    RouteModel::Node* NextNode();
    
    //add unexplored neighbors
    void AddNeighbors(RouteModel::Node *);
};
