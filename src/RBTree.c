    #include "RBTreeImpl.h"
    #include "RBIterator.h"

    typedef struct{
        unsigned dir : 1;
    }Dir;

    static
    Node*
    findNode(
        Node *root,
        void *toFind){

        int res = 0;

        while(root){

            res =
                root->tree->comparator(root->key,toFind);

            if(!res){
                break;
            }

            if(res < 0)
                root = root->right;
            else
                root = root->left;
        }

        return root;
    }

    static
    Node*
    getInorderSucc(Node *curr){

      Node *succ = NULL;

      if(!curr || !(curr->right)){
        return NULL;
      }

      for(succ = curr->right;
          succ->left;
          succ = succ->left);

      return succ;
    }

    static
    void
    rotate(
       Node **rot,
       const Dir dir){

        Node *n = NULL,
             *newParent = NULL,
             *newParentSon = NULL;

        if(!rot || !(*rot)){
          return;
        }

        n = *rot;
        newParent = (RIGHT_ROTATE(dir) ? n->left : n->right);

        // when we  rotate we have to have a child to replace this node
        assert(newParent != NULL);
        newParentSon = (RIGHT_ROTATE(dir) ? newParent->right : newParent->left);

      if(RIGHT_ROTATE(dir)){
        n->left = newParentSon;
        newParent->right = n;
      }
      else {
        n->right = newParentSon;
        newParent->left = n;
      }

      newParent->parent = n->parent;

      if(newParentSon) {
        newParentSon->parent = n;
      }

      if(n->parent) {
        if (n == n->parent->left) {
          n->parent->left = newParent;
        } else {
          n->parent->right = newParent;
        }
      }
      else{
        assert(n == n->tree->root);
        n->tree->root = newParent;
      }

      n->parent = newParent;
      *rot = newParent;
    }

    static
    void
    adjustInsert(
      Node *ins){

      assert(ins != NULL);
      Node *p = NULL,*gp = NULL,*uncle = NULL;
      Node *root = ins->tree->root;
      int uncleDir = 0;
      static Dir leftDir = {.dir = 0}, rightDir = {.dir = 1};

      while(ins != root &&
            GET_COLOR(ins->parent) == RED){

        p = ins->parent;
        gp = p->parent;
        uncle = (gp->left == p ? gp->right : gp->left);
        uncleDir = (gp->left == p);

        if(GET_COLOR(p) == BLACK){
          break;
        }

        //
        // a NULL uncle is a black uncle
        //
        if(uncle &&
           GET_COLOR(p) == RED &&
           GET_COLOR(uncle) == RED){

          COLOR_BLACK(p);
          COLOR_BLACK(uncle);
          COLOR_RED(gp);

          //
          //the gradfather could be violating RB props
          //
          ins = gp;
        }
        else{

          //
          // if uncle is left son of gp and ins is left son of p
          // or uncle is right son of gp and ins is right son of p
          // need to do a single rotation on p
          //

          if(ins == p->left && !uncleDir){
            rotate(&p,rightDir);
            assert(p == ins);
          }
          else if(ins == p->right && uncleDir){
            rotate(&p,leftDir);
            assert(p == ins);
          }

          //
          // uncle and ins are of opposite direction
          //
          SWAP_COLORS(p);
          SWAP_COLORS(gp);

          if(p == gp->left){
            rotate(&gp,rightDir);
          }
          else{
            rotate(&gp,leftDir);
          }

          break;
        }
      }

      COLOR_BLACK(ins->tree->root);
    }

    static
    Node*
    getInsPoint(
        Node *root,
        void *key){

      assert(root != NULL);
      Node *curr = root;
      Comparator comp = curr->tree->comparator;
      int res = 0;

      while(curr){

        res = comp(curr->key,key);

        if(res == 0){
          break;
        }

        if(res < 0){
          if(curr->right)
            curr = curr->right;
          else
            break;
        }
        else{
          if(curr->left)
            curr = curr->left;
          else
            break;
        }
      }

      return curr;
    }

    static
    void
    adjustDeleteByDir(Node **nodeToFix, Dir dir){

      static Dir leftDir = {.dir = 0}, rightDir = {.dir = 1};
      Tree *t = (*nodeToFix)->tree;
      Node *curr = *nodeToFix,*sibling = NULL, *p = NULL;
      sibling = dir.dir ? RIGHT_OF(curr->parent) : LEFT_OF(curr->parent);

      //sibling can't be NULL - if sibling is NULL that means that the tree was
      // violating the RB properties (since nodeToFix has a black count of 2)
      assert(sibling != NULL);

      if (GET_COLOR(sibling) == RED) {
        COLOR_BLACK(sibling);
        COLOR_RED(curr->parent);
        p = curr->parent;
        rotate(&p, (dir.dir ? leftDir : rightDir));
        sibling = dir.dir ? curr->parent->right : curr->parent->left;
      }

      if ((sibling->left == NULL ||
           GET_COLOR(sibling->left) == BLACK) &&
          (sibling->right == NULL ||
           GET_COLOR(sibling->right) == BLACK)) {

        COLOR_RED(sibling);
        *nodeToFix = (curr->parent);
      }
      else {

        if((dir.dir && (
           sibling->right == NULL ||
           GET_COLOR(sibling->right) == BLACK)) ||
           (!dir.dir && (sibling->left == NULL || GET_COLOR(sibling->left) == BLACK))){

          if(dir.dir){
            COLOR_BLACK(sibling->left);
            COLOR_RED(sibling);
            rotate(&sibling,rightDir);
            sibling= curr->parent->right;
          }
          else{
            COLOR_BLACK(sibling->right);
            COLOR_RED(sibling);
            rotate(&sibling,leftDir);
            sibling = curr->parent->left;
          }
        }

        sibling->color = curr->parent->color;
        COLOR_BLACK(curr->parent);

        if(dir.dir){
          COLOR_BLACK(sibling->right);
        }
        else{
          COLOR_BLACK(sibling->left);
        }

        p = curr->parent;
        rotate(&p,dir.dir ? leftDir : rightDir);
        *nodeToFix = curr->tree->root;
      }
    }

    static
    void
    adjustDelete(Node *nodeToFix){

        Tree *t = nodeToFix->tree;
        Node *sibling = NULL, *p = NULL;
        static Dir leftDir = {.dir = 0}, rightDir = {.dir = 1};

        // we only deal with nodeToFix which is BLACK
        // and is not the root - this is called double black node
        while(nodeToFix != t->root &&
                GET_COLOR(nodeToFix) == BLACK) {

            if (nodeToFix == LEFT_OF(nodeToFix->parent)) {
              adjustDeleteByDir(&nodeToFix,rightDir);
            }
            else{
              adjustDeleteByDir(&nodeToFix,leftDir);
            }
        }

        // if nodeToFix is either the root or red
        COLOR_BLACK(nodeToFix);
    }

    static
    BOOL
    del(RBTree_t *tree,
           void *toDelete){

        Tree *treeImpl = TO_TREE(tree);
        Node *deleteLoc = NULL;
        Node *root = treeImpl->root;
        Node *inOrder = NULL;
        Node *inOrderChild = NULL;
        Node sentinel;

        if(!toDelete || !tree){
            return FALSE;
        }

        //
        // The sentinel will be used in the case of a black leaf node
        // that is being deleted
        //

        memset(&sentinel,0,sizeof(Node));
        sentinel.color.rb = BLACK;
        sentinel.tree = treeImpl;
        sentinel.left = &sentinel;
        sentinel.right = &sentinel;

        if(!(deleteLoc =
            findNode(root,toDelete))){
            return FALSE;
        }

        if(!deleteLoc->left || !deleteLoc->right){
            inOrder = deleteLoc;
        }
        else{
            inOrder = deleteLoc->right;

            while (inOrder->left){
                inOrder = inOrder->left;
            }
        }

        inOrderChild = inOrder->left ? inOrder->left :
          (inOrder->right ? inOrder->right : &sentinel);

           inOrderChild->parent = inOrder->parent;

            //
            // we want to update the parent of the
            // only child of inOrder (the node we are deleting)
            //

            if(inOrder->parent){

                if(inOrder->parent->left == inOrder){
                    inOrder->parent->left = inOrderChild;
                }
                else{
                    inOrder->parent->right = inOrderChild;
                }
            }
            else{

                //
                // inOrder does not have a parent -> it is the root
                //
                assert(inOrder == root);
                treeImpl->root = (inOrderChild == &sentinel ? NULL : inOrderChild);

                if(!treeImpl->root){
                    return TRUE;
                }
            }

        //
        // Update the keys such that the original node
        // we wanted to delete has the key of the inOrder node which we'll be deleting
        //

        if(deleteLoc!= inOrder) {
            treeImpl->copy(&(deleteLoc->key),&(inOrder->key));
        }

        if(GET_COLOR(inOrder) == BLACK){

            //
            // inOrderChild has to exist given that inOrder is not the root
            // and that it is black
            //
            assert(inOrderChild);
            adjustDelete(inOrderChild);
        }

        //
        // make sure to disconnect the sentinel node from the tree
        //

        if(sentinel.parent){

          if((sentinel.parent)->left == &sentinel){
              (sentinel.parent)->left = NULL;
          }
          else if((sentinel.parent)->right == &sentinel){
              (sentinel.parent)->right = NULL;
          }

          sentinel.parent = NULL;
        }

        if(treeImpl->deleteCB){
            treeImpl->deleteCB(inOrder->key);
        }

        treeImpl->dealloc(inOrder);
        return TRUE;
    }

    static
    BOOL
    insert(
        RBTree_t *tree,
        void *toInsert){

        Tree *treeImpl = TO_TREE(tree);
        Node *toIns = NULL;
        Node *insPoint = NULL;
        Node **root = NULL;
        Comparator comp = NULL;
        int res = 0;

        if(treeImpl == NULL){
          return FALSE;
        }

        root = &(treeImpl->root);
        comp = treeImpl->comparator;
        ALLOC(Node,toIns,treeImpl->alloc,sizeof(Node),FALSE);
        toIns->key = toInsert;
        toIns->tree = treeImpl;
        COLOR_RED(toIns);

        if(!(*root)){
          *root = toIns;
          COLOR_BLACK(*root);
        }
        else{

          insPoint = getInsPoint(*root,toInsert);
          assert(insPoint != NULL);
          res = comp(insPoint->key,toInsert);

          if(res == 0){
            treeImpl->dealloc(toIns);
            return FALSE;
          }

          if(res < 0){
            insPoint->right = toIns;
          }
          else{
            insPoint->left = toIns;
          }

          toIns->parent = insPoint;

          if(GET_COLOR(insPoint) == RED){
            adjustInsert(toIns);
          }
        }

        return (TRUE);
    }

    #ifdef _DEBUG_RBTREE_
    static
    void
    showRecursively(RBTree_t *t, Node *n){

      if(!n){
        return;
      }

      if(n->left)
        printf("<---\n");

      showRecursively(t,n->left);
      printf("node color is: %s\n",(n->color.rb ? "BLACK" : "RED"));
      TO_TREE(t)->show(n->key);

      if(n->right)
        printf("--->\n");

      showRecursively(t,n->right);
    }

    static
    BOOL
    showTree(RBTree_t *t){

      if(!t || TO_TREE(t)->show == NULL){
        return FALSE;
      }

      Node *n = TO_TREE(t)->root;

      printf("root is: \n");
      TO_TREE(t)->show(n->key);
      printf("printing the rest \n");

      showRecursively(t,n);
      return TRUE;
    }
    #endif

    static
    void *
    find(RBTree_t * t,
         void *toFind){

      if(!t){
        return NULL;
      }

      Node *n = findNode(TO_TREE(t)->root,toFind);

      if(!n){
        return NULL;
      }

      return n->key;
    }

    static
    RBIter_t*
    getIterator(
            RBTree_t *tree,
            RBIter_t* curr){

        Node *root = NULL;

        if(!tree || ((root = TO_TREE(tree)->root) == NULL)) {
            return NULL;
        }

        Node *toStartFrom = root;

        if(curr) {
            toStartFrom =
                    getNodeFromIter(curr);
        }

        return getIteratorFromNode(
                toStartFrom,
                TO_TREE(tree)->alloc,
                TO_TREE(tree)->dealloc);
    }

    static
    void
    deleteNodes(Node **root){

      //TODO - do it iteratively
      if(!(*root)){
        return;
      }

      deleteNodes(&((*root)->left));
      deleteNodes(&((*root)->right));

      if((*root)->tree->deleteCB){
          (*root)->tree->deleteCB((*root)->key);
      }

      (*root)->tree->dealloc(*root);
      *root = NULL;
    }

    RBTree_t *
    createRBTree(
        const Allocator alloc,
        const Deallocator dealloc,
        const Comparator comparator,
        const Copy copy
    #ifdef _DEBUG_RBTREE_
        ,const shower show
    #endif
                 ){

      Tree *tree = NULL;
      ALLOC(Tree,tree,alloc,sizeof(Tree),NULL);

      tree->alloc = alloc;
      tree->dealloc = dealloc;
      tree->comparator = comparator;
      tree->copy = copy;
      tree->api.insert = &insert;
      tree->api.del = &del;
      tree->api.find = &find;
      tree->api.getIterator = &getIterator;

    #ifdef _DEBUG_RBTREE_
      tree->show = show;
      tree->api.showTree = showTree;
    #endif

      return &(tree->api);
    }

    RBTree_t *
    createRBTreeWithCB(
            const Allocator alloc,
            const Deallocator dealloc,
            const Comparator comparator,
            const Copy copy,
#ifdef _DEBUG_RBTREE_
            const shower show,
#endif
        const OnDeleteCB delCB
    ){
        Tree *tree =
                (Tree*)createRBTree(
                        alloc,
                        dealloc,
                        comparator,
                        copy
#ifdef _DEBUG_RBTREE_
        ,show
#endif
                );

        if(!tree){
            return NULL;
        }

        tree->deleteCB = delCB;
        return &(tree->api);
    }

    BOOL
    deleteRBTree(RBTree_t *tree){

      if(!tree){
        return TRUE;
      }

      deleteNodes(&((TO_TREE(tree)->root)));
      TO_TREE(tree)->dealloc(tree);
      return TRUE;
    }