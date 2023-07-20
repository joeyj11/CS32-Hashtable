//
//  NameTable.h
//  Project 4 CS32
//
//  Created by Colin Bresnahan on 5/26/22.
//
#ifndef NAMETABLE_INCLUDED
#define NAMETABLE_INCLUDED

#include <string>

class NameTableImpl;

class NameTable
{
  public:
    NameTable();
    ~NameTable();
    void enterScope();
    bool exitScope();
    bool declare(const std::string& id, int lineNum);
    int find(const std::string& id) const;
      // We prevent a NameTable object from being copied or assigned
    NameTable(const NameTable&) = delete;
    NameTable& operator=(const NameTable&) = delete;

  private:
    NameTableImpl* m_impl;
};

#endif // NAMETABLE_INCLUDED
