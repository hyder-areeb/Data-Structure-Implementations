

#ifndef SINGLE_NODE_H
#define SINGLE_NODE_H

#ifndef nullptr
#define nullptr 0
#endif

#include <string>
#include <cctype>
#include "ece250.h"

class Trie;

class Trie_node {
	private:
		Trie_node **children;
		bool is_terminal;
		static int const CHARACTERS = 26;


	public:
		Trie_node();
		Trie_node *child( int ) const;

		bool member( std::string const &, int ) const;

		bool insert( std::string const &, int );
		bool erase( std::string const &, int, Trie_node *& );
		void clear();

		friend class Trie;

		// if ptr is a pointer to a Trie_node object
		// in one of the friendly classes, you should:
		//    use   ptr->next_   to modify it
		//    use   ptr->next()      to access it
};

Trie_node::Trie_node():
children( nullptr ),
is_terminal( false ) {
	// empty constructor
}

Trie_node *Trie_node::child( int n ) const {
	if ( children == nullptr ) {
		return nullptr;
	} else {
		return children[n];
	}
}

bool Trie_node::member( std::string const &str, int depth ) const {

	//If the the node is at the end of the word call is_Terminal
	if(depth == str.size()){
		return is_terminal;
	}

	//now lets set the character of a string into a int
	char nodeChar = str[depth]; 	//nodeChar holds the integer value of the character at a depth
	int element = tolower(nodeChar) - 'a';		//Make sure the char is a lower case

	//Now we recursively call the member function to look for the char

	if (this->children == nullptr) {
		return false;
	}

	if(children[element] == nullptr){
		return false;
	}

	else{
		depth = depth +1;
		//why do i have the pointer
		return this->children[element]->member(str, depth);
	}
}

bool Trie_node::insert( std::string const &str, int depth ) {

	if(str.size() == depth){
		//set is_terminal to true
		is_terminal = !false;
		return true;
	}


	//get the int
	int element = (tolower(str[depth])) - 'a';


	//what is children is nullptr?

	if(this->children == nullptr){

		this->children = new Trie_node *[CHARACTERS];

		for(int j =0; j<CHARACTERS; j++){
			this->children[j] = nullptr;
		}
	}

	if ( (this->children[element] == nullptr)){
		this->children[element] = new Trie_node;
	}

	depth = depth +1;
	return this->children[element]->insert(str, depth);

}

bool Trie_node::erase( std::string const &str, int depth, Trie_node *&ptr_to_this ) {

	//get the int
	char nodeChar = str[depth];
	int element = tolower(nodeChar) - 'a';

	//at the end
//	int end = (children == nullptr) && (is_terminal) && (depth != str.size() );
//	if (end == true) {
//	return false;
//}

	//we recurse downwards
	if (str.size() == depth ) {
		//set is_terminal to false
		is_terminal = !true;

		if (this->children == nullptr) {
			delete this;
			ptr_to_this = nullptr;
		}
		return true;
	}

	depth = depth +1;
    return this->children[element]->erase(str, depth, children[element]);


}

void Trie_node::clear() {

	if (this->children != nullptr){ //some indexs are not null

		int j = 0;

		while(j < this->CHARACTERS){

			if (this->children[j] != nullptr) {

				//recursive call
				this->children[j] -> clear();

				delete children[j];
			}

			j++;
		}

		delete[] this->children;
	}

	return;
}


// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
