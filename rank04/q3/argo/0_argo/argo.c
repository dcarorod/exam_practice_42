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

char	*extract_string(FILE *stream, char scout)
{
	char	*str = NULL;
       
	str = calloc(4096, sizeof(char));
	if (!str)
		return NULL;
	accept(stream, scout);
	scout = peek(stream);
	int	i = -1;
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
		scout = peek(stream);
		if (scout == EOF)
			break ;
		while (scout == ' ')
			scout = accept_and_get_next_scout(stream, scout);
		if (isdigit(scout) || scout == '-')
		{
			fscanf(stream, "%d", &num);
			*dst = (json){.type = INTEGER, .integer = num};
			return 1;
		}
		if (scout == '\"')
		{
			str = extract_string(stream, scout);
			if (!str)
				return -1;
			*dst = (json){.type = STRING, .string = str};
			return 1;
		}
		if (scout == '{')
		{
			dst->map.data = NULL;

			accept(stream, scout);
			while (1)
			{
				dst->map.data = realloc(dst->map.data, \
						(dst->map.size + 1) * sizeof(pair));
				if (!dst->map.data)
					return -1;
				scout = peek(stream);
				while (scout == ' ')
					scout = accept_and_get_next_scout(stream, scout);
				if (scout == '\"')
				{
					str = extract_string(stream, scout);
					if (!str)
						return -1;
					dst->map.data[dst->map.size].key = str;
					scout = peek(stream);
					while (scout == ' ')
						scout = accept_and_get_next_scout(stream, scout);
					if (!expect(stream, ':'))
						return -1;
					scout = peek(stream);
					while (scout == ' ')
						scout = accept_and_get_next_scout(stream, scout);
					// printf("scout before recursive call: %c\n", scout);
					if (argo(&dst->map.data[dst->map.size].value, stream) == -1)
						return -1; // Error in recursive call
					scout = peek(stream);
					// printf("dst->map.data.key: %s\n", dst->map.data[0].key);
					// printf("dst->map.data.val: %d\n", dst->map.data[0].value.integer);
					// printf("scout after recursive call: %c\n", scout);
				}
				dst->map.size++;
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
			if (expect(stream, '}'))
				return 1;
			return -1;
		}
	}
}
