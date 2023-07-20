//
//  main.cpp
//  Project 4 CS32
//
//  Created by Colin Bresnahan on 5/26/22.
//


#include "NameTable.h"
#include <string>
#include <vector>
#include <list>
using namespace std;


struct Node // this is going to be what is stored in the hash table
{
    Node(string name, int line, int scope) : m_name(name), m_line(line), m_scope(scope){}
    string m_name;
    int m_line;
    int m_scope;
};

class hashTable
{
    
public:
    hashTable() {table = new list<Node>[numBuckets];}// constructor makes a table which is an array of linked lists
    ~hashTable();
    int hashFunction(const string& id) const; // this is the hash funciton
    int find(const string& id) const;
    int findscope(const string& id) const;
    void insert(Node& var);
    void remove(string& id);
private:
    list<Node> *table; //pointer to the hash table
    int numBuckets = 19997;
};



class NameTableImpl
{
  public:
    void enterScope();
    bool exitScope();
    bool declare(const string& id, const int& lineNum);
    int find(const string& id) const;
  private:
    vector<string> m_ids; // i am going to use the vector of ids to control what gets popped off when we exit scope
    hashTable vars; // hashtable for the name table
    int m_scope= 0;

};



void NameTableImpl::enterScope()
{
      // Extend the id vector with an empty string that
      // serves as a scope entry marker.

    m_ids.push_back(""); // this is how we keep track of variables in the scope
    m_scope++;
    
}

bool NameTableImpl::exitScope()
{
      // Remove ids back to the last scope entry.

    while (!m_ids.empty()  &&  m_ids.back() != "")
    {
        string id = m_ids.back();
        m_ids.pop_back();
        vars.remove(id); // this removes the node from the hash table
    }
    if (m_ids.empty())
        return false;

      // Remove the scope entry marker itself.
    m_ids.pop_back();
    m_scope--;
    
    return true;
}

bool NameTableImpl::declare(const string& id,const int& lineNum)
{
    if (id.empty())
        return false;

    if(vars.findscope(id) == m_scope) // this is to check we already have the variable in the scope
        return false;

      // Save the declaration

    m_ids.push_back(id);
    Node temp(id, lineNum, m_scope);
    vars.insert(temp);
    return true;
}

int NameTableImpl::find(const string& id) const
{
    if (id.empty())
        return -1;
    

      // Search back for the most recent declaration still
      // available.

    return vars.find(id);
    

}

//***** HASH TABLE IMPLEMENTATION
void hashTable::insert(Node& var)
{
    int i = hashFunction(var.m_name);
    table[i].push_front(var); // add the node to the linked list in the
}

void hashTable::remove(string& id)
{
    int i = hashFunction(id);
    list<Node>::iterator it = table[i].begin(); //giving the iterator the begining of the linked list
    while(it->m_name != id) //CHECKING FOR COLLISIONS
    {
        it++;
        if(it == table[i].end())
            return;
    }
    table[i].erase(it); // this is the part that removes the no
}

int hashTable::hashFunction(const string& id) const // THIS IS THE HASH FUNCTION
{
    unsigned int h = 216613626u; // the u marks it as an unsigned int
    for(int i = 0; i < id.length(); i++)
    {
        char c = id[i];
        h+= c;
        h*= 16777619;
    }
    
    return (h) % numBuckets;
}

int hashTable::find(const string& id) const //DO WE NEED TO SEE IF IT IS NOT IN IT
{
    int i = hashFunction(id);
    
    if(table[i].empty())
        return -1;
    
    
    
    list<Node>::iterator it = table[i].begin(); //giving the iterator the begining of the linked list

    while(it->m_name != id) //CHECKING FOR COLLISIONS
    {
        it++;
        if(it == table[i].end())
            return -1;
    }
    return it->m_line; // once we find the correct name we return the line
}


int hashTable::findscope(const string& id) const // this is so we can access lastest scope of the id
{
    int i = hashFunction(id);
    
    if(table[i].empty())
        return -1;
    
    
    list<Node>::iterator it = table[i].begin(); //giving the iterator the begining of the linked list
    
    while(it->m_name != id) //CHECKING FOR COLLISIONS
    {
        it++;
        if(it == table[i].end())
            return -1;
    }
    return it->m_scope; // once we find the correct name we return the line
}

hashTable::~hashTable() {
    
    for(int i = 0; i < numBuckets; i++){
        table[i].clear();
    }
    delete [] table;
}

//*********** NameTable functions **************

// For the most part, these functions simply delegate to NameTableImpl's
// functions.

NameTable::NameTable()
{
    m_impl = new NameTableImpl;
}

NameTable::~NameTable()
{
    delete m_impl;
}

void NameTable::enterScope()
{
    m_impl->enterScope();
}

bool NameTable::exitScope()
{
    return m_impl->exitScope();
}

bool NameTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int NameTable::find(const string& id) const
{
    return m_impl->find(id);
}
