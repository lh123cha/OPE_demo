//
// Created by 14913 on 2021/4/12.
//

#ifndef OPE_DEMO_BSTREE_H
#define OPE_DEMO_BSTREE_H

#endif //OPE_DEMO_BSTREE_H
#include <iostream>
#include <stack>
#include<unordered_map>
#include <queue>
#include<bits/stdc++.h>

using namespace std;

typedef struct STreeNode* pSTreeNode;
typedef pair<string,int64_t> TreeKeyType;

struct STreeNode
{
    TreeKeyType key;
    pSTreeNode pLeftChild;
    pSTreeNode pRightChild;

    STreeNode( TreeKeyType Value )
    {
        key = Value;
        pLeftChild = NULL;
        pRightChild = NULL;
    }
};

int vec_left[200] = {0};

class CBinTree
{
public:
    CBinTree();
    ~CBinTree();

    void Insert( TreeKeyType Value );
    void Insert( pSTreeNode pNode, TreeKeyType Value );

    pSTreeNode Search( int64_t Value );
    pSTreeNode Search( pSTreeNode pNode, int64_t Value );

    void Delete( TreeKeyType Value );


    void PreorderRecursively( pSTreeNode pNode );   //  前序遍历，递归调用
    void InorderRecursively( pSTreeNode pNode ,vector<TreeKeyType>& t);    //  中序遍历，递归调用
    void PostorderRecursively( pSTreeNode pNode );  //  后序遍历，递归调用
    void output_impl(pSTreeNode n, bool left, string const& indent);
    void output(pSTreeNode root);
    void Display(pSTreeNode root, int ident);
    void RebuildtheTree(pSTreeNode root);

    void Print_tree(pSTreeNode pNode);
    pSTreeNode GetMaxKey();     //  获得二叉查找树中元素值最大的节点
    pSTreeNode GetMinKey();     //  获得二叉查找树中元素值最小的节点

    void FreeMemory( pSTreeNode pNode );    //  释放内存
public:
    pSTreeNode pRoot;
};

CBinTree::CBinTree()
{
    pRoot = NULL;
}

CBinTree::~CBinTree()
{
    if ( pRoot == NULL )
        return;

    FreeMemory( pRoot );
}

void CBinTree::FreeMemory( pSTreeNode pNode )
{
    if ( pNode == NULL )
        return;

    if ( pNode->pLeftChild != NULL )
        FreeMemory( pNode->pLeftChild );

    if ( pNode->pRightChild != NULL )
        FreeMemory( pNode->pRightChild );

    delete pNode;
    pNode = NULL;
}

void CBinTree::Insert( TreeKeyType Value )
{
    if ( pRoot == NULL )
        pRoot = new STreeNode( Value );
    else
        Insert( pRoot, Value );
}

void CBinTree::Insert( pSTreeNode pNode, TreeKeyType Value )
{
    if ( pNode->key.second > Value.second )
    {
        if ( pNode->pLeftChild == NULL)
            pNode->pLeftChild = new STreeNode( Value );
        else
            Insert( pNode->pLeftChild, Value );
    }
    else
    {
        if ( pNode->pRightChild == NULL)
            pNode->pRightChild = new STreeNode( Value );
        else
            Insert( pNode->pRightChild, Value );
    }
}

pSTreeNode CBinTree::Search( int64_t Value )
{
    return Search( pRoot, Value );
}

pSTreeNode CBinTree::Search( pSTreeNode pNode, int64_t Value )
{
    if ( pNode == NULL )
        return NULL;

    if ( pNode->key.second == Value )
        return pNode;
    else
    {
        if ( pNode->key.second > Value )
            return Search( pNode->pLeftChild, Value );
        else
            return Search( pNode->pRightChild, Value );
    }
}

void CBinTree::Delete( TreeKeyType Value )
{
    pSTreeNode pParentNode = pRoot;
    pSTreeNode pFindNode = pRoot;
    //  找到Value元素对应的节点
    while ( pFindNode != NULL )
    {
        if ( pFindNode->key == Value )
            break;

        pParentNode = pFindNode;
        if ( pFindNode->key.second > Value.second )
            pFindNode = pFindNode->pLeftChild;
        else
            pFindNode = pFindNode->pRightChild;
    }

    if ( pFindNode == NULL )
        return;


    //  处理Value元素的父节点和Value元素的节点
    if ( pFindNode->pLeftChild == NULL || pFindNode->pRightChild == NULL )
    {
        //  一个子结点为空或者两个子结点都为空
        pSTreeNode pTemp = NULL;
        if ( pFindNode->pLeftChild != NULL)
            pTemp = pFindNode->pLeftChild;
        else if ( pFindNode->pRightChild != NULL )
            pTemp = pFindNode->pRightChild;

        if ( pParentNode->pLeftChild == pFindNode )
            pParentNode->pLeftChild = pTemp;
        else
            pParentNode->pRightChild = pTemp;

        delete pFindNode;
        pFindNode = NULL;
    }
    else
    {
        //  找到前驱节点
        pSTreeNode pTemp = pFindNode->pLeftChild;
        pSTreeNode pTempParent = pFindNode;

        while ( pTemp->pRightChild != NULL )
        {
            pTempParent = pTemp;
            pTemp = pTemp->pRightChild;
        }

        pFindNode->key = pTemp->key;
        pTempParent->pRightChild = NULL;
        delete pTemp;
        pTemp = NULL;
    }
}


void CBinTree::PreorderRecursively( pSTreeNode pNode )
{
    if (pNode == NULL)
        return;

    cout << " " << pNode->key.second << " ";
    PreorderRecursively( pNode->pLeftChild );
    PreorderRecursively( pNode->pRightChild );
}

void CBinTree::InorderRecursively( pSTreeNode pNode ,vector<TreeKeyType>& t)
{
    if (pNode == NULL)
        return;

    InorderRecursively( pNode->pLeftChild ,t);
    t.push_back(pNode->key);
    InorderRecursively( pNode->pRightChild ,t);
}

void CBinTree::PostorderRecursively( pSTreeNode pNode )
{
    if (pNode == NULL)
        return;

    PostorderRecursively( pNode->pLeftChild );
    PostorderRecursively( pNode->pRightChild );
    cout << " " << pNode->key.second<< " ";
}

pSTreeNode CBinTree::GetMaxKey() {
    pSTreeNode temp=pRoot;
    while(temp->pRightChild!=NULL){
        temp=temp->pRightChild;
    }
    return temp;
}
pSTreeNode CBinTree::GetMinKey() {
    pSTreeNode temp=pRoot;
    while(temp->pLeftChild!=NULL){
        temp=temp->pLeftChild;
    }
    return temp;
}

void CBinTree::output_impl(pSTreeNode n, bool left, const string &indent) {
    if (n->pRightChild)
    {
        output_impl(n->pRightChild, false, indent + (left ? "|     " : "      "));
    }
    cout << indent;
    cout << (left ? '\\' : '/');
    cout << "-----";
    cout <<"<"<<n->key.first<<","<<n->key.second<<">"<<endl;
    if (n->pLeftChild)
    {
        output_impl(n->pLeftChild, true, indent + (left ? "      " : "|     "));
    }
}



// 显示二叉树的函数，只要调用Display(root, 0)即可
void CBinTree::Display(pSTreeNode root, int ident)
{
    if(ident > 0)
    {
        for(int i = 0; i < ident - 1; ++i)
        {
            printf(vec_left[i] ? "│   " : "    ");
        }
        printf(vec_left[ident-1] ? "├── " : "└── ");
    }

    if(! root)
    {
        printf("(null)\n");
        return;
    }

    cout<<"("<<root->key.first<<","<<root->key.second<<")"<<endl;
    if(!root->pLeftChild && !root->pRightChild)
    {
        return;
    }

    vec_left[ident] = 1;
    Display(root->pLeftChild, ident + 1);
    vec_left[ident] = 0;
    Display(root->pRightChild, ident + 1);
}

void CBinTree::RebuildtheTree(pSTreeNode root) {
    vector<TreeKeyType> a;
    InorderRecursively(root,a);
    vector<TreeKeyType>::iterator it;
    pSTreeNode temp=NULL;
    for(TreeKeyType k:a){
        Delete(k);
    }
    for(it=a.begin(); it!=a.end(); it++){
        it->second=it->second*2;
    }
    if(pRoot){
        pRoot=NULL;
    }
    for(TreeKeyType k:a){
        Insert(k);
    }
}