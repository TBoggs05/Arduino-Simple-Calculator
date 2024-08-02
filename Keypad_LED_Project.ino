#include <Keypad.h>
#include <LiquidCrystal.h>
#include <string.h>
#include "stack.h"

// initialize the lcd w/ chosen digital pins
LiquidCrystal lcd(13, 12, 11, 10, 0, 1);

//ROWS/COLS of keypad
const byte ROWS = 4; 
const byte COLS = 4; 
//hexakeys library value for keypad object
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', '/'},
  {'4', '5', '6', '*'},
  {'7', '8', '9', '-'},
  {'.', '0', '=', '+'}
};

//initialize digital pins for column and row
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

//initialize keypad object with keymap, pins, and bytes passed
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

String problemText;//problem string that is taken from input stream
int screenIndex = 15; //index of position on screen

/*
Input and Output Function
-called in loop()
-handles input, when '=' is inputted it returns an output to the LCD.
@returns VOID
*/
void Input_Output_Update(){
 char customKey = customKeypad.getKey();//check 4 input
  //append new value to problem char array
  if(customKey && customKey != '='){
    if(problemText.length() == 16){
      lcd.clear();
      lcd.setCursor(8,1);
      lcd.display();
      lcd.print("TOO LONG");
      delay(1000);
      lcd.clear();
      lcd.setCursor(screenIndex,0);
      lcd.display();
      lcd.print(problemText);
    }
    else{
      problemText+=customKey;
      lcd.clear();
      lcd.setCursor(screenIndex--,0);  
      lcd.display();
      lcd.print(problemText);
    }
  }
  //equal sign is inputted, so begin calculation.
  else if(customKey == '='){
    screenIndex = 15;
    lcd.clear();  //Clears the LCD screen and positions the cursor in the upper-left corner.
    lcd.setCursor(8,0);
    lcd.display();
      if(convert_to_postfix()){
          problemText = (String)calculate(problemText);//COMMENT THIS LINE TO TURN IT INTO INFIX-POSTFIX CONVERTER!  
          lcd.setCursor(16-problemText.length(),0);
           lcd.print(problemText);
      }
      else{
          lcd.setCursor(11,1);
           lcd.print("ERROR");
      }
    problemText = "";
  }
}
/*
Postfix-Converter Function
-loops thru infix and uses stack to convert to postfix
@returns BOOL
*/
bool convert_to_postfix(){
 String tempProblem;
 stack<char> opStack;
 short i = 0;
 short operatorCounter = 0;
 short termCounter = 0;
  while(i < problemText.length()){ //loop thru string
        switch(problemText[i]){
          //since we dont worry about parenthesis matching, if operator is + or -, any operators in stack have >= priority. so empty stack.
        case '+':
        case '-':
            operatorCounter++;//count operators for validity check
            while(!opStack.isEmpty())
              tempProblem += opStack.pop();//empty whole stack
            opStack.push(problemText[i]);//push new operator onto stack
            i++;
        break;
        //top of stack could be lower prio, so pop any equal prio operators and push when case fails
        case '/':
        case '*':
            operatorCounter++;//count operators for validity check
            while(opStack.peek() == '/' || opStack.peek() == '*')//pop equal prio operators
              tempProblem += opStack.pop();
            opStack.push(problemText[i]);//push new op onto stack
            i++;
        break;
        default: //number case
        while(i < problemText.length() && problemText[i] != '/' && problemText[i] != '*' && problemText[i] != '-' && problemText[i] != '+'){
          tempProblem+=(problemText[i++]);
        }   
        tempProblem+='_';//space for separating terms
        termCounter++;//for validity check
        break;
        }
  }
  if(operatorCounter < 1 || operatorCounter != termCounter-1)//validity check
    return false;
  else{
    while(!opStack.isEmpty()){tempProblem+=opStack.pop();}//empty the stack
    problemText = tempProblem;
    return true;    
  } 
}
/*
Calculator Function
-loops thru postfix and uses stacks to do calculations
@returns double
*/
double calculate(String postfix){
 stack<double> s;
 short i = 0;
 double op1, op2, temp = 0.0;
 char ch;
  while(i < postfix.length()){
    //space, ignore
    if(postfix[i] == '_'){i++;}
    //is digit, so run the values together and store
    else if(isdigit(postfix[i]) || postfix[i] == '.'){
      temp = 0.0;
      bool isDecimal = false;
      int decimalIndex;
        while(i < postfix.length() && (isdigit(postfix[i]) || postfix[i] == '.')){
          if(postfix[i] != '.' && !isDecimal){//handles before decimal point
            temp = temp * 10 + (postfix[i]-'0');//convert to int using ascii logic
          }
          else if(isDecimal){ //handles past decimal point
            temp = temp + ((1/pow(10, i-decimalIndex)) * (postfix[i]-'0'));
          }
          else{//flag for decimal state
          isDecimal = true;
          decimalIndex = i;
          }
          i++;
        }
        s.push(temp);
    }
    //operator, do that operation between operands in stack
    else{
        ch = postfix[i];//operator
        if(!s.isEmpty())
        op1 = s.pop();//operand 1
        if(!s.isEmpty())
          op2 = s.pop();//operand 2
          else{
            lcd.print("EMPTY STACK"); //error case, should never happen
            return NULL;
          }
        switch(ch){
          case '+':
            s.push(op2+op1);
            break;
          case '-':
            s.push(op2-op1);
            break;
          case '/':
            if(op1 != 0.0)
            s.push(op2/op1);
            else{
              lcd.setCursor(11,1);
              lcd.print("ERROR");
              return NULL;
            }
            break;
          case '*':
            s.push(op2*op1);
            break;
          default:
            lcd.setCursor(11,1);
            lcd.print("ERROR");
            return NULL;
            break;
        }
        i++;
    }  
  }
 double result = s.pop(); //store final result
  if(!s.isEmpty()){return NULL;} //ERROR CASE!
   else{return result;}//print the outcome to the LCD
}
//called before first loop
void setup(){
  lcd.begin(16, 2);  // set up the LCD's number of columns and rows: 
}
//called as infinite loop
void loop(){
 Input_Output_Update(); // handle inputs/outputs of calculator while turned on.
}