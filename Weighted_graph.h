#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

#include <iostream>
#include <limits>
#include "Exception.h"
#include <queue>

struct vertex;
struct AdjacencyList;
struct largerWeight;


struct vertex {

    int curr_vertex;
    double weight;

    vertex(int curr_vertex, double weight) {

        this->curr_vertex = curr_vertex;
        this->weight = weight;
    }
};


struct AdjacencyList {

    int degree;
    int trackNode;
    double weight;
    AdjacencyList *nextNode;
};

struct largerWeight {
    bool operator()(vertex const &A, vertex const &B) {
        return A.weight > B.weight;
    }

};


class Weighted_graph {
private:
    static const double INF;
    int edges;
    int vertices_count;
    AdjacencyList *ListOfVertexes;

public:
    Weighted_graph(int = 50);

    ~Weighted_graph();

    int degree(int) const;

    int edge_count() const;

    double adjacent(int, int) const;

    double minimum_spanning_tree(int) const;

    bool is_connected() const;

    void insert(int, int, double);

    void newWeight( int, int, double);

    void removeExistingEdge(int, int, double);

    void addNewEdge(int, int, double);

    // Friends

    friend std::ostream &operator<<(std::ostream &, Weighted_graph const &);
};

const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

// Enter definitions for all public functions here

Weighted_graph::Weighted_graph(int size) {

    if (size > 0) {

        ListOfVertexes = new AdjacencyList[size];

        this->edges = 0;
        this->vertices_count = size;
        int j = 0;
        while (j < size) {
            this->ListOfVertexes[j].weight = 0;
            this->ListOfVertexes[j].degree= 0;
            this->ListOfVertexes[j].trackNode = j;
            this->ListOfVertexes[j].nextNode = nullptr;

            j++;
        }



    }

}

Weighted_graph::~Weighted_graph() {

    int j =0;
    while(j< vertices_count){

        AdjacencyList *curr;
        curr = ListOfVertexes[j].nextNode;


        for(int i = 0; curr != nullptr; i++){

            ListOfVertexes[j].nextNode = curr->nextNode;
            curr->nextNode = nullptr;
            delete curr;

            curr = ListOfVertexes[j].nextNode;

            if(curr == nullptr){
                break;
            }
        }

       // ListOfVertexes[j].nextNode = NULL;



        if(j > vertices_count){
            break;
        }
        j++;
    }

    delete[] ListOfVertexes;

}

int Weighted_graph::edge_count() const {
    return this->edges;
}

int Weighted_graph::degree(int vertex) const {

    if (vertex < 0) {
        throw illegal_argument();
    }

    if (vertex >= vertices_count) {
        throw illegal_argument();
    }

    return ListOfVertexes[vertex].degree;

}


double Weighted_graph::adjacent(int A, int B) const {

    if (A >= vertices_count) {
        throw illegal_argument();
    }

    if (A < 0) {
        throw illegal_argument();
    }

    if (B >= vertices_count) {
        throw illegal_argument();
    }

    if (B < 0) {
        throw illegal_argument();
    }

    if (A == B) {
        return 0;
    }

    AdjacencyList *current_vertex = &ListOfVertexes[A];
    for (int j = 0; current_vertex->nextNode != nullptr; j++) {

        current_vertex = current_vertex->nextNode;

        if (current_vertex->trackNode == B) {
            break;
        }

        if(current_vertex->nextNode == nullptr){
            break;
        }
    }

    if (current_vertex->trackNode != B) {
        return INF;
    } else {
        return current_vertex->weight;
    }

}

double Weighted_graph::minimum_spanning_tree(int A) const {

    if (A < 0) {
        throw illegal_argument();
    }

    if (A >= vertices_count) {
        throw illegal_argument();
    }


    bool reached[this->vertices_count];
    double weight = 0;

    int j = 0;
    while (j < this->vertices_count) {
        reached[j] = false;
        j++;
    }


    std::priority_queue<vertex, std::vector<vertex>, largerWeight> Que;
    Que.push(vertex(A, 0));


    for (int i = 0; !Que.empty(); i++) {

        vertex vert = Que.top();

        bool visited = reached[vert.curr_vertex];
        if (visited == false) {
            weight = weight + vert.weight;
        }
        Que.pop();
        bool isvisited = true;
        reached[vert.curr_vertex] = isvisited;

        AdjacencyList *curr = &ListOfVertexes[vert.curr_vertex];

        while (curr->nextNode != nullptr) {
            curr = curr->nextNode;

            bool notVisited = reached[curr->trackNode] == false;
            bool holdsWeight = curr->weight != 0;
            bool notInfinity = curr->weight != INF;

            if (notVisited && holdsWeight && notInfinity) {
                Que.push(vertex(curr->trackNode, curr->weight));
            }
        }

        if (Que.empty()) {
            break;
        }
    }
    return weight;


}

//CHECK OVER
bool Weighted_graph::is_connected() const {

    if(vertices_count == 0){
        return false;
    }


    std:: queue<int> Que;

    bool reached[vertices_count];
    int uniqV = 1;

    int j =0;
    while (j<vertices_count){
        reached[j] = false;
        j++;
    }

    reached[0] = !false;
    Que.push(0);


    while(!Que.empty()){
        int vert = Que.front();

        AdjacencyList *curr = &ListOfVertexes[vert];

        while(curr->nextNode != nullptr){
            curr = curr->nextNode;

            bool visited = reached[curr->trackNode];
            if(visited == false){
                uniqV = uniqV + 1;
                Que.push(curr->trackNode);
                reached[curr->trackNode] = !false;

            }

            if(curr->nextNode == nullptr){
                break;
            }
        }

        Que.pop();

        if(Que.empty()){
            break;
        }

    }

    if(uniqV != vertices_count){
        return !true;
    }else{
        return !false;
    }


}


void Weighted_graph::addNewEdge(int A, int B, double weight) {
    this->ListOfVertexes[A].degree = this->ListOfVertexes[A].degree + 1;

    AdjacencyList *_new = new AdjacencyList;
    _new->weight = weight;
    _new->trackNode = B;
    _new->nextNode = nullptr;

    AdjacencyList *curr = &ListOfVertexes[A];

    for (int i = 0; curr->nextNode != nullptr; i++) {

        curr = curr->nextNode;

        if (curr->nextNode == nullptr) {
            break;
        }
    }

    curr->nextNode = _new;
}

void Weighted_graph::removeExistingEdge(int A, int B, double weight) {

    this->ListOfVertexes[A].degree= this->ListOfVertexes[A].degree - 1 ;
    AdjacencyList *curr = &ListOfVertexes[A];
    AdjacencyList *prev = ListOfVertexes;

    for (int i = 0; curr->trackNode != B; i++) {

        prev = curr;
        curr = curr->nextNode;


        if (curr->trackNode == B) {
            break;
        }
    }

    prev->nextNode = curr->nextNode;
    delete curr;


}

void Weighted_graph::newWeight(int A, int B, double weight) {

    AdjacencyList *curr;
    curr = &ListOfVertexes[A];



    for(int i =0; curr->nextNode != nullptr; i++){

        curr = curr->nextNode;

        if(curr->trackNode == B){
            break;
        }

        if(curr->nextNode == nullptr){
            break;
        }
    }

    int currValue = curr->trackNode;

    if(currValue == B){
        curr->weight = weight;
    }
}

void Weighted_graph::insert(int A, int B, double weight) {

    bool doWeHaveAnEdge = false;

    bool negativeWeight = (weight < 0);
    bool infinityWeight = (weight == INF);
    bool sameValue = (A == B);
    bool largeA = (A >= vertices_count);
    bool largeB = B >= vertices_count;
    bool negativeA = A < 0;
    bool negativeB = B < 0;

    if (negativeWeight) {
        throw illegal_argument();
    }
    if (infinityWeight) {
        throw illegal_argument();
    }
    if (sameValue) {
        throw illegal_argument();
    }
    if (largeA) {
        throw illegal_argument();
    }
    if (largeB) {
        throw illegal_argument();
    }
    if (negativeA) {
        throw illegal_argument();
    }
    if (negativeB) {
        throw illegal_argument();
    }


    AdjacencyList *curr = &ListOfVertexes[A];

    for (int j = 0; curr->nextNode != nullptr; j++) {
        curr = curr->nextNode;

        bool sameAsB = (curr->trackNode == B);
        if (sameAsB) {
            break;
        }

        if (curr->nextNode == nullptr) {
            break;
        }
    }

    bool _sameAsB = (curr->trackNode == B);
    if (_sameAsB) {
        newWeight(A, B, weight);
        newWeight(B, A, weight);
        doWeHaveAnEdge = true;
    }


    if (weight == 0 && doWeHaveAnEdge == true) {
        removeExistingEdge(A, B, weight);
        removeExistingEdge(B, A, weight);
        this->edges = this->edges - 1;

    } else if (weight == 0) {

        return;
    }

    if (doWeHaveAnEdge == false) {
        addNewEdge(A, B, weight);
        addNewEdge(B, A, weight);
        this->edges = this->edges + 1;
    }

    return;

}


std::ostream &operator<<(std::ostream &out, Weighted_graph const &graph) {
    // Your implementation

    return out;
}

//end


#endif