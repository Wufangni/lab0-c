#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(*head));

    if (!head)
        return NULL;

    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;
    element_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, head, list)
        q_release_element(entry);
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;
    new->value = strdup(s);
    if (!(new->value)) {
        free(new);
        return false;
    }
    list_add(&new->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;
    new->value = strdup(s);
    if (!(new->value)) {
        free(new);
        return false;
    }
    list_add(&new->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!(head) || list_empty(head))
        return NULL;
    element_t *remove_h_list = list_entry(head->next, element_t, list);
    if (sp) {
        strncpy(sp, remove_h_list->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_del(head->next);
    return remove_h_list;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!(head) || list_empty(head))
        return NULL;
    element_t *remove_h_list = list_entry(head->prev, element_t, list);
    if (sp) {
        strncpy(sp, remove_h_list->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_del(head->prev);
    return remove_h_list;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!(head) || list_empty(head))
        return 0;
    int node_count = 0;
    for (struct list_head *cur = head->next; cur != head; cur = cur->next)
        node_count++;
    return node_count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!(head) || list_empty(head))
        return false;
    struct list_head *cur = head->next;
    for (int i = 0; i < q_size(head) / 2; i++)
        cur = cur->next;
    element_t *removed_element = list_entry(cur, element_t, list);
    list_del(cur);
    free(removed_element->value);
    free(removed_element);
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!(head) || list_empty(head))
        return false;
    struct list_head *ori = head->next;
    struct list_head *cur = head->next->next;
    bool flag = false;
    while (true) {
        if (cur == head)
            break;
        element_t *ori_element = list_entry(ori, element_t, list);
        element_t *cur_element = list_entry(cur, element_t, list);
        if (strcmp(cur_element->value, ori_element->value) != 0 &&
            flag == false) {
            ori = cur;
            cur = cur->next;
        } else if (strcmp(cur_element->value, ori_element->value) != 0 &&
                   flag == true) {
            element_t *tmp_element = list_entry(ori, element_t, list);
            list_del(&tmp_element->list);
            ori = cur;
            free(tmp_element->value);
            free(tmp_element);
            cur = cur->next;
            flag = false;
        } else {
            flag = true;
            cur = cur->next;
            element_t *tmp_element = list_entry(cur->prev, element_t, list);
            list_del(&tmp_element->list);
            free(tmp_element->value);
            free(tmp_element);
            if (cur == head) {
                element_t *tmp2_element = list_entry(ori, element_t, list);
                list_del(&tmp2_element->list);
                free(tmp2_element->value);
                free(tmp2_element);
                break;
            }
        }
    }
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *front = head->next, *behind = front->next;
    while (true) {
        if (front == head || behind == head)
            break;
        (front->prev)->next = behind;
        behind->prev = front->prev;
        (behind->next)->prev = front;
        front->next = behind->next;
        front->prev = behind;
        behind->next = front;
        front = front->next;
        behind = front->next;
    }
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *nex, *s;
    list_for_each_safe (nex, s, head) {
        element_t *tmp_element = list_entry(nex->next, element_t, list);
        nex->next = nex->prev;
        nex->prev = &tmp_element->list;
    }
    struct list_head *tmp_head_next = head->next;
    head->next = head->prev;
    head->prev = tmp_head_next;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
