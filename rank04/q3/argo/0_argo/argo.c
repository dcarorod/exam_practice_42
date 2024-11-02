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

char	accept_and_get_next_scout(FILE *stream, char scout)
{
	accept(stream, scout);
	return (peek(stream));
}

void	pace_whitespaces(FILE *stream, char *scout)
{
	*scout = peek(stream);
	while (*scout == ' ')
	{
		accept(stream, *scout);
		*scout = peek(stream);
	}
}

// opening \" has already been swallowed
char	*extract_string(FILE *stream)
{
	char	*str = NULL;
	char	scout;
	int	i = -1;
       
	str = calloc(4096, sizeof(char));
	if (!str)
		return NULL;
	scout = peek(stream);
	while (scout != '\"' && scout != EOF)
	{
		if (scout == '\\')
		{
			accept(stream, scout);
			scout = peek(stream);
			if (scout == '\"')
				str[++i] = '\"';
			else
			{
				str[++i] = '\\';
				str[++i] = scout;
			}
		}
		else
			str[++i] = scout;
		accept(stream, scout);
		scout = peek(stream);
	}
	if (expect(stream, '\"'))  // will print 'unexpected end of input'
		return str;
	free(str); // if EOF, then no closing \", report
	return NULL;
}

int	argo(json *dst, FILE *stream)
{
	char	scout = 0;
	char	*str = NULL;
	int	num = 0;

	dst->map.size = 0;
	while (1)
	{
		pace_whitespaces(stream, &scout);
		if (scout == EOF)
			break ;
		if (isdigit(scout) || scout == '-')
		{
			fscanf(stream, "%d", &num);
			*dst = (json){.type = INTEGER, .integer = num};
			pace_whitespaces(stream, &scout);
			if (scout != EOF && scout != '}' && scout != ',')
			{
				expect(stream, EOF);
				return -1;
			}
			return 1;
		}
		else if (scout == '\"')
		{
			accept(stream, scout);
			str = extract_string(stream);
			if (!str)
				return -1;
			*dst = (json){.type = STRING, .string = str};
			return 1;
		}
		else if (scout == '{')
		{
			dst->map.data = NULL;

			accept(stream, scout);
			while (1)
			{
				dst->map.data = realloc(dst->map.data, \
						(dst->map.size + 1) * sizeof(pair));
				if (!dst->map.data)
					return -1;
				pace_whitespaces(stream, &scout);
				if (!expect(stream, '\"'))
					return -1;
				str = extract_string(stream);
				if (!str)
					return -1;
				dst->map.data[dst->map.size].key = str;
				
				pace_whitespaces(stream, &scout);
				if (!expect(stream, ':'))
					return -1;
				pace_whitespaces(stream, &scout);
				
				if (argo(&dst->map.data[dst->map.size].value, stream) == -1)
					return -1; // Error in recursive call
				dst->map.size++;
				pace_whitespaces(stream, &scout);
				if (scout == ',')
				{
					scout = accept_and_get_next_scout(stream, scout);
					pace_whitespaces(stream, &scout);
					continue ;
				}
				if (scout == '}')
				{
					accept(stream, scout);
					return 1;
				}
				else if (scout == EOF)
				{
					expect(stream, '}');
					return -1;
				}
			}
		}
		else
		{
			expect(stream, EOF);
			return -1;
		}
	}
}
