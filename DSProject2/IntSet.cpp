// FILE: IntSet.cpp - header file for IntSet class
//       Implementation file for the IntStore class
//       (See IntSet.h for documentation.)
// INVARIANT for the IntSet class:
// (1) Distinct int values of the IntSet are stored in a 1-D,
//     dynamic array whose size is stored in member variable
//     capacity; the member variable data references the array.
// (2) The distinct int value with earliest membership is stored
//     in data[0], the distinct int value with the 2nd-earliest
//     membership is stored in data[1], and so on.
//     Note: No "prior membership" information is tracked; i.e.,
//           if an int value that was previously a member (but its
//           earlier membership ended due to removal) becomes a
//           member again, the timing of its membership (relative
//           to other existing members) is the same as if that int
//           value was never a member before.
//     Note: Re-introduction of an int value that is already an
//           existing member (such as through the add operation)
//           has no effect on the "membership timing" of that int
//           value.
// (4) The # of distinct int values the IntSet currently contains
//     is stored in the member variable used.
// (5) Except when the IntSet is empty (used == 0), ALL elements
//     of data from data[0] until data[used - 1] contain relevant
//     distinct int values; i.e., all relevant distinct int values
//     appear together (no "holes" among them) starting from the
//     beginning of the data array.
// (6) We DON'T care what is stored in any of the array elements
//     from data[used] through data[capacity - 1].
//     Note: This applies also when the IntSet is empry (used == 0)
//           in which case we DON'T care what is stored in any of
//           the data array elements.
//     Note: A distinct int value in the IntSet can be any of the
//           values an int can represent (from the most negative
//           through 0 to the most positive), so there is no
//           particular int value that can be used to indicate an
//           irrelevant value. But there's no need for such an
//           "indicator value" since all relevant distinct int
//           values appear together starting from the beginning of
//           the data array and used (if properly initialized and
//           maintained) should tell which elements of the data
//           array are actually relevant.
//
// DOCUMENTATION for private member (helper) function:
//   void resize(int new_capacity)
//     Pre:  (none)
//           Note: Recall that one of the things a constructor
//                 has to do is to make sure that the object
//                 created BEGINS to be consistent with the
//                 class invariant. Thus, resize() should not
//                 be used within constructors unless it is at
//                 a point where the class invariant has already
//                 been made to hold true.
//     Post: The capacity (size of the dynamic array) of the
//           invoking IntSet is changed to new_capacity...
//           ...EXCEPT when new_capacity would not allow the
//           invoking IntSet to preserve current contents (i.e.,
//           value for new_capacity is invalid or too low for the
//           IntSet to represent the existing collection),...
//           ...IN WHICH CASE the capacity of the invoking IntSet
//           is set to "the minimum that is needed" (which is the
//           same as "exactly what is needed") to preserve current
//           contents...
//           ...BUT if "exactly what is needed" is 0 (i.e. existing
//           collection is empty) then the capacity should be
//           further adjusted to 1 or DEFAULT_CAPACITY (since we
//           don't want to request dynamic arrays of size 0).
//           The collection represented by the invoking IntSet
//           remains unchanged.
//           If reallocation of dynamic array is unsuccessful, an
//           error message to the effect is displayed and the
//           program unconditionally terminated.

#include "IntSet.h"
#include <iostream>
#include <cassert>
using namespace std;

void IntSet::resize(int new_capacity)
{
   if (new_capacity < 1)
      new_capacity = 1;
   capacity = new_capacity;
   int * newData = new int[capacity];
   for (int i = 0; i < used; ++i)
      newData[i] = data[i];
   delete [] data;
   data = newData;
}

IntSet::IntSet(int initial_capacity) : capacity(initial_capacity),
used(0)
{
   if (capacity < 1)
      capacity = DEFAULT_CAPACITY;
   data = new int[capacity];
}

IntSet::IntSet(const IntSet& src) 
  : capacity(src.capacity), used(src.used)
{
   data = new int[capacity];
   for (int i = 0; i < used; ++i)
      data[i] = src.data[i];
}


IntSet::~IntSet()
{
   delete [] data;
}

IntSet& IntSet::operator=(const IntSet& rhs)
{
   if (this != &rhs)
   {
      int* newData = new int[rhs.capacity];
      for (int i = 0; i < rhs.used; ++i)
         newData[i] = rhs.data[i];
      delete [] data;
      data = newData;
      capacity = rhs.capacity;
      used = rhs.used;
   }
   return *this;
}

int IntSet::size() const
{
   return used;
}
/*
true is returned if invoking IntSet has zero relavent 
elements otherwise it returns false
*/
bool IntSet::isEmpty() const
{
   if(used == 0){
      return true;
   }else{
      return false;
   }
}
/*
true is returned if invoking IntSet has anInt as an 
element otherwise false is returned
*/
bool IntSet::contains(int anInt) const
{
   for(int i = 0; i < used; i++){
      if(data[i] == anInt){
         return 1;
      }else{
         return false;
      }
   }
}
/*
checks if the passed set is a subset of the active set
returns true otherwise returns false
*/
bool IntSet::isSubsetOf(const IntSet& otherIntSet) const
{
   for(int i = 0; i < used; i++){
      if(otherIntSet.contains(data[i]) == 0){
         return false;
      }else{
         return true;
      }
   }
}
/*
contents of invoking IntSet inserted into 'out' w/ 2 spaces 
seperating one item from another if 2 or more items
*/
void IntSet::DumpData(ostream& out) const
{  // already implemented ... DON'T change anything
   if (used > 0)
   {
      out << data[0];
      for (int i = 1; i < used; ++i)
         out << "  " << data[i];
   }
}
/*
an IntSet representing the union of the invoking IntSet and 
otherIntSet is returned
*/
IntSet IntSet::unionWith(const IntSet& otherIntSet) const
{
   // checks if the union of current IntSet and the other IntSet
   // at 'capacity' or below
   assert(size() + (otherIntSet.subtract(*this)).size() <= capacity);

   //copy of invoking IntSet
   IntSet unionIntSet = (*this);

   //added to set if loop through IntSet, if duplicated elements 
   //found, it won't union otherwise union
   for(int index = 0; index < otherIntSet.size(); ++index){
      if(!unionIntSet.contains(otherIntSet.data[index])){
         unionIntSet.add(otherIntSet.data[index]);
      }
   }
   return unionIntSet;
}
/*
IntSet representing the intersection of invoking IntSet and otherIntSet
is returned
*/
IntSet IntSet::intersect(const IntSet& otherIntSet) const
{
   //copy of invoking IntSet
   IntSet interSet = (*this);

   //compares each elements of invoking IntSet, if there is no match it 
   //removes that element from the IntSet
   for(int index = 0; index < size(); index++){
      if(!otherIntSet.contains(data[index])){
         interSet.remove(data[index]);
      }
   }
   return interSet;
}
/*
difference between the invoking IntSet and otherIntSet is returned
*/
IntSet IntSet::subtract(const IntSet& otherIntSet) const
{
   //copy of invoking IntSet
   IntSet subSet = (*this);

   //if an element in IntSet and also in otherIntSet removes it, if 
   //not keep the elements as is
   for(int index = 0; index < otherIntSet.size(); index++){
      if(subSet.contains(otherIntSet.data[index])){
         subSet.remove(otherIntSet.data[index]);
      }
   }
   //returns subtracted IntSet
   return subSet;
}
/*
invoking IntSet resets to become an empty IntSet
*/
void IntSet::reset()
{
   used = 0;
}
/*
if conatins anInt returns false, anInt gets added to 
invoking IntSet as a new element and returns true 
otherwise invoking IntSet is unchanged and returns false
*/
bool IntSet::add(int anInt)
{
   //if val isn't in set then adds val to invoking set
   if(contains(anInt) == 0){
      data[used] = anInt;
      used++;
      return true;
   }
   return false;
}
/*
if conatins anInt returns true, anInt gets removed from 
invoking IntSet and returns true otherwise invoking IntSet
is unchanged and returns false
*/
bool IntSet::remove(int anInt)
{
   bool alreadyFound = false;
   //checks if contains anInt is false
   if(contains(anInt) == 0){
      return false;
   }
   //if element is in the set then it searches the set  
   //and removes it and returns true
   for(int j = 0; j < used; j++){
      if(data[j] == anInt){
         alreadyFound = true;
      }
      if(alreadyFound == true && j != used - 1){
         data[j] = data[j+1];
      }
   }
   used --;
   return true;
}

bool operator==(const IntSet& is1, const IntSet& is2)
{
   if (is1.size() != is2.size()) return false;
   for (int i = 0; i < is1.size(); ++i)
      if (is1.contains(i) != is2.contains(i)) return false;
   return true;
}
