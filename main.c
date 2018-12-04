#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum bval {
	FALSE = 0,
	TRUE = 1,
	UNKNOWN = 2,
	ANY = 2,
};

struct entry {
	int id;
	char *name;
	int val;
};

struct literal {
	int inv;
	struct entry *lit;
};

void
debug (struct literal **expr)
{
	int i, j;
	int inv;
	struct entry *lit;

	for (i = 0; expr[i] != NULL; ++i) {
		for (j = 0; expr[i][j].lit != NULL; j++) {
			lit = expr[i][j].lit;
			inv = expr[i][j].inv;
			printf("%s%s: ", inv ? "-" : "", lit->name);

			if (lit->val == ANY)
				printf("A");
			else
				printf("%d", inv ? !lit->val : lit->val);

			printf("\t");
		}

		printf("\b\n");
	}
}

int
check(struct literal **expr)
{
	int i, j;
	int inv, realval;
	int expval, ret;
	struct entry *lit;

	for (i = 0, ret = TRUE; expr[i] != NULL; ++i) {
		for (j = 0, expval = FALSE; expr[i][j].lit != NULL; j++) {
			lit = expr[i][j].lit;
			inv = expr[i][j].inv;
			realval = lit->val == UNKNOWN ? UNKNOWN : inv ? !lit->val : lit->val;

			if (realval == UNKNOWN)
				expval = UNKNOWN;
			else if (realval == TRUE) {
				expval = TRUE;
				break;
			}
		}

		if (expval == UNKNOWN)
			ret = UNKNOWN;
		else if (expval == FALSE)
			return FALSE;
	}

	return ret;
}

int
solver(struct entry *list, struct literal **expr)
{
	int i;
	int ret;

	for (i = 0; list[i].id != 0;) {
		list[i].val =
		    list[i].val == UNKNOWN ? TRUE :
		    list[i].val == TRUE ? FALSE : UNKNOWN;

		if (list[i].val == UNKNOWN) {
			if (i == 0)
				break;

			list[i--].val = UNKNOWN;
			continue;
		}

		debug(expr);

		ret = check(expr);
		if (ret == TRUE) {
#if 1
			int i;
			printf("TRUE: ");
			for (i = 0; list[i].id != 0; ++i)
				printf("%s: %d, ", list[i].name, list[i].val);
			printf("\b\b \n");
#endif
		}
		else if (ret == FALSE)
			list[i--].val = UNKNOWN;
		else if (ret == UNKNOWN)
			++i;

		putchar('\n');
	}

	return 0;
}

int
main(void)
{
	size_t size;
	struct literal **expr;
	struct entry *list;

	list = malloc(4*sizeof (struct entry));

	list[0].id = 1;
	list[0].name = "a";
	list[0].val = UNKNOWN;
	list[1].id = 2;
	list[1].name = "b";
	list[1].val = UNKNOWN;
	list[2].id = 3;
	list[2].name = "c";
	list[2].val = UNKNOWN;
	list[3].id = 0;
	list[3].name = NULL;
	list[3].val = UNKNOWN;

	expr = malloc(3*sizeof (struct literal*));

	size = 4*sizeof (struct literal);
	expr[0] = malloc(size);
	expr[1] = malloc(size);
	expr[2] = NULL;

	expr[0][0].inv = 0;
	expr[0][0].lit = list;
	expr[0][1].inv = 0;
	expr[0][1].lit = list + 1;
	expr[0][2].inv = 0;
	expr[0][2].lit = list + 2;
	expr[0][3].inv = 0;
	expr[0][3].lit = NULL;

	expr[1][0].inv = 1;
	expr[1][0].lit = list;
	expr[1][1].inv = 0;
	expr[1][1].lit = list + 1;
	expr[1][2].inv = 1;
	expr[1][2].lit = list + 2;
	expr[1][3].inv = 0;
	expr[1][3].lit = NULL;

	solver(list, expr);

	return 0;
}
