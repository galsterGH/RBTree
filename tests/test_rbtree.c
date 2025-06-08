#include "RBTree.h"
#include "RBTreeImpl.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static void* std_alloc(size_t s){ return malloc(s); }
static void std_dealloc(void* p){ free(p); }
static int int_comp(void* a, void* b){
    int ia = *(int*)a;
    int ib = *(int*)b;
    return ia < ib ? -1 : (ia > ib ? 1 : 0);
}
static void ptr_copy(void **dst, void **src){ *dst = *src; }

#define ASSERT_TRUE(expr) do { \
    if(!(expr)) { \
        fprintf(stderr, "Assertion failed: %s (%s:%d)\n", #expr, __FILE__, __LINE__); \
        exit(1); \
    } \
} while(0)

static int cb_count = 0;
static void count_cb(void *key){ (void)key; cb_count++; }

static int subtree_height(Node *n){
    if(!n) return 0;
    int lh = subtree_height(n->left);
    int rh = subtree_height(n->right);
    return (lh > rh ? lh : rh) + 1;
}

static int ilog2u(unsigned int x){
    int l = 0;
    while(x > 1){
        x >>= 1;
        l++;
    }
    return l;
}

static void assert_balanced(RBTree_t *t, size_t count){
    Tree *tree = (Tree*)t;
    int h = subtree_height(tree->root);
    int lg = ilog2u((unsigned int)count + 1);
    int max_h = 2 * (lg + 1);
    ASSERT_TRUE(h <= max_h);
    if(tree->root)
        ASSERT_TRUE(GET_COLOR(tree->root) == BLACK);
}

static void test_insert_find(void){
    RBTree_t *t = createRBTree(std_alloc, std_dealloc, int_comp, ptr_copy);
    int values[] = {5,1,9,3,7};
    for(size_t i=0;i<5;i++)
        ASSERT_TRUE(t->insert(t,&values[i]));
    /* inserting duplicate should fail */
    ASSERT_TRUE(!t->insert(t,&values[0]));
    for(size_t i=0;i<5;i++){
        int *found = t->find(t,&values[i]);
        ASSERT_TRUE(found && *found == values[i]);
    }
    deleteRBTree(t);
}

static void test_delete(void){
    RBTree_t *t = createRBTree(std_alloc, std_dealloc, int_comp, ptr_copy);
    int vals[] = {1,2,3,4,5,6};
    for(size_t i=0;i<6;i++)
        ASSERT_TRUE(t->insert(t,&vals[i]));
    ASSERT_TRUE(t->del(t,&vals[2]));
    ASSERT_TRUE(t->find(t,&vals[2]) == NULL);
    ASSERT_TRUE(t->del(t,&vals[0]));
    ASSERT_TRUE(t->find(t,&vals[0]) == NULL);
    for(size_t i=1;i<6;i++){
        if(i==2) continue;
        ASSERT_TRUE(t->find(t,&vals[i]) != NULL);
    }
    deleteRBTree(t);
}

static void test_iterator(void){
    RBTree_t *t = createRBTree(std_alloc, std_dealloc, int_comp, ptr_copy);
    int vals[] = {5,1,9,3,7};
    for(size_t i=0;i<5;i++)
        t->insert(t,&vals[i]);
    int expected[] = {1,3,5,7,9};
    size_t idx = 0;
    RBIter_t *iter = t->getIterator(t,NULL);
    for(; iter != NULL; iter = iter->getNext(&iter)){
        int *curr = (int*)iter->get(iter);
        ASSERT_TRUE(*curr == expected[idx]);
        idx++;
    }
    ASSERT_TRUE(idx == 5);
    deleteRBTree(t);
}

static void test_callback(void){
    cb_count = 0;
    RBTree_t *t = createRBTreeWithCB(std_alloc, std_dealloc, int_comp, ptr_copy, count_cb);
    int vals[] = {1,2,3};
    for(int i=0;i<3;i++)
        t->insert(t,&vals[i]);
    t->del(t,&vals[1]);
    ASSERT_TRUE(cb_count == 1);
    deleteRBTree(t);
    ASSERT_TRUE(cb_count == 3);
}

static void test_balance_ops(void){
    RBTree_t *t = createRBTree(std_alloc, std_dealloc, int_comp, ptr_copy);
    int values[150];
    for(int i=0;i<150;i++) values[i] = i;
    size_t count = 0;

    /* insert first batch */
    for(int i=0;i<100;i++){
        ASSERT_TRUE(t->insert(t,&values[i]));
        count++;
        assert_balanced(t,count);
    }

    /* delete a subset */
    for(int i=0;i<50;i++){
        ASSERT_TRUE(t->del(t,&values[i]));
        count--;
        assert_balanced(t,count);
    }

    /* insert second batch */
    for(int i=100;i<150;i++){
        ASSERT_TRUE(t->insert(t,&values[i]));
        count++;
        assert_balanced(t,count);
    }

    /* remove all remaining */
    for(int i=50;i<150;i++){
        ASSERT_TRUE(t->del(t,&values[i]));
        count--;
        assert_balanced(t,count);
    }

    deleteRBTree(t);
}

int main(void){
    test_insert_find();
    test_delete();
    test_iterator();
    test_callback();
    test_balance_ops();
    printf("All tests passed\n");
    return 0;
}

