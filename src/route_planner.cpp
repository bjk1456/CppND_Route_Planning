#include "route_planner.h"
#include <algorithm>
using std::sort; 

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    //start_node = &m_Model.FindClosestNode(start_x, start_y);


    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
    
    std::cout << "start_node parents is " << start_node->parent << "'\n";
    std::cout << "Inside RoutePlanner::RoutePlanner";
}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    float h_value = node->distance(*end_node);
    return h_value;


}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    std::cout << "Inside AddNeighbors()" << "\n";
    std::cout << "current_node is " << current_node;

    current_node->FindNeighbors();


    
    for(RouteModel::Node *n : current_node->neighbors){
        if((current_node->x != start_node->x) && (current_node->y != start_node->y)){
        n->parent = current_node;
        }
        n->h_value = CalculateHValue(n);
        n->g_value = n->g_value + 1;
        n->visited = true;

        std::cout << "No";
        std::cout << n->x;
        std::cout << "\n";
        std::cout << n->y;
        open_list.push_back(n);
    }



    //RouteModel::Node::FindNeighbors

}

// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

RouteModel::Node *RoutePlanner::NextNode() {
    //sort(open_list.begin(), openlist.end(), Compare);

    //std::vector<RouteModel::Node*> open_list;
    std::cout << "Before sort" << '\n';
    for(RouteModel::Node *n : open_list){
        std::cout << "Inside open_list() iterator" << '\n';
        std::cout << "h value == " << n->h_value << '\n';
        std::cout << "g value == " << n->g_value << '\n';
    }
    

    sort(open_list.begin(), open_list.end(), [](RouteModel::Node* a,RouteModel::Node* b) {
        float a_node_score = a->g_value + a->h_value;
        float b_node_score = b->g_value + b->h_value;
        std::cout << "Hello";

        return a_node_score > b_node_score;   
    });

    std::cout << "After sort" << '\n';
    for(RouteModel::Node *n : open_list){
        std::cout << "h value == " << n->h_value;
    }
    RouteModel::Node *first_node = open_list.back();
    open_list.pop_back();
    return first_node;


}

bool Compare(RouteModel::Node *a_node, RouteModel::Node *b_node) {
    float a_node_score = a_node->g_value + a_node->h_value;
    float b_node_score = b_node->g_value + b_node->h_value;

  return a_node_score < b_node_score; 
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
    path_found.push_back(*current_node);
    while(current_node->parent){
        std::cout << "While parent exist " << '\n';
        std::cout << "parent->x is " << current_node->parent->x << '\n';
        std::cout << "parent->y is " << current_node->parent->y << '\n';
        std::cout << "current_node->parent is " << current_node->parent << '\n';
        distance += current_node->distance(*current_node->parent);
        current_node = current_node->parent;
        path_found.push_back(*current_node);


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
    std::cout << "The start_node is \n";
    std::cout << "start_node->x is : " << start_node->x << "\n";
    std::cout << "start_node->y is : " << start_node->y << "\n";
    std::cout << "current_node: " << current_node << "\n";
    std::cout << "end_node->x is : " << end_node->x << "\n";
    std::cout << "end_node->y is : " << end_node->y << "\n";

    current_node = start_node;
    std::cout << "start_node: " << start_node << "\n";
    AddNeighbors(current_node);
    current_node = NextNode();
    std::cout << "current_node->x is : " << current_node->x << "\n";
    std::cout << "current_node->y is : " << current_node->y << "\n";
    std::cout << "About to loop"  << "\n";
    AddNeighbors(current_node);
    std::cout << "current_node->x is : " << current_node->x << "\n";
    std::cout << "current_node->y is : " << current_node->y << "\n";
    std::cout << "About to loop";
    current_node = NextNode();
    std::cout << "current_node->x is : " << current_node->x << "\n";
    std::cout << "current_node->y is : " << current_node->y << "\n";

    while((current_node->x != end_node->x) || (current_node->y != end_node->y)){
        AddNeighbors(current_node);
        current_node = NextNode();
        std::cout << "current_node->x is : " << current_node->x << "\n";
        std::cout << "current_node->y is : " << current_node->y << "\n";


    }
    m_Model.path = ConstructFinalPath(current_node);





    //*current_node = *start_node;
    //start_node->FindNeighbors();

    //start_node->FindNeighbors();
    //open_list.push_back(current_node->FindNeighbors());

    //start_node->h_value;

    //CalculateHValue

    // TODO: Implement your solution here.

}