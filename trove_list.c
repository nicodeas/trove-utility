#include <string.h>
#include "trove.h"

PATH_LINK *new_path_link()
{
    PATH_LINK *new_link = malloc(sizeof(PATH_LINK));
    new_link->filepath = NULL;
    new_link->next = NULL;
    return new_link;
}

bool path_exists(HEAD_LINK *head, char *target_path)
{
    PATH_LINK *path_link = head->path_list;
    while (path_link != NULL)
    {
        if (strcmp(path_link->filepath, target_path) == 0)
        {
            return true;
        }
        path_link = path_link->next;
    }
    return false;
}

void free_path_link(PATH_LINK *path_link)
{
    free(path_link->filepath);
    free(path_link);
}

HEAD_LINK *new_head_link()
{
    HEAD_LINK *new_head = malloc(sizeof(HEAD_LINK));
    new_head->word = NULL;
    new_head->next = NULL;
    new_head->path_list = NULL;
    return new_head;
}

HEAD_LINK *find_head_link(HEAD_LINK *head, char *target_word)
{
    HEAD_LINK *current_head = head;
    while (current_head != NULL)
    {
        if (strcmp(head->word, target_word) == 0)
        {
            return current_head;
        }
        current_head = current_head->next;
    }
    return NULL;
}

void free_head_link(HEAD_LINK *head)
{
    free(head->word);
    free(head);
}