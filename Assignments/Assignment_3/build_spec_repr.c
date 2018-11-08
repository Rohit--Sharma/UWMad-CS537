///////////////////////////////////////////////////////////////////////////////
//
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      October 25, 2018
//
///////////////////////////////////////////////////////////////////////////////

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include "text_parsing.h"
#include "build_spec_graph.h"
#include "build_spec_repr.h"

extern const int debug;

command *create_command (char *rule) {
    command *cmd = (command *) malloc(sizeof(command));
    cmd->rule = rule;
    cmd->next = NULL;
    return cmd;
}

void delete_command (command *cmd) {
    command *temp = NULL;
    while (cmd) {
        temp = cmd->next;
        free(cmd->rule);
        cmd = temp;
    }
}

MakeNode *create_node (char *target_line, command *cmds_head) {
    // fprintf(stdout, "Creating node\n");
    char **tokens = tokenize_string(target_line);
    char *target = tokens[0];
    char **dependencies = NULL;
    if (target[strlen(target)-1] == ':') {
    	target[strlen(target) - 1] = '\0';    // remove the : char
        dependencies = tokens + 1;
    }
    else {
        // ':' is a separate token. The dependencies start from index 2 in the tokenized strings
        dependencies = tokens + 2;
    }

    struct stat file_stat;
    if (stat(target, &file_stat) < 0) {
        // TODO: Display error info according to errno
        // fprintf(stderr, "Error in getting file statistics for %s\n", target);
    }
    // fprintf(stdout, "Last modified time for %s: %s\n", target, ctime(&file_stat.st_mtime));
    // to compare timestamps, use difftime(time_t t1, time_t t2) 

    MakeNode *makenode = (MakeNode *) malloc (sizeof(MakeNode));

    makenode->name = target;
    makenode->timestamp = file_stat.st_mtime;
    makenode->rules = cmds_head;
    makenode->isLeafNode = 0;
    makenode->children = dependencies;

    return makenode;
}

void delete_makenode (MakeNode *node) {
    free(node->name);
    delete_command(node->rules);

    char **dependencies = node->children;
    char *temp = NULL;
    int i = 0;
    while ((temp = *(dependencies + i)) != NULL) {
        free(temp);
        i++;
    }
}

void display_node (MakeNode *makenode) {
    if (makenode != NULL) {
        fprintf(stdout, "Target: %s\n", makenode->name);
        command *cmds_head = makenode->rules;
        while (cmds_head != NULL) {
            fprintf(stdout, "  Command: %s\n", cmds_head->rule);
            cmds_head = cmds_head->next;
        }
        fprintf(stdout, "\n");
        int i = 0;
        printf("   Dependencies: ");
        char **dependencies = makenode->children;
        while (*(dependencies + i) != NULL) {
            printf("%s | ", *(dependencies + i));
            i++;
        }
        printf("\n");
    }
    else
        fprintf(stderr, "Error: Cannot display makenode. makenode is NULL\n");
}

/**
 * Jenkins One At A Time Hash
 */
uint32_t hash_code (hash_table *map, char *key, size_t len)
{
    uint32_t hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % map->size;
}

hash_table *create_hash_table (int size) {
    hash_table *map = (hash_table *) malloc (sizeof(hash_table));
    map->size = size;
    map->list = (hash_node **) malloc (sizeof(hash_node *) * size);
    int i;
    for(i = 0; i < size; i++)
        map->list[i] = NULL;
    return map;
}

void delete_hash_table (hash_table *map, int size) {
    for (int i = 0; i < size; i++) {
        hash_node *list = map->list[i];
        hash_node *temp = NULL;
        while (list) {
            temp = list->next;
            free(list);
            list = temp;
        }
    }
}

void hash_insert (hash_table *t, char *key, MakeNode *val) {
    int pos = hash_code(t, key, strlen(key));
    hash_node *list = t->list[pos];
    hash_node *temp = list;
    while (temp) {
        if (strcmp(temp->key, key) == 0) {
            temp->val = val;
            return;
        }
        temp = temp->next;
    }
    hash_node *newNode = (hash_node *) malloc (sizeof(hash_node));
    newNode->key = key;
    newNode->val = val;
    newNode->next = list;
    t->list[pos] = newNode;
}

MakeNode *hash_lookup (hash_table *t, char *key) {
    int pos = hash_code(t, key, strlen(key));
    hash_node *list = t->list[pos];
    hash_node *temp = list;
    while (temp) {
        if (strcmp(temp->key, key) == 0) {
            return temp->val;
        }
        temp = temp->next;
    }
    return NULL;
}

// int main() {
//     hash_table *my_map = create_hash_table(100);
//     hash_insert(my_map, "hello", 3);
//     hash_insert(my_map, "rohit", 71);

//     printf("%d %d %d\n", hash_lookup(my_map, "rohit"), hash_lookup(my_map, "sharma"), hash_lookup(my_map, "hello"));

//     return 0;
// }
