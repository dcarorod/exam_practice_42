#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	enum {
		ADD,
		MULTI,
		VAL
	} type;
	int		val;
	struct node	*l;
	struct node	*r;
} node;

node	*new_node(node n);
void	unexpected(char c);
int	accept(char *s, char c);
int	expect(char *s, char c);
int	eval_tree(node *tree);
node	*pars_expr(char *s);

/*

./vbc '1'
1
./vbc '3*4+5'
17
./vbc '(3+4)*5'
35
./vbc (((((((1)))))))
1
./vbc '(((((2+2)*2+2)*2+2)*2+2)*2+2)*2'
188
./vbc '1+2+3+4+5'
15
./vbc '((1+3)*12+(3*(2+6)))'
unexpected token '2'

*/
