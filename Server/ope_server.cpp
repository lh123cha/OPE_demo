//
// Created by 14913 on 2021/4/12.
//

#include "ope_server.h"
#include"BSTree.h"
#include <pthread.h>
#include "..\Util\AES.h"
#include "..\Util\Base64.h"
#include<unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

#define N 100

const char g_key[17] = "asdfwetyhjuytrfd";//加密公钥
const char g_iv[17] = "gfdertfghjkuyrtg";//加密私钥
string EncryptionAES(const string& strSrc) //AES加密
{
    size_t length = strSrc.length();
    int block_num = length / BLOCK_SIZE + 1;
    //明文
    char* szDataIn = new char[block_num * BLOCK_SIZE + 1];
    memset(szDataIn, 0x00, block_num * BLOCK_SIZE + 1);
    strcpy(szDataIn, strSrc.c_str());

    //进行PKCS7Padding填充。
    int k = length % BLOCK_SIZE;
    int j = length / BLOCK_SIZE;
    int padding = BLOCK_SIZE - k;
    for (int i = 0; i < padding; i++)
    {
        szDataIn[j * BLOCK_SIZE + k + i] = padding;
    }
    szDataIn[block_num * BLOCK_SIZE] = '\0';

    //加密后的密文
    char *szDataOut = new char[block_num * BLOCK_SIZE + 1];
    memset(szDataOut, 0, block_num * BLOCK_SIZE + 1);

    //进行进行AES的CBC模式加密
    AES aes;
    aes.MakeKey(g_key, g_iv, 16, 16);
    aes.Encrypt(szDataIn, szDataOut, block_num * BLOCK_SIZE, AES::CBC);
    string str = base64_encode((unsigned char*) szDataOut,
                               block_num * BLOCK_SIZE);
    delete[] szDataIn;
    delete[] szDataOut;
    return str;
}
string DecryptionAES(const string& strSrc) //AES解密
{
    string strData = base64_decode(strSrc);
    size_t length = strData.length();
    //密文
    char *szDataIn = new char[length + 1];
    memcpy(szDataIn, strData.c_str(), length+1);
    //明文
    char *szDataOut = new char[length + 1];
    memcpy(szDataOut, strData.c_str(), length+1);

    //进行AES的CBC模式解密
    AES aes;
    aes.MakeKey(g_key, g_iv, 16, 16);
    aes.Decrypt(szDataIn, szDataOut, length, AES::CBC);

    //去PKCS7Padding填充
    if (0x00 < szDataOut[length - 1] <= 0x16)
    {
        int tmp = szDataOut[length - 1];
        for (int i = length - 1; i >= length - tmp; i--)
        {
            if (szDataOut[i] != tmp)
            {
                memset(szDataOut, 0, length);
                cout << "去填充失败！解密出错！！" << endl;
                break;
            }
            else
                szDataOut[i] = 0;
        }
    }
    string strDest(szDataOut);
    delete[] szDataIn;
    delete[] szDataOut;
    return strDest;
}
inline bool comp(string a1,string a2){
    if(a1>a2){
        return 1;
    }
    else{
        return 0;
    }
}
inline bool to_update(int64_t yk,int64_t ykplus){
    if(ykplus-yk>1){
        return false;
    }
    else{
        return true;
    }
}
void insert_tree(CBinTree* tr1){//初始化二叉搜索树
    tr1->Insert(pair<string,int64_t>(EncryptionAES("a"),5));
    tr1->Insert(pair<string,int64_t>(EncryptionAES("b"),10));
    tr1->Insert(pair<string,int64_t>(EncryptionAES("d"),25));
    tr1->Insert(pair<string,int64_t>(EncryptionAES("e"),60));
    tr1->Insert(pair<string,int64_t>(EncryptionAES("f"),80));
}
/**
 先这样写，之后有时间在将客户端服务器分为两个线程，将insert、serach、widelysearch分别实现为三个函数。
*/
int main ()
{//模拟客户端和服务器间的交互
    CBinTree* pBinTree = new CBinTree();
    //初始化编码书（先加入几个节点防止出错）
    insert_tree(pBinTree);
    string operat="";
    int i=0;
    unordered_map<string, int64_t> map;//在客户端缓存明文，编码哈希表。
    while(operat!="#") {
        string p="";
        string obverse_str="";
        string obverse_str_right="";
        cin >> operat;
        if(operat=="insert") {//模拟插入（交互式的插入）
            pSTreeNode temp=pBinTree->pRoot;
            cin >> obverse_str;
            cout<<"already insert: "<<obverse_str<<endl;
            string chiper = EncryptionAES(obverse_str);//对输入要插入的密文加密。
            while(temp!=NULL) {
                string insert_get_back = temp->key.first;//交互获得树根节点的密文
                string get_obver = DecryptionAES(insert_get_back);
                //map.insert({obverse_str, i++});//哈希表映射记住编码值，假装当作缓存
                if (comp(obverse_str, get_obver)) {//返回的密文解密后小于插入明文
                    cout << "It's bigger than getted_mingwen: "<<get_obver<<endl;
                    temp = temp->pRightChild;
                    p += '1';
                } else {//返回密文解密后大于明文
                    cout << "It's smaller than getted_mingwen: "<<get_obver<<endl;
                    temp = temp->pLeftChild;
                    p += '0';
                }
            }
                //计算插入数据的保序编码（服务器上的操作）


            temp=pBinTree->pRoot;
            vector<int64_t> ordered_code;
            ordered_code.push_back(temp->key.second);//记录路径上节点的保序编码
            for(int i=0;i<p.length()-1;i++){//因为最后一位表示节点temp移动到NULL位置，判断是在排序后的保序编码的左边还是右边。
                if(p[i]=='0'){
                    temp=temp->pLeftChild;
                    ordered_code.push_back(temp->key.second);
                }else{
                    temp=temp->pRightChild;
                    ordered_code.push_back(temp->key.second);
                }
            }
            int64_t the_last_we_find=ordered_code[p.length()-1];//路径中最后一一个节点
            sort(ordered_code.begin(),ordered_code.end());//排序
            vector<int64_t>::iterator it;
            int64_t final_get_ordered_code;//最后的保序编码
            for(it=ordered_code.begin();it!=ordered_code.end();it++){
                if(*it==the_last_we_find){
                    break;//找到排序后我们最后一个的位置
                }
            }


            if(it+1!=ordered_code.end()&&it!=ordered_code.begin()) {
                if (p[p.length()-1] == '1' && !to_update(*it, *(it + 1))) {
                    final_get_ordered_code = (*it + *(it + 1)) / 2;//则排序后最后一个的下一个为y_k_1
                }
                if (p[p.length()-1] == '0' && !to_update(*(it-1), *it)) {
                    final_get_ordered_code = (*it + *(it - 1)) / 2;
                }
            }else if(it+1==ordered_code.end()){
                if (p[p.length()-1] == '1' &&N-*it>1) {
                    final_get_ordered_code = (*it + N) / 2;//则排序后最后一个的下一个为y_k_1
                }
                if (p[p.length()-1] == '0' && !to_update(*(it-1), *it) ){
                    final_get_ordered_code = (*it +*(it-1)) / 2;
                }
            }
            else if(it==ordered_code.begin()){
                if (p[p.length()-1] == '1' && !to_update(*it,*(it+1))) {
                    final_get_ordered_code = (*it + *(it+1)) / 2;//则排序后最后一个的下一个为y_k_1
                }
                if (p[p.length()-1] == '0' && !to_update(*(it-1), *it)) {
                    final_get_ordered_code = (*it -1) / 2;
                }
            }
            pBinTree->Insert(pair<string, int64_t>(chiper, final_get_ordered_code));//插入密文、编码值。
            cout<<"insert ordered_code successfully,which is: "<<final_get_ordered_code<<endl;
            pBinTree->Display(pBinTree->pRoot,0);
        }
        if(operat=="search") {
            cin >> obverse_str;
            pSTreeNode temp = pBinTree->pRoot;
            string chiper1 = EncryptionAES(obverse_str);//查找时发送密文
            while (temp != NULL){
                string get_back_chiper = temp->key.first;//从根节点开始获得密文


                string get_obver = DecryptionAES(get_back_chiper);//返回密文并解密
                cout <<"the getted_mingwen is: "<<get_obver << endl;
                cout << "please confirm whether it's bigger or less?"<<endl;
                if(comp(obverse_str, get_obver)){//判断大小
                    cout << "It's bigger than getted_mingwen: "<<get_obver<<endl;
                    temp = temp->pRightChild;
                }
                else if(obverse_str==get_obver){//相等返回
                    cout<<"already find the wanted text: "<<get_obver<<endl;
                    //map.insert({obverse_str,temp->key.second});//将查找到的明文、编码对存入客户端缓存。
                    break;
                }else{
                    cout << "It's smaller than getted_mingwen: "<<get_obver<<endl;
                    temp = temp->pLeftChild;
                }
            }
        }
        if(operat=="widelysearch"){
            cin>>obverse_str;
            cin>>obverse_str_right;
            string obverse_str_left=obverse_str;
            cout<<"输入的左边界为"<<obverse_str_left<<"您输入的右边界为:"<<obverse_str_right;
            if(obverse_str>obverse_str_right){
                cout<<"请输入正确的查询范围";
                break;
            }

            //交互查询左边界

            pSTreeNode temp = pBinTree->pRoot;
            string chiper1 = EncryptionAES(obverse_str);//查找时发送密文
            while (temp != NULL){
                string get_back_chiper = temp->key.first;//从根节点开始获得密文


                string get_obver = DecryptionAES(get_back_chiper);//返回密文并解密
                cout <<"the getted_mingwen is: "<<get_obver << endl;
                cout << "please confirm whether it's bigger or less?"<<endl;
                if(comp(obverse_str, get_obver)){//判断大小
                    cout << "It's bigger than getted_mingwen: "<<get_obver<<endl;
                    temp = temp->pRightChild;
                }
                else if(obverse_str==get_obver){//相等返回
                    cout<<"already find the wanted text: "<<get_obver<<endl;
                    map.insert({obverse_str_left,temp->key.second});
                    break;
                }else{
                    cout << "It's smaller than getted_mingwen: "<<get_obver<<endl;
                    temp = temp->pLeftChild;
                }
            }
            //交互查询右边界
            temp = pBinTree->pRoot;
            string chiper1_right = EncryptionAES(obverse_str_right);//查找时发送密文
            while (temp != NULL){
                string get_back_chiper = temp->key.first;//从根节点开始获得密文


                string get_obver = DecryptionAES(get_back_chiper);//返回密文并解密
                cout <<"the getted_mingwen is: "<<get_obver << endl;
                cout << "please confirm whether it's bigger or less?"<<endl;
                if(comp(obverse_str_right, get_obver)){//判断大小
                    cout << "It's bigger than getted_mingwen: "<<get_obver<<endl;
                    temp = temp->pRightChild;
                }
                else if(obverse_str_right==get_obver){//相等返回
                    cout<<"already find the wanted text: "<<get_obver<<endl;
                    map.insert({obverse_str_right,temp->key.second});
                    break;
                }else{
                    cout << "It's smaller than getted_mingwen: "<<get_obver<<endl;
                    temp = temp->pLeftChild;
                }
            }
            vector<TreeKeyType> result;
            pBinTree->InorderRecursively(pBinTree->pRoot,result);
            vector<TreeKeyType>::iterator it;
            for(it=result.begin();it!=result.end();it++){
                if(it->second==map[obverse_str_left]){
                    break;
                }
            }
            for (auto iter = map.begin(); iter != map.end(); ++iter) {
                cout << "<" << iter->first << ", " << iter->second << ">" << endl;
            }
            for(it;it->second!=map[obverse_str_right];it++){
                cout<<"要查找的范围中的值为："<<DecryptionAES(it->first)<<endl;
            }
            cout<<"要查找的范围中的值为："<<DecryptionAES(it->first)<<endl;
        }

    }


}

