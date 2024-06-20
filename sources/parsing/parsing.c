/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 07:33:24 by fberthou          #+#    #+#             */
/*   Updated: 2024/06/17 13:18:33 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

#include <stdio.h>

// ###### TEMP PROTOTYPES ######

int		expand_management(t_data *data, char **envp);
int		token_cleaner(t_data *data);

void	print_tab(t_table tab);
void	print_struct(t_data *data, int tab_size);

// ###### TEMP PROTOTYPES ######


/*	
	#### parse_prompt description ####

	* pre_treatment :
		// analyse of the prompt and add spaces between chars separators
	* tokenizer
		// cut prompt as table
		// ignore unused spaces before and after token 
		// but keep all chars in tokens
	* init_struct
		// alloc t_data table
		// fill all tables in the structure
			-> args
			-> env
			-> input files
			-> output files
*/

int	parse_prompt(char **prompt, char **env, t_data **data)
{
	t_table	tokens; // only for test ?
	int		struc_tab_size;
	int		ret_value;

	*prompt = pre_treatment(*prompt, 0);
	if (!*prompt)
		return (-1);
	//printf("p_prompt == %s\n", *prompt);
	tokens = tokenizer(*prompt);
	if (!tokens.tab)
		return (free(*prompt), -1);
	// printf("TOKEN TAB :\n");
	//print_tab(tokens);
	struc_tab_size = init_struct(data, &tokens, 0, 0);
	if (struc_tab_size == -1)
		return (free_tab(&tokens, 0), free(*prompt), -1);
	free_tab(&tokens, 0);
	//print_struct(*data, struc_tab_size);



// ### expand_management + token_cleaner in exec ### //

// ### expand variables in token struct  ### //
	ret_value = expand_management(*data, env);
	if (ret_value == -1)
		return (-1);
	if (ret_value == 1)
		return(0);

	//print_struct(*data, struc_tab_size);


// ### clean tokens to real cmd / args ### //

	ret_value = token_cleaner(*data); //0 = ok -1 error_crash, 1 syntaxe
	if (ret_value == 1)
		return (0);
	if (ret_value == -1)
		return (free(*prompt), -1); // free all is temporary, t_table token.tab is free in token_cleaner
	//print_tab(*args);
	print_struct(*data, struc_tab_size);

// ### clean tokens to real cmd / args END ### //

	return (struc_tab_size);
}
