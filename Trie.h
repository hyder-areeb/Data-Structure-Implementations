
#ifndef TRIE_H
#define TRIE_H

#ifndef nullptr
#define nullptr 0
#endif

#include "ece250.h"
#include "Trie_node.h"
#include "Exception.h"
#include <iostream>
#include <string>
#include <cctype>

class Trie {
	private:
		Trie_node *root_node;
		int trie_size;
		//check if the string contains only letters
		bool properString(std::string const &) const;

	public:
		Trie();
		~Trie();

		// Accessors

		int size() const;
		bool empty() const;
		bool member( std::string const & ) const;
		Trie_node *root() const;
		// Mutators

		bool insert( std::string const & );
		bool erase( std::string const & );
		void clear();

	// Friends

	friend std::ostream &operator<<( std::ostream &, Trie const & );
};

Trie::Trie():
root_node( nullptr ),
trie_size( 0 ) {
	// empty constructor
}

Trie::~Trie() {
	if (this->root_node != nullptr){
		this->clear();
	}
	delete this->root_node;
}

int Trie::size() const {
	return this->trie_size;
}

bool Trie::empty() const {
	if(this->trie_size > 0){
		return false;
	}
	else{
		return true;
	}
}

Trie_node *Trie::root() const {
	return this->root_node;
}

bool Trie::properString(std::string const &str) const {
    int i = 0;
    int stringSize = str.size();

    while(i < stringSize){
        if (isalpha(str[i]) == false ){	//checking each character of the string to make sure its all letters
            return false;
        }
        i++;
    }

    return true;
}

bool Trie::member( std::string const &str ) const {
	//first make sure the string is not an illegal string

	if(properString(str) == false){
		throw illegal_argument();
	}

	if ( this->root_node == nullptr ){ //if root is null then no string exist in the tree
		return false;
	}

	if(this->empty() == true){
		return false;
	}

	if(root_node->member(str, 0)){ //call the member function on the root_rode
		return true;
	}else{
		return false;
	}


}

bool Trie::insert( std::string const &str ) {

	//first check to make sure the string is valid
	if(properString(str) == false){
		throw illegal_argument();
	}

	if(this->root_node == nullptr){
		this->root_node = new Trie_node;
	}

	//we want to check if the thing already exists in there
	bool stringExist = this->member(str);

	if(stringExist == false){
		//now we try to insert the element
		bool inserted = this->root_node->insert(str, 0); //try inserting the str

		if(inserted == true){
			this->trie_size = this->trie_size + 1; //increment the size
			return true;
		}
	}

	return false;

}

bool Trie::erase( std::string const &str ) {

	//first check to see if the str is proper
	if(properString(str) == false){
		throw illegal_argument();
	}

	//what is the trie is empty
	bool empty = this->empty();
	if(empty == true){
		return false;
	}

	//now check if the string exists in the trie
	bool exist = this->member(str);
	if(exist == false){
		return false;
	}

		//if size is 1
	   else if(this->trie_size == 1) {
		this->root_node->clear();
		delete this->root_node;
		this->root_node = nullptr;
		this->trie_size = this->trie_size -1;
	}

	   else{
		this->root_node->erase(str, 0, root_node);
		this->trie_size = this->trie_size - 1;
	}

	return true;

}

void Trie::clear() {
	if (this->empty() == false){

		this->root_node->clear();
		delete this->root_node;
		this->root_node = nullptr;
		this->trie_size = 0;

	}
}

// You can modify this function however you want:  it will not be tested

std::ostream &operator<<( std::ostream &out, Trie const &trie ) {

	return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
