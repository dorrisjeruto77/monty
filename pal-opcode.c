#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Monty.h"

/**
 * push - Pushes an element onto the stack.
 * @stack: Double pointer to the head of the stack.
 * @line_number: Line number in the file.
 */
void push(stack_t **stack, unsigned int line_number)
{
    char *arg;
    int num;

    // Check if there is an argument
    if (!stack)
    {
        fprintf(stderr, "L%u: usage: push integer\n", line_number);
        exit(EXIT_FAILURE);
    }

    // Get the integer from the argument
    arg = strtok(NULL, " \n\t\r");
    if (!arg || *arg == '\0')
    {
        fprintf(stderr, "L%u: usage: push integer\n", line_number);
        exit(EXIT_FAILURE);
    }

    // Check if the argument is a valid integer
    for (int i = 0; arg[i]; i++)
    {
        if (!isdigit(arg[i]) && arg[i] != '-')
        {
            fprintf(stderr, "L%u: usage: push integer\n", line_number);
            exit(EXIT_FAILURE);
        }
    }

    // Convert the argument to an integer
    num = atoi(arg);

    // Create a new node
    stack_t *new_node = malloc(sizeof(stack_t));
    if (!new_node)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    new_node->n = num;
    new_node->prev = NULL;
    new_node->next = *stack;

    if (*stack)
        (*stack)->prev = new_node;

    *stack = new_node;
}

/**
 * pall - Prints all the values on the stack.
 * @stack: Double pointer to the head of the stack.
 * @line_number: Line number in the file.
 */
void pall(stack_t **stack, unsigned int line_number)
{
    // Check if the stack is empty
    if (!stack || !*stack)
        return;

    // Print values on the stack
    stack_t *current = *stack;
    while (current)
    {
        printf("%d\n", current->n);
        current = current->next;
    }
}

/**
 * main - Monty interpreter main function.
 * @argc: Argument count.
 * @argv: Argument vector.
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
int main(int argc, char *argv[])
{
    // Check if there is a file name argument
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Initialize the stack
    stack_t *stack = NULL;

    // Read lines from the file
    char *line = NULL;
    size_t len = 0;
    unsigned int line_number = 0;

    while (getline(&line, &len, file) != -1)
    {
        line_number++;
        char *opcode = strtok(line, " \n\t\r");

        if (opcode)
        {
            if (strcmp(opcode, "push") == 0)
                push(&stack, line_number);
            else if (strcmp(opcode, "pall") == 0)
                pall(&stack, line_number);
            // Add more opcodes as needed
            else
            {
                fprintf(stderr, "L%u: unknown instruction %s\n", line_number, opcode);
                exit(EXIT_FAILURE);
            }
        }
    }

    // Close the file
    fclose(file);

    // Free the stack
    while (stack)
    {
        stack_t *temp = stack;
        stack = stack->next;
        free(temp);
    }

    // Free allocated memory
    free(line);

    return EXIT_SUCCESS;
}
