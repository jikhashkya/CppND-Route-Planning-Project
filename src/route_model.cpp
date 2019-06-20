#include "route_model.h"
#include <iostream>

//constructor
RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
  
  //initialize RouteModel::Node object where
  // node_idx = count
  int count = 0;
  for(Model::Node node : this-> Nodes()){
    this->m_Nodes.push_back(RouteModel::Node(count, this , node));
     count += 1;
  }
  
  CreateNodeToRoadHashMap();
}


void RouteModel::CreateNodeToRoadHashMap(){
	
  	for(auto &road: this->Roads()){
      if(road.type != Model::Road::Type::Footway){
      	for(int node_idx : Ways()[road.way].nodes){
        	if(node_to_road.find(node_idx) == node_to_road.end()){
            	node_to_road[node_idx] = std::vector<const Model::Road*> ();
             }
          	node_to_road[node_idx].push_back(&road);
        }
      }
    
    }
}


RouteModel::Node* RouteModel::Node::FindNeighbor(std::vector<int> node_indices){
	RouteModel::Node *closest_node = nullptr;
  	RouteModel::Node node;
  	
  	for(int node_index: node_indices){
    	node = parent_model->SNodes()[node_index];
      	if(this->distance(node)>0 && node.visited == false){
          if((closest_node == nullptr) || (this->distance(node) < this->distance(*closest_node))){
            closest_node = &parent_model->SNodes()[node_index];          
          }
        }
    }
	return closest_node;
}


void RouteModel::Node::FindNeighbors(){
  for(auto &road: parent_model->node_to_road[this->index]){
  	RouteModel::Node * node_ptr=this->FindNeighbor(parent_model->Ways()[road->way].nodes);
    
    if(node_ptr){
    	this->neighbors.emplace_back(node_ptr);
    }
  }
}


RouteModel::Node& RouteModel::FindClosestNode(float x, float y)
{
	RouteModel::Node node;
  	node.x = x;
  	node.y = y;
  
  	float min_dist = std::numeric_limits<float>::max();
  	int closest_idx;
  
  	for(auto &road: Roads()){
    	if(road.type != Model::Road::Type::Footway){
        	for(int node_idx: Ways()[road.way].nodes){
            	if(SNodes()[node_idx].distance(node) < min_dist){
                  min_dist = SNodes()[node_idx].distance(node);
                  closest_idx = node_idx;
                }
            }
        }
    }
	return SNodes()[closest_idx];
}
