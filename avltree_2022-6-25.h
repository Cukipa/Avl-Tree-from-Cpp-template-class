//作者：曲奇饼
//作者QQ：2718088693
//作者B站昵称：暴力法师
//说明：
//此库是一个经过大量优化的AVL树的实现，你不需要改动任何地方
//如果对该库的性能有疑问，建议对各项操作进行时间测试
//
//

#ifndef LITTLESEARCHENGINE_AVLTREE_H
#define LITTLESEARCHENGINE_AVLTREE_H

//AVL树，TKey是键，TValue是值域
template<class TKey,class TValue>
class AvlTree{
    //将迭代器类定义为友元类
    friend class Iterator_Base;
    friend class Iterator;
    friend class Reverse_Interator;
private:

    //二叉树的节点类
    class AvlTreeNode{
    public:
        TValue value;//值域
        TKey key;//键值
        unsigned char bf;//平衡因子
        AvlTreeNode* parent;//父节点
        AvlTreeNode* left,*right;//左右节点的指针
        AvlTreeNode(const TKey& k,const TValue& val,AvlTreeNode* p):key(k),value(val){
            bf=0;
            parent=p;
            left=right=nullptr;
        }
    };

    //最大可能产生的堆栈层数
    static const unsigned char Max_AvlTree_High=55;

    //节点数
    unsigned long long count;

    //树的头指针
    AvlTreeNode* head;

    //树高
    unsigned char heigh;

    //记录节点链接的堆栈
    //注：所有的“节点链接”均指代的是指向AVL树节点“AvlTreeNode”类的指针的指针，即双重指针。
    AvlTreeNode*** pLstack=new AvlTreeNode**[Max_AvlTree_High];

    //记录节点地址的堆栈
    AvlTreeNode** pstack=new AvlTreeNode*[Max_AvlTree_High];

    //记录连接指向方向的堆栈
    unsigned char *direction=new unsigned char[Max_AvlTree_High];

    //左旋函数，详细说明见定义
    static unsigned char left_rotate(AvlTreeNode** node);

    //右旋函数，详细说明见定义
    static unsigned char right_rotate(AvlTreeNode** node);

    //调整节点平衡因子值的函数，详细说明见定义
    static void balance_factor(AvlTreeNode* node, unsigned char rt, unsigned char* ub);

    //静态迭代器基类
    class Iterator_Base{
    public:
        explicit Iterator_Base(AvlTree<TKey,TValue>* tree):current(tree->head){
            pstack=new AvlTreeNode*[Max_AvlTree_High];//为迭代器分配堆栈，存放其上层节点
            stack_len=iter_type=0;//迭代器的堆栈长度和迭代状态
        }

        ~Iterator_Base(){
            delete[] pstack;
        }

        //检查是否迭代到末尾，迭代器有效返回true，否则返回false
        bool nempty()const{return current!=nullptr;}
        //获取迭代器指向的Key
        const TKey& getkey()const{return current->key;}
        //获取迭代器指向的Value
        TValue& getValue(){return current->value;}
    protected:
        //当前迭代器指向的树节点
        AvlTreeNode* current;
        //存放上层节点的链接的堆栈
        AvlTreeNode** pstack;
        //堆栈长度
        unsigned char stack_len;
        //迭代状态
        unsigned char iter_type;
    };
public:

    //正向静态迭代器
    // 静态：特指不能在使用迭代器过程中insert和remove，否则会引发错误
    class Iterator:public Iterator_Base{
    public:
        explicit Iterator(AvlTree<TKey,TValue>* tree): Iterator_Base(tree){}
        Iterator & operator++();
    };

    //反向静态迭代器
    // 静态：特指不能在使用迭代器过程中insert和remove，否则会引发错误
    class Reverse_Interator:public Iterator_Base{
    public:
        explicit Reverse_Interator(AvlTree<TKey,TValue>* tree): Iterator_Base(tree){}
        Reverse_Interator& operator++();
    };

    AvlTree();
    ~AvlTree();

    //添加一个新的节点
    TValue* insert(const TKey& k,const TValue& val,int(*cmp)(const TKey&,const TKey&));
    //查找某个Key的值域
    TValue* find(const TKey& k,int(*cmp)(const TKey&,const TKey&))const;
    //删除指定Key的某个节点
    bool remove(const TKey& k,int(*cmp)(const TKey&,const TKey&));
    //获取节点数量
    unsigned long long getSize()const;
    //获取树高
    unsigned char getHeigh()const;
    //删除全部节点
    void clear();
    //获取正向静态迭代器，不能在具有静态迭代器的代码域内包含insert或remove语句，否则将有错误！
    Iterator begin();
    //获取反向静态迭代器，不能在具有静态迭代器的代码域内包含insert或remove语句，否则将有错误！
    Reverse_Interator rbegin();
};


template<class TKey, class TValue>
typename AvlTree<TKey,TValue>::Reverse_Interator & AvlTree<TKey, TValue>::Reverse_Interator::operator++(){

    if(Iterator_Base::current==nullptr)
        return *this;

    //状态1，更新当前指针
    if(Iterator_Base::iter_type==1){
        //迭代左节点
        if(Iterator_Base::current->left!=nullptr){
            Iterator_Base::current=Iterator_Base::current->left;
            Iterator_Base::iter_type=0;
        }
        else{//判断是否迭代完成
            if(Iterator_Base::stack_len==0){
                Iterator_Base::current=nullptr;
            }
            else{//更新到状态2，将指针移到左侧节点
                Iterator_Base::current=Iterator_Base::pstack[--Iterator_Base::stack_len];
                Iterator_Base::iter_type=2;
            }
            return *this;
        }
    }//状态2，节点出栈
    else if(Iterator_Base::iter_type==2){
        if(Iterator_Base::current->left==nullptr && Iterator_Base::stack_len!=0){
            Iterator_Base::current=Iterator_Base::pstack[--Iterator_Base::stack_len];
        }
        else{
            Iterator_Base::current=Iterator_Base::current->left;
            if(Iterator_Base::current!=nullptr)
                Iterator_Base::iter_type=0;
        }
    }

    //状态0，找到最右端节点
    if(Iterator_Base::iter_type==0){
        //将左链接不为空的结点放入堆栈，然后往下钻
        while (Iterator_Base::current->right) {
            Iterator_Base::pstack[Iterator_Base::stack_len++] = Iterator_Base::current;
            Iterator_Base::current = Iterator_Base::current->right;
        }
        Iterator_Base::iter_type=1;
    }
    return *this;
}

template<class TKey, class TValue>
typename AvlTree<TKey,TValue>::Iterator & AvlTree<TKey, TValue>::Iterator::operator++() {
    if(Iterator_Base::current==nullptr)
        return *this;

    //状态1，更新当前指针
    if(Iterator_Base::iter_type==1){
        //迭代右节点
        if(Iterator_Base::current->right!=nullptr){
            Iterator_Base::current=Iterator_Base::current->right;
            Iterator_Base::iter_type=0;
        }
        else{//判断是否迭代完成
            if(Iterator_Base::stack_len==0){
                Iterator_Base::current=nullptr;
            }
            else{//更新到状态2，将指针移到右侧节点
                Iterator_Base::current=Iterator_Base::pstack[--Iterator_Base::stack_len];
                Iterator_Base::iter_type=2;
            }
            return *this;
        }
    }//状态2，节点出栈
    else if(Iterator_Base::iter_type==2){
        if(Iterator_Base::current->right==nullptr && Iterator_Base::stack_len!=0){
            Iterator_Base::current=Iterator_Base::pstack[--Iterator_Base::stack_len];
        }
        else{
            Iterator_Base::current=Iterator_Base::current->right;
            if(Iterator_Base::current!=nullptr)
                Iterator_Base::iter_type=0;
        }
    }

    //状态0，找到最左端节点
    if(Iterator_Base::iter_type==0){
        //将左链接不为空的结点放入堆栈，然后往下钻
        while (Iterator_Base::current->left) {
            Iterator_Base::pstack[Iterator_Base::stack_len++] = Iterator_Base::current;
            Iterator_Base::current = Iterator_Base::current->left;
        }
        Iterator_Base::iter_type=1;
    }
    return *this;
}


//获取树的高度
template<class TKey, class TValue>
unsigned char AvlTree<TKey, TValue>::getHeigh() const {
    return heigh;
}

//节点的左旋函数
//node:待旋转的节点
//rt:记录旋转情况的标记位
template<class TKey,class TValue>
unsigned char AvlTree<TKey,TValue>::left_rotate(AvlTreeNode **node) {
    AvlTreeNode* son,* pson;
    son = (*node)->right;
    pson = son->left;
    if (son->bf^1) {//只需进行纯粹的左旋

        son->parent=(*node)->parent;
        (*node)->parent=son;
        if(pson)
            son->left->parent=*node;

        (*node)->right = son->left;
        son->left = *node;
        *node = son;
        return 1;
    }
    else {//需要先对其孙子节点进行右旋，然后对根节点做左旋

        pson->parent=(*node)->parent;
        (*node)->parent=son->parent=pson;
        if(pson->left)
            pson->left->parent=*node;
        if(pson->right)
            pson->right->parent=son;


        son->left = pson->right;
        pson->right = son;
        (*node)->right = pson->left;
        pson->left = *node;
        *node = pson;
        return 0;
    }
}

//节点的右旋函数
//node:待旋转的节点
//rt:记录旋转情况的标记位
template<class TKey,class TValue>
unsigned char AvlTree<TKey,TValue>::right_rotate(AvlTreeNode **node) {
    AvlTreeNode* son,* pson;
    son = (*node)->left;
    pson=son->right;
    if (son->bf^2) {//只需要进行纯粹的右旋

        son->parent=(*node)->parent;
        (*node)->parent=son;
        if(pson)
            son->right->parent=*node;

        (*node)->left = son->right;
        son->right = *node;
        *node = son;
        return 2;
    }
    else {//需要先对其孙子节点进行左旋，然后对根节点做右旋

        pson->parent=(*node)->parent;
        (*node)->parent=son->parent=pson;
        if(pson->right)
            pson->right->parent=*node;
        if(pson->left)
            pson->left->parent=son;

        son->right = pson->left;
        pson->left = son;
        (*node)->left = pson->right;
        pson->right = *node;
        *node = pson;
        return 0;
    }
}

//调整经过旋转之后的当前旋转位置”根节点“及其后继节点的平衡因子
//node:经过旋转之后的”根“节点，此处的”根“是相对的
//rt:魔法数字，记录上一次旋转操作之后的类型。
//ub:记录本次操作是否可以使得原本失衡的树重新达到平衡，视不同的操作会有不同的值
template<class TKey,class TValue>
void AvlTree<TKey,TValue>::balance_factor(AvlTreeNode *node, unsigned char rt, unsigned char *ub) {
    AvlTreeNode* left_node,* right_node;

    //根节点的左子节点
    left_node = node->left;
    //根节点的右子节点
    right_node = node->right;

    //以下是最简洁也是最复杂的部分，经过了大量优化
    //获取状态值
    unsigned char rotate_type=((rt!=0)|(node->bf<<1));

    //重置根节点平衡因子
    if(node->bf!=0)
        node->bf=0;

    //根据状态值判断如何调整其余节点的平衡因子
    //此处较为复杂，不在过多说明
    switch(rotate_type){
        case 0:
            left_node->bf = right_node->bf = 0;
            break;
        case 1:
            node->bf = rt;
            *ub = 0;
            break;
        case 2:
            left_node->bf = 0;
            right_node->bf = 2;
            break;
        case 3:
            right_node->bf = 0;
            break;
        case 4:
            right_node->bf = 0;
            left_node->bf = 1;
            break;
        default:
            left_node->bf = 0;
    }
}

//构造函数
template<class TKey,class TValue>
AvlTree<TKey,TValue>::AvlTree() {
    head=nullptr;//头指针指向空
    count=0;//树的节点个数为0
    heigh=0;//树高为0
}

//向树中插入一个新的键值及其值域，无论插入是否成功，均返回新节点/已有节点的值域指针
//k:待插入的键值
//val:待插入的值域
//cmp:键值的比较函数接口
template<class TKey,class TValue>
TValue* AvlTree<TKey,TValue>::insert(const TKey &k,const TValue& val,int(*cmp)(const TKey&,const TKey&)) {

    //指向当前节点的链接
    AvlTreeNode** cp_head = &head;

    //新节点的值域指针
    TValue * tag;

    //栈的高度
    unsigned char stack_len = 0;

    //记录详细的旋转状态
    unsigned char rt;

    //键值的比较结果
    int cmp_result;

    //记录下栈的最大长度
    unsigned char new_heigh;

    //向下找到叶子节点
    while (*cp_head) {
        //根据比较函数的接口获取比较结果
        cmp_result = cmp(k,(*cp_head)->key);

        //若比较结果是0，说明存在相同键值的节点，取消添加
        if (cmp_result == 0)
            return &(*cp_head)->value;
        //将指向当前节点的链接存入堆栈
        pLstack[stack_len] = cp_head;

        //记录下钻的方向，并更新当前链接
        if (cmp_result > 0) {
            direction[stack_len] = 1;
            cp_head = &(*cp_head)->right;
        }
        else {
            direction[stack_len] = 0;
            cp_head = &(*cp_head)->left;
        }
        ++stack_len;
    }

    //记录下最大的栈高
    new_heigh=stack_len+1;

    //向叶子节点的链接处添加新节点
    *cp_head = new AvlTreeNode(k,val,(stack_len)?(*pLstack[stack_len-1]): nullptr);

    //记录新节点的值域
    tag=&(*cp_head)->value;

    //现有节点数量加1
    ++count;

    //向根节点回溯
    while (stack_len > 0) {
        //去除链接栈的顶部链接
        cp_head = pLstack[--stack_len];

        //计算出旋转后的状态组合
        rt=direction[stack_len]|((*cp_head)->bf<<1);

        switch(rt){
            case 0://0或1表示回溯节点的平衡因子为0，只需根据其下钻方向改变回溯节点的平衡度即可
            case 1:
                (*cp_head)->bf=direction[stack_len]+1;
                break;
            case 2://2表示回溯节点过度左倾，需要进行右旋
                rt=right_rotate(cp_head);
                goto Tk_Ba;//旋转完成后跳转到平衡函数入口，重新计算当前节点的平衡因子
            case 3://3或4表示回溯节点的双边分支高度一至，达到平衡
            case 4:
                (*cp_head)->bf=0;
                return tag;
            default://默认情况，与2相反
                rt=left_rotate(cp_head);
            Tk_Ba://计算当前节点及其孙子节点的平衡因子
                balance_factor(*cp_head,rt, nullptr);
                return tag;
        }
    }

    //若能到达此处，并且最大堆栈长度大于当前树高，那么树的高度必定会增加1
    //更新树高
    if(new_heigh>heigh)
        heigh=new_heigh;

    //返回新节点的值域指针
    return tag;
}

//从树中查找一个指定键值的节点的值域，查找成功返回值域的指针，否则返回空指针
//k:带查找的键值
//cmp:键值的比较函数接口
template<class TKey,class TValue>
TValue *AvlTree<TKey,TValue>::find(const TKey &k,int(*cmp)(const TKey&,const TKey&)) const {
    //将当前节点定向到树的头节点
    AvlTreeNode* node=head;
    int cmp_result;//保存比较结果
    //若节点不为空，则可以继续查找
    while(node){
        //根据比较接口获取比较结果
        cmp_result=cmp(k,node->key);

        //若结果为0，说明找到了节点，返回节点的值域指针
        if(cmp_result==0)
            return &node->value;
        //若比较结果大于0，说明待查找的值必定在右支
        if(cmp_result>0)
            node=node->right;
        else//否则在左支
            node=node->left;
    }
    //当迭代到空节点时，说明查找失败，返回空指针
    return nullptr;
}


//从树中移除一个节点，删除成功返回true，否则返回false
//k:待移除节点的键值
//cmp: 键值的比较函数接口
template<class TKey,class TValue>
bool AvlTree<TKey,TValue>::remove(const TKey &k,int(*cmp)(const TKey&,const TKey&)) {
    //获取AVL树根节点的连接
    AvlTreeNode** cp_head=&head;

    //保存指向上层结点的链接；指向待删除结点的链接的指针；指向最后一个叶子节点
    AvlTreeNode ** delete_node,* last_leap;
    //保存比较结果
    int cmp_result;
    //堆栈长度; 待删除的结点的堆栈索引位置，未平衡标记
    unsigned char stack_len, delete_index, unbalance;
    //保存旋转的状态
    unsigned char rt;

    //头节点为空，取消删除
    if (head == nullptr)return false;

    //堆栈长度初始化
    stack_len = 0;

    //未平衡标记初始化
    unbalance = 1;

    //找出所有与给定节点的值不相等的节点，并存入堆栈中
    while (*cp_head) {

        //获取待删除键值与节点键值的比较结果
        cmp_result = cmp(k,(*cp_head)->key);

        //存在相同的比较结点，退出迭代
        if (cmp_result == 0)
            break;

        //将当前链接存入堆栈
        pLstack[stack_len] =cp_head;

        //根据比较结果，保存下钻的方向，并移动指向链接的指针
        if (cmp_result > 0) {
            direction[stack_len] = 0;//在删除的实现中，用0表示向右下方向移动
            cp_head = &(*cp_head)->right;
        }
        else {
            direction[stack_len] = 1;//用1表示向左下方向移动
            cp_head = &(*cp_head)->left;
        }
        ++stack_len;//栈高度+1
    }

    //未出现和键值相同的结点，删除失败
    if (cmp_result != 0)
        return false;

    //存放指向待删除节点的链接
    pLstack[stack_len] = cp_head;

    //保存指向待删除节点的链接
    delete_node = cp_head;

    //若待删除节点的左节点不为空
    if ((*cp_head)->left != nullptr) {

        //将待删除节点的下支方向定位到左边
        direction[stack_len++] = 1;

        //将待删除节点的左支的下支方向定向到右边
        direction[stack_len] = 0;

        //保存待删除节点的下一个堆栈位置
        delete_index = stack_len++;

        //将当前链接定位到待删除节点的左节点的右节点的位置
        cp_head = &(*cp_head)->left->right;

        //若链接不指向空，则将当前链接存入堆栈，并继续向当前链接指向的节点的右支移动
        while (*cp_head) {
            pLstack[stack_len] = cp_head;
            direction[stack_len++] = 0;
            cp_head = &(*cp_head)->right;
        }

        //回退一个堆栈位置
        --stack_len;
        //若待删除节点的下一个堆栈位置等于当前位置，则说明待删除节点的左节点作为替换节点
        if (stack_len == delete_index)
            //用指针保存替换节点
            last_leap = (*delete_node)->left;
        else {
            //末尾堆栈节点即为替换节点
            last_leap = *pLstack[stack_len];
            //将指向替换节点的链接定向到替换节点的左支
            *pLstack[stack_len] = last_leap->left;
            //将替换节点的左支定向到待删除节点的左支
            last_leap->left = (*delete_node)->left;
            //将保存指向替换节点的左支的链接到堆栈中待删除结点下一个的位置堆栈
            pLstack[delete_index] = &last_leap->left;
        }
        //将替换节点的右支定向到待删除节点的右支
        last_leap->right = (*delete_node)->right;
        //将替换节点的平衡度等于待删除节点的平衡度
        last_leap->bf = (*delete_node)->bf;
    }
    else if ((*cp_head)->right != nullptr) //若待删除节点的右节点不为空，则将其右节点地址写入到替换节点的值
        last_leap = (*delete_node)->right;
    else//待删除节点是叶子节点，替换节点指向空
        last_leap = nullptr;

    //若替换节点不是空，则重置其父节点指针
    if(last_leap)
        last_leap->parent=(*delete_node)->parent;

    //删除待删除节点
    delete *delete_node;

    //树的节点数量减1
    --count;

    //将指向待删除节点的链接重新定向为替换节点的地址
    *delete_node = last_leap;

    //开始往上回溯进行平衡旋转
    while (stack_len > 0) {
        //用delete_node保存链接的信息
        delete_node = pLstack[--stack_len];

        //获取状态参数
        rt=direction[stack_len]|((*delete_node)->bf<<1);

        switch(rt){
            case 0://0或1表示回溯节点的平衡因子是0，不会因为其子树的删除改变该节点的平衡
            case 1:
                (*delete_node)->bf = direction[stack_len]+1;
                return true;//
            case 2://表示回溯节点发生了过度左倾，需要进行右旋
                rt=right_rotate(delete_node);
                goto Tk_Ba;//完成旋转后跳转到计算平衡因子值的函数入口
            case 3://3或4表示回溯节点的平衡因子与其下钻方向相反，回溯节点的平衡因子归0
            case 4:
                (*delete_node)->bf=0;
                break;
            default://默认情况，与情况2相反
                rt=left_rotate(delete_node);
            Tk_Ba://对进行过旋转的节点重新计算其与孙子节点的平衡因子
                balance_factor(*delete_node,rt,&unbalance);
                if(!unbalance)//若旋转过后并不会导致树的高度发生变化，则可以退出
                    return true;
        }
    }

    //若删除的过程能够到达此处，说明树高必定减少
    //树高-1
    --heigh;

    return true;
}

//获取现有节点数量
template<class TKey,class TValue>
unsigned long long AvlTree<TKey,TValue>::getSize() const {
    return count;
}

//析构
template<class TKey, class TValue>
AvlTree<TKey, TValue>::~AvlTree() {
    //删除所有节点
    clear();
    //删除堆栈
    delete[] pLstack;
    delete[] pstack;
    delete[] direction;
}

//删除所有的树节点
template<class TKey, class TValue>
void AvlTree<TKey, TValue>::clear() {
    //空树，直接退出
    if (head == nullptr)
        return;

    //保存上层结点的堆栈；临时变量
    AvlTreeNode * temp;
    //堆栈的长度
    unsigned char stack_len = 0;

    do {
        while (head->left) {//若当前节点的左支不为空，则将其链接保存到堆栈，然后移动到节点的左支链接
            pstack[stack_len++] = head;
            head = head->left;
        }

        //保存最后一个左链接为空的结点的右链接
        temp = head->right;

        //释放节点内存
        delete head;

        //被释放内存的结点的右链接不为空，将当前结点定向到被释放内存的结点的右链接
        if (temp)
            head = temp;
        else {
            //堆栈为空，完成迭代，退出
            if (stack_len == 0)break;
            do {
                //从堆栈中取出结点，保存其右链接并释放被取出的结点的内存
                head = pstack[--stack_len];
                temp = head->right;
                //
                delete head;
            } while (temp == nullptr && stack_len != 0);//若被取出的结点的右链接为空，并且堆栈不为空，则继续取出结点

            //若被取出的结点的右链接为空，则迭代完成，退出
            if (temp == nullptr)
                break;
            head = temp;//将当前结点定向到被取出结点的右链接
        }
    } while (true);

    //将树的高度和节点数量以及头指针都重置为0
    count=0;
    heigh=0;
    head=nullptr;
}

//获取正向静态迭代器
template<class TKey, class TValue>
typename AvlTree<TKey,TValue>::Iterator AvlTree<TKey, TValue>::begin() {
    Iterator beg(this);
    ++beg;
    return beg;
}

//获取反向静态迭代器
template<class TKey, class TValue>
typename AvlTree<TKey,TValue>::Reverse_Interator AvlTree<TKey, TValue>::rbegin() {
    Reverse_Interator rbeg(this);
    ++rbeg;
    return rbeg;
}

#endif
