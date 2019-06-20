#pragma once

#include <limits>
#include <cmath>
#include <unordered_map>
#include "model.h"
#include <iostream>

class RouteModel : public Model {

  public:
    class Node : public Model::Node {
      public:
        // Add public Node variables and methods here.
        RouteModel::Node *parent = nullptr;
      	float h_value = std::numeric_limits<float>::max();
      	float g_value = 0.0;
      	bool visited = false;
      	std::vector<RouteModel::Node*> neighbors;
        
        Node(){}
        Node(int idx, RouteModel * search_model, Model::Node node) : Model::Node(node), parent_model(search_model), index(idx) {}
        void FindNeighbors(); //populate the neighbors vector above
        
        // distance between two nodes
        float distance  (const Node &obj1) const {
          float x1 = obj1.x;
          float y1 = obj1.y;
          float x2 = this->x;
          float y2 = this->y;
          return sqrt(pow((x1-x2),2) + pow((y1-y2),2));
        }
        
      private:
        // Add private Node variables and methods here.
        int index;
        RouteModel * parent_model = nullptr;
        RouteModel::Node* FindNeighbor(std::vector<int> node_indices);
    }; //end of Node class
    
    // Add public RouteModel variables and methods here.
    RouteModel(const std::vector<std::byte> &xml);
    
    // a getter method that returns a reference to the m_Nodes
  	std::vector<Node>& SNodes(){return m_Nodes;}
  
    // public getter function to get node_to_road information
    std::unordered_map<int,std::vector<const Model::Road *>> & GetNodeToRoadMap(){return node_to_road;}
    
    RouteModel::Node& FindClosestNode(float x, float y);
    
    std::vector<Node> path; // This variable will eventually store the path that is found by the A* search.

  private:
    // Add private RouteModel variables and methods here.
    std::vector<Node> m_Nodes{};   // stores all nodes from Open Street Map data
    std::unordered_map<int, std::vector<const Model::Road *>> node_to_road;
  
    //private member function
    void CreateNodeToRoadHashMap();

};
