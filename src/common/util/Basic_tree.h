
#ifndef BASIC_TREE_H
#define BASIC_TREE_H

template <class Data> class Tree_node;
template <class Data> class Tree_iter;

#include <fstream.h>

#ifndef IO_STREAM_H
#include "IO_stream.h"
#endif

#ifndef PERSIST_METHOD_H
#include "Persist_method.h"
#endif

template <class Data> class Basic_tree_node<Data>;

template <class Data> class Basic_tree
{
  // stream insertion and extraction use binary format
  friend ostream& operator << (ostream&, const Basic_tree<Data>&);
  friend istream& operator >> (istream&, Basic_tree<Data>&);

public:
  enum {BINARY_MODE='b', ASCII_MODE='a', DEF_CHILD_COUNT=4};

  typedef Data value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef const value_type& const_reference;
  typedef ptrdiff_t difference_type; /**/
  typedef size_t size_type;
  typedef Basic_tree_iter<Data> iterator;
  typedef const iterator const_iterator;

  /**/
  typedef Basic_tree_node<Data>* Basic_tree_node_p_t;
  typedef Basic_tree<Data> Val_t;
  typedef Basic_tree<Data>& Ref_t;
  typedef const Basic_tree<Data>& Const_ref_t;
  /**/

  Basic_tree();
  Basic_tree(const Data& value, int def_child_cnt); 
  Basic_tree(Const_ref_t other); 	

  virtual ~Basic_tree();

  Ref_t	operator=(Const_ref_t other);

  // appends new child node containing value to children of parent 
  virtual iterator append_def_child(const iterator& parent);
  virtual iterator append_child(const iterator& parent, const Data& value);

  // inserts at_index a child node with given value to children of parent
  virtual iterator insert_def_child(const iterator& par, int at_index);
  virtual iterator insert_child(const iterator& par, const Data& value, 
				int at_index);

  // inserts a sibling node with val just before a_child (cannot be root)
  virtual iterator insert_def_at(iterator& a_child);
  virtual iterator insert_at(iterator& a_child, const Data& val);

  // inserts a tree oth so oth.root() becomes the last child of parent
  virtual iterator append_child(const iterator& parent, Ref_t other);

  // inserts a tree other so that its root becomes the at_index 
  // child of parent
  virtual iterator insert_child(const iterator& parent, Ref_t other,
			    int at_index);

  // insert a tree other so that its root becomes the previous sibling
  // of a_child
  virtual iterator insert_at(iterator& a_child, Ref_t other);
  
  // prune() returns the sibling index of the_node, or -1 if it is the root
  virtual int prune(const iterator& the_node, Ref_t pruned_branch);

  virtual void clear();  
  bool is_empty() const;  

  void write(O_stream& st, const Persist_method<Data>& pers
	     =Persist_method<Data>()) const;
  int read(I_stream& st, const Persist_method<Data>& pers
	    =Persist_method<Data>());

  iterator end();                     // FORWARD
  iterator root();                    // FORWARD

  int node_count() const;
  void set_def_child_count(int def_child_count);
   
protected:
  void equate(Const_ref_t);
	
  iterator insert(const iterator& parent, Basic_tree_node_p_t a_node,
		    int at_index);
  iterator insert_at(iterator& a_child, Basic_tree_node_p_t a_node);

  iterator append(const iterator& parent, Basic_tree_node_p_t a_node);
  iterator make_root(Basic_tree_node_p_t a_node);

  void count_nodes() const;

  Basic_tree_node_p_t d_root;
  int d_default_cnt;
  int d_node_cnt;
  friend class Basic_tree_iter<Data>;
};

//---------- Basic_tree_iter declaration ----------

template <class Data> class Basic_tree_iter
{
public:

  typedef Basic_tree_iter<Data> Val_t;
  typedef A_STD vector<Val_t A_STD_ALLOC > Val_tv;

  typedef Basic_tree_iter<Data>& Ref_t;
  typedef const Basic_tree_iter<Data>& Const_ref_t;
  enum DIRECTION {FORWARD, REVERSE}; 
  // ++ is preorder traversal (bottom-up, child first) 
  // -- is postorder traversal (top-down, parent first) 
  // FORWARD++scans LeftToRight, FORWARD-- RightToLeft 
  // REVERSE++scans RightToLeft, REVERSE-- LeftToRight 
 
 Basic_tree_iter()
	  : d_tree(NULL), d_node(NULL), d_direction(FORWARD) {}

  Basic_tree_iter(Basic_tree<Data>& the_tree)
    : d_tree(&the_tree), d_node(the_tree.d_root), d_direction(FORWARD) {}

  Basic_tree_iter(Const_ref_t other)
    : d_tree(other.d_tree), d_node(other.d_node), 
    d_direction(other.d_direction) {}

  ~Basic_tree_iter() {}

  Ref_t operator=(Const_ref_t other);
  bool operator==(Const_ref_t other) const;
  bool operator != (Const_ref_t oth) const {return ! (*this == oth);}

  // is it a valid iterator?
  bool operator()() const;

  // make it iterate through a different tree
  void reset(Basic_tree<Data>& a_tree);

  // some accessors
  Data& operator*();
  Data operator*() const;

  // Get the tree from the iterator
  Basic_tree<Data>* tree_p() const;

  // How many children does the node of the iterator have
  int child_count() const;

  // Various iteration methods. Can be chained.

  Val_t get_parent() const;
  Ref_t goto_parent();
  Val_t get_child(int index) const;
  Ref_t goto_child(int index);

  Ref_t operator++();          //bott-up, FORWARD:L-R, REV:R-L
  const Val_t operator++(int); //bott-up, postfix
  Ref_t operator--();          //top-down, FORWARD:R-L, REV:L-R 
  const Val_t operator--(int); //top-down, postfix

  // does it scan children left to right or the opposite?
  void set_direction(DIRECTION the_dir);

  bool reserve_children(int child_count);

  Basic_tree<Data>::iterator left_most() const;    // FORWARD
  Basic_tree<Data>::iterator right_most() const;   // REVERSE 

private:
  Basic_tree_iter(Basic_tree<Data>* the_tree_p,
		  Basic_tree_node<Data>* the_node_p, 
		  DIRECTION the_dir=FORWARD)
    : d_tree(the_tree_p), d_node(the_node_p), d_direction(the_dir) {}

  void next_for();
  void prev_for();
  void next_rev();
  void prev_rev();

  Basic_tree<Data>* d_tree;
  Basic_tree_node<Data>* d_node;
  DIRECTION d_direction;
  friend class Basic_tree<Data>;
};

#ifdef INCLUDE_TEMPLATE_IMPL
#include "Basic_tree.cpp"
#endif

#endif // BASIC_TREE_H
