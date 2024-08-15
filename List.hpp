#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>
#include <iterator> 
#include <cassert> 

//Doubly Linked
template <typename T>
class List {
public:

  //EFFECTS:  returns true if the list is empty
  bool empty() const {
    return num == 0;
  }

  //EFFECTS: returns the number of elements in this List
  int size() const {
    return num;
  }

  //REQUIRES: list is not empty
  T & front() {
    assert(!empty());
    return first->datum;
  }

  //EFFECTS: Returns the last element in the list by reference
  T & back() {
    assert(!empty());
    return last->datum;
  }

  //EFFECTS:  inserts datum into the front of the list
  void push_front(const T &datum) {
    Node *n = new Node;
    n->datum = datum;
    n->prev = nullptr;
    if (empty()) {
      last = n;
      n->next = nullptr;
    }
    else {
      n->next = first;
      first->prev = n;
    }
    first = n;
    num++;
  }

  //EFFECTS:  inserts datum into the back of the list
  void push_back(const T &datum) {
    Node *n = new Node;
    n->datum = datum;
    n->next = nullptr;
    if (empty()) {
      n->prev = nullptr;
      first = n;
    }
    else {
      n->prev = last;
      last->next = n;
    }
    last = n;
    num++;
  }

  //EFFECTS:  removes the item at the front of the list
  void pop_front() {
    assert(!empty());
    Node *toDel = first;
    if (num == 1){
      first = nullptr;
      last = nullptr;
    }
    else {
      first = first->next;
      first->prev = nullptr;
    }
    delete toDel;
    num--;
  }

  //EFFECTS:  removes the item at the back of the list
  void pop_back(){
    assert(!empty());
    Node* toDel = last;
    if (num == 1) {
      first = nullptr;
      last = nullptr;
    }
    else {
      last = last->prev;
      last->next = nullptr;
    }
    delete toDel;
    num--;
  }

  //EFFECTS:  removes all items from the list
  void clear(){ 
    while (first) {
      pop_front();
    }
  }

  List(): first(nullptr), last(nullptr), num(0){}
  ~List() {
    clear();
  }
  List(List<T> &other) {
    copy_all(other);
  }
  List<T> &operator=(const List<T> &other) {
    if (this == &other) {
      return *this;
    }
    clear();
    copy_all(other);
    return *this;
  }



private:
 
  //EFFECTS:  copies all nodes from other to this
  void copy_all(const List<T> &other) {
    for (List<T>::Iterator it = other.begin(); it != other.end(); ++it) {
      this->push_back(*it);
    }
  }
  struct Node {
    Node *next;
    Node *prev;
    T datum;
  };
  Node *first;   // points to first Node in list, or nullptr if list is empty
  Node *last;    // points to last Node in list, or nullptr if list is empty
  int num;

public:
  class Iterator {
  public:

    Iterator() : list_ptr(nullptr), node_ptr(nullptr){}

      T &operator*() const {
        assert(node_ptr);
        return node_ptr->datum;
      }

      Iterator &operator++() {
        assert(node_ptr);
        node_ptr = node_ptr->next;
        return *this;
      }

      Iterator operator++(int) {
        assert(node_ptr);
        Iterator val = *this;
        operator++();
        return val;
      }

      bool operator==(Iterator r) const {
        return (node_ptr == r.node_ptr) && (list_ptr == r.list_ptr);
      }

      bool operator!=(Iterator r) const {
        return (node_ptr != r.node_ptr) || (node_ptr != r.node_ptr);
      }
  
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    // EFFECTS:  moves this Iterator to point to the previous element
    //           and returns a reference to this Iterator
    Iterator& operator--() { 
      assert(list_ptr);
      assert(*this != list_ptr->begin());
      if (node_ptr) {
        node_ptr = node_ptr->prev;
      } else { 
        node_ptr = list_ptr->last;
      }
      return *this;
    }

    // EFFECTS:  moves this Iterator to point to the previous element
    //           and returns a copy of the original Iterator
    Iterator operator--(int ) { 
      Iterator copy = *this;
      operator--();
      return copy;
    }

 
    // EFFECTS: returns the underlying element by pointer
    T* operator->() const {
      return &operator*();
    }

  private:
    const List *list_ptr; 
    Node *node_ptr; 
    friend class List;

    Iterator(const List *lp, Node *np): list_ptr(lp), node_ptr(np){}

  };

  // return an Iterator pointing to the first element
  Iterator begin() const {
    return Iterator(this, first);
  }

  // return an Iterator pointing to "past the end"
  Iterator end() const {
    return Iterator(this, last->next);
  }

  
  //EFFECTS: Removes a single element from the list container.
  //         Returns An iterator pointing to the element that followed the
  //         element erased by the function call
  Iterator erase(Iterator i) {
    if (empty()) {
      return this->end();
    } 
    if (this->last == i.node_ptr){
      if (num == 1) {
        first = nullptr;
        last = nullptr;
      }
      else {
        last = i.node_ptr->prev;
        last->next = nullptr;
      }
      delete i.node_ptr;
      num--;
      return this->end();
    }
    if (this->first == i.node_ptr) {
      if (num == 1) {
        first = nullptr;
        last = nullptr;
      }
      else {
        first = i.node_ptr->next;
        first->prev = nullptr;
      }
      delete i.node_ptr;
      num--;
      return this->begin();
    }
    else {
      i.node_ptr->prev->next = i.node_ptr->next;
      i.node_ptr->next->prev = i.node_ptr->prev;
      Node* val = i.node_ptr;
      i.node_ptr = i.node_ptr->next;
      delete val;
      num--;
      return i;
    }
  }

  //EFFECTS: Inserts datum before the element at the specified position.
  //         Returns an iterator to the the newly inserted element.
  Iterator insert(Iterator i, const T &datum){
    Node *insertNode = new Node;
    insertNode->datum = datum;
    insertNode->next = i.node_ptr;
    if (i.node_ptr == first) {
      first = insertNode;
      insertNode->prev = nullptr;
      insertNode->next = i.node_ptr;
      if (i.node_ptr) {
        i.node_ptr->prev = insertNode;
      }
      if (num == 0) {
        last = insertNode;
      }
      if (num == 1) {
        last = i.node_ptr;
      }
    }
    else {
      if (i == this->end()) {
	      insertNode->prev = last;
	      last->next = insertNode;
	      last = insertNode;
  	  } 
      else {
	      insertNode->prev = i.node_ptr->prev;
	      i.node_ptr->prev->next = insertNode;
	      i.node_ptr->prev = insertNode;
	    }
    }
    num++;
    return Iterator(this, insertNode);
    
  }
};

#endif 
