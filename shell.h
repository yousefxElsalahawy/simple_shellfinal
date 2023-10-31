#ifndef SHELL_H
#define SHELL_H


#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@histcount: the history line number count
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@history: the history node
 *@alias: the alias node
 *@environ: custom modified copy of environ from LL env
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 */

typedef struct passinfo
{
	int status;
	int env_changed;
	char **environ;
	list_t *alias;
	list_t *history;
	list_t *env;
	char *fname;
	int linecount_flag;
	int err_num;
	unsigned int line_count;
	int argc;
	char *path;
	char **argv;
	char *arg;
	int readfd;
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int histcount;
} info_t;

#define INFO_INIT  \
{ \
	.status = 0, \
	.env_changed = 0, \
	.environ = NULL, \
	.alias = NULL, \
	.history = NULL, \
	.env = NULL, \
	.fname = NULL, \
	.linecount_flag = 0, \
	.err_num = 0, \
	.line_count = 0, \
	.argc = 0, \
	.path = NULL, \
	.argv = NULL, \
	.arg = NULL, \
	.readfd = 0, \
	.cmd_buf_type = 0, \
	.cmd_buf = NULL, \
	.histcount = 0 \
}


/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* hsh.c */
void prompt_user(info_t *info);
void _handle_with_input_(info_t *info, char **av, ssize_t *r,
		int *builtin_ret);
void handle_exit(info_t *info, int builtin_ret);
void cleanup(info_t *info);
int hsh(info_t *info, char **av);
builtin_table *initialize_builtin_table(void);
int execute_builtin(info_t *info, builtin_table *builtintbl, int i);
int find_builtin(info_t *info);
void increment_line_count(info_t *info);
void count_non_delim(info_t *info, int *k);
void handle_path_found(info_t *info, char *path);
void handle_path_not_found(info_t *info);
void find_cmd(info_t *info);
void handle_fork_error(pid_t child_pid);
void handle_exec_error(info_t *info);
void handle_child_process(info_t *info);
void handle_parent_process(info_t *info);
void fork_cmd(info_t *info);
/* path.c */
int check_path(char *path, struct stat *st);
int check_file_mode(struct stat *st);
int is_cmd(info_t *info, char *path);
char *_COpYY_cHaRS_(char *pathstr, int start, int stop, char *buf);
char *dup_chars(char *pathstr, int start, int stop);
char *check_cmd(info_t *info, char *cmd);
char *build_path(char *path, char *cmd);
char *find_cmd_in_path(info_t *info, char *pathstr,
		char *cmd, int i, int curr_pos);
char *find_path(info_t *info, char *pathstr, char *cmd);
/* loophsh.c */
int loophsh(char **);

/* err_string_functions.c */

void _eputchar_loop(char *str, int i);
void _eputs(char *str);
void _buffer_flush(char *buf, int *i);
int _eputchar(char c);
void _write_to_buffer(char **buf_ptr, char *buf_end, int fd);
int _putfd(char c, int fd);
int _putfd_loop(char **str, int fd, int i);
int _putsfd(char *str, int fd);

/* string_functions.c */
int _increment_while_not_null(char **s, int *i);
int _strlen(char *s);
int compare_chars(char c1, char c2);
int _strcmp(char *s1, char *s2);
int check_chars(const char **needle, const char **haystack);
char *starts_with(const char *haystack, const char *needle);
char *find_end(char *str);
void copy_str(char *dest, char *src);
char *_strcat(char *dest, char *src);
/* string_functions2.c */
char *_copy(char *dest, char *src, int i);
char *_strcpy(char *dest, char *src);
int _strlen___(const char *str);
char *_strdup(const char *str);
void _print_char(char *str, char *end);
void _puts(char *str);
void write_buffer(char *buf, int *i);
void add_to_buffer(char *buf, int *i, char c);
int _putchar(char c);
/* string_functions3.c */
char *_copy_chars_(char *dest, char *src, int n);
char *fill_remaining_with_null(char *dest, int start, int n);
char *_strncpy(char *dest, char *src, int n);
int find_length(char *str);
void cpy_chars(char *dest, char *src, int start, int n);
char *_strncat(char *dest, char *src, int n);
char *check_char(char *s, char c);
char *_strchr(char *s, char c);
/* string_functions4.c */
int delimiter_detector(char c, char *d);
int lexeme_counter(char *str, char *d);
char **_mem_allocate_(int numwords);
int lexeme_length_finder(char *str, char *d, int i);
void memory_liberator(char **s, int j);
char *word_allocator(int k);
void lexeme_copier(char *dest, char *src, int len);
char **strtow(char *str, char *d);
int is_str_void_or_vacant(char *str);
int tally_lexemes(char *str, char d);
char **reserve_memory(int lexemeCount);
int bypass_separators(char *str, char d, int i);
int determine_lexeme_extent(char *str, char d, int i);
char *reserve_lexeme(int k);
void relinquish_memory(char **s, int j);
void replicate_lexeme(char **s, char *str, int j, int k, int *i);
char **strtow2(char *str, char d);
/* memory_functions */

void fillMemory(char *s, char b, unsigned int n, unsigned int i);
char *_memset(char *s, char b, unsigned int n);
void freeArray(char **pp, int i);
void ffree(char **pp);
void *allocate_memory(unsigned int size);
void copy_memory(void *ptr, char *p, unsigned int size);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);

/* memory_functions2.c */
int is_null(void **ptr);
void free_and_nullify(void **ptr);
int bfree(void **ptr);
/* more_functions.c */
int interactive(info_t *info);
int _CHEcK_chARs_(char c, char *temp);
int is_delim(char c, char *delim);
int _islower(int c);
int _isupper(int c);
int _isalpha(int c);
int process_sign(char s, int sign);
unsigned int process_digit(char s, unsigned int result, int *flag);
int _atoi(char *s);
/* more_functions2.c */
char *process_plus_sign(char *s);
unsigned long int process_digits(char *s);
int _erratoi(char *s);
void print_message(char *message);
void print_error_part1(info_t *info);
void print_error_part2(info_t *info);
void print_error_part3(info_t *info, char *estr);
void print_error(info_t *info, char *estr);
int handle_putchar(int fd);
unsigned int handle_abs(int input, int (*__putchar)(char), int *count);
int handle_current(unsigned int _abs_, int (*__putchar)(char), int *count);
void handle_last_digit(unsigned int current,
		int (*__putchar)(char), int *count);
int print_d(int input, int fd);
char *handle_sign(long int num, int flags, unsigned long *n, char *sign);
void convert_base(unsigned long *n, int base, char **ptr, char *array);
void handle_negative_sign(char **ptr, char sign);
char *convert_number(long int num, int base, int flags);
void set_to_null(char *p);
void remove_comments(char *buf);
/* builtin_emulators.c */
int _myexit(info_t *);
int _mycd(info_t *);
int _myhelp(info_t *);

/* builtin_emulators2.c */
int _myhistory(info_t *);
int _myalias(info_t *);

/* getline.c module */
void reset_buffer(info_t *info, char **buf);
ssize_t input_reader(info_t *info, char **buf, size_t *len_p);
void input_processor(info_t *info, char **buf, ssize_t *r);
void command_chain_handler(info_t *info, char **buf, size_t *len, ssize_t r);
ssize_t input_buf(info_t *info, char **buf, size_t *len);
ssize_t handle_input(info_t *info, char **buf, size_t *len);
void handle_chain(info_t *info, char *buf, size_t *i, size_t len);
void handle_buffer_end(info_t *info, size_t *i, size_t *len);
ssize_t get_input(info_t *info);
ssize_t read_from_fd(info_t *info, char *buf);
ssize_t read_buf(info_t *info, char *buf, size_t *i);
ssize_t buffer_reader(info_t *info, char *buf, size_t *len);
char *locate_newline(char *buf, size_t i);
char *memory_allocator(char *p, size_t s, size_t k);
void buffer_copier(char *new_p, char *buf, size_t i, size_t k, size_t s);
int _getline(info_t *info, char **ptr, size_t *length);
void sigintHandler(int sig_num);
/* info.c module */
void set_value(info_t *info, int argc_value, char *arg_value);

void clear_info(info_t *info);
void allocate_argv(info_t *info);
void count_args(info_t *info);
void replace_info(info_t *info);
void set_info(info_t *info, char **av);
void free_argv_and_path(info_t *info);
void free_lists(info_t *info);
void free_remaining(info_t *info);
void free_info(info_t *info, int all);
/* env.c module */
int _myenv(info_t *info);
char *search_in_list(list_t *node, const char *name);
char *_getenv(info_t *info, const char *name);
int validate_args(info_t *info);
int _mysetenv(info_t *info);
int check_argc(info_t *info);
int _myunsetenv(info_t *info);
list_t *create_env_node();
int populate_env_list(info_t *info);
/* env2.c module */
char **refresh_environ(info_t *info);
char **get_environ(info_t *info);
char *get_starting_string(list_t *node, char *var);
int reset_environment(info_t *info, size_t *i, list_t **node);
int _unsetenv(info_t *info, char *var);
char *create_buf(char *var, char *value);
list_t *find_var_in_env(list_t *node, char *var, char *buf, info_t *info);
void add_node_and_free_buf(list_t **env, char *buf, info_t *info);
int _setenv(info_t *info, char *var, char *value);
/* file_io_functions.c */

char *get_dir(info_t *info);

char *allocate_buf(char *dir);

char *get_history_file(info_t *info);

int open_file(char *filename);

void write_to_file(list_t *node, ssize_t fd);

int write_history(info_t *info);

char *get_file(info_t *info);

ssize_t open_and_read_file(char *filename, char **buf, struct stat *st);

void process_buffer(info_t *info, char *buf, ssize_t fsize, int *linecount);

void update_history(info_t *info, int linecount);

int read_history(info_t *info);

int update_node_numbers(list_t **node);

int renumber_history(info_t *info);

list_t *get_node(info_t *info);

int build_history_list(info_t *info, char *buf, int linecount);

/* liststr.c module */
list_t *generate_unique_node(void);
void populate_node_data(list_t *node, const char *str, int num);
void link_new_head(list_t **head, list_t *new_head);
list_t *add_node(list_t **head, const char *str, int num);
list_t *create_new_node(const char *str, int num);
list_t *find_last_node(list_t *node);
void link_new_node_to_end(list_t *node, list_t *new_node);
list_t *add_node_end(list_t **head, const char *str, int num);
void print_node(const list_t *node);
size_t print_list_str(const list_t *h);
int is_head_null(list_t **head);
void delete_node(list_t **node);
void delete_head_node(list_t **head);
void delete_index_node(list_t **node, list_t *prev_node);
int iterate_through_nodes(list_t **head, unsigned int index);
int delete_node_at_index(list_t **head, unsigned int index);
void annihilate_node(list_t *node);
list_t *fetch_next_in_line(list_t *node);
void free_list(list_t **head_ptr);
/* liststr2.c module */
bool is_list_empty(const list_t *h);
size_t list_len(const list_t *h);
char **allocate_string_array(size_t count);
char *allocate_string(size_t length);
void deallocate_string_array(char **string_array, size_t count);
char **populate_string_array(list_t *node, char **string_array);
char **list_to_strings(list_t *head);
void _Print_The_Node_(const list_t *node, size_t *i);
size_t traverse_list(const list_t *h, void (*func)(const list_t*, size_t*));
size_t print_list(const list_t *h);
char *check_prefix_and_condition(list_t *node, char *prefix, char c);
list_t *node_starts_with(list_t *node, char *prefix, char c);
ssize_t check_node(list_t *head, list_t *node, size_t *i);
ssize_t get_node_index(list_t *head, list_t *node);
/* chain.c */

int check_operator(info_t *info, char *buf, size_t *j);
int is_chain(info_t *info, char *buf, size_t *p);
int check_condition(info_t *info);
void update_buffer_and_index(info_t *info, char *buf,
		size_t *index, size_t len);
void check_chain(info_t *info, char *buf, size_t *p,
		size_t i, size_t len);
list_t *get_node(info_t *info);
char *get_new_p(list_t *node);
int replace_alias(info_t *info);
int is_dollar_sign_present(char *arg);
void replace_status(info_t *info, int i);
void replace_pid(info_t *info, int i);
void replace_env_var(info_t *info, int i);
void replace_empty(info_t *info, int i);
int replace_vars(info_t *info);
int replace_string(char **old, char *new);

/*************************new bulit 1******/

int process_exit_arg(info_t *info, int exitcheck);
void print_help_message(info_t *info);
char *get_current_dir(char *buffer);
int change_dir(info_t *info, char *s, char *dir);
void update_env_vars(info_t *info, char *buffer, int chdir_ret);

/********************************new function bulit 11*******/
char *find_equal_sign(char *str);
int delete_alias_node(info_t *info, char *str);
int unset_alias(info_t *info, char *str);
char *find_equal_sign_in_node(list_t *node);
void print_node_until_char(list_t *node, char *p);
int print_alias(list_t *node);
void print_all_aliases(info_t *info);
void handle_alias(info_t *info, char *arg);
int set_alias(info_t *info, char *str);
int handle_unset_alias(info_t *info, char *str, char *p);

#endif
