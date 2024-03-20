#include "shuffle.h"

/* Return number of elements in queue */
static int q_size(struct list_head *head)
{
    if (!(head) || list_empty(head))
        return 0;
    int node_count = 0;
    for (struct list_head *cur = head->next; cur != head; cur = cur->next)
        node_count++;
    return node_count;
}

static inline void swap(struct list_head *a, struct list_head *b)
{
    if (a == b)
        return;

    //  b --> a
    struct list_head *a_prev = a->prev, *b_prev = b->prev;

    if (b != a_prev)
        list_move(b, a_prev);
    if (a != b_prev)
        list_move(a, b_prev);
}

void q_shuffle(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *last = head->prev;
    for (int len = q_size(head); len > 1; len--) {
        srand(time(NULL));
        int random_idx = rand() % len;

        struct list_head *random_node = head->next;

        while (random_idx--)
            random_node = random_node->next;

        swap(last, random_node);

        last = last->prev;
    }
}