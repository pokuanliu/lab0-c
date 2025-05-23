#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new() {
  struct list_head *newhead = malloc(sizeof(struct list_head));
  if (!newhead) {
    return NULL;
  }
  INIT_LIST_HEAD(newhead);
  return newhead;
}

/* Free all storage used by queue */
void q_free(struct list_head *head) {
  if (!head) {
    return;
  }
  struct list_head *curr, *next;

  list_for_each_safe(curr, next, head) {
    element_t *node = list_entry(curr, element_t, list);
    list_del(curr);
    free(node->value);
    free(node);
  }
  free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s) {
  if (!head || !s) {
    return false;
  }
  element_t *newhead = malloc(sizeof(element_t));
  if (!newhead) {
    return false;
  }

  newhead->value = strdup(s);
  if (!newhead->value) {
    free(newhead);
    return false;
  }
  list_add(&newhead->list, head);
  return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s) {
  if (!head || !s) {
    return false;
  }
  element_t *newtail = malloc(sizeof(element_t));
  if (!newtail) {
    return false;
  }

  newtail->value = strdup(s);
  if (!newtail->value) {
    free(newtail);
    return false;
  }
  list_add_tail(&newtail->list, head);
  return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize) {
  if (!head) {
    return NULL;
  }
  if (head->next == head) {
    return NULL;
  }
  if (!sp) {
    return NULL;
  }
  if (bufsize < 0) {
    return NULL;
  }
  element_t *node = list_entry(head->next, element_t, list);
  list_del(head->next);
  strncpy(sp, node->value,
          bufsize - 1); // bufsize-1是因為要讓sp有一個 \0 結束字元的空間。
  sp[bufsize - 1] = '\0';
  return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize) {
  if (!head) {
    return NULL;
  }
  if (head->next == head) {
    return NULL;
  }
  if (!sp) {
    return NULL;
  }
  if (bufsize < 0) {
    return NULL;
  }
  element_t *node = list_entry(head->prev, element_t, list);
  list_del(head->prev);
  strncpy(sp, node->value,
          bufsize - 1); // bufsize-1是因為要讓sp有一個 \0 結束字元的空間。
  sp[bufsize - 1] = '\0';
  return node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head) {
  if (!head) {
    return 0;
  }
  struct list_head *curr, *next;
  int count = 0;
  list_for_each_safe(curr, next, head) { count = count + 1; }

  return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head) {
  // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
  if (!head) {
    return false;
  }
  if (!head->next) {
    return false;
  }
  if (head->next->next == head) {
    return head;
  }
  struct list_head *fast = head->next;
  struct list_head *slow = head;
  while (fast != head && fast->next != head) {
    fast = fast->next->next;
    slow = slow->next;
  }
  fast = slow->next;
  list_del(fast);
  element_t *node = list_entry(fast, element_t, list);
  free(node->value);
  free(node);

  return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head) {
  // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
  if (!head) {
    return false;
  }
  if (head->next == head) {
    return false;
  }
  if (head->next->next == head) {
    return true;
  }

  struct list_head dummy;
  struct list_head *prev = &dummy;
  prev->next = head->next;
  prev = prev->next;
  struct list_head *curr = prev->next;
  struct list_head *temp;
  element_t *prev_node;
  element_t *curr_node;
  bool dedup = false;
  int same;

  prev_node = list_entry(prev, element_t, list);
  curr_node = list_entry(curr, element_t, list);

  while(curr != head){
    prev_node = list_entry(prev, element_t, list);
    curr_node = list_entry(curr, element_t, list);
    same = strcmp(prev_node->value, curr_node->value);
    if(same == 0){
      dedup = true;
      temp = prev;
      prev = curr;
      curr = curr->next;
      list_del(temp);
      element_t *temp_node = list_entry(temp, element_t, list);
      free(temp_node->value);
      free(temp_node);
    }
    else{
      if(dedup){
        temp = prev;
        prev = curr;
        curr = curr->next;
        list_del(temp);
        element_t *temp_node = list_entry(temp, element_t, list);
        free(temp_node->value);
        free(temp_node);
        dedup = false;
      }
      else{
        prev = curr;
        curr = curr->next;
      }
    }
  }
  if(dedup && curr==head){
    temp = prev;
    prev = prev->prev;
    list_del(temp);
    element_t *temp_node = list_entry(temp, element_t, list);
    free(temp_node->value);
    free(temp_node);
  }


  return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head) {
  // https://leetcode.com/problems/swap-nodes-in-pairs/
  if (!head) {
    return;
  }
  if (!head->next) {
    return;
  }
  if (head->next->next == head) {
    return;
  }
  struct list_head *prevnode = head;
  struct list_head *currnode = head->next;

  while (currnode != head && currnode->next != head) {
    prevnode->next = currnode->next;
    currnode->next = prevnode->next->next;
    prevnode->next->next = currnode;

    prevnode->next->prev = prevnode;
    currnode->prev = prevnode->next;
    currnode->next->prev = currnode;

    prevnode = currnode;
    currnode = currnode->next;
  }
  if (currnode == head) {
    head->prev = prevnode;
  } else if (currnode->next == head) {
    head->prev = currnode;
  }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {
  if (!head) {
    return;
  }
  struct list_head *curr;
  struct list_head *next;
  struct list_head *prev;
  list_for_each_safe(curr, next, head) {
    prev = curr->prev;
    curr->prev = curr->next;
    curr->next = prev;
  }
  prev = head->prev;
  head->prev = head->next;
  head->next = prev;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k) {
  // https://leetcode.com/problems/reverse-nodes-in-k-group/
  
}

/* Sort elements of queue in ascending/descending order */
struct list_head *mergetwolist(struct list_head *left, struct list_head *right,
                               bool descend) {
  struct list_head dummy;
  struct list_head *temp = &dummy;
  INIT_LIST_HEAD(&dummy);
  bool split;

  while (left && right) {
    element_t *left_node = list_entry(left, element_t, list);
    element_t *right_node = list_entry(right, element_t, list);
    int cmp = strcmp(left_node->value, right_node->value);
    if (descend) {
      split = (cmp > 0);
    } else {
      split = (cmp <= 0);
    }

    if (split) {
      temp->next = left;
      left->prev = temp;
      left = left->next;
    } else {
      temp->next = right;
      right->prev = temp;
      right = right->next;
    }
    temp = temp->next;
  }
  if (left) {
    temp->next = left;
    left->prev = temp;
    // left->next不要設置為NULL
  } else if (right) {
    temp->next = right;
    right->prev = temp;
    // right->next不要設置為NULL
  }
  return dummy.next;
}
struct list_head *mergesort(struct list_head *head, bool descend) {
  if (!head) {
    return head;
  }
  if (!head->next) {
    return head;
  }
  struct list_head *fast = head->next;
  struct list_head *slow = head;
  while (fast && fast->next) {
    fast = fast->next->next;
    slow = slow->next;
  }
  fast = slow->next;
  slow->next = NULL;

  struct list_head *left = mergesort(head, descend);
  struct list_head *right = mergesort(fast, descend);

  return mergetwolist(left, right, descend);
}
void q_sort(struct list_head *head, bool descend) {
  if (!head) {
    return;
  }
  if (head->next == head) {
    return;
  }
  if (q_size(head) == 1) {
    return;
  }
  head->prev->next = NULL;
  head->next = mergesort(head->next, descend);
  head->next->prev = head;
  struct list_head *arr = head;
  while (arr->next) {
    arr = arr->next;
  }
  arr->next = head;
  head->prev = arr;
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head) {
  // https://leetcode.com/problems/remove-nodes-from-linked-list/
  return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head) {
  // https://leetcode.com/problems/remove-nodes-from-linked-list/
  return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend) {
  // https://leetcode.com/problems/merge-k-sorted-lists/
  if (!head) {
    return 0;
  }
  if (!head->next) {
    return 0;
  }
  queue_contex_t *first = list_entry(head->next, queue_contex_t, chain);
  if (head->next->next == head) {
    return first->size;
  }
  for (struct list_head *curr = head->next->next; curr != head;
       curr = curr->next) {
    queue_contex_t *curr_queue = list_entry(curr, queue_contex_t, chain);
    list_splice_init(curr_queue->q, first->q);
  }
  q_sort(first->q, descend);
  first->size = q_size(first->q);

  return first->size;
}
