#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 100

char stack[SIZE];
int top = -1;

void push(char item) {
    if(top >= SIZE-1) {
        printf("\nStack Overflow.");
    } else {
        stack[++top] = item;
    }
}

char pop() {
    char item ;

    if(top <0) {
        printf("stack under flow: invalid infix expression");
        getchar();
        exit(1);
    } else {
        item = stack[top--];
        return(item);
    }
}

int is_operator(char symbol) {
    if(symbol == '^' || symbol == '*' || symbol == '/' || symbol == '+' || symbol =='-') {
        return 1;
    } else {
        return 0;
    }
}

int precedence(char symbol) {
    if(symbol == '^') {
        return(3);
    } else if(symbol == '*' || symbol == '/') {
        return(2);
    } else if(symbol == '+' || symbol == '-') {
        return(1);
    } else {
        return(0);
    }
}

void InfixToPostfix(char infix_exp[], char postfix_exp[]) {
    int i, j;
    char item;
    char x;

    push('(');                               
    strcat(infix_exp,")");                  

    i=0;
    j=0;
    item=infix_exp[i];         

    while(item != '\0') {       
        if(item == '(') {
            push(item);
        } else if(isdigit(item)) {
            postfix_exp[j++] = item;              
            while(isdigit(infix_exp[i+1])) {
                item = infix_exp[++i];
                postfix_exp[j++] = item;
            }
            postfix_exp[j++] = ' ';
        } else if(is_operator(item) == 1) {       
            if(is_operator(infix_exp[i+1])) {
                printf("\nInvalid infix Expression.\n");        
                getchar();
                exit(1);
            }
            x=pop();
            while(is_operator(x) == 1 && precedence(x)>= precedence(item)) {
                postfix_exp[j++] = x;                  
                postfix_exp[j++] = ' ';
                x = pop();                       
            }
            push(x);
            push(item);                 
        } else if(item == ')') {         
            x = pop();                   
            while(x != '(') {                
                postfix_exp[j++] = x;
                postfix_exp[j++] = ' ';
                x = pop();
            }
        } else {                                
            printf("\nInvalid infix Expression.\n");        
            getchar();
            exit(1);
        }
        i++;
        item = infix_exp[i]; 
    } 
    while(top>0 && stack[top] != '(') {
        postfix_exp[j++] = pop();
        postfix_exp[j++] = ' ';
    }
    if(top>0 && stack[top] == '(') {
        top--;
    }
    postfix_exp[j] = '\0'; 
}

int evaluatePostfix(char* postfix_exp) {
    int stack[SIZE];
    int top = -1;
    char ch;
    int val = 0;
    int A, B;

    for (int i = 0; postfix_exp[i] != '\0'; i++) {
        ch = postfix_exp[i];
        if (isdigit(ch)) {
            // Si es un dígito, conviértelo a int y apílalo.
            // Pero primero verifica si el dígito anterior también era un número.
            val = (val*10) + (ch - '0');
        } else if (ch == ' ') {
            // Si es un espacio, apila el valor acumulado.
            if (val != 0) {
                stack[++top] = val;
                val = 0;
            }
        } else {
            // Si es un operador, desapila dos elementos de la pila, aplica el operador y apila el resultado.
            B = stack[top--];
            A = stack[top--];
            switch (ch) {
                case '+': stack[++top] = A + B; break;
                case '-': stack[++top] = A - B; break;
                case '*': stack[++top] = A * B; break;
                case '/': stack[++top] = A / B; break;
            }
        }
    }

    // Al final, la pila contiene el resultado.
    return stack[top];
}

int main() {
    char infix[SIZE], postfix[SIZE];         
    printf("\nEnter Infix expression : ");
    gets(infix);

    InfixToPostfix(infix,postfix);                   
    printf("Postfix Expression: ");
    puts(postfix);                     

    int result = evaluatePostfix(postfix);
    printf("Result: %d\n", result);

    return 0;
}