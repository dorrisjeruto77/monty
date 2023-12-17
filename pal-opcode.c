#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Monty.h"

/**
 * push - Pushes an element onto the stack.
 * @stack: Double pointer to the head of the stack.
 * @line_number: Line number in the file.
 */
void push(stack_t **stack, unsigned int line_number)
{
    // Check if there is an argument
    if (!stack || !line_number)
    {
        fprintf(stderr, "L%u: usage: push integer\n", line_number);
        exit(EXIT_FAILURE);
    }

    // Get the integer from the argument
    char *arg = strtok(NULL, " \n\t\r");
    if (!arg || !isdigit(*arg))
    {
        fprintf(stderr, "L%u: usage: push integer\n", line_number);
        exit(EXIT_FAILURE);
    }

    // Create a new node
    stack_t *new_node = malloc(sizeof(stack_t));
    if (!new_node)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    new_node->n = atoi(arg);
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
    (void)line_number; // Unused parameter

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
 * main - Example usage.
 * @argc: Argument count.
 * @argv: Argument vector.
 * Return: Always 0.
 */
int main(int argc, char *argv[])
{
    // Check if there is a file name argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
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

    return 0;
}
