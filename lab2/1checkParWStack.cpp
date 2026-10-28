#include <iostream>
#include <string>

using namespace std;

class Stack {
private:
    char* data;
    int top;
    int capacity;
public:
    Stack(int size) {
        capacity = size;
        data = new char[capacity];
        top = -1;
    }

    ~Stack() {
        delete[] data;
    }

    bool isfull() {
        return (top >= capacity - 1);
    }

    bool isempty() {
        return (top < 0);
    }

    void push(char value) {
        if (isfull()) {
            cout << "Stack Overflow!\n";
            return;
        }
        data[++top] = value;
    }

    char pop() {
        if (isempty()) {
            return '\0';
        }
        return data[top--];
    }
};

bool checkBalanced(string expression) {

    Stack myStack(expression.length());

    for (int i = 0; i < expression.length(); i++) {
        if (expression[i] == '(') {
            myStack.push(expression[i]);
        }
        else if (expression[i] == ')') {
            if (myStack.isempty()) {
                return false;
            }
            myStack.pop();
        }
    }

    return myStack.isempty();
}

int main() {
    string expr;

    cout << "Enter a mathematical expression: ";
    getline(cin, expr);

    if (checkBalanced(expr)) {
        cout << "The parentheses in the expression are balanced.\n";
    } else {
        cout << "The parentheses in the expression are NOT balanced.\n";
    }

    return 0;
}
