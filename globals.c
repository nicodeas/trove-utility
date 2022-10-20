#include "globals.h"

HEAD_LINK *head_link_find(HEAD_LINK *head, char *wanted)
{
    while (head != NULL)
    {
        if (strcmp(head->word, wanted) == 0)
        {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

HEAD_LINK *new_head_link(char *string)
{
    HEAD_LINK *new = malloc(sizeof(HEAD_LINK));
    new->word = strdup(string);
    new->next = NULL;
    new->link_to_paths = NULL;
    return new;
}

LINK *new_link(char *path)
{
    LINK *new = malloc(sizeof(LINK));
    new->next = NULL;
    new->path = strdup(path);
    return new;
}

HEAD_LINK *head_link_add(HEAD_LINK *curr_head, char *string)
{
    HEAD_LINK *new = new_head_link(string);
    new->next = curr_head;
    return new;
}

bool path_link_find(LINK *link, char *wanted_path)
{
    while (link != NULL)
    {
        if (strcmp(link->path, wanted_path) == 0)
        {
            return true;
        }
        link = link->next;
    }
    return false;
    return true;
}

uint32_t hash_string(char *string)
{
    uint32_t hash = 0;

    while (*string != '\0')
    {
        hash = hash * 33 + *string;
        ++string;
    }
    return hash;
}

HASHTABLE *hashtable_new(void)
{
    HASHTABLE *new = calloc(HASHTABLE_SIZE, sizeof(HEAD_LINK *));

    return new;
}

HEAD_LINK *hashtable_add(HASHTABLE *hashtable, char *string)
{
    uint32_t h = hash_string(string) % HASHTABLE_SIZE; // choose list

    hashtable[h] = head_link_add(hashtable[h], string);
    return hashtable[h];
}

HEAD_LINK *hashtable_find(HASHTABLE *hashtable, char *string)
{
    char *cpy = string;
    uint32_t h = hash_string(cpy) % HASHTABLE_SIZE; // choose list
    return head_link_find(hashtable[h], string);
}