#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

    class trie{        
        struct trieNode{
            char data;
            int numOfPaths = 1;
            trieNode *parent;
            trieNode *left = NULL;
            trieNode *right = NULL; 
        };

        public:
        int size;
        trieNode *root;
        //defualt trieNode
        trie() : root (new trieNode), size(1){
            root->numOfPaths = 1;
        }
        //methods
        public:
         void addLeft(trieNode *curr){
			 curr->left = new trieNode;
			 this->size++; 
             curr->left->parent = curr;
          }
        void addRight(trieNode *curr){
             curr->right = new trieNode;
			 this->size++; 
             curr->right->parent  = curr;
        }

        void add(string key, int prefix, char action){
            char index;
            trieNode *curr = root;
                        //(go through/add to) the tree until prefix size-1
            for (int i = 0; i < prefix-1; i++){
                index = key.at(i);
                //go left
                if(index == '0'){ 
                    //if left son doesn't exist - add a new trieNode to the tree
			        if(curr->left == NULL){ 
                        addLeft(curr);
                        if(curr->right != NULL){
                            curr->numOfPaths++;
                        }
                    }
                    //if there is left son- update path
                    else{
                         curr->numOfPaths++;
                    }
                    curr = curr->left;
                }
                //go right
                else if(index == '1'){ 
                    //if  right son doesn't exist - add a new trieNode to the tree
			        if(curr->right == NULL){
                        addRight(curr);
                        if(curr->left != NULL){
                            curr->numOfPaths++;
                        }
                    }
                    //if there is right son -update  path
                    else {
                        curr->numOfPaths++;
                    }
                    curr = curr->right;
                }
            }
            //check if can do optimization
            if(key.at(prefix-1) == '1'){
                //check if there is left son with the same action
                if(curr->left != NULL && curr->left->data == action){
                    //if only one path go through over left son node
                     //delete him update action to father
                    if(curr->left->numOfPaths == 1){
                        curr->left = NULL;
                        delete(curr->left);
                        size--;
                        curr->data = action;
                    }
                    //if there is more than one path- update tne numOfPaths and the data
                    //and set action to father
                    else{
                        curr->left->numOfPaths--;
                        curr->left->data = '\0';
                        curr->data = action;
                    }
                    curr = curr->parent;
                    //update numOfPaths til the root
                    while(curr != NULL){
                        curr->numOfPaths--;
                        curr= curr->parent;
                    }  
                }
                //if left son doesn't have  the same action
                else{ 
                    //if right son is NULL add new node
                   if(curr->right == NULL){
                       if(curr->left != NULL){
                           curr->numOfPaths++;
                       }
				       addRight(curr);
				        curr->right->left = NULL;
				        curr->right->right = NULL;
                    }
                    curr->right->data = action;
                }
            }
            if(key.at(prefix-1) == '0'){
                //if there is right son with the same action
                if(curr->right != NULL && curr->right->data == action){
                  //if only one path go through over right son node
                     //delete him update action to father
                    if(curr->right->numOfPaths == 1){
                        curr->right = NULL;
                        delete(curr->right);
                        size--;
                        curr->data = action;
                    }
                   //if there is more than one path- update tne numOfPaths and the data
                    //and set action to father
                    else{
                        curr->right->numOfPaths--;
                        curr->right->data = '\0';
                        curr->data = action;
                    };
                    //update numOfPaths til the root
                    curr = curr->parent;
                    while(curr != NULL){
                        curr->numOfPaths--;
                        curr= curr->parent;
                    }   
                }
                //if there isn't a right son with the same action
                else{ 
                    //if left son is NULL add a new node 
                   if(curr->left == NULL){
                       if(curr->right != NULL)curr->numOfPaths++;
				        addLeft(curr);
				        curr->left->left = NULL;
				        curr->left->right = NULL;
                   }
                    curr->left->data = action;
                }
            }
        }
            int find(string key){
            int numOfSteps = 0;
	        if(root != NULL){
                trieNode *curr = root;
                char index;
                //go through the given key and count steps
                for (int i = 0; i < key.length(); i++){
                    index = key.at(i);
                    //go right
                    if(index == '1'){ 
                        if(curr->right != NULL) {
                            numOfSteps++;
                            curr = curr->right;
                        }
                        else break;
                    }
                    //go left
                    if(index == '0'){ 
                        if(curr->left != NULL) {
                            numOfSteps++;
                            curr = curr->left;
                        }
                        else break;
                    }    
                }
                int count  = numOfSteps;
                while(count < key.length()){
                    if(key.at(count) != '0') return 0;
                    count++; 
                }
            }
            return numOfSteps;
        }

          bool remove(string key,int  prefix, char action){
            //find the last node of the given key
            trieNode* toRemove = removeHelp(key, prefix, action);
             //there is a node - need to remove
            if(toRemove != NULL){
               myRemove(key, prefix, toRemove);
                return true;
            }
            else return false;
        }

        //help function for remove
        //find the last node in the IP adress
        trieNode* removeHelp(string key, int prefix, char action){
	        if(root != NULL){
                trieNode *node = root;
                char index;
                //go on the path of the key
                for (int i = 0; i < prefix; i++){
                    index = key.at(i);
                    //go right
                    if(index == '1'){ 
                        if(node->right != NULL) node = node->right;
                        else return NULL;
                    }
                    //go left
                    if(index == '0'){ 
                        if(node->left != NULL) node = node->left;
                        else return NULL;
                    }    
                }
                //if the action is the same for what we search return this node
                if(node->data == action) return node;
                else return NULL;                
            }
            else return NULL;
        }


        
        //recursive function to remove the ip from the tree
        bool myRemove(string key, int prefix, trieNode* toRemove) {
            if (prefix == 0) return true;
            trieNode* temp = toRemove->parent;
            if(prefix > 0){
                //if there is more than one path go through this node- ocunt --
                if(toRemove->numOfPaths != 1) toRemove->numOfPaths--;
                // if there is one path - delete this node from tree
                else {
                    toRemove = nullptr;
                    if(key.at(prefix-1)=='1') temp->right = nullptr;
                    if(key.at(prefix-1)=='0') temp->left = nullptr;
                    delete toRemove;
                    size--;
                }
            }
                        //recursive call for remove
            return myRemove(key, --prefix, temp);
        }	 
        //calculate the depth of the tree from a given node
        int depth(trieNode* node){
            if(node== NULL) return 0;
            else{
                int Ldepth = depth(node->left);
                int Rdepth = depth(node->right);

                if(Ldepth > Rdepth) return (Ldepth+1); //get the max depth
                else return (Rdepth+1);
            }
        }
    };
//convert to binary
string to_binary(string key){
    int num = stoi(key); //convert string to int
    string ans;
    for (int i = 0; i < 8; i++){
        if(num%2 == 1) ans = "1" + ans;
        else ans = "0" + ans;
        num = num/2;
    }
    return ans;
}

int main(int argc , char** argv){
    //read from file
    string line;
    ifstream readFromFile(argv[1]);
    trie *tr = new trie();
    while (getline (readFromFile, line)) {
        istringstream ss(line);
        string operation, ipAddress;
        char action;
        ss>>operation>>ipAddress>>action;

        //split the IP adress and prefix
        string temp = ipAddress;
        replace(temp.begin(), temp.end(), '/', ' ');
        stringstream splitPrefix(temp);
        string ip; //ip address
        int prefix; // prefix

        splitPrefix >> ip >> prefix;
        replace(ip.begin(), ip.end(), '.', ' ');
        //split the IP adress and convert to binary
        stringstream split(ip);
        string number, key;
        //convert to binary;
        while (split >> number){
            key = key + to_binary(number);
        }

        if(line!=""){
            if(operation == "ADD"){
                tr->add(key, prefix, action);
                cout << "Added " << ipAddress << " " << action << " at the depth " << (tr->depth(tr->root)-1) << ", total nodes " <<  tr->size << endl;
            }
            else if(operation == "REMOVE"){
                if(tr->remove(key, prefix, action)){
                    cout<< "Removed " << ipAddress << " " << action << " at the depth " <<  (tr->depth(tr->root)-1) << ", total nodes " << tr->size <<endl;
                }
                else cout << "There is no " << ipAddress << " " << action << endl;
                
            }
            else if(operation == "FIND"){//FIND
                int steps = tr->find(key);
                if(steps > 0){
                    cout << "Found " << ipAddress << " at the depth " << steps << endl;
                }
                else cout << ipAddress << " not found" << endl;
            }
            else{
                throw("illegal operation");
            }
        }
    }
}
