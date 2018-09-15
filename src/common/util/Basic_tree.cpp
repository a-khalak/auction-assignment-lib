#ifdef BASIC_TREE_H

#include <vector>
#include "e_assert.h"

//---------- Basic_tree_node class ----------

template <class Data> class Basic_tree_node
{
protected:
  typedef Basic_tree_node<Data> TREE_NODE_TYPE;
  typedef A_STD vector<TREE_NODE_TYPE* A_STD_ALLOC> NODE_VEC;
  typedef NODE_VEC::iterator NODE_VEC_ITER;

  Basic_tree_node(int child_count)
    : d_parent(NULL), d_ndx(-1)
    { d_child.reserve(child_count);}

  ~Basic_tree_node() {}

  Basic_tree_node(const Data& value, int child_count)
    : d_parent(NULL), d_ndx(-1), d_data(value)
    {
      d_child.reserve(child_count);
    }

  Basic_tree_node<Data>* parent() { return d_parent; }
  void parent(Basic_tree_node<Data>* parent) { d_parent=parent; }

  TREE_NODE_TYPE* sibling(int i)
    {
      return parent()->child(i);
    }

  int sib_count()
    {
      return parent()->d_child.size();
    }

  int child_count() const { return d_child.size(); }

  TREE_NODE_TYPE* child(int i)
    {
      return d_child[i];
    }

  void append(TREE_NODE_TYPE* a_child)
    {
      d_child.push_back(a_child);
      a_child->d_parent = this;
      a_child->ndx(child_count()-1);
    }

  void insert(int child_index, TREE_NODE_TYPE* a_child)
    {
      d_child.insert(d_child.begin()+child_index, a_child);
      a_child->d_parent = this;

      int sz = child_count();
      for (int i=child_index; i<sz; ++i)
	d_child[i]->ndx(i);
    }

  void erase(int child_index)
    {
      int sz = child_count();
      for (int i=child_index+1; i<sz; ++i)
	d_child[i]->ndx(i-1);

      d_child.erase(d_child.begin() + child_index);
    }

  void ndx(int val) {d_ndx=val;}
  int ndx() {return d_ndx;}

  void reserve(int child_count)
    {
      d_child.reserve(child_count);
    }

  Basic_tree_node<Data>* d_parent;
  NODE_VEC d_child;
  int d_ndx;

  Data d_data;

  friend class Basic_tree<Data>;
  friend class Basic_tree_iter<Data>;

private:
  Basic_tree_node(const Basic_tree_node<Data>&); // not impl
  Basic_tree_node<Data>& operator=(const Basic_tree_node<Data>&); // not impl
};

// -------------Basic_tree_iter IMPLEMENTATION-----------------------

template <class Data>
Basic_tree_iter<Data>::Ref_t
Basic_tree_iter<Data>::operator=(Const_ref_t other)
{
  d_tree=other.d_tree;
  d_node=other.d_node;
  d_direction=other.d_direction;
  return *this;
}

template <class Data>
bool
Basic_tree_iter<Data>::operator==(Const_ref_t other) const
{
  if(d_tree==other.d_tree && d_node==other.d_node)
    return true;
  else
    return false;
}

template <class Data>
bool
Basic_tree_iter<Data>::operator()() const
{
  if (d_tree&&d_node)
    return true;
  else
    return false;
}

template <class Data>
void
Basic_tree_iter<Data>::reset(Basic_tree<Data>& a_tree)
{
  *this = a_tree.root();
}

template <class Data>
Data&
Basic_tree_iter<Data>::operator*()
{
  e_assert(d_node!=NULL, Logical_error);
  return d_node->d_data;
}

template <class Data>
Data
Basic_tree_iter<Data>::operator*() const
{
  e_assert(d_node!=NULL, Logical_error);
  return d_node->d_data;
}

template <class Data>
Basic_tree<Data>*
Basic_tree_iter<Data>::tree_p() const
{
  return d_tree;
}

template <class Data>
int
Basic_tree_iter<Data>::child_count() const
{
  e_assert(d_node!=NULL, Logical_error);
  return d_node->child_count();
}

template <class Data>
Basic_tree_iter<Data>::Val_t
Basic_tree_iter<Data>::get_parent() const
{
  e_assert(d_node!=NULL, Logical_error);
  return Basic_tree_iter<Data>(d_tree, d_node->d_parent);
}

template <class Data>
Basic_tree_iter<Data>::Ref_t
Basic_tree_iter<Data>::goto_parent()
{
  e_assert(d_node!=NULL, Logical_error);
  d_node=d_node->d_parent;
  return *this;
}

template <class Data>
Basic_tree_iter<Data>::Val_t
Basic_tree_iter<Data>::get_child(int index) const
{
  e_assert(d_node!=NULL, Logical_error);
  e_assert(index>=0 && index<child_count(), Logical_error);

  return Basic_tree_iter<Data>(d_tree, d_node->child(index));
}

template <class Data>
Basic_tree_iter<Data>::Ref_t
Basic_tree_iter<Data>::goto_child(int index)
{
  e_assert(d_node!=NULL, Logical_error);
  e_assert(index>=0 && index<child_count(), Logical_error);

  d_node=d_node->child(index);
  return *this;
}

template <class Data>
Basic_tree_iter<Data>::Ref_t
Basic_tree_iter<Data>::operator++()
{
  if (d_direction==FORWARD)
    next_for();
  else
    next_rev();
  return *this;
}

template <class Data>
const Basic_tree_iter<Data>::Val_t
Basic_tree_iter<Data>::operator++(int)
{
  Basic_tree_iter<Data> temp=*this;
  ++(*this);
  return temp;
}

template <class Data>
Basic_tree_iter<Data>::Ref_t
Basic_tree_iter<Data>::operator--()
{
  if (d_direction==FORWARD)
    prev_for();
  else
    prev_rev();

  return *this;
}

template <class Data>
const Basic_tree_iter<Data>::Val_t
Basic_tree_iter<Data>::operator--(int)
{
  Basic_tree_iter<Data> temp=*this;
  --(*this);
  return temp;
}

template <class Data>
void
Basic_tree_iter<Data>::set_direction(DIRECTION the_dir)
{
  d_direction = the_dir;
}

template <class Data>
bool
Basic_tree_iter<Data>::reserve_children(int child_count)
{
  if (child_count<=0 || d_node==0)
    return false;

  d_node->reserve(child_count);
  return true;
}

template <class Data>
void
Basic_tree_iter<Data>::next_for()
{
  if (d_node==NULL)
    *this = left_most();
  else if (d_node->d_parent==NULL)
    *this = d_tree->end();
  else
    {
      int next_sibling_index = d_node->ndx() + 1;

      if(next_sibling_index == d_node->sib_count())
	// done with siblings -- move to parent
	goto_parent();
      else
	{
	  // go as far down as possible, keeping as far left as possible
	  d_node=d_node->sibling(next_sibling_index);
	  while(d_node->child_count()>0)
            goto_child(0);
	}
    }
}

template <class Data>
void
Basic_tree_iter<Data>::prev_for()
{
  if (d_node==NULL)
    d_node=d_tree->d_root;
  else
    {
      // determine if iter can move down
      int ch_count=child_count();
      if (ch_count>0)
	goto_child(ch_count-1);        // rightmost child
      else
	{
	  // iterator points to a leaf
	  while(d_node->d_parent!=NULL)
	    {
	      if (d_node->ndx() > 0)
		{
		  d_node = d_node->sibling(d_node->ndx()-1);
		  break;
		}
	      else
		goto_parent();
	    }
	  if (d_node->d_parent==NULL)
	    {
	      *this = d_tree->end();  // walked past the beginning of tree
	    }
	}
    }
}

template <class Data>
void
Basic_tree_iter<Data>::next_rev()
{
  if (d_node==NULL)
    *this = right_most();
  else if (d_node->d_parent==NULL)
    {
      *this = d_tree->end();
      set_direction(REVERSE);
    }
  else
    {
      // see if current is the beginning of the sibling array
      if(d_node->ndx()==0)
	// done with siblings -- move to parent
	goto_parent();
      else
	{
	  d_node = d_node->sibling(d_node->ndx()-1);
	  // go as far down as possible, keeping as far right as possible
	  while(d_node->child_count()>0)
            goto_child(d_node->child_count()-1);
	}
    }
}

template <class Data>
void
Basic_tree_iter<Data>::prev_rev()
{
  if (d_node==NULL)
    d_node=d_tree->d_root;
  else
    {
      // determine if iter can move down
      int ch_count=child_count();
      if (ch_count>0)
	goto_child(0);        // leftmost child
      else
	{
	  // iterator points to a leaf
	  while(d_node->d_parent!=NULL)
	    {
	      if (d_node->ndx() + 1 < d_node->sib_count())
		{
		  d_node = d_node->sibling(d_node->ndx() + 1);
		  break;
		}
	      else
		goto_parent();
	    }
	  if (d_node->d_parent==NULL)
	    {
	      *this = d_tree->end();  // walked past the beginning of tree
	      set_direction(REVERSE);
	    }
	}
    }
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree_iter<Data>::left_most() const
{
  Basic_tree<Data>::iterator res=*this;
  if (res.d_node!=NULL)
    {
      while(res.d_node->child_count()>0)
	res.goto_child(0);
    }

  res.set_direction(FORWARD);
  return res;
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree_iter<Data>::right_most() const
{
  Basic_tree<Data>::iterator res=*this;
  if (res.d_node!=NULL)
    {
      while(res.d_node->child_count()>0)
	res.goto_child(res.d_node->child_count()-1);
    }
  res.set_direction(REVERSE);
  return res;
}

// ------------- Basic_tree IMPLEMENTATION -----------------

template <class Data>
Basic_tree<Data>::Basic_tree()
: d_root(NULL), d_default_cnt(DEF_CHILD_COUNT) ,d_node_cnt(0)
{
}

template <class Data>
Basic_tree<Data>::Basic_tree(const Data& value, int ch_count)
  : d_root(new Basic_tree_node<Data>(value, ch_count)), d_default_cnt(ch_count),
    d_node_cnt(1)
{
}

template <class Data>
Basic_tree<Data>::Basic_tree(Const_ref_t other)
  : d_root(NULL), d_default_cnt(DEF_CHILD_COUNT)
{
  equate(other);
}

template <class Data>
Basic_tree<Data>::~Basic_tree()
{
  clear();
}

template <class Data>
Basic_tree<Data>::Ref_t
Basic_tree<Data>::operator=(Const_ref_t other)
{
  if(this!=&other)
    {
      clear();
      equate(other);
    }
  return *this;
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::append_def_child(const iterator& parent)
{
  e_require(this==parent.d_tree);
  return append(parent, new Basic_tree_node<Data>(d_default_cnt));
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::append_child(const iterator& parent, const Data& value)
{
  e_require(this==parent.d_tree);
  return append(parent, new Basic_tree_node<Data>(value, d_default_cnt));
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::insert_def_child(const iterator& parent, int at_index)
{
  e_require(this==parent.d_tree);
  if (parent.d_node!=NULL)
    {
      e_require(at_index>=0);
      e_require(at_index<=parent.child_count());
    }
  return insert(parent,
		new Basic_tree_node<Data>(d_default_cnt), at_index);
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::insert_child(const iterator& parent,
			       const Data& value, int at_index)
{
  e_require(this==parent.d_tree);
  if (parent.d_node!=NULL)
    {
      e_require(at_index>=0);
      e_require(at_index<=parent.child_count());
    }
  return insert(parent,
		new Basic_tree_node<Data>(value, d_default_cnt), at_index);
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::insert_def_at(iterator& a_child)
{
  e_require(a_child.d_tree==this);
  e_require(a_child!=root());
  e_require(a_child!=end());

  return insert_at(a_child, new Basic_tree_node<Data>(d_default_cnt));
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::insert_at(iterator& a_child, const Data& val)
{
  e_require(a_child.d_tree==this);
  e_require(a_child!=root());
  e_require(a_child!=end());

  return insert_at(a_child, new Basic_tree_node<Data>(val, d_default_cnt));
}


template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::append_child(const iterator& parent, Ref_t other)
{
  e_require(this==parent.d_tree);
  if (other.is_empty())
    return parent;
  Basic_tree_node<Data>* the_root=other.d_root;
  other.d_root=NULL;
  return append(parent, the_root);
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::insert_child(const iterator& parent,
		    Ref_t other, int at_index)
{
  e_require(this==parent.d_tree);
  if (parent.d_node!=NULL)
    {
      e_require(at_index>=0);
      e_require(at_index<=parent.child_count());
    }

  if (other.is_empty())
    return parent;

  Basic_tree_node<Data>* the_root=other.d_root;
  other.d_root=NULL;
  return insert(parent, the_root, at_index);
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::insert_at(iterator& a_child, Ref_t other)
{
  e_require(a_child.d_tree==this);

  if (other.is_empty())
    return a_child;

  e_require(a_child!=root());
  e_require(a_child!=end());

  Basic_tree_node<Data>* the_root=other.d_root;
  other.d_root=NULL;
  return insert_at(a_child, the_root);
}

template <class Data>
int
Basic_tree<Data>::prune(const iterator& the_node,
                   Ref_t pruned_branch)
{
  e_require(the_node.d_tree==this);

  int at_index=-1;
  pruned_branch.clear();

  Basic_tree_node<Data>* tree_node=the_node.d_node;
  if (tree_node==NULL)
    return at_index;  //leave pruned_branch alone
  Basic_tree_node<Data>* parent=tree_node->d_parent;

  if (parent==NULL)
    {
      e_assert(tree_node==d_root, Logical_error);
      d_root = NULL;
      d_node_cnt = 0;
    }
  else
    {
      at_index = tree_node->ndx();
      parent->erase(at_index);

      // fix the node count
      count_nodes();
    }

  tree_node->d_parent=NULL;
  tree_node->ndx(-1);
  pruned_branch.d_root=tree_node;
  pruned_branch.count_nodes();

  return at_index;
}

template <class Data>
void
Basic_tree<Data>::clear()
{
  iterator iter = root().left_most(), curr;
  while(iter!=end())
    {
      // get next from curr BEFORE deleting curr
      curr=iter++;
      delete curr.d_node;
    }
  d_root=NULL;
  d_node_cnt=0;
}

template <class Data>
bool
Basic_tree<Data>::is_empty() const
{
  if (d_root)
    return false;
  else
    return true;
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::end()
{
  return iterator(this, NULL, iterator::FORWARD);
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::root()
{
  return Basic_tree_iter<Data>(this, d_root, iterator::FORWARD);
}

template <class Data>
int
Basic_tree<Data>::node_count() const
{
  return d_node_cnt;
}

template <class Data>
void
Basic_tree<Data>::count_nodes() const
{
  int node_cnt=0;
  Basic_tree<Data>& self = const_cast<Basic_tree<Data>& > (*this);
  iterator iter=self.root(), the_end=self.end();

  iter.set_direction(iterator::REVERSE);
  if (iter()==false)
    {
      self.d_node_cnt=0;
      return;
    }
  iter--; node_cnt++; // count the root
  while(iter!=the_end)
    {
      e_assert(iter.d_node->parent()!=NULL, Logical_error);
      iter--; node_cnt++; // count the node, decrement iter
    }
  self.d_node_cnt = node_cnt;
}

template <class Data>
void
Basic_tree<Data>::set_def_child_count(int ch_count)
{
  d_default_cnt = ch_count;
}

template <class Data>
void
Basic_tree<Data>::equate(Const_ref_t other)
{
  Basic_tree<Data>& oth = const_cast<Basic_tree<Data>& >(other);
  iterator oth_it=oth.root(), oth_end=oth.end();
  iterator self_it=root();

  if (oth_it()==false)
    return;

  // create the root node
  self_it = append(self_it, *oth_it);
  self_it.set_direction(iterator::REVERSE);
  oth_it.set_direction(iterator::REVERSE);
  // self_it, and oth_it are REVERSE iterators pointing to root

  // then for each node create all child nodes
  int i, sz;
  while(oth_it!=oth_end)
    {
      sz = oth_it.d_node->child_count();
      for (i=0; i<sz; ++i)
	append(self_it, oth_it.d_node->child(i)->d_data);

      --oth_it; --self_it;
    }
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::insert(const Basic_tree<Data>::iterator& a_parent,
		    Basic_tree_node_p_t the_node, int at_index)
{
  Basic_tree_node<Data>* parent_node=a_parent.d_node;

  // is it going to be root?
  if (parent_node==NULL)
    {
      e_assert(at_index==0, Logical_error);
      return make_root(the_node);
    }

  // can we append it?
  if (a_parent.child_count()==at_index)
    return append(a_parent, the_node);

  parent_node->insert(at_index, the_node);

  ++d_node_cnt;

  return Basic_tree_iter<Data>(this, the_node);
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::insert_at(Basic_tree<Data>::iterator& a_child,
		      Basic_tree_node_p_t a_node)
{
  iterator parentIter=a_child.get_parent();
  Basic_tree_node<Data>* parent_node=parentIter.d_node;

  int at_index = a_child.d_node->ndx();

  parent_node->insert(at_index, a_node);

  ++d_node_cnt;

  return Basic_tree_iter<Data>(this, a_node);
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::append(const Basic_tree<Data>::iterator& parent,
		    Basic_tree_node_p_t the_node)
{
  Basic_tree_node<Data>* parent_node=parent.d_node;
  if(parent_node==NULL)
    return make_root(the_node);

  parent_node->append(the_node);

  ++d_node_cnt;

  return Basic_tree_iter<Data>(this, the_node);
}

template <class Data>
Basic_tree<Data>::iterator
Basic_tree<Data>::make_root(Basic_tree_node_p_t the_node)
{
  e_require(the_node!=NULL);
  if (d_root!=NULL)
    the_node->append(d_root);

  the_node->parent((Basic_tree_node<Data>*)0);
  d_root=the_node;

  // we leave the_node.ndx to its default value of -1

  d_node_cnt = 1;

  return Basic_tree_iter<Data>(this, the_node);
}

// ---------- read and write ----------

template <class Data>
void
Basic_tree<Data>::write(O_stream& ostr, const Persist_method<Data>& pers) const
{
  unsigned char flag;

  if (ostr.is_binary())
    flag = BINARY_MODE;
  else
    flag = ASCII_MODE;

  ostr().write(&flag, sizeof(char));

  Basic_tree<Data>& self = const_cast<Basic_tree<Data>& > (*this);

  iterator iter=self.root(), the_end=self.end();
  iter.set_direction(iterator::REVERSE);

  ostr.write(node_count());
  if (node_count()==0) return;

  // save tree structure by child count per node run-length encoded
  int ch_cnt=0, curr_cnt, rep_cnt=0;
  while(1)
    {
      curr_cnt = iter.child_count();
      if (rep_cnt>0 && curr_cnt!=ch_cnt)
	{
	  ostr.write(rep_cnt); ostr.write(ch_cnt);
	  ch_cnt = curr_cnt;
	  rep_cnt = 1;
	}
      else if (rep_cnt==0)
	{
	  ch_cnt = curr_cnt;
	  rep_cnt = 1;
	}
      else
	++rep_cnt;

      iter--;

      if (iter==the_end)
	{
	  ostr.write(rep_cnt); ostr.write(ch_cnt);
	  break;
	}
    }

  iter=self.root();
  iter.set_direction(iterator::REVERSE);
  while(iter!=the_end)
    {
      pers.write(ostr, iter.d_node->d_data);
      iter--;
    }
}

template <class Data>
int
Basic_tree<Data>::read(I_stream& fs, const Persist_method<Data>& pers)
{
  int res = 1;
  unsigned char ch_flag;

  iterator iter;
  Basic_tree_node<Data>* child_node;

  int child_index, node_cnt, node_tally;

  clear();

  fs().read(&ch_flag, sizeof(char));
  if (fs().gcount()!=sizeof(char))
    return 0;

  if ( ch_flag == BINARY_MODE )
    fs.binary_mode(true);
  else
    fs.binary_mode(false);

  fs.read(node_cnt);
  if (!fs()) return 0;
  if (node_cnt==0) return 0;

  // enter the root since there is at least one node
  child_node = new Basic_tree_node<Data>(d_default_cnt);

  append(root(), child_node);
  node_tally = 1;
  iter = root();
  iter.set_direction(iterator::REVERSE);

  // for each node starting with root read the child_count
  // and create blank children - iter points to valid parent

  // get tree structure by child count per node run-length encoded
  int ch_cnt, rep_cnt;
  while (iter != end())
  {
    fs.read(rep_cnt); fs.read(ch_cnt);
    node_tally += (rep_cnt*ch_cnt);
    while (rep_cnt)
      {
	iter.reserve_children(ch_cnt);
	for (child_index=0; child_index<ch_cnt; ++child_index)
	  {
	    child_node = new Basic_tree_node<Data>(d_default_cnt);
	    append(iter, child_node);
	  }
	--iter;
	--rep_cnt;
      }
  }

  b_assert(node_tally==node_cnt);

  iter = root();
  iter.set_direction(iterator::REVERSE);
  while (iter!=end())
  {
    if (!pers.read(fs, iter.d_node->d_data))
      {res=0; break;}
    --iter;
  }
  return res;
}

// -------------Basic_tree friends IMPLEMENTATION--------------------

template <class Data>
ostream&
operator << (ostream& fs, const Basic_tree<Data>& a_tree)
{
  O_stream ofs(fs);
  ofs.binary_mode(false);
  a_tree.write(ofs);
  return fs;
}

template <class Data>
istream&
operator >> (istream& fs, Basic_tree<Data>& a_tree)
{
  I_stream ifs(fs);
  a_tree.read(ifs);
  return fs;
}

#endif // ifdef BASIC_TREE_H
