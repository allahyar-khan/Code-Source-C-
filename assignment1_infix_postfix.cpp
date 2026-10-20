#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <iomanip>
using namespace std;

bool isOperator(char ch)
{
    return ch == '+' || ch == '-' ||
           ch == '*' || ch == '/' || ch == '^';
}

int precedence(char op)
{
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

vector<string> tokenizeInfix(const string& expr)
{
    vector<string> tokens;

    for (size_t i = 0; i < expr.size();)
    {
        if (isspace(static_cast<unsigned char>(expr[i])))
        {
            i++;
        }
        else if (isalnum(static_cast<unsigned char>(expr[i])) ||
                 expr[i] == '.')
        {
            string token;
            bool dotFound = false;

            while (i < expr.size() &&
                  (isalnum(static_cast<unsigned char>(expr[i])) ||
                   expr[i] == '.'))
            {
                if (expr[i] == '.')
                {
                    if (dotFound)
                        throw runtime_error("Invalid number.");
                    dotFound = true;
                }

                token += expr[i++];
            }

            tokens.push_back(token);
        }
        else
        {
            tokens.push_back(string(1, expr[i]));
            i++;
        }
    }

    return tokens;
}

void displayStack(stack<string> s)
{
    vector<string> items;

    while (!s.empty())
    {
        items.push_back(s.top());
        s.pop();
    }

    cout << "[ ";
    for (int i = static_cast<int>(items.size()) - 1; i >= 0; i--)
        cout << items[i] << " ";
    cout << "]";
}

string infixToPostfix(const string& infix)
{
    vector<string> tokens = tokenizeInfix(infix);
    stack<string> st;
    string postfix;

    cout << "\nStep-by-step Stack Operations:\n";
    cout << left << setw(18) << "Token"
         << setw(30) << "Stack" << "Postfix\n";

    for (const string& token : tokens)
    {
        if (isalnum(static_cast<unsigned char>(token[0])) ||
            token[0] == '.')
        {
            postfix += token + " ";
        }
        else if (token == "(")
        {
            st.push(token);
        }
        else if (token == ")")
        {
            while (!st.empty() && st.top() != "(")
            {
                postfix += st.top() + " ";
                st.pop();
            }

            if (st.empty())
                throw runtime_error("Mismatched parentheses.");

            st.pop();
        }
        else if (token.size() == 1 && isOperator(token[0]))
        {
            char op = token[0];

            while (!st.empty() && st.top() != "(" &&
                  (precedence(st.top()[0]) > precedence(op) ||
                  (precedence(st.top()[0]) == precedence(op)
                   && op != '^')))
            {
                postfix += st.top() + " ";
                st.pop();
            }

            st.push(token);
        }
        else
        {
            throw runtime_error("Invalid token in expression.");
        }

        cout << left << setw(18) << token;
        displayStack(st);
        cout << setw(10) << " " << postfix << '\n';
    }

    while (!st.empty())
    {
        if (st.top() == "(")
            throw runtime_error("Mismatched parentheses.");

        postfix += st.top() + " ";
        st.pop();
    }

    return postfix;
}

double evaluatePostfix(const string& postfix)
{
    stack<double> st;
    stringstream ss(postfix);
    string token;

    cout << "\nPostfix Evaluation Steps:\n";

    while (ss >> token)
    {
        if (token.size() == 1 && isOperator(token[0]))
        {
            if (st.size() < 2)
                throw runtime_error("Invalid postfix expression.");

            double b = st.top();
            st.pop();

            double a = st.top();
            st.pop();

            double result;

            switch (token[0])
            {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/':
                    if (b == 0)
                        throw runtime_error("Division by zero.");
                    result = a / b;
                    break;
                case '^': result = pow(a, b); break;
                default:
                    throw runtime_error("Unknown operator.");
            }

            st.push(result);
            cout << a << " " << token << " " << b
                 << " = " << result << '\n';
        }
        else
        {
            size_t pos;
            double value = stod(token, &pos);

            if (pos != token.size())
                throw runtime_error("Use numeric operands only.");

            st.push(value);
            cout << "Push " << value << '\n';
        }
    }

    if (st.size() != 1)
        throw runtime_error("Invalid postfix expression.");

    return st.top();
}

int main()
{
    cout << "====================================\n";
    cout << " INFIX TO POSTFIX AND EVALUATION\n";
    cout << "====================================\n";

    try
    {
        string infix;

        cout << "\nEnter an infix expression: ";
        getline(cin, infix);

        string postfix = infixToPostfix(infix);

        cout << "\nOriginal Infix: " << infix << '\n';
        cout << "Postfix Expression: " << postfix << '\n';

        cout << "\n====================================\n";
        cout << " POSTFIX EVALUATION TESTS\n";
        cout << "====================================\n";

        vector<string> tests = {
            "6 2 / 3 * 4 +",
            "5 3 + 2 *",
            "10 2 3 * -"
        };

        for (int i = 0; i < static_cast<int>(tests.size()); i++)
        {
            cout << "\nTest " << i + 1 << ": "
                 << tests[i] << '\n';

            double result = evaluatePostfix(tests[i]);
            cout << "Result = " << result << '\n';
        }

        string choice;
        cout << "\nEvaluate another postfix expression? (yes/no): ";
        getline(cin, choice);

        if (choice == "yes" || choice == "Yes" || choice == "YES")
        {
            string input;
            cout << "Enter postfix expression (separate tokens with spaces): ";
            getline(cin, input);

            double result = evaluatePostfix(input);
            cout << "Final Result = " << result << '\n';
        }
    }
    catch (const exception& e)
    {
        cout << "\nError: " << e.what() << '\n';
    }

    return 0;
}
