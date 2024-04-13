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
        // Verifica si la expresión está vacía
    if(strlen(infix_exp) == 0) {
        printf("\nInvalid infix Expression. (empty)\n");
        exit(1);
    }

    // Verifica si la expresión comienza o termina con un operador
    if(is_operator(infix_exp[0]) || is_operator(infix_exp[strlen(infix_exp) - 1])) {
        printf("\nInvalid infix Expression. (operator at the start or end)\n");
        exit(1);
    }
    
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
            x=pop();
            while(is_operator(x) == 1 && precedence(x)>= precedence(item)) {
                RPN_exp[j++] = x;                  
                RPN_exp[j++] = ' ';
                x = pop();                       
            }
            push(x);
            push(item); 

            // Verificación adicional para detectar operadores consecutivos
            if(is_operator(infix_exp[i+1])) {
                printf("\nInvalid infix Expression(Consecutive Operator).\n");        

                char error_exp[SIZE];
                strncpy(error_exp, infix_exp, strlen(infix_exp) - 1);  //elimina el parentesis final
                error_exp[strlen(infix_exp) - 1] = '\0';  

                printf("%s\n", error_exp);
                printf("%*c\n", i+2, '^');  // Imprime una flecha en la posición del error
                getchar();
                exit(1);
            }           
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
                printf("\nInvalid infix Expression.\n");        

                char error_exp[SIZE];
                strncpy(error_exp, infix_exp, strlen(infix_exp) - 1);   //elimina el parentesis final
                error_exp[strlen(infix_exp) - 1] = '\0';  

                printf("%s\n", error_exp);
                printf("%*c\n", i+1, '^');  // Imprime una flecha en la posición del error
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

    int result = evaluateRPN(RPN);

    // Abre el archivo txt
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // input sin el paréntesis de cierre ")" al final
    char input[SIZE];
    strncpy(input, infix, strlen(infix) - 1);  
    input[strlen(infix) - 1] = '\0';  

    // Escribe en el archivo
    fprintf(file, "Input: %s\n", input);
    fprintf(file, "RPN: %s\n", RPN);
    fprintf(file, "Resultado: %d\n", result);
    printf("Input: %s\n", input);
    printf("RPN: %s\n", RPN);
    printf("Resultado: %d\n", result);


    // Cierra el archivo
    fclose(file);

    return 0;
}