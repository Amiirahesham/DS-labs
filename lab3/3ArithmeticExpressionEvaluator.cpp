
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <cctype>
#include <stdexcept>
using namespace std;

// ==================== GENERIC NODE ====================

template <class T>
struct Node {
    T data;
    Node<T>* next;

    Node(T value) {
        data = value;
        next = NULL;
    }
};

// ==================== GENERIC STACK ====================

template <class T>
class Stack {
private:
    Node<T>* top;

    void displayBottomToTop(Node<T>* current) {
        if (current == NULL)
            return;

        displayBottomToTop(current->next);
        cout << current->data << " ";
    }

public:
    Stack() {
        top = NULL;
    }

    ~Stack() {
        while (!isEmpty())
            pop();
    }

    bool isEmpty() {
        return top == NULL;
    }

    void push(T value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->next = top;
        top = newNode;
    }

    T pop() {
        if (isEmpty())
            throw runtime_error("Stack is empty.");

        Node<T>* temp = top;
        T value = temp->data;

        top = top->next;
        delete temp;

        return value;
    }

    T peek() {
        if (isEmpty())
            throw runtime_error("Stack is empty.");

        return top->data;
    }

    void display() {
        displayBottomToTop(top);
        cout << endl;
    }
};

// ==================== EXPRESSION EVALUATOR ====================

class ExpressionEvaluator {
private:
    string expression;
    string postfix;

    bool isNumber(const string& token) {
        if (token.empty())
            return false;

        bool hasDigit = false;
        bool hasDot = false;

        for (int i = 0; i < (int)token.length(); i++) {
            char ch = token[i];

            if (isdigit((unsigned char)ch)) {
                hasDigit = true;
            }
            else if (ch == '.' && !hasDot) {
                hasDot = true;
            }
            else {
                return false;
            }
        }

        return hasDigit;
    }

    vector<string> tokenize(const string& exp) {
        vector<string> tokens;
        int i = 0;

        while (i < (int)exp.length()) {
            char ch = exp[i];

            if (isspace((unsigned char)ch)) {
                i++;
                continue;
            }

            if (isdigit((unsigned char)ch) || ch == '.') {
                string number;
                int dots = 0;

                while (i < (int)exp.length() &&
                       (isdigit((unsigned char)exp[i]) ||
                        exp[i] == '.')) {
                    if (exp[i] == '.')
                        dots++;

                    number += exp[i];
                    i++;
                }

                if (dots > 1 || !isNumber(number))
                    throw runtime_error("Invalid number.");

                tokens.push_back(number);
            }
            else if (isOperator(ch) ||
                     ch == '(' || ch == ')') {
                tokens.push_back(string(1, ch));
                i++;
            }
            else {
                throw runtime_error(
                    "Invalid character in expression.");
            }
        }

        return tokens;
    }

public:
    ExpressionEvaluator() {
        expression = "";
        postfix = "";
    }

    bool isOperator(char ch) {
        return ch == '+' || ch == '-' ||
               ch == '*' || ch == '/' || ch == '^';
    }

    int precedence(char op) {
        switch (op) {
        case '+':
        case '-':
            return 1;

        case '*':
        case '/':
            return 2;

        case '^':
            return 3;

        default:
            return 0;
        }
    }

    // Check whether parentheses are balanced.
    bool isBalanced(const string& exp) {
        Stack<char> brackets;

        for (int i = 0; i < (int)exp.length(); i++) {
            if (exp[i] == '(') {
                brackets.push('(');
            }
            else if (exp[i] == ')') {
                if (brackets.isEmpty())
                    return false;

                brackets.pop();
            }
        }

        return brackets.isEmpty();
    }

    // Validate expression syntax.
    bool validateExpression(const string& exp) {
        try {
            if (!isBalanced(exp))
                return false;

            vector<string> tokens = tokenize(exp);

            if (tokens.empty())
                return false;

            bool expectingOperand = true;
            int openBrackets = 0;

            for (int i = 0; i < (int)tokens.size(); i++) {
                string token = tokens[i];

                if (isNumber(token)) {
                    if (!expectingOperand)
                        return false;

                    expectingOperand = false;
                }
                else if (token == "(") {
                    if (!expectingOperand)
                        return false;

                    openBrackets++;
                    expectingOperand = true;
                }
                else if (token == ")") {
                    if (expectingOperand || openBrackets == 0)
                        return false;

                    openBrackets--;
                    expectingOperand = false;
                }
                else if (token.length() == 1 &&
                         isOperator(token[0])) {
                    if (expectingOperand)
                        return false;

                    expectingOperand = true;
                }
                else {
                    return false;
                }
            }

            return !expectingOperand && openBrackets == 0;
        }
        catch (...) {
            return false;
        }
    }

    // Convert infix expression to postfix.
    string infixToPostfix(const string& exp) {
        if (!validateExpression(exp))
            throw runtime_error("Invalid expression.");

        vector<string> tokens = tokenize(exp);
        Stack<char> operators;
        string result = "";

        for (int i = 0; i < (int)tokens.size(); i++) {
            string token = tokens[i];

            if (isNumber(token)) {
                result += token + " ";
            }
            else if (token == "(") {
                operators.push('(');
            }
            else if (token == ")") {
                while (!operators.isEmpty() &&
                       operators.peek() != '(') {
                    result += operators.pop();
                    result += " ";
                }

                if (operators.isEmpty())
                    throw runtime_error(
                        "Unbalanced parentheses.");

                operators.pop();
            }
            else {
                char op = token[0];

                while (!operators.isEmpty() &&
                       operators.peek() != '(' &&
                       (precedence(operators.peek()) >
                            precedence(op) ||
                        (precedence(operators.peek()) ==
                            precedence(op) && op != '^'))) {
                    result += operators.pop();
                    result += " ";
                }

                operators.push(op);
            }
        }

        while (!operators.isEmpty()) {
            if (operators.peek() == '(')
                throw runtime_error("Unbalanced parentheses.");

            result += operators.pop();
            result += " ";
        }

        postfix = result;
        return postfix;
    }

    // Apply one arithmetic operator.
    double applyOperator(double a, double b, char op) {
        switch (op) {
        case '+':
            return a + b;

        case '-':
            return a - b;

        case '*':
            return a * b;

        case '/':
            if (b == 0.0)
                throw runtime_error("Division by zero.");

            return a / b;

        case '^':
            return pow(a, b);

        default:
            throw runtime_error("Unknown operator.");
        }
    }

    // Evaluate postfix expression.
    double evaluatePostfix(const string& post) {
        istringstream input(post);
        string token;
        Stack<double> operands;

        while (input >> token) {
            if (isNumber(token)) {
                stringstream converter(token);
                double value;
                converter >> value;
                operands.push(value);
            }
            else if (token.length() == 1 &&
                     isOperator(token[0])) {
                if (operands.isEmpty())
                    throw runtime_error(
                        "Invalid postfix expression.");

                double b = operands.pop();

                if (operands.isEmpty())
                    throw runtime_error(
                        "Invalid postfix expression.");

                double a = operands.pop();

                double result =
                    applyOperator(a, b, token[0]);

                operands.push(result);
            }
            else {
                throw runtime_error(
                    "Invalid postfix token.");
            }
        }

        if (operands.isEmpty())
            throw runtime_error("Invalid expression.");

        double result = operands.pop();

        if (!operands.isEmpty())
            throw runtime_error("Invalid postfix expression.");

        return result;
    }

    // Display evaluation steps.
    void displaySteps(const string& post) {
        istringstream input(post);
        string token;
        Stack<double> operands;
        int step = 1;

        cout << "\nStep-by-step evaluation:\n";
        cout << "------------------------------\n";

        while (input >> token) {
            cout << "Step " << step++
                 << " | Token: " << token << '\n';

            if (isNumber(token)) {
                stringstream converter(token);
                double value;
                converter >> value;
                operands.push(value);
            }
            else if (token.length() == 1 &&
                     isOperator(token[0])) {
                double b = operands.pop();
                double a = operands.pop();

                double result =
                    applyOperator(a, b, token[0]);

                operands.push(result);
            }

            cout << "Stack (Bottom -> Top): ";
            operands.display();
            cout << "------------------------------\n";
        }

        if (!operands.isEmpty())
            cout << "Final Result: "
                 << operands.peek() << '\n';
    }

    void setExpression(const string& exp) {
        expression = exp;
        postfix = "";
    }

    string getExpression() {
        return expression;
    }

    string getPostfix() {
        return postfix;
    }
};

// ==================== MAIN MENU ====================

int main() {
    ExpressionEvaluator evaluator;
    int choice;
    string exp;

    do {
        cout << "\n=======================================\n";
        cout << "   ARITHMETIC EXPRESSION EVALUATOR\n";
        cout << "=======================================\n";
        cout << "1. Enter Mathematical Expression\n";
        cout << "2. Check Balanced Parentheses\n";
        cout << "3. Convert Infix to Postfix\n";
        cout << "4. Evaluate Expression\n";
        cout << "5. Display Evaluation Steps\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid menu choice.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        try {
            switch (choice) {
            case 1:
                cout << "Enter expression: ";
                cin >> ws;
                getline(cin, exp);

                evaluator.setExpression(exp);

                if (evaluator.validateExpression(exp))
                    cout << "Expression is valid.\n";
                else
                    cout << "Error: Invalid expression.\n";

                break;

            case 2:
                cout << "Enter expression: ";
                cin >> ws;
                getline(cin, exp);

                if (evaluator.isBalanced(exp))
                    cout << "Parentheses are balanced.\n";
                else
                    cout << "Error: Unbalanced parentheses.\n";

                break;

            case 3:
                cout << "Enter expression: ";
                cin >> ws;
                getline(cin, exp);

                evaluator.setExpression(exp);

                if (!evaluator.validateExpression(exp)) {
                    cout << "Error: Invalid expression.\n";
                    break;
                }

                cout << "Postfix: "
                     << evaluator.infixToPostfix(exp) << '\n';
                break;

            case 4:
                cout << "Enter expression: ";
                cin >> ws;
                getline(cin, exp);

                evaluator.setExpression(exp);

                if (!evaluator.validateExpression(exp)) {
                    cout << "Error: Invalid expression.\n";
                    break;
                }

                {
                    string post = evaluator.infixToPostfix(exp);
                    double result = evaluator.evaluatePostfix(post);

                    cout << "Postfix: " << post << '\n';
                    cout << "Result = " << result << '\n';
                }
                break;

            case 5:
                cout << "Enter expression: ";
                cin >> ws;
                getline(cin, exp);

                evaluator.setExpression(exp);

                if (!evaluator.validateExpression(exp)) {
                    cout << "Error: Invalid expression.\n";
                    break;
                }

                {
                    string post = evaluator.infixToPostfix(exp);

                    cout << "Postfix: " << post << '\n';
                    evaluator.displaySteps(post);
                }
                break;

            case 6:
                cout << "Exiting program. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << '\n';
        }

    } while (choice != 6);

    return 0;
}
