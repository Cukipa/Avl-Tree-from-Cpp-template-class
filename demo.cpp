#include<iostream>
#include"avltree.h"

using namespace std;

//�ӿں������Ƚϼ�ֵ
int cmpInt(const int& i1,const int& i2){
    if(i1>i2)
        return 1;
    if(i1<i2)
        return -1;
    return 0;
}

int main(){

    AvlTree<int,int> tree;

    tree.insert(10,0,cmpInt);

    tree.insert(11,0,cmpInt);

    if(tree.find(10,cmpInt)!= nullptr)
        cout<<"�ҵ���"<<endl;

    tree.remove(10,cmpInt);

    for(auto i=tree.begin();i.nempty();++i)
        cout<<i.getkey()<<"\t"<<i.getValue()<<endl;

    cout<<"���Ľڵ�������"<<tree.getSize()<<endl;

    return 0;
}