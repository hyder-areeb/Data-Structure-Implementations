
#ifndef DYNAMIC_STACK_AS_ARRAY_H
#define DYNAMIC_STACK_AS_ARRAY_H

#ifndef nullptr
#define nullptr 0
#endif

#include <algorithm>
#include "Exception.h"

class Dynamic_range_stack {
	private:
		int entry_count;
		int max_count;
		int min_count;
		int initial_capacity;
		int current_capacity;

		int *stack_array;
		int *maximum_array;
		int *minimum_array;

		// You may wish to include a number of helper functions
		// in order to abstract out some operations


	public:
		Dynamic_range_stack( int = 10 );
		~Dynamic_range_stack();

		int top() const;
		int size() const;
		bool empty() const;
		int capacity() const;
		void newStackSize();


		int maximum() const;
		int minimum() const;

		
		void push( int const & );
		int pop();
		void clear();


	// Friends

	friend std::ostream &operator<<( std::ostream &, Dynamic_range_stack const & );
};

Dynamic_range_stack::Dynamic_range_stack( int n ):
entry_count( 0 ),
min_count( 0 ),
max_count( 0 ),
initial_capacity( std::max( 1, n ) ),
current_capacity( initial_capacity ),
stack_array( new int[current_capacity] ),
maximum_array( new int[current_capacity] ),
minimum_array( new int[current_capacity] ) {
	// empty constructor
}

Dynamic_range_stack::~Dynamic_range_stack() {

    //we need to delete all the things in memory
    delete[] stack_array;
    delete[] minimum_array;
    delete[] maximum_array;

}

int Dynamic_range_stack::top() const {
    int top_pointer = this->entry_count-1; //the top pointer points the size of the stack -1.
    if (entry_count <= 0){ // if the stack was empty we need to return an underflow exception
        throw underflow();
    }else { // if the stack was not empty we need to return what the top value was
        int top_value = this->stack_array[top_pointer];
        return top_value;

    }

}

int Dynamic_range_stack::maximum() const {

   if(max_count <= 0){
       throw underflow();
   }
	int max_value = this->maximum_array[max_count-1]; // return ths top element on the max stack
	return max_value;
}

int Dynamic_range_stack::minimum() const {
    if(min_count <= 0){
        throw underflow();
    }

	int min_value = this->minimum_array[min_count -1]; //return the top element of the min stack
	return min_value;
}

int Dynamic_range_stack::size() const {
	return this->entry_count; //return the entry_count variable as it keep tracks of how many elements are in stack
}

bool Dynamic_range_stack::empty() const {
	if (entry_count == 0){  //simple if-else check to see if we have any elements in array
	    return true;
	}else{
	    return false;
	}

}

int Dynamic_range_stack::capacity() const {
	return this->current_capacity;  //return the vairable that keep tracks of wheather the stack is empty of not.
}

//this method is used to make new stacks of double size
void Dynamic_range_stack::newStackSize()  {

    //initalize 3 new arrays with double size
    int *newSize_stack_array = new int[current_capacity * 2];
    int *newSize_maximum_array = new int[current_capacity * 2];
    int *newSize_minimum_array = new int[current_capacity * 2];


    //copy over old elements into new array
    for(int i = 0; i <= this->entry_count ; i++){
        newSize_stack_array[i] = stack_array[i];
        newSize_maximum_array[i] = maximum_array[i];
        newSize_minimum_array[i] = minimum_array[i];
    }

    delete[] stack_array;
    delete[] minimum_array;
    delete[] maximum_array;


    //store the new arrays back into the old arrays
    stack_array = newSize_stack_array;
    maximum_array = newSize_maximum_array;
    minimum_array = newSize_minimum_array;

    //we change the size of the current capacity
    this->current_capacity = this->current_capacity *2;

}

void Dynamic_range_stack::push( int const &obj ) {

    //This method can be broken down into 3 sub-problems for each of the stack. Main stack, Min stack and Max stack

	//if our stack is empty then we need to fill all 3 stacks with this initial value
	if(entry_count == 0){
		this->stack_array[this->entry_count] = obj;
		this->maximum_array[this->max_count] = obj;
		this->minimum_array[this->min_count] = obj;

		//increment counters
		entry_count++;
		max_count++;
		min_count++;
		return;
	}

    //first we need to see if out stack is full
    if(entry_count >= this->current_capacity) {       //if the stack is full we need new arrays
        this->newStackSize();
    }

    //When the stack was not intially empty we need to add elements accordingly
    //stack main:
    this->stack_array[this->entry_count] = obj;
    this->entry_count++;

    //stack Max:
    //we need to check and see if this obj argument would be the max value in the stack
    int currentMax = this->max_count -1;
    if(maximum_array[currentMax] < obj){
        maximum_array[max_count] = obj;
        max_count++;
    }

    //stack Min:
    //we need to check and see if this obj argument would be the Min value in the stack
    int currentMin = this->min_count -1;
    if(minimum_array[currentMin] > obj){
        minimum_array[min_count] = obj;
        min_count++;
    }

	return;
}

int Dynamic_range_stack::pop() {

    //first check to see if we need to remove values from the top and min stacks

    if(this->entry_count == 0){
        throw underflow();
    }
    //if the element being poped is in max array remove it
    if(this->stack_array[entry_count -1] == this->maximum_array[max_count-1]){
        this->maximum_array[max_count -1] = 0;
        this->max_count--;
    }

    //if the element being poped is in min array remove it
    if(this->stack_array[entry_count -1] == this->minimum_array[min_count-1]){
        this->minimum_array[min_count -1] = 0;
        this->min_count--;
    }

    //now pop it off the main stack
    int popped_value = this->stack_array[entry_count-1];
    this->stack_array[entry_count-1] = 0;
    entry_count--;
    return popped_value;
}

void Dynamic_range_stack::clear() {

    int *newSize_stack_array = new int[initial_capacity];
    int *newSize_maximum_array = new int[initial_capacity];
    int *newSize_minimum_array = new int[initial_capacity];

    delete[] stack_array;
    delete[] minimum_array;
    delete[] maximum_array;

    stack_array = newSize_stack_array;
    maximum_array = newSize_maximum_array;
    minimum_array = newSize_minimum_array;

    entry_count = 0;
    max_count = 0;
    min_count = 0;
    current_capacity = initial_capacity;



}

// You can modify this function however you want:  it will not be tested

std::ostream &operator<<( std::ostream &out, Dynamic_range_stack const &stack ) {
	// Print out your stacks

	return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
