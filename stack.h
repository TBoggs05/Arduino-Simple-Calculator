#ifndef STACK_H_
#define STACK_H_
//NOTE: stack has time + space complexity of O(1) for operations. This stack is minimized to lower space compared to STL stack
template <typename T>
class stack{
public:
  stack(); //constructor
  T pop();//pop from top of stack, return value.
  void push(T val);//push new value onto top of stack
  bool isFull();
  bool isEmpty();
  T peek();//returns top value without popping
private:
  T data[16]; //array values
  int headIndex; //top of stack

};

//CLASS IMPLEMENTATION IN HEADER SINCE THIS IS A TEMPLATE CLASS AND C++ IS A HEADACHE ABOUT TEMPLATES
template <class T> stack<T>::stack(){
  headIndex = -1;
}
template <class T> T stack<T>::pop(){
  if(!isEmpty()){
  return data[headIndex--];
  }
  else{
    return '?'; //error case returns question mark
  }
}
template <class T> T stack<T>::peek(){
  if(!isEmpty()){
  return data[headIndex];
  }
  else{
    return '?'; //error case returns question mark
  }
}
template <class T> void stack<T>::push(T val){
  if(isFull()){
     //cant push, its full(would have a debug serial message here but i dont have room for serial ports on the uno board)
  }
  else{
    headIndex+=1;
    data[headIndex] = val;
  }
}
template <class T> bool stack<T>::isFull(){
if(headIndex == 15)
  return true;
return false;
}
template <class T> bool stack<T>::isEmpty(){
if(headIndex == -1)
  return true;
return false;
}
#endif