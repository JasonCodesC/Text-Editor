#include <list>
#include <string>
#include "TextBuffer.hpp"

  //EFFECTS: Creates an empty text buffer. Its cursor is at the past-the-end
  //         position, with row 1, column 0, and index 0.
  TextBuffer::TextBuffer() : row(1), column(0), index(0)  {
    data = std::list<char>();
    cursor = data.end();
  }


  //EFFECTS:  Moves the cursor one position forward and returns true,
  //          unless the cursor is already at the past-the-end position,
  //          in which case this does nothing and returns false.
  bool TextBuffer::forward() {
    if (cursor != data.end()) {
	    index++;
	    column++;
	    if (*cursor == *"\n") {
	        column = 0;
	        row++;
	    }
	    cursor++;
	    return true;
	}
	return false;
  }


  //EFFECTS:  Moves the cursor one position backward and returns true,
  //          unless the cursor is is already at the first character in
  //          the buffer, in which case this does nothing and returns false.
  //HINT:     Implement and use the private compute_column() member
  //          function to compute the column when moving left from the
  //          beginning of a line to the end of the previous one.
  bool TextBuffer::backward() {
    if (cursor != data.begin()) {
	    index--;
	    cursor--;
	    if (column == 0) {
	      column = compute_column();
	      row--;
	    } 
        else {
            column--;
	    }
	    return true;
	  }
	  return false;
  }

 
  //EFFECTS:  Inserts a character in the buffer before the cursor position.
  //          If the cursor is at the past-the-end position, this means the
  //          inserted character is the last character in the buffer.
  //          The cursor remains in the same place as before the insertion.
  void TextBuffer::insert(char c) {
    cursor = data.insert(cursor, c);
    forward();
  }


  //EFFECTS:  Removes the character from the buffer that is at the cursor and
  //          returns true, unless the cursor is at the past-the-end position,
  //          in which case this does nothing and returns false.
  //          The cursor will now point to the character that was after the
  //          deleted character, or the past-the-end position if the deleted
  //          character was the last one in the buffer.
  bool TextBuffer::remove() {
    if (is_at_end()) {
        return false;
    }
    cursor = data.erase(cursor);
    return true;
  }


  //EFFECTS:  Moves the cursor to the start of the current row (column 0).
  void TextBuffer::move_to_row_start() {
    for (int i = 0; i < column; ++i) {
        backward();
    }
  }


  //EFFECTS:  Moves the cursor to the end of the current row (the
  //          newline character that ends the row, or the past-the-end
  //          position if the row is the last one in the buffer).
  void TextBuffer::move_to_row_end() {
    while (*cursor != *"\n" || cursor != data.end()) {
        forward();
    }
  }


  //EFFECTS:  Moves the cursor to the given column in the current row,
  //          if it exists. If the row does not have that many columns,
  //          moves to the end of the row (the newline character that
  //          ends the row, or the past-the-end position if the row is
  //          the last one in the buffer).
  void TextBuffer::move_to_column(int new_column) {
    
    if (new_column > column) {
        while (*cursor != *"\n" && cursor != data.end() && new_column > column) {
            forward();
        }
    }
    else {
        while (column > new_column) {
            backward();
        }
    }
    
    
  }


  //EFFECTS:  Moves the cursor to the previous row, retaining the
  //          current column if possible. If the previous row is
  //          shorter than the current column, moves to the end of the
  //          previous row (the newline character that ends the row).
  //          Does nothing if the cursor is already in the first row.
  //          Returns true if the position changed, or false if it did
  //          not (i.e. if the cursor was already in the first row).
  bool TextBuffer::up() {
	int old = column;
	if (row == 1) {	   
        return false;
	}
	if (!backward()) {
	    return false;
	}
	while (column != old) {
        if (*cursor == *"\n" && column < old) {
	      break;
	    }
	    if (!backward()) {
	      return false;
	    }
	}
	return true;
 
}


  //EFFECTS:  Moves the cursor to the next row, retaining the current
  //          column if possible. If the next row is shorter than the
  //          current column, moves to the end of the next row (the
  //          newline character that ends the row, or the past-the-end
  //          position if the row is the last one in the buffer). Does
  //          nothing if the cursor is already in the last row.
  //          Returns true if the position changed, or false if it did
  //          not (i.e. if the cursor was already in the last row).
  bool TextBuffer::down() {
    int old = column;
	Iterator old_cursor = cursor;
	if (!forward()) {
	    return false;
	}
	while (column != old) {
	    if (cursor == data.end()) {
	      move_to_column(old);
	      return false;
	    }
	    if ((cursor == data.end() || (*cursor == *"\n" && column < old))) {
	      break;
	    }
	    if (!forward()) {
	      cursor = old_cursor;
	      column = old;
	      return false;
	    }
	 }
	return true;
  }

  //EFFECTS:  Returns whether the cursor is at the past-the-end position.
  bool TextBuffer::is_at_end() const {
    return (cursor == data.end());
  }

  //EFFECTS:  Returns the character at the current cursor
  char TextBuffer::data_at_cursor() const {
    return *cursor;
  }

  
  int TextBuffer::get_row() const {
    return row;
  }

  
  int TextBuffer::get_column() const {
    return column;
  }

  
  int TextBuffer::get_index() const {
    if (is_at_end()) {
        return size();
    }
    return index;
  }

  //EFFECTS:  Returns the number of characters in the buffer.
  int TextBuffer::size() const {
    return data.size();
  }

  //EFFECTS:  Returns the contents of the text buffer as a string.
  std::string TextBuffer::stringify() const {
    return std::string(data.begin(), data.end());
  }


  //EFFECTS: Computes the column of the cursor within the current row.
  int TextBuffer::compute_column() const {
    int col = -1;
	Iterator copy = cursor;
	if (copy == data.end() || *copy == *"\n") {
	    col = 0;
       	if (copy != data.begin()) {
	      copy--;
	    }
	}
	while (copy != data.begin() && *copy != *"\n") {
        copy--;
	    col++;
	}
	col += (copy == data.begin() && *copy != *"\n");
	return col;
  }



