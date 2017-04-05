/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/linked_list.cpp                                                 */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "linked_list.h"
using namespace std;


/* Constructor */
linked_list::linked_list() {}

void linked_list::Init(int M, int b)
{
    head_pointer = (char*) malloc(M);
    block_size = b;
    mem_size = M;
    max_data_size = b-sizeof(node*);
    initialized = true;
    free_data_pointer = free_pointer = front_pointer = (node*) head_pointer;
}

void linked_list::Destroy()
{
    free(head_pointer);
}

/* Insert an element into the list with a given key, given data element, and with a given length*/
void linked_list::Insert (int k, char * data_ptr, int data_len)
{
    if ((free_data_pointer + block_size) > (front_pointer + mem_size)) {
        //not enough room to add another node
        cerr << "not enough space for new node " << k << endl;
        return;
    } else if (data_len > (block_size - 16)) {
        //too long of a string to fit inside space given for node
        cerr << "string is too long for node " << k << endl;
        return;
    }
    free_pointer->next = (node*) free_data_pointer;
    node* new_node = (node*) free_data_pointer;
    new_node->next = NULL;
    new_node->key = k;
    new_node->value_len = data_len;
    memcpy((free_data_pointer + 16), data_ptr, data_len);
    free_data_pointer = free_data_pointer + block_size;
    free_pointer = new_node;
}


int linked_list::Delete(int delete_key)
{
    node* pnode = (node*) front_pointer;
    bool found = false;
    node* node_deleter = NULL;
    if (pnode->key == delete_key)
    {
        front_pointer = front_pointer->next;
        return delete_key;
    } else {
        node_deleter = front_pointer->next;
        while (found == false) {
            if ((pnode->next == free_pointer && pnode->next->key != delete_key) || pnode->next == NULL) {
                found = true;
                cerr << "Node not found. " << std::endl;
            } else if (pnode->next->key == delete_key) {
                if (pnode->next == free_pointer) {
                    free_pointer = pnode;
                    pnode->next = NULL;
                } else {
                    pnode->next = pnode->next->next;
                }
                found = true;
                return delete_key;
            } else {
                pnode = pnode->next;
            }
        }
    }
    return NULL;
}

/* Iterate through the list, if a given key exists, return the pointer to it's node */
/* otherwise, return NULL                                                           */
struct node* linked_list::Lookup(int lookup_key)
{
    node* pnode = (node*) front_pointer;
    while (pnode != NULL)
    {
        if(pnode->key == lookup_key){
            return ((node*)pnode);
        }
        pnode = pnode->next;
    }
    return NULL;
}

/* Prints the list by printing the key and the data of each node */
void linked_list::PrintList()
{
    node* pnode = (node*) front_pointer;
    if(pnode == NULL) {
        cout << "The list is empty. Try adding something first. " << endl;
        return;
    }
    while(pnode != NULL)
    {
        cout << "Node: " << std::endl;
        cout << " - Key: " << pnode->key << std::endl;
        cout << " - Data: " << pnode->value_len << std::endl;
        pnode = pnode->next;
    }
    
    /* IMPORTANT NOTE!
     *
     * In order for the script that will grade your assignment to work
     * (i.e. so you get a grade higher than a 0),
     * you need to print out each member of the list using the format below.
     * Your print list function should be written as a while loop that prints
     * the following three lines exactly for each node and nothing else.  If
     * you have any difficulties, talk to your TA and he will explain it further.
     *
     * The output lines that you should use are provided so that you will know
     * exactly what you should output.
     */
    //std::cout << "Node: " << std::endl;
    //std::cout << " - Key: " << <KEY GOES HERE!> << std::endl;
    //std::cout << " - Data: " << <KEY GOES HERE!> << std::endl;
    
    /* Short example:
     *   - Assume that you have a list with 4 elements.
     *     Your output should appear as follows
     *
     * Node:
     *  - Key: 1
     *  - Data: Hello
     * Node:
     *  - Key: 2
     *  - Data: World!
     * Node:
     *  - Key: 3
     *  - Data: Hello
     * Node:
     *  - Key: 4
     *  - Data: World!
     *
     * ^^ Your output needs to exactly match this model to be counted as correct.
     * (With the exception that the values for key and data will be different
     * depending on what insertions you perform into your list.  The values provided
     * here are for pedagogical purposes only)
     */
}

/* Getter Functions */

char* linked_list::getHeadPointer()
{
    return head_pointer;
}

node* linked_list::getFrontPointer()
{
    return front_pointer;
}

node* linked_list::getFreePointer()
{
    return free_pointer;
}

node* linked_list::getFreeDataPointer()
{
    return free_data_pointer;
}

int linked_list::getBlockSize()
{
    return block_size;
}

int linked_list::getMemSize()
{
    return mem_size;
}

int linked_list::getMaxDataSize()
{
    return max_data_size;
}

bool linked_list::getInitialized()
{
    return initialized;
}

/* Setter Functions */
void linked_list::setHeadPointer(char *new_pointer)
{
    
}

void linked_list::setFrontPointer(node* new_pointer)
{
    
}

void linked_list::setFreePointer(node* new_pointer)
{
    
}

void linked_list::setFreeDataPointer(node* new_pointer)
{
    
}

void linked_list::setBlockSize(int new_block_size)
{
    
}

void linked_list::setMemSize(int new_mem_size)
{
    
}

void linked_list::setMaxDataSize(int new_max_data_size)
{
    
}

void linked_list::setInitialized(bool new_initialized)
{
    
}
