#include"catarray.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void add_cat(catarray_t* cat_array, char* cat_name, int cat_len){

	int i = (int)cat_array->n_cats;
	cat_array->catlist = (category_t**)realloc(cat_array->catlist, (i+1)*sizeof(category_t*));
	cat_array->catlist[i] = (category_t*)malloc(sizeof(category_t));
	cat_array->catlist[i]->category = cat_name;
	cat_array->catlist[i]->n_words = 0;
	cat_array->catlist[i]->wordlist = NULL;
	cat_array->n_cats += 1;
}

void add_word_to_cat(catarray_t* cat_array, char* word_name, int word_len){
	int index = (int)(cat_array->n_cats-1);
	int j = (int)cat_array->catlist[index]->n_words;
	cat_array->catlist[index]->wordlist = (char**)realloc(cat_array->catlist[index]->wordlist, (j+1)*sizeof(char*));
	cat_array->catlist[index]->wordlist[j] = word_name;
	cat_array->catlist[index]->n_words += 1;
}


catarray_t* read_words(FILE* f){
	catarray_t* cat_array = (catarray_t*) malloc(sizeof(catarray_t));
	cat_array->catlist = NULL;
	cat_array->n_cats = 0; //initialize

	char* line = NULL;
	size_t len = 0;
	while(getline(&line, &len, f) != -1){
		char* word_name;
		const char look_for = ':';

		word_name = strchr(line, look_for); //get word name
		word_name[(int)(strnlen(word_name, 1000))] = '\0';

		int line_len = (int)(strnlen(line, 1000));
		int word_len = (int)(strnlen(word_name, 1000));
		int cat_len = line_len - word_len;

		char* cat_name = (char*)malloc(sizeof(char)*(cat_len+1));
		strncpy(cat_name, line, cat_len);
		cat_name[cat_len] = '\0';

		word_name++; //point to cat first char
		word_len -= 2;
		char* temp = word_name;
		word_name = (char*)malloc(sizeof(char)*(word_len+1));
		strncpy(word_name, temp, word_len);
		word_name[word_len] = '\0';

		int i = (int)cat_array->n_cats;

		if(i == 0) {
			add_cat(cat_array, cat_name, cat_len);
		}
		else{
			if(strcmp(cat_name, cat_array->catlist[i-1]->category) != 0){
				add_cat(cat_array, cat_name, cat_len);
			}
			else{
				free(cat_name);
			}
		}
		add_word_to_cat(cat_array, word_name, word_len);
		free(line);
		line = NULL;
	}
	free(line);
	// for(int i=0; i<cat_array->n_cats; i++){
	// 	printf("Category: %s\n", cat_array->catlist[i]->category);
	// 	for(int j=0; j<cat_array->catlist[i]->n_words; j++){
	// 		printf("%s ", cat_array->catlist[i]->wordlist[j]);
	// 	}
	// 	printf("\n" );
	// }
	// printf("\n" );
	return cat_array;
}



void read_story(FILE* f, catarray_t* cat_array, int r){
	char* line = NULL;
	size_t len = 0;
	char* mem[50];
	int count = 0;

	while(getline(&line, &len, f) != -1){
		const char look_for = '_';
		char* temp = line;
		while(*temp != '\n'){
			if(*temp != '_') {
				printf("%c", *temp); //find first "_"
			}
			else{
				count++;
				temp++;
				char* next = strchr(temp, look_for);
				int type_len = (int)(next-temp);
				char type_name[type_len+1];
				strncpy(type_name, temp, type_len);
				type_name[type_len] = '\0';
				if(type_len == 1){
					printf("%s", mem[count-atoi(type_name)-1]);
					mem[count-1] = mem[count-atoi(type_name)-1];
				}
				else{
					for(int j=0; j<cat_array->n_cats; j++){
						if(strcmp(type_name, cat_array->catlist[j]->category) == 0){
							r = rand();
							r = r%(cat_array->catlist[j]->n_words);
							// printf("\n R is: %d\n", r);
							printf("%s", cat_array->catlist[j]->wordlist[r]);
							mem[count-1] = cat_array->catlist[j]->wordlist[r];
						}
					}
				}
				temp += type_len;
			}
			temp++;
		}
		free(line);
		line = NULL;
		printf("\n");
	}
	free(line);
}

void free_words(catarray_t* cat_array){
	for(int i=0; i<cat_array->n_cats; i++){
		for(int j=0; j<cat_array->catlist[i]->n_words; j++){
			free(cat_array->catlist[i]->wordlist[j]);
		}
		free(cat_array->catlist[i]->category);
		free(cat_array->catlist[i]->wordlist);
		free(cat_array->catlist[i]);
	}
	free(cat_array->catlist);
	free(cat_array);
}


int main(int argc, char **argv){
	FILE* story_temp = fopen(argv[1], "r");
	FILE* word_list = fopen(argv[2], "r");

	srand(atoi(argv[3]));

	catarray_t* cat_array = read_words(word_list);
	read_story(story_temp, cat_array, atoi(argv[3]));

	free_words(cat_array);
	fclose(story_temp);
	fclose(word_list);
	return 0;
}
