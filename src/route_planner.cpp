#include "route_planner.h"
#include <algorithm>
#include <iostream>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node = &m_Model.FindClosestNode(start_x,start_y);
    end_node = &m_Model.FindClosestNode(end_x,end_y);
} 


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    float h_value = node->distance(*end_node);

//std::cout <<"this is the answer: "<< node->distance(*end_node);

    return  h_value;
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
 // - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
current_node->FindNeighbors();
//RouteModel::Node *new_node;
 for(RouteModel::Node *new_node : current_node->neighbors){
     if(new_node->visited != true){
    // - For each node in current_node.neighbors, set the parent, the h_value, the g_value.
    //new_node = current_node->neighbors.at(i);
    new_node->parent = current_node;
    // - Use CalculateHValue below to implement the h-Value calculation.
    new_node->h_value=CalculateHValue(new_node);

    new_node->g_value= current_node->g_value+new_node->distance(*current_node);
    
    // - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.
    open_list.push_back(new_node);
    new_node->visited = true;
     }
} 

}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

RouteModel::Node *RoutePlanner::NextNode() {
//   - Sort the open_list according to the sum of the h value and g value.
sort(open_list.begin(),open_list.end(),[]( RouteModel::Node *t1,  RouteModel::Node *t2){return (t1->g_value+t1->h_value)< (t2->g_value+t2->h_value);});
// - Create a pointer to the node in the list with the lowest sum.
RouteModel::Node *pointer = open_list.at(0);
// - Remove that node from the open_list.
open_list.erase(open_list.begin());
// - Return the pointer.
return pointer;

}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;
    path_found.emplace(path_found.begin(),*current_node);
    while(current_node != start_node){
        RouteModel::Node *new_node = current_node->parent;
        // - For each node in the chain, add the distance from the node to its parent to the distance variable.
        distance =distance+ current_node->distance(*new_node);
        
        current_node = new_node;
        
        path_found.emplace(path_found.begin(),*new_node);
    }

    // TODO: Implement your solution here.

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.
current_node = start_node;
 //AddNeighbors(current_node);
 start_node->visited = true;
 open_list.push_back(start_node);
  while(current_node->distance(*end_node)!=0){
    // - Use the NextNode() method to sort the open_list and return the next node.  
    current_node= NextNode();
    AddNeighbors(current_node);
  }
  // - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
  m_Model.path = ConstructFinalPath(current_node); 
  
}