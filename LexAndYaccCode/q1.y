%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variables representing a very small symbol table with just 'p' and 'q'
int p = 0, q = 0;

// Error handler for parse errors
void yyerror(const char *s) { fprintf(stderr, "Error: %s\n", s); }
int yylex(void); // Lexer prototype

// --- AST Node Definitions ---

// Forward declarations for mutual recursion among nodes
struct stmt_node;
struct expr_node;
struct cond_node;

// Enumerations for different kinds of statements
typedef enum {
    STMT_ASSIGN,
    STMT_IF,
    STMT_WHILE,
    STMT_PRINT
} StmtType;

// Enumerations for expression types
typedef enum {
    EXPR_NUMBER,
    EXPR_IDENTIFIER,
    EXPR_PLUS,
    EXPR_MINUS
} ExprType;

// Enumerations for condition types
typedef enum {
    COND_GT, // Greater than
    COND_LT, // Less than
    COND_EQ  // Equal to
} CondType;

// Expression node structure
struct expr_node {
    ExprType type;
    union {
        int num_val; // For number literals
        char *var_name; // For identifiers (e.g., "p", "q")
        struct {
            struct expr_node *left;
            struct expr_node *right;
        } bin_op; // For binary operations
    } data;
};

// Condition node structure
struct cond_node {
    CondType type;
    char *left_var_name;
    char *right_var_name;
};

// Statement node structure
struct stmt_node {
    StmtType type;
    union {
        struct {
            char *var_name;
            struct expr_node *expr;
        } assign_stmt;
        struct {
            struct cond_node *condition;
            struct stmt_node *then_body;
            struct stmt_node *else_body;
        } if_stmt;
        struct {
            struct cond_node *condition;
            struct stmt_node *body;
        } while_stmt;
        struct {
            char *var_name;
        } print_stmt;
    } data;
    struct stmt_node *next; // For chaining statements
};

// --- AST Creation Helper Functions ---

// Create a number expression node
struct expr_node* create_num_expr(int val) {
    struct expr_node *node = (struct expr_node*)malloc(sizeof(struct expr_node));
    node->type = EXPR_NUMBER;
    node->data.num_val = val;
    return node;
}

// Create an identifier expression node
struct expr_node* create_id_expr(char *var) {
    struct expr_node *node = (struct expr_node*)malloc(sizeof(struct expr_node));
    node->type = EXPR_IDENTIFIER;
    node->data.var_name = strdup(var);
    return node;
}

// Create a binary expression node
struct expr_node* create_bin_expr(ExprType type, struct expr_node *left, struct expr_node *right) {
    struct expr_node *node = (struct expr_node*)malloc(sizeof(struct expr_node));
    node->type = type;
    node->data.bin_op.left = left;
    node->data.bin_op.right = right;
    return node;
}

// Create a condition node
struct cond_node* create_condition(CondType type, char *left_id, char *right_id) {
    struct cond_node *node = (struct cond_node*)malloc(sizeof(struct cond_node));
    node->type = type;
    node->left_var_name = strdup(left_id);
    node->right_var_name = strdup(right_id);
    return node;
}

// Create assignment statement node
struct stmt_node* create_assign_stmt_node(char *var, struct expr_node *expr) {
    struct stmt_node *node = (struct stmt_node*)malloc(sizeof(struct stmt_node));
    node->type = STMT_ASSIGN;
    node->data.assign_stmt.var_name = strdup(var);
    node->data.assign_stmt.expr = expr;
    node->next = NULL;
    return node;
}

// Create if-else statement node
struct stmt_node* create_if_stmt_node(struct cond_node *cond, struct stmt_node *then_b, struct stmt_node *else_b) {
    struct stmt_node *node = (struct stmt_node*)malloc(sizeof(struct stmt_node));
    node->type = STMT_IF;
    node->data.if_stmt.condition = cond;
    node->data.if_stmt.then_body = then_b;
    node->data.if_stmt.else_body = else_b;
    node->next = NULL;
    return node;
}

// Create while loop statement node
struct stmt_node* create_while_stmt_node(struct cond_node *cond, struct stmt_node *body) {
    struct stmt_node *node = (struct stmt_node*)malloc(sizeof(struct stmt_node));
    node->type = STMT_WHILE;
    node->data.while_stmt.condition = cond;
    node->data.while_stmt.body = body;
    node->next = NULL;
    return node;
}

// Create print statement node
struct stmt_node* create_print_stmt_node(char *var) {
    struct stmt_node *node = (struct stmt_node*)malloc(sizeof(struct stmt_node));
    node->type = STMT_PRINT;
    node->data.print_stmt.var_name = strdup(var);
    node->next = NULL;
    return node;
}

// --- Execution / Interpreter Functions ---

// Retrieve value of a variable (p or q)
int get_var_value(char *var_name) {
    if (strcmp(var_name, "p") == 0) return p;
    else if (strcmp(var_name, "q") == 0) return q;
    fprintf(stderr, "Runtime Error: Undefined variable '%s'\n", var_name);
    exit(1);
}

// Set value of a variable (p or q)
void set_var_value(char *var_name, int val) {
    if (strcmp(var_name, "p") == 0) p = val;
    else if (strcmp(var_name, "q") == 0) q = val;
}

// Evaluate an expression AST node recursively
int evaluate_expr(struct expr_node *node) {
    if (!node) return 0;
    switch (node->type) {
        case EXPR_NUMBER: return node->data.num_val;
        case EXPR_IDENTIFIER: return get_var_value(node->data.var_name);
        case EXPR_PLUS: return evaluate_expr(node->data.bin_op.left) + evaluate_expr(node->data.bin_op.right);
        case EXPR_MINUS: return evaluate_expr(node->data.bin_op.left) - evaluate_expr(node->data.bin_op.right);
    }
    return 0;
}

// Evaluate a condition node (assumes both sides are variables)
int evaluate_condition(struct cond_node *node) {
    if (!node) return 0;
    int left_val = get_var_value(node->left_var_name);
    int right_val = get_var_value(node->right_var_name);
    switch (node->type) {
        case COND_GT: return left_val > right_val;
        case COND_LT: return left_val < right_val;
        case COND_EQ: return left_val == right_val;
    }
    return 0;
}

// Forward declaration for recursion
void execute_stmt(struct stmt_node *node);

// Execute a list of statements
void execute_stmt_list(struct stmt_node *head) {
    struct stmt_node *current = head;
    while (current != NULL) {
        execute_stmt(current);
        current = current->next;
    }
}

// Execute a single statement node
void execute_stmt(struct stmt_node *node) {
    if (!node) return;
    switch (node->type) {
        case STMT_ASSIGN: {
            int val = evaluate_expr(node->data.assign_stmt.expr);
            set_var_value(node->data.assign_stmt.var_name, val);
            printf("DEBUG(Exec): %s assigned %d. p=%d, q=%d\n", node->data.assign_stmt.var_name, val, p, q);
            break;
        }
        case STMT_IF:
            printf("DEBUG(Exec): Entering IF. p=%d, q=%d.\n", p, q);
            if (evaluate_condition(node->data.if_stmt.condition)) {
                printf("DEBUG(Exec): IF condition TRUE. Executing THEN block.\n");
                execute_stmt_list(node->data.if_stmt.then_body);
            } else {
                printf("DEBUG(Exec): IF condition FALSE. Executing ELSE block.\n");
                execute_stmt_list(node->data.if_stmt.else_body);
            }
            printf("DEBUG(Exec): Exiting IF. p=%d, q=%d.\n", p, q);
            break;
        case STMT_WHILE:
            printf("DEBUG(Exec): Entering WHILE loop. p=%d, q=%d.\n", p, q);
            while (evaluate_condition(node->data.while_stmt.condition)) {
                printf("DEBUG(Exec): Inside WHILE loop iteration. p=%d, q=%d.\n", p, q);
                execute_stmt_list(node->data.while_stmt.body);
            }
            printf("DEBUG(Exec): Exiting WHILE loop. p=%d, q=%d.\n", p, q);
            break;
        case STMT_PRINT:
            if (strcmp(node->data.print_stmt.var_name, "p") == 0)
                printf("DEBUG(Exec): Print p: %d\n", p);
            else if (strcmp(node->data.print_stmt.var_name, "q") == 0)
                printf("DEBUG(Exec): Print q: %d\n", q);
            break;
    }
}

// --- AST Memory Cleanup ---

// Free expression node recursively
void free_expr_node(struct expr_node *node) {
    if (!node) return;
    if (node->type == EXPR_IDENTIFIER) free(node->data.var_name);
    else if (node->type == EXPR_PLUS || node->type == EXPR_MINUS) {
        free_expr_node(node->data.bin_op.left);
        free_expr_node(node->data.bin_op.right);
    }
    free(node);
}

// Free condition node
void free_cond_node(struct cond_node *node) {
    if (!node) return;
    free(node->left_var_name);
    free(node->right_var_name);
    free(node);
}

// Free statement node recursively (and the whole statement list)
void free_stmt_node(struct stmt_node *node) {
    if (!node) return;
    struct stmt_node *next_stmt = node->next;
    switch(node->type) {
        case STMT_ASSIGN:
            free(node->data.assign_stmt.var_name);
            free_expr_node(node->data.assign_stmt.expr);
            break;
        case STMT_IF:
            free_cond_node(node->data.if_stmt.condition);
            free_stmt_node(node->data.if_stmt.then_body);
            free_stmt_node(node->data.if_stmt.else_body);
            break;
        case STMT_WHILE:
            free_cond_node(node->data.while_stmt.condition);
            free_stmt_node(node->data.while_stmt.body);
            break;
        case STMT_PRINT:
            free(node->data.print_stmt.var_name);
            break;
    }
    free(node);
    free_stmt_node(next_stmt);
}
%}

// --- Bison Declarations ---
%union {
    int ival;
    char* sval;
    struct stmt_node *stmt_ptr;
    struct expr_node *expr_ptr;
    struct cond_node *cond_ptr;
}

// Token declarations with types
%token <sval> IDENTIFIER
%token <ival> NUMBER
%token WHILE IF THEN ELSE PRINT
%token ASSIGN GT LT EQ PLUS MINUS SEMI LBRACE RBRACE

// Non-terminal return types
%type <expr_ptr> expr
%type <cond_ptr> condition
%type <stmt_ptr> stmt_list stmt

%%

// Top-level grammar rule
program:
    stmt_list {
        printf("DEBUG(Parse): Parsing complete. Starting execution.\n");
        execute_stmt_list($1); // Interpret the parsed AST
        printf("DEBUG(Parse): Execution complete. Final p=%d, q=%d\n", p, q);
        free_stmt_node($1); // Clean up
    }
    ;

// Sequence of statements
stmt_list:
      stmt_list stmt {
          struct stmt_node *temp = $1;
          while (temp->next != NULL) temp = temp->next;
          temp->next = $2;
          $$ = $1;
      }
    | stmt { $$ = $1; }
    ;

// Individual statements
stmt:
      IDENTIFIER ASSIGN expr SEMI {
          printf("DEBUG(Parse): Parsed assignment: %s = expr\n", $1);
          $$ = create_assign_stmt_node($1, $3);
      }
    | WHILE '(' condition ')' LBRACE stmt_list RBRACE {
          printf("DEBUG(Parse): Parsed WHILE loop.\n");
          $$ = create_while_stmt_node($3, $6);
      }
    | IF condition THEN stmt ELSE stmt {
          printf("DEBUG(Parse): Parsed IF-ELSE statement.\n");
          $$ = create_if_stmt_node($2, $4, $6);
      }
    | PRINT IDENTIFIER SEMI {
          printf("DEBUG(Parse): Parsed PRINT statement: %s\n", $2);
          $$ = create_print_stmt_node($2);
      }
    ;

// Boolean condition expressions
condition:
      IDENTIFIER GT IDENTIFIER {
          printf("DEBUG(Parse): Parsed condition: %s > %s\n", $1, $3);
          $$ = create_condition(COND_GT, $1, $3);
      }
    | IDENTIFIER LT IDENTIFIER {
          printf("DEBUG(Parse): Parsed condition: %s < %s\n", $1, $3);
          $$ = create_condition(COND_LT, $1, $3);
      }
    | IDENTIFIER EQ IDENTIFIER {
          printf("DEBUG(Parse): Parsed condition: %s == %s\n", $1, $3);
          $$ = create_condition(COND_EQ, $1, $3);
      }
    ;

// Arithmetic expressions
expr:
      NUMBER {
          printf("DEBUG(Parse): Parsed number expression: %d\n", $1);
          $$ = create_num_expr($1);
      }
    | IDENTIFIER {
          printf("DEBUG(Parse): Parsed identifier expression: %s\n", $1);
          $$ = create_id_expr($1);
      }
    | expr PLUS expr {
          printf("DEBUG(Parse): Parsed PLUS expression.\n");
          $$ = create_bin_expr(EXPR_PLUS, $1, $3);
      }
    | expr MINUS expr {
          printf("DEBUG(Parse): Parsed MINUS expression.\n");
          $$ = create_bin_expr(EXPR_MINUS, $1, $3);
      }
    ;

%%

// Entry point of the program
int main() {
    yyparse(); // Start parsing
    return 0;
}
