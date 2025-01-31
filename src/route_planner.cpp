#include "route_planner.h"
#include <algorithm>

//constructor
RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    //scaling 
  	start_x *= 0.01;
  	start_y *= 0.01;
  	end_x *= 0.01;
  	end_y *= 0.01;
  
    start_node = &m_Model.FindClosestNode(start_x, start_y);
  	end_node = &m_Model.FindClosestNode(end_x, end_y);
}


std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node * current_node){
  std::vector<RouteModel::Node> path_found;//stores the path from start to end node
  distance = 0.0f;
  
  while(current_node->parent != nullptr){
  	path_found.push_back(*current_node);
    //keep track of distance
    distance += current_node->distance(*current_node->parent);
    current_node = current_node->parent;
  }
  path_found.push_back(*current_node);
  distance *= m_Model.MetricScale();
  
  return path_found;
}


void RoutePlanner::AStarSearch(){

  	start_node->visited = true;
  	open_list.push_back(start_node);
  
  	RouteModel::Node *current_node = nullptr;
  	while(open_list.size() >0){
      		// select the lowest f-value node
    		current_node = NextNode();
      		//find the distance between this and the end node
      		//to check if we've reached the end node.
      		if(current_node->distance(*end_node) == 0){
          	//if we did reach the end_node, construct the path
          	//from end to the start node.
        	m_Model.path = ConstructFinalPath(current_node);
          	return;
        	}
		//if end_node not reaced, keep adding neighbors
        	// and exploring.
        	AddNeighbors(current_node);
        }
	return ;
}


float RoutePlanner::CalculateHValue(RouteModel::Node * const node_ptr){
	return node_ptr->distance(*end_node);
}


//helper function for the sort function used below
bool compareFVal(RouteModel::Node *node1, RouteModel::Node *node2){
      return (node1->g_value + node1->h_value) < (node2->g_value + node2->h_value);
    }
RouteModel::Node* RoutePlanner::NextNode(){
  	
	std::sort(open_list.begin(), open_list.end(), compareFVal);
  	RouteModel::Node *smallest_fval_node = open_list.front();
  	open_list.erase(open_list.begin());
  	
  	return smallest_fval_node;
}


void RoutePlanner::AddNeighbors(RouteModel::Node *current_node){
  current_node->FindNeighbors();
  
  for(auto neighbor: current_node->neighbors){
    neighbor->parent = current_node;
    neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);
    neighbor->h_value = CalculateHValue(neighbor);
    
    open_list.push_back(neighbor);
    neighbor->visited = true;
  }
}



