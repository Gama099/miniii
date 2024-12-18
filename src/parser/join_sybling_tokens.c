#include "../../includes/minishell.h"

t_tokens	*create_node_join(t_tokens *token_a, t_tokens *token_b)
{
	t_tokens	*new_node;

	if(!token_a || !token_b)
		return (NULL);
	new_node = malloc(sizeof(t_tokens));
	if(!new_node)
		return (NULL);
	//TODO
	new_node->token = ft_strjoin(token_a->token, token_b->token);
	new_node->tokenType = NULL;
	new_node->qoute_type = 0;
	new_node->join_with_next = token_b->join_with_next;
	new_node->expand_env = token_b->expand_env;
	new_node->next = token_b->next;
	return (new_node);
}



void	join_token_syblings(t_tokens **token)
{
	t_tokens	*list_iter;
	t_tokens	*merged_nodes;
	t_tokens	*garb_node;
	t_tokens	*prev;

	//if (token || *token)
	//	return ;
	list_iter = *token;
	prev = NULL;
	while (list_iter)
	{
		if (!is_spaces(list_iter->token) && list_iter->qoute_type != 2)
		{
			printf(">>>>>>i will free this %p\n", list_iter);
			t_tokens *tmp = list_iter;
			if (prev){
				tmp = list_iter;
				list_iter = list_iter->next;
				prev->next = list_iter;
				free(tmp);
				if (list_iter == NULL)
					break;
			}
			else
			{
				tmp = list_iter;
				list_iter = list_iter->next;
				free(tmp);
				*token = list_iter;
				if (*token == NULL)
					break;
			}
		}
		// printf("adress = %p\n", list_iter);
		else if ((!ft_strcmps(list_iter->token, "$") && list_iter->join_with_next))
		{
			// free that token with '$'
			printf("<<i will free this %p\n", list_iter);
			t_tokens *tmp;
			if (prev)
			{
				tmp = list_iter;
				list_iter = list_iter->next;
				prev->next = list_iter;
				free(tmp);
			}
			else
			{
				tmp = list_iter;
				list_iter = list_iter->next;
				*token = list_iter;
				free(tmp);
			}
			if (!*token)
				break;
		}
		else if (list_iter->join_with_next)
		{
			merged_nodes = create_node_join(list_iter, list_iter->next);
            if (!merged_nodes)
				return ;
			//printf("merged[%s]\n", merged_nodes->token);
            //sleep(5);
            garb_node = list_iter;
            list_iter = list_iter->next;
            free(garb_node);
            free(list_iter);
			// we need previous to be linked with the merged account
			if (prev)
				prev->next = merged_nodes;
			else
				*token = merged_nodes;
            list_iter = merged_nodes;
		}
		else{
			prev = list_iter;
			list_iter = list_iter->next;
		}
	}
}
