//
// Created by 14913 on 2021/4/12.
//
#include<iostream>
#include"BSTree.h"
#include<unordered_map>
using namespace std;

inline bool to_update(int64_t yk,int64_t ykplus){
    if(ykplus-yk>1){
        return false;
    }
    else{
        return true;
    }
}
int main()
{
    CBinTree* pBinTree = new CBinTree();
    if ( pBinTree == NULL )
        return 0;

    pBinTree->Insert( pair<string,int64_t>("a",10));
    pBinTree->Insert( pair<string,int64_t>("c",11) );
    pBinTree->Insert( pair<string,int64_t>("d",40) );
    pBinTree->Insert( pair<string,int64_t>("e",45) );
    pBinTree->Display(pBinTree->pRoot,0);
    if(to_update(10,11)){
        cout<<"yes"<<endl;
        pBinTree->RebuildtheTree(pBinTree->pRoot);
        pBinTree->Insert( TreeKeyType ("b",21));
    }
    pBinTree->Display(pBinTree->pRoot,0);
    system( "pause" );
    return 1;
}
