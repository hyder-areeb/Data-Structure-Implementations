#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H


#include "Exception.h"
#include "ece250.h"

enum state { EMPTY, OCCUPIED, DELETED };

template<typename T>
class DoubleHashTable {
	private:
		int count;
		int power;
		int array_size;
		T *array;
		state *array_state;

		int h1( T const & ) const; // first hash function
		int h2( T const & ) const; // second hash function

	public:
		DoubleHashTable( int = 5 );
		~DoubleHashTable();
		int size() const;
		int capacity() const;		
		bool empty() const;
		bool member( T const & ) const;
		int findObjKey( T const & ) const;
		T bin( int ) const;

		void print() const;

		void insert( T const & );
		bool remove( T const & );
		void clear();
};

template<typename T >
DoubleHashTable<T >::DoubleHashTable( int m ):
count( 0 ), power( m ),
array_size( 1 << power ),
array( new T [array_size] ),
array_state( new state[array_size] ) {

	for ( int i = 0; i < array_size; ++i ) {
		array_state[i] = EMPTY;
	}
}

template<typename T >
DoubleHashTable<T >::~DoubleHashTable() {

	delete[] array_state;
	delete[] array;
	count = 0;
}

template<typename T >
int DoubleHashTable<T >::size() const {
	return count; //return # of entries
}

template<typename T >
int DoubleHashTable<T >::capacity() const {
	return array_size; // return array size
}

template<typename T >
bool DoubleHashTable<T >::empty() const {

	if(count == 0){ //check to see if array is empty
		return true;
	} else{
		return false;
	}

}

template<typename T >
int DoubleHashTable<T >::h1( T const &obj ) const {

	int probe1 = static_cast<int>(obj);
	probe1 = probe1 % array_size; //get the hash key

	if(probe1 < 0){ //check to see if hashkey is negative
		probe1 = probe1 + array_size;
	}

	return probe1;
}

template<typename T >
int DoubleHashTable<T >::h2( T const &obj ) const {

	int probe2 = static_cast<int>(obj) / array_size; //divide obj by size of array
	probe2 = probe2 % array_size; //get hashkey2

	if(probe2 < 0){ //check to see if negative
		probe2 = probe2 + array_size;
	}

	if((probe2 % 2) == 0){ //check to see if even
		probe2 = probe2 + 1;
	}

	return probe2;
}

template<typename T >

//ssmnauma helped me with my implementation for the member function
bool DoubleHashTable<T >::member( T const &obj ) const {
//check to see if obj is in hash table
    int i = 0;
    int probe;

    do {
        probe = (h1(obj) + i*h2(obj)) % array_size;

        if ( (array_state[probe] == OCCUPIED && array[probe] != obj) || (array_state[probe] == DELETED)) {
            i++;
        }

        else if (array_state[probe] == EMPTY) {
            return false;
        }
    }
    while (array[probe] != obj && i < array_size); //Exit the while loop if i>array_size or u found the obj


    if (array_state[probe] == OCCUPIED && array[probe] == obj) // check again to see if we have the object
        return true;
    return false;
}

template<typename T >
int DoubleHashTable<T >::findObjKey( T const &obj ) const {

	int ObjKey;

	for(int i = 0; i< array_size; i++){

		int probe = (h1(obj) + i*h2(obj)) % array_size;

		if((array_state[probe] == OCCUPIED) && (array[probe] == obj)){
			ObjKey = probe;
			return ObjKey;
		}

	}
}


template<typename T >
T DoubleHashTable<T >::bin( int n ) const {
		return array[n];
}

template<typename T >
void DoubleHashTable<T >::insert( T const &obj ) {

	if(count >= array_size){
		throw overflow();
	}

	//this part of the insert code i used similar implentation found in ppt
	int probe = h1(obj);
	int offset = h2(obj);


    if(array_state[probe] == OCCUPIED) {
        while (array_state[probe] == OCCUPIED) {
            probe = (probe + offset) % array_size;
        }
    }else {
        probe = probe % array_size;
    }


	//now that I have found the index of the array, called probe
	array[probe] = obj;
	array_state[probe] = OCCUPIED;
	count++;

}

template<typename T >
bool DoubleHashTable<T >::remove( T const &obj ) {

	//first check if the object is in the table
	if(member(obj) == false){
        return false;
    }

    if(count == 0){
        return false;
    }

	//now i need to find the key of the obj
	int key = findObjKey(obj);

	//now delete the obj
	array_state[key] = DELETED;
	array[key] = 0;

	count--;
	return true;
}

template<typename T >
void DoubleHashTable<T >::clear() {

	for(int i = 0; i< array_size; i++){

		if(array_state[i] == OCCUPIED){

			remove(array[i]);
		}
	}

}

//Print function won't be tested
template<typename T >
void DoubleHashTable<T >::print() const {

	std::cout << "Values in regular array";
	for (int i = 0; i < array_size; i++){
		std::cout << array[i];
	}

	std::cout << "Values in state array";
	for (int i = 0; i < array_size; i++){
		std::cout << array_state[i];
	}



	return;
}

#endif
