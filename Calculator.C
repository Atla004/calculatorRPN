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

void InfixToRPN(char infix_exp[], char RPN_exp[]) {
    int i, j;
    char item;
    char x;

    push('(');  // Empuja un '(' al inicio de la pila
    strcat(infix_exp,")");  // Agrega un ')' al final de la expresión normal

    i=0;
    j=0;
    item=infix_exp[i];  // Inicializa el item con el primer carácter de la expresión normal

    // Itera a través de la expresión normal hasta que se encuentre un carácter nulo
    while(item != '\0') {       
        if(item == '(') {
            push(item);  // Si el item es '(', lo empuja a la pila
        } else if(isdigit(item)) {
            // Si el item es un dígito, lo agrega a la expresión postfija
            // y sigue agregando dígitos hasta que no sea un número
            RPN_exp[j++] = item;              
            while(isdigit(infix_exp[i+1])) {
                item = infix_exp[++i];
                RPN_exp[j++] = item;
            }
            RPN_exp[j++] = ' ';
        } else if(is_operator(item) == 1) {       
            // Si el item es un operador, comprueba si el siguiente carácter también es un operador
            // Si es así, la expresión normal es inválida
            if(is_operator(infix_exp[i+1])) {
                printf("\nInvalid infix Expression.\n");        
                getchar();
                exit(1);
            }
            x=pop();
            // Mientras el operador en la cima de la pila tenga mayor o igual precedencia que el item,
            // lo saca de la pila y lo agrega a la expresión postfija
            while(is_operator(x) == 1 && precedence(x)>= precedence(item)) {
                RPN_exp[j++] = x;                  
                RPN_exp[j++] = ' ';
                x = pop();                       
            }
            push(x);
            push(item);  // Empuja el item a la pila                 
        } else if(item == ')') {         
            // Si el item es ')', saca los operadores de la pila y los agrega a la expresión postfija
            // hasta que se encuentre un '(' en la pila
            x = pop();                   
            while(x != '(') {                
                RPN_exp[j++] = x;
                RPN_exp[j++] = ' ';
                x = pop();
            }
        } else {                                
            // Si el item no es un operador, un dígito, '(' o ')', la expresión normal es inválida
            printf("\nInvalid infix Expression.\n");        
            getchar();
            exit(1);
        }
        i++;
        item = infix_exp[i]; 
    } 

    // Si hay operadores restantes en la pila después de recorrer la expresión normal,
    // los saca de la pila y los agrega a la expresión postfija
    while(top>0 && stack[top] != '(') {
        RPN_exp[j++] = pop();
        RPN_exp[j++] = ' ';
    }
    if(top>0 && stack[top] == '(') {
        top--;
    }
    RPN_exp[j] = '\0';  // Agrega un carácter nulo al final de la expresión postfija para terminar el string
}

int evaluateRPN(char* RPN_exp) {
    int stack[SIZE];
    int top = -1;
    char ch;
    int val = 0;
    int A, B;

    for (int i = 0; RPN_exp[i] != '\0'; i++) {
        ch = RPN_exp[i];
        if (isdigit(ch)) {
            // Si es un dígito,verifica si el dígito anterior también era un número, convierte a int y apila.
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

   
    return stack[top];
}

int main() {
    char infix[SIZE], RPN[SIZE];         
    printf("\nEnter Infix expression : ");
    fgets(infix, SIZE, stdin);
    infix[strcspn(infix, "\n")] = '\0';

    InfixToRPN(infix,RPN);                   
    printf("RPN Expression: ");
    puts(RPN);                     

    int result = evaluateRPN(RPN);
    printf("Result: %d\n", result);

    return 0;
}