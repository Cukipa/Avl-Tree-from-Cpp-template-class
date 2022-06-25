//���ߣ������
//����QQ��2718088693
//����Bվ�ǳƣ�������ʦ
//˵����
//�˿���һ�����������Ż���AVL����ʵ�֣��㲻��Ҫ�Ķ��κεط�
//����Ըÿ�����������ʣ�����Ը����������ʱ�����
//
//

#ifndef LITTLESEARCHENGINE_AVLTREE_H
#define LITTLESEARCHENGINE_AVLTREE_H

//AVL����TKey�Ǽ���TValue��ֵ��
template<class TKey,class TValue>
class AvlTree{
    //���������ඨ��Ϊ��Ԫ��
    friend class Iterator_Base;
    friend class Iterator;
    friend class Reverse_Interator;
private:

    //�������Ľڵ���
    class AvlTreeNode{
    public:
        TValue value;//ֵ��
        TKey key;//��ֵ
        unsigned char bf;//ƽ������
        AvlTreeNode* parent;//���ڵ�
        AvlTreeNode* left,*right;//���ҽڵ��ָ��
        AvlTreeNode(const TKey& k,const TValue& val,AvlTreeNode* p):key(k),value(val){
            bf=0;
            parent=p;
            left=right=nullptr;
        }
    };

    //�����ܲ����Ķ�ջ����
    static const unsigned char Max_AvlTree_High=55;

    //�ڵ���
    unsigned long long count;

    //����ͷָ��
    AvlTreeNode* head;

    //����
    unsigned char heigh;

    //��¼�ڵ����ӵĶ�ջ
    //ע�����еġ��ڵ����ӡ���ָ������ָ��AVL���ڵ㡰AvlTreeNode�����ָ���ָ�룬��˫��ָ�롣
    AvlTreeNode*** pLstack=new AvlTreeNode**[Max_AvlTree_High];

    //��¼�ڵ��ַ�Ķ�ջ
    AvlTreeNode** pstack=new AvlTreeNode*[Max_AvlTree_High];

    //��¼����ָ����Ķ�ջ
    unsigned char *direction=new unsigned char[Max_AvlTree_High];

    //������������ϸ˵��������
    static unsigned char left_rotate(AvlTreeNode** node);

    //������������ϸ˵��������
    static unsigned char right_rotate(AvlTreeNode** node);

    //�����ڵ�ƽ������ֵ�ĺ�������ϸ˵��������
    static void balance_factor(AvlTreeNode* node, unsigned char rt, unsigned char* ub);

    //��̬����������
    class Iterator_Base{
    public:
        explicit Iterator_Base(AvlTree<TKey,TValue>* tree):current(tree->head){
            pstack=new AvlTreeNode*[Max_AvlTree_High];//Ϊ�����������ջ��������ϲ�ڵ�
            stack_len=iter_type=0;//�������Ķ�ջ���Ⱥ͵���״̬
        }

        ~Iterator_Base(){
            delete[] pstack;
        }

        //����Ƿ������ĩβ����������Ч����true�����򷵻�false
        bool nempty()const{return current!=nullptr;}
        //��ȡ������ָ���Key
        const TKey& getkey()const{return current->key;}
        //��ȡ������ָ���Value
        TValue& getValue(){return current->value;}
    protected:
        //��ǰ������ָ������ڵ�
        AvlTreeNode* current;
        //����ϲ�ڵ�����ӵĶ�ջ
        AvlTreeNode** pstack;
        //��ջ����
        unsigned char stack_len;
        //����״̬
        unsigned char iter_type;
    };
public:

    //����̬������
    // ��̬����ָ������ʹ�õ�����������insert��remove���������������
    class Iterator:public Iterator_Base{
    public:
        explicit Iterator(AvlTree<TKey,TValue>* tree): Iterator_Base(tree){}
        Iterator & operator++();
    };

    //����̬������
    // ��̬����ָ������ʹ�õ�����������insert��remove���������������
    class Reverse_Interator:public Iterator_Base{
    public:
        explicit Reverse_Interator(AvlTree<TKey,TValue>* tree): Iterator_Base(tree){}
        Reverse_Interator& operator++();
    };

    AvlTree();
    ~AvlTree();

    //���һ���µĽڵ�
    TValue* insert(const TKey& k,const TValue& val,int(*cmp)(const TKey&,const TKey&));
    //����ĳ��Key��ֵ��
    TValue* find(const TKey& k,int(*cmp)(const TKey&,const TKey&))const;
    //ɾ��ָ��Key��ĳ���ڵ�
    bool remove(const TKey& k,int(*cmp)(const TKey&,const TKey&));
    //��ȡ�ڵ�����
    unsigned long long getSize()const;
    //��ȡ����
    unsigned char getHeigh()const;
    //ɾ��ȫ���ڵ�
    void clear();
    //��ȡ����̬�������������ھ��о�̬�������Ĵ������ڰ���insert��remove��䣬�����д���
    Iterator begin();
    //��ȡ����̬�������������ھ��о�̬�������Ĵ������ڰ���insert��remove��䣬�����д���
    Reverse_Interator rbegin();
};


template<class TKey, class TValue>
typename AvlTree<TKey,TValue>::Reverse_Interator & AvlTree<TKey, TValue>::Reverse_Interator::operator++(){

    if(Iterator_Base::current==nullptr)
        return *this;

    //״̬1�����µ�ǰָ��
    if(Iterator_Base::iter_type==1){
        //������ڵ�
        if(Iterator_Base::current->left!=nullptr){
            Iterator_Base::current=Iterator_Base::current->left;
            Iterator_Base::iter_type=0;
        }
        else{//�ж��Ƿ�������
            if(Iterator_Base::stack_len==0){
                Iterator_Base::current=nullptr;
            }
            else{//���µ�״̬2����ָ���Ƶ����ڵ�
                Iterator_Base::current=Iterator_Base::pstack[--Iterator_Base::stack_len];
                Iterator_Base::iter_type=2;
            }
            return *this;
        }
    }//״̬2���ڵ��ջ
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

    //״̬0���ҵ����Ҷ˽ڵ�
    if(Iterator_Base::iter_type==0){
        //�������Ӳ�Ϊ�յĽ������ջ��Ȼ��������
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

    //״̬1�����µ�ǰָ��
    if(Iterator_Base::iter_type==1){
        //�����ҽڵ�
        if(Iterator_Base::current->right!=nullptr){
            Iterator_Base::current=Iterator_Base::current->right;
            Iterator_Base::iter_type=0;
        }
        else{//�ж��Ƿ�������
            if(Iterator_Base::stack_len==0){
                Iterator_Base::current=nullptr;
            }
            else{//���µ�״̬2����ָ���Ƶ��Ҳ�ڵ�
                Iterator_Base::current=Iterator_Base::pstack[--Iterator_Base::stack_len];
                Iterator_Base::iter_type=2;
            }
            return *this;
        }
    }//״̬2���ڵ��ջ
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

    //״̬0���ҵ�����˽ڵ�
    if(Iterator_Base::iter_type==0){
        //�������Ӳ�Ϊ�յĽ������ջ��Ȼ��������
        while (Iterator_Base::current->left) {
            Iterator_Base::pstack[Iterator_Base::stack_len++] = Iterator_Base::current;
            Iterator_Base::current = Iterator_Base::current->left;
        }
        Iterator_Base::iter_type=1;
    }
    return *this;
}


//��ȡ���ĸ߶�
template<class TKey, class TValue>
unsigned char AvlTree<TKey, TValue>::getHeigh() const {
    return heigh;
}

//�ڵ����������
//node:����ת�Ľڵ�
//rt:��¼��ת����ı��λ
template<class TKey,class TValue>
unsigned char AvlTree<TKey,TValue>::left_rotate(AvlTreeNode **node) {
    AvlTreeNode* son,* pson;
    son = (*node)->right;
    pson = son->left;
    if (son->bf^1) {//ֻ����д��������

        son->parent=(*node)->parent;
        (*node)->parent=son;
        if(pson)
            son->left->parent=*node;

        (*node)->right = son->left;
        son->left = *node;
        *node = son;
        return 1;
    }
    else {//��Ҫ�ȶ������ӽڵ����������Ȼ��Ը��ڵ�������

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

//�ڵ����������
//node:����ת�Ľڵ�
//rt:��¼��ת����ı��λ
template<class TKey,class TValue>
unsigned char AvlTree<TKey,TValue>::right_rotate(AvlTreeNode **node) {
    AvlTreeNode* son,* pson;
    son = (*node)->left;
    pson=son->right;
    if (son->bf^2) {//ֻ��Ҫ���д��������

        son->parent=(*node)->parent;
        (*node)->parent=son;
        if(pson)
            son->right->parent=*node;

        (*node)->left = son->right;
        son->right = *node;
        *node = son;
        return 2;
    }
    else {//��Ҫ�ȶ������ӽڵ����������Ȼ��Ը��ڵ�������

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

//����������ת֮��ĵ�ǰ��תλ�á����ڵ㡰�����̽ڵ��ƽ������
//node:������ת֮��ġ������ڵ㣬�˴��ġ���������Ե�
//rt:ħ�����֣���¼��һ����ת����֮������͡�
//ub:��¼���β����Ƿ����ʹ��ԭ��ʧ��������´ﵽƽ�⣬�Ӳ�ͬ�Ĳ������в�ͬ��ֵ
template<class TKey,class TValue>
void AvlTree<TKey,TValue>::balance_factor(AvlTreeNode *node, unsigned char rt, unsigned char *ub) {
    AvlTreeNode* left_node,* right_node;

    //���ڵ�����ӽڵ�
    left_node = node->left;
    //���ڵ�����ӽڵ�
    right_node = node->right;

    //����������Ҳ����ӵĲ��֣������˴����Ż�
    //��ȡ״ֵ̬
    unsigned char rotate_type=((rt!=0)|(node->bf<<1));

    //���ø��ڵ�ƽ������
    if(node->bf!=0)
        node->bf=0;

    //����״ֵ̬�ж���ε�������ڵ��ƽ������
    //�˴���Ϊ���ӣ����ڹ���˵��
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

//���캯��
template<class TKey,class TValue>
AvlTree<TKey,TValue>::AvlTree() {
    head=nullptr;//ͷָ��ָ���
    count=0;//���Ľڵ����Ϊ0
    heigh=0;//����Ϊ0
}

//�����в���һ���µļ�ֵ����ֵ�����۲����Ƿ�ɹ����������½ڵ�/���нڵ��ֵ��ָ��
//k:������ļ�ֵ
//val:�������ֵ��
//cmp:��ֵ�ıȽϺ����ӿ�
template<class TKey,class TValue>
TValue* AvlTree<TKey,TValue>::insert(const TKey &k,const TValue& val,int(*cmp)(const TKey&,const TKey&)) {

    //ָ��ǰ�ڵ������
    AvlTreeNode** cp_head = &head;

    //�½ڵ��ֵ��ָ��
    TValue * tag;

    //ջ�ĸ߶�
    unsigned char stack_len = 0;

    //��¼��ϸ����ת״̬
    unsigned char rt;

    //��ֵ�ıȽϽ��
    int cmp_result;

    //��¼��ջ����󳤶�
    unsigned char new_heigh;

    //�����ҵ�Ҷ�ӽڵ�
    while (*cp_head) {
        //���ݱȽϺ����Ľӿڻ�ȡ�ȽϽ��
        cmp_result = cmp(k,(*cp_head)->key);

        //���ȽϽ����0��˵��������ͬ��ֵ�Ľڵ㣬ȡ�����
        if (cmp_result == 0)
            return &(*cp_head)->value;
        //��ָ��ǰ�ڵ�����Ӵ����ջ
        pLstack[stack_len] = cp_head;

        //��¼����ķ��򣬲����µ�ǰ����
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

    //��¼������ջ��
    new_heigh=stack_len+1;

    //��Ҷ�ӽڵ�����Ӵ�����½ڵ�
    *cp_head = new AvlTreeNode(k,val,(stack_len)?(*pLstack[stack_len-1]): nullptr);

    //��¼�½ڵ��ֵ��
    tag=&(*cp_head)->value;

    //���нڵ�������1
    ++count;

    //����ڵ����
    while (stack_len > 0) {
        //ȥ������ջ�Ķ�������
        cp_head = pLstack[--stack_len];

        //�������ת���״̬���
        rt=direction[stack_len]|((*cp_head)->bf<<1);

        switch(rt){
            case 0://0��1��ʾ���ݽڵ��ƽ������Ϊ0��ֻ����������귽��ı���ݽڵ��ƽ��ȼ���
            case 1:
                (*cp_head)->bf=direction[stack_len]+1;
                break;
            case 2://2��ʾ���ݽڵ�������㣬��Ҫ��������
                rt=right_rotate(cp_head);
                goto Tk_Ba;//��ת��ɺ���ת��ƽ�⺯����ڣ����¼��㵱ǰ�ڵ��ƽ������
            case 3://3��4��ʾ���ݽڵ��˫�߷�֧�߶�һ�����ﵽƽ��
            case 4:
                (*cp_head)->bf=0;
                return tag;
            default://Ĭ���������2�෴
                rt=left_rotate(cp_head);
            Tk_Ba://���㵱ǰ�ڵ㼰�����ӽڵ��ƽ������
                balance_factor(*cp_head,rt, nullptr);
                return tag;
        }
    }

    //���ܵ���˴�����������ջ���ȴ��ڵ�ǰ���ߣ���ô���ĸ߶ȱض�������1
    //��������
    if(new_heigh>heigh)
        heigh=new_heigh;

    //�����½ڵ��ֵ��ָ��
    return tag;
}

//�����в���һ��ָ����ֵ�Ľڵ��ֵ�򣬲��ҳɹ�����ֵ���ָ�룬���򷵻ؿ�ָ��
//k:�����ҵļ�ֵ
//cmp:��ֵ�ıȽϺ����ӿ�
template<class TKey,class TValue>
TValue *AvlTree<TKey,TValue>::find(const TKey &k,int(*cmp)(const TKey&,const TKey&)) const {
    //����ǰ�ڵ㶨������ͷ�ڵ�
    AvlTreeNode* node=head;
    int cmp_result;//����ȽϽ��
    //���ڵ㲻Ϊ�գ�����Լ�������
    while(node){
        //���ݱȽϽӿڻ�ȡ�ȽϽ��
        cmp_result=cmp(k,node->key);

        //�����Ϊ0��˵���ҵ��˽ڵ㣬���ؽڵ��ֵ��ָ��
        if(cmp_result==0)
            return &node->value;
        //���ȽϽ������0��˵�������ҵ�ֵ�ض�����֧
        if(cmp_result>0)
            node=node->right;
        else//��������֧
            node=node->left;
    }
    //���������սڵ�ʱ��˵������ʧ�ܣ����ؿ�ָ��
    return nullptr;
}


//�������Ƴ�һ���ڵ㣬ɾ���ɹ�����true�����򷵻�false
//k:���Ƴ��ڵ�ļ�ֵ
//cmp: ��ֵ�ıȽϺ����ӿ�
template<class TKey,class TValue>
bool AvlTree<TKey,TValue>::remove(const TKey &k,int(*cmp)(const TKey&,const TKey&)) {
    //��ȡAVL�����ڵ������
    AvlTreeNode** cp_head=&head;

    //����ָ���ϲ�������ӣ�ָ���ɾ���������ӵ�ָ�룻ָ�����һ��Ҷ�ӽڵ�
    AvlTreeNode ** delete_node,* last_leap;
    //����ȽϽ��
    int cmp_result;
    //��ջ����; ��ɾ���Ľ��Ķ�ջ����λ�ã�δƽ����
    unsigned char stack_len, delete_index, unbalance;
    //������ת��״̬
    unsigned char rt;

    //ͷ�ڵ�Ϊ�գ�ȡ��ɾ��
    if (head == nullptr)return false;

    //��ջ���ȳ�ʼ��
    stack_len = 0;

    //δƽ���ǳ�ʼ��
    unbalance = 1;

    //�ҳ�����������ڵ��ֵ����ȵĽڵ㣬�������ջ��
    while (*cp_head) {

        //��ȡ��ɾ����ֵ��ڵ��ֵ�ıȽϽ��
        cmp_result = cmp(k,(*cp_head)->key);

        //������ͬ�ıȽϽ�㣬�˳�����
        if (cmp_result == 0)
            break;

        //����ǰ���Ӵ����ջ
        pLstack[stack_len] =cp_head;

        //���ݱȽϽ������������ķ��򣬲��ƶ�ָ�����ӵ�ָ��
        if (cmp_result > 0) {
            direction[stack_len] = 0;//��ɾ����ʵ���У���0��ʾ�����·����ƶ�
            cp_head = &(*cp_head)->right;
        }
        else {
            direction[stack_len] = 1;//��1��ʾ�����·����ƶ�
            cp_head = &(*cp_head)->left;
        }
        ++stack_len;//ջ�߶�+1
    }

    //δ���ֺͼ�ֵ��ͬ�Ľ�㣬ɾ��ʧ��
    if (cmp_result != 0)
        return false;

    //���ָ���ɾ���ڵ������
    pLstack[stack_len] = cp_head;

    //����ָ���ɾ���ڵ������
    delete_node = cp_head;

    //����ɾ���ڵ����ڵ㲻Ϊ��
    if ((*cp_head)->left != nullptr) {

        //����ɾ���ڵ����֧����λ�����
        direction[stack_len++] = 1;

        //����ɾ���ڵ����֧����֧�������ұ�
        direction[stack_len] = 0;

        //�����ɾ���ڵ����һ����ջλ��
        delete_index = stack_len++;

        //����ǰ���Ӷ�λ����ɾ���ڵ����ڵ���ҽڵ��λ��
        cp_head = &(*cp_head)->left->right;

        //�����Ӳ�ָ��գ��򽫵�ǰ���Ӵ����ջ����������ǰ����ָ��Ľڵ����֧�ƶ�
        while (*cp_head) {
            pLstack[stack_len] = cp_head;
            direction[stack_len++] = 0;
            cp_head = &(*cp_head)->right;
        }

        //����һ����ջλ��
        --stack_len;
        //����ɾ���ڵ����һ����ջλ�õ��ڵ�ǰλ�ã���˵����ɾ���ڵ����ڵ���Ϊ�滻�ڵ�
        if (stack_len == delete_index)
            //��ָ�뱣���滻�ڵ�
            last_leap = (*delete_node)->left;
        else {
            //ĩβ��ջ�ڵ㼴Ϊ�滻�ڵ�
            last_leap = *pLstack[stack_len];
            //��ָ���滻�ڵ�����Ӷ����滻�ڵ����֧
            *pLstack[stack_len] = last_leap->left;
            //���滻�ڵ����֧���򵽴�ɾ���ڵ����֧
            last_leap->left = (*delete_node)->left;
            //������ָ���滻�ڵ����֧�����ӵ���ջ�д�ɾ�������һ����λ�ö�ջ
            pLstack[delete_index] = &last_leap->left;
        }
        //���滻�ڵ����֧���򵽴�ɾ���ڵ����֧
        last_leap->right = (*delete_node)->right;
        //���滻�ڵ��ƽ��ȵ��ڴ�ɾ���ڵ��ƽ���
        last_leap->bf = (*delete_node)->bf;
    }
    else if ((*cp_head)->right != nullptr) //����ɾ���ڵ���ҽڵ㲻Ϊ�գ������ҽڵ��ַд�뵽�滻�ڵ��ֵ
        last_leap = (*delete_node)->right;
    else//��ɾ���ڵ���Ҷ�ӽڵ㣬�滻�ڵ�ָ���
        last_leap = nullptr;

    //���滻�ڵ㲻�ǿգ��������丸�ڵ�ָ��
    if(last_leap)
        last_leap->parent=(*delete_node)->parent;

    //ɾ����ɾ���ڵ�
    delete *delete_node;

    //���Ľڵ�������1
    --count;

    //��ָ���ɾ���ڵ���������¶���Ϊ�滻�ڵ�ĵ�ַ
    *delete_node = last_leap;

    //��ʼ���ϻ��ݽ���ƽ����ת
    while (stack_len > 0) {
        //��delete_node�������ӵ���Ϣ
        delete_node = pLstack[--stack_len];

        //��ȡ״̬����
        rt=direction[stack_len]|((*delete_node)->bf<<1);

        switch(rt){
            case 0://0��1��ʾ���ݽڵ��ƽ��������0��������Ϊ��������ɾ���ı�ýڵ��ƽ��
            case 1:
                (*delete_node)->bf = direction[stack_len]+1;
                return true;//
            case 2://��ʾ���ݽڵ㷢���˹������㣬��Ҫ��������
                rt=right_rotate(delete_node);
                goto Tk_Ba;//�����ת����ת������ƽ������ֵ�ĺ������
            case 3://3��4��ʾ���ݽڵ��ƽ�������������귽���෴�����ݽڵ��ƽ�����ӹ�0
            case 4:
                (*delete_node)->bf=0;
                break;
            default://Ĭ������������2�෴
                rt=left_rotate(delete_node);
            Tk_Ba://�Խ��й���ת�Ľڵ����¼����������ӽڵ��ƽ������
                balance_factor(*delete_node,rt,&unbalance);
                if(!unbalance)//����ת���󲢲��ᵼ�����ĸ߶ȷ����仯��������˳�
                    return true;
        }
    }

    //��ɾ���Ĺ����ܹ�����˴���˵�����߱ض�����
    //����-1
    --heigh;

    return true;
}

//��ȡ���нڵ�����
template<class TKey,class TValue>
unsigned long long AvlTree<TKey,TValue>::getSize() const {
    return count;
}

//����
template<class TKey, class TValue>
AvlTree<TKey, TValue>::~AvlTree() {
    //ɾ�����нڵ�
    clear();
    //ɾ����ջ
    delete[] pLstack;
    delete[] pstack;
    delete[] direction;
}

//ɾ�����е����ڵ�
template<class TKey, class TValue>
void AvlTree<TKey, TValue>::clear() {
    //������ֱ���˳�
    if (head == nullptr)
        return;

    //�����ϲ���Ķ�ջ����ʱ����
    AvlTreeNode * temp;
    //��ջ�ĳ���
    unsigned char stack_len = 0;

    do {
        while (head->left) {//����ǰ�ڵ����֧��Ϊ�գ��������ӱ��浽��ջ��Ȼ���ƶ����ڵ����֧����
            pstack[stack_len++] = head;
            head = head->left;
        }

        //�������һ��������Ϊ�յĽ���������
        temp = head->right;

        //�ͷŽڵ��ڴ�
        delete head;

        //���ͷ��ڴ�Ľ��������Ӳ�Ϊ�գ�����ǰ��㶨�򵽱��ͷ��ڴ�Ľ���������
        if (temp)
            head = temp;
        else {
            //��ջΪ�գ���ɵ������˳�
            if (stack_len == 0)break;
            do {
                //�Ӷ�ջ��ȡ����㣬�����������Ӳ��ͷű�ȡ���Ľ����ڴ�
                head = pstack[--stack_len];
                temp = head->right;
                //
                delete head;
            } while (temp == nullptr && stack_len != 0);//����ȡ���Ľ���������Ϊ�գ����Ҷ�ջ��Ϊ�գ������ȡ�����

            //����ȡ���Ľ���������Ϊ�գ��������ɣ��˳�
            if (temp == nullptr)
                break;
            head = temp;//����ǰ��㶨�򵽱�ȡ������������
        }
    } while (true);

    //�����ĸ߶Ⱥͽڵ������Լ�ͷָ�붼����Ϊ0
    count=0;
    heigh=0;
    head=nullptr;
}

//��ȡ����̬������
template<class TKey, class TValue>
typename AvlTree<TKey,TValue>::Iterator AvlTree<TKey, TValue>::begin() {
    Iterator beg(this);
    ++beg;
    return beg;
}

//��ȡ����̬������
template<class TKey, class TValue>
typename AvlTree<TKey,TValue>::Reverse_Interator AvlTree<TKey, TValue>::rbegin() {
    Reverse_Interator rbeg(this);
    ++rbeg;
    return rbeg;
}

#endif
