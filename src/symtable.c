 /***************************************
* Project - Compiler for IFJ21
* 
* @brief Symtable for IFJ21
* 
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author Samuel Popelář <xpopel22> 
* 
* 
* @file: symtable.c
***************************************/

#include "symtable.h"

void tree_init(nodeptr *root) {
    *root = NULL;
}

nodeptr tree_search(nodeptr root, String *k) {
    if (root == NULL) {
        return NULL;
    }

    if (root->key == k) {
        return root;
    }

    if (string_string_cmp(k, root->key) < 0) { //go left
        return tree_search(root->lptr, k);
    } else if (string_string_cmp(k, root->key) > 0) { //go right
        return tree_search(root->rptr, k);
    }
    return root;    // return found node

}

void tree_insert(nodeptr *root, String *k, node_type type, function_data *data) {
    if (*root == NULL) {
        nodeptr tmp = malloc(sizeof(struct node));
        if (tmp == NULL) { //malloc fail
            return;
        }

        tmp->key = k;
        tmp->type = type;
        tmp->func_data = data;
        tmp->lptr = NULL;
        tmp->rptr = NULL;
        *root = tmp;

    } else {
        if (string_string_cmp(k, (*root)->key) < 0) { //go left
            tree_insert(&(*root)->lptr, k, type, data);
        } else if (string_string_cmp(k, (*root)->key) > 0) { //go right
            tree_insert(&(*root)->rptr, k, type, data);
        } else {
            (*root)->func_data = data;    
        }
    }
}

void ReplaceByRightmost(nodeptr ptr_replaced, nodeptr *root) {

    if (*root == NULL) {        // If *root is NULL just quit
        return;
    }
    
    if ((*root)->rptr == NULL) {        // If there are no rightmost nodes
        ptr_replaced->key = (*root)->key;        // Assign the values to the ptrReplaced
        ptr_replaced->type = (*root)->type;
        ptr_replaced->func_data = (*root)->func_data;
        nodeptr tmp = (*root);    // We need to keep the left node
        *root = (*root)->lptr;
        free(tmp);
    } else {
        ReplaceByRightmost(ptr_replaced, &((*root)->rptr));    // Calling the function again, bc the rightmost node wasn't found
    }
}

void tree_delete(nodeptr *root, String *k) {
    if (*root == NULL) {        // If *root is NULL just quit
        return;
    }

    if (string_string_cmp(k, (*root)->key) < 0) { //go left
        tree_delete(&(*root)->lptr, k);
    } else if (string_string_cmp(k, (*root)->key) > 0) { //go right
        tree_delete(&(*root)->rptr, k);
    } else {
        nodeptr tmp = *root;
        if ((*root)->lptr == NULL && (*root)->rptr == NULL) {
            *root = NULL;
            free(tmp);
            return;
        } else {
            if ((*root)->rptr == NULL) {
                *root = tmp->lptr;
                free(tmp);
            } else if ((*root)->lptr == NULL) {
                *root = tmp->rptr;
                free(tmp);
            } else {
                ReplaceByRightmost(*root, &(*root)->lptr);
            }
        }
    }
}

void tree_dispose(nodeptr *root) {
    if (*root != NULL) {
        tree_dispose(&(*root)->lptr);
        tree_dispose(&(*root)->rptr);
        free((*root)->func_data);
        (*root)->func_data = NULL;
        free(*root);
        *root = NULL;
    }
}

void symtable_init(symtable *sroot) {
    tree_init(&(sroot->root));
}

nodeptr symtable_search(symtable *sroot, String *k) {
    return tree_search(sroot->root, k);
}

void symtable_insert(symtable *sroot, String *k, node_type type, bool decl, bool def, int params, int returns) {
    if (type == func) { // its function
        // TO-DO: pass arrays of num params,  ret params, num params, num return, defined, declared
        
        function_data *tmp_data;
        tmp_data = malloc(sizeof(struct function_data_struct));
        
        if (tmp_data == NULL) {  //malloc fail
            tree_dispose(&(sroot->root));
            //return ERR_INTERNAL;
            return;
        }

        tmp_data->declared = decl;
        tmp_data->defined = def;
        tmp_data->num_params = params;
        tmp_data->num_return = returns;
        //tmp_data->pole_params = NULL; //?
        //tmp_data->pole_returns = NULL; //?
        if (tmp_data->stack_params.top == NULL) {
            stack_init(&tmp_data->stack_params);
        }
        if (tmp_data->stack_returns.top == NULL) {
            stack_init(&tmp_data->stack_returns);
        }

        tree_insert(&(sroot->root), k, type, tmp_data);
        
        
    } else if (type == var) {   // its variable
        tree_insert(&(sroot->root), k, type, NULL);
    } else {
        return; //should return err, bc cant have anythign else than var or func
    }
}

void symtable_delete(symtable *sroot, String *k) {
    tree_delete(&(sroot->root), k);
}

void symtable_dispose(symtable *sroot) {
    tree_dispose(&(sroot->root));
}