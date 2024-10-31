#include "argo.h"

int	peek(FILE *stream)
{
	int	c = getc(stream);
	ungetc(c, stream);
	return c;
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("unexpected token '%c'\n", peek(stream));
	else
		printf("unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return 1;
	}
	return 0;
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return 1;
	unexpected(stream);
	return 0;
}

size_t	ft_strlen(char *s)
{
	size_t	i = 0;

	while (s && s[i])
		i++;
	return (i);
}

#define BAD -1
#define MAP 0
#define INTEGER 1
#define STRING 2
#define	SINGLE 1
#define DOUBLE 2

int	get_mode(char *s)
{
	if (!s || !s[0] || s[0] == '\"')
		return STRING;
	if (s[0] == '{')
		return MAP;
	if ((isdigit(s[0]) || (s[0] == '-' && isdigit(s[1]))))
		return INTEGER;
	return BAD;
}

json	parse_map(FILE *stream)
{
	;
}

int	argo(json *dst, FILE *stream)
{
	char	scout;
	char	*s = malloc(2);
	s[1] = 0;
	size_t	len;
	int	mode;
	int	num = 0;
	char	rest[1000];

	scout = peek(stream);
	if (isdigit(scout) || scout == '-')
	{
		fscanf(stream, "%d%s", &num, &rest[0]);
		printf("num: %d\n", num);
		printf("rest: %s\n", rest);
		if (!rest[0])
			printf("no rest\n");
		mode = INTEGER;
	}
	else
	{
		while (1)
		{
			scout = peek(stream);
			if (scout == EOF)
				break ;
			if (!expect(stream, scout))
			{
				free(s);
				return -1;
			}
			len = ft_strlen(s);
			s = realloc(s, len + 2);
			s[len] = scout;
			s[len + 1] = '\0';
		}
		mode = get_mode(s);
	}
	if (mode == BAD)
		return -1;
	if (mode == INTEGER)
	{
		*dst = (json){.type = INTEGER, .integer = num};
	}
	else if (mode == MAP)
	{
		json	res_json;
		res_json = parse_map(stream);
		//if (!res_json)
		//	return -1;
		*dst = res_json;
	}
	else if (mode == STRING)
	{
		int	i = -1;
		int	j = -1;
		int	quotes = 0;
		int	len = ft_strlen(s);
		char	*res = malloc(sizeof(char) * len + 1);

		while (++i < len)
		{
			if (s[i] == '\'')
			{
				if (quotes == SINGLE)
					quotes = 0;
				else
					quotes = SINGLE;
			}
			else if (s[i] == '\"')
			{
				if (quotes == DOUBLE)
					quotes = 0;
				else
					quotes = DOUBLE;
			}
			else if (s[i] == '\\')
			{
				if (s[i + 1] == '\"')
				{
					res[++j] = '\"';
					i++;
				}
				else
					res[++j] = '\\';
			}
			else
				res[++j] = s[i];
		}
		res[++j] = '\0';
		*dst = (json){.type = STRING, .string = res};
	}
	free(s);
	return 1;
}
