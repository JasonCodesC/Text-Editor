#ifndef TEXTBUFFER_HPP
#define TEXTBUFFER_HPP

#include <list>
#include <string>


class TextBuffer {
  
  using CharList = std::list<char>;
  using Iterator = std::list<char>::iterator;
  

private:
  CharList data;           // linked list that contains the characters
  Iterator cursor;         // iterator to current element in the list
  int row;                 // current row
  int column;              // current column
  int index;               // current index

  
public:
 
  TextBuffer();
  bool forward();
  bool backward();
  void insert(char c);
  bool remove();
  void move_to_row_start();
  void move_to_row_end();
  void move_to_column(int new_column);
  bool up();
  bool down();
  bool is_at_end() const;
  char data_at_cursor() const;
  int get_row() const;
  int get_column() const;
  int get_index() const;
  int size() const;
  std::string stringify() const;

private:
  
  int compute_column() const;
  
};

#endif // TEXTBUFFER_HPP
