#include <iostream>
#include <queue>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
using namespace std;

// 树结点
struct node {
    struct node *left;
    int data;
    int height;
    string type;    // leaf, lchild, rchild, bchild   没有孩子/只有左孩子/只有右孩子/有两个孩子
    struct node *right;

};


// AVL Tree
class AVL
{
private:
    
public:
    struct node * root;
    AVL(){
        this->root = NULL;

    }
    string get_node_type(struct node *p){
        // 左右子树都有
        if (p->left && p->right){
            return string((char*)"bchild");
        }

        // 右子树为空
        else if (p->left && p->right == NULL){
            return string((char*)"lchild");
        }

        // 左子树为空
        else if (p->left ==NULL && p->right){
            return string((char*)"rchild");
        }

        // 左右都为空
        else{
            return string((char*)"leaf");
        }
    }


    // 计算给定结点的高度
    int calheight(struct node *p){
        if (p == nullptr) return 0; 
        int thisheight = 1 + max(calheight(p->left), calheight(p->right)); 
        p->height=thisheight;
        return thisheight;

    }

    // 计算左右子树高度差，后续依据不同的高度差进行对应旋转操作
    int bf(struct node *n){
        string node_type(get_node_type(n));

        if(node_type == "bchild"){
            /* cout<<n->left->data<<" height:"<<n->left->height<<endl;
            cout<<n->right->data<<" height:"<<n->right->height<<endl; */
            return n->left->height - n->right->height; 
        }
        else if(node_type == "lchild"){
            return n->left->height; 
        }
        else if(node_type =="rchild" ){
            return -n->right->height;
        }
        else{return 0;}
    }

    // 右旋
    struct node * llrotation(struct node *n){
        struct node *p;
        struct node *tp;
        p = n;
        tp = p->left;

        p->left = tp->right;
        tp->right = p;

        return tp; 
    }

    // 左旋
    struct node * rrrotation(struct node *n){
        struct node *p;
        struct node *tp;
        p = n;
        tp = p->right;

        p->right = tp->left;
        tp->left = p;

        return tp; 
    }


    // 先右旋后左旋
    struct node * rlrotation(struct node *n){
        struct node *p;
        struct node *tp;
        struct node *tp2;
        p = n;
        tp = p->right;
        tp2 =p->right->left;

        p -> right = tp2->left;
        tp ->left = tp2->right;
        tp2 ->left = p;
        tp2->right = tp; 
        
        return tp2; 
    }

    // 先左旋后右旋
    struct node * lrrotation(struct node *n){
        struct node *p;
        struct node *tp;
        struct node *tp2;
        p = n;
        tp = p->left;
        tp2 =p->left->right;

        p -> left = tp2->right;
        tp ->right = tp2->left;
        tp2 ->right = p;
        tp2->left = tp; 
        
        return tp2; 
    }

    struct node* insert(struct node *r,int data){
        
        // 创建根节点
        if(r==NULL){
            struct node *n;
            n = new struct node;
            n->data = data;
            r = n;
            r->left = r->right = NULL;
            r->height = 1; 
            return r;             
        }

        // 插入结点
        else{
            if(data < r->data)
            r->left = insert(r->left,data);
            else if (data > r->data){
                r->right = insert(r->right,data);
            }
            else{
                cout<< endl;
                cout << "**************************************"<< endl;
                cout << "Failed"<<endl;
                cout << "Data " << data <<" already exists!" << endl;
                cout << "**************************************"<< endl;
                return r;
            }
            
        }

        r->height = calheight(r);
       /*  cout<<endl;
        cout<<r->data<<" height:"<<r->height<<endl;
        cout<<endl; */

        // 插入点在左子树的左孩子，右旋
        if(bf(r)==2 && bf(r->left)==1){
            r = llrotation(r);
            calheight(r);

        }
        // 左旋
        else if(bf(r)==-2 && bf(r->right)==-1){
            r = rrrotation(r);
            calheight(r);
        }
        // 先右旋再左旋
        else if(bf(r)==-2 && bf(r->right)==1){
            r = rlrotation(r);
            calheight(r);
        }
        // 先左旋再右旋
        else if(bf(r)==2 && bf(r->left)==-1){
            r = lrrotation(r);
            calheight(r);
        }        

        return r;

        }
 
    struct node * deleteNode(struct node *p,int data){

        string node_type(get_node_type(p));

        if(node_type == "leaf"){
            if (p->data == data){
                if(p==this->root)
                    this->root = NULL;
                delete p;
                return NULL;
            }

            // 所删除的结点不在AVL树中
            else{
                show_delete_error(data);
                return p;
            }
        }

        struct node *t;
        struct node *q;
        if(p->data < data){
            // 所删除的结点不在AVL树中
            string temp_type(get_node_type(p->right));
            if (temp_type == "leaf" && p->right->data != data){
                show_delete_error(data);
                return p;
            } 

            p->right = deleteNode(p->right,data);
            
        }
        else if(p->data > data){
            // 所删除的结点不在AVL树中
            string temp_type(get_node_type(p->left));
            if (temp_type == "leaf" && p->left->data != data){
                show_delete_error(data);
                return p;
            } 

            p->left = deleteNode(p->left,data);
        }
        else{
            if(p->left != NULL){
                q = inpre(p->left);
                p->data = q->data;
                p->left=deleteNode(p->left,q->data);
            }
            else{
                q = insuc(p->right);
                p->data = q->data;
                p->right = deleteNode(p->right,q->data);
            }
        }

        // 删除后维护树平衡
        if(bf(p)==2 && bf(p->left)==1){ p = llrotation(p); calheight(p);}                  
        else if(bf(p)==2 && bf(p->left)==-1){ p = lrrotation(p); calheight(p);}
        else if(bf(p)==2 && bf(p->left)==0){ p = llrotation(p); calheight(p);}
        else if(bf(p)==-2 && bf(p->right)==-1){ p = rrrotation(p); calheight(p);}
        else if(bf(p)==-2 && bf(p->right)==1){ p = rlrotation(p); calheight(p);}
        else if(bf(p)==-2 && bf(p->right)==0){ p = llrotation(p); calheight(p);}

        
        return p;
    }

    // 遍历右子节点
    struct node* inpre(struct node* p){
        while(p->right!=NULL)
            p = p->right;
        return p;    
    }

    // 遍历左子节点
    struct node* insuc(struct node* p){
        while(p->left!=NULL)
            p = p->left;

        return p;    
    }

    // 插入已经存在的结点时打印以下错误
    void show_insert_error(int data){
        cout<< endl;
        cout << "**************************************"<< endl;
        cout << "Failed"<<endl;
        cout << "Data " << data <<" already exists!" << endl;
        cout << "**************************************"<< endl;
    }


    // 当删除一个不存在的结点时，打印以下信息
    void show_delete_error(int data){
        cout<< endl;
        cout << "**************************************"<< endl;
        cout << "Failed"<<endl;
        cout << "Data " << data <<" does not exist!" << endl;
        cout << "**************************************"<< endl;
    }

    // 可视化AVLTree
    void show_tree(AVL T, const char* elem_fmt="%d")
    {
        if (T.root == NULL)
            return;
        const int _MAX_STR_LEN = 128;        //单个元素打印成str最大长度
        const int _MAX_NODE_NUM = 1024;      //结点最大个数
        const char horiz_conj_char ='_';    //连接横线
        const char vert_conj_char = '|';     //竖线
        const char left_bracket_char = '[';  //每个元素左边的括号
        const char right_bracket_char = ']'; //元素右边的括号

        //思路: 层序遍历算深度, 中序遍历算横坐标, 用结构体数组把这些信息存好再统一打印
        typedef struct _node_print_info
        {
            node* address; //二次遍历时的唯一区分标志，即key //用hash表将地址映射会更省时间空间
            int str_len;
            int depth;
            int left_margin; // horizontal coordintate
        } _Info;

        node* tree_deque[_MAX_NODE_NUM];
        int depth_queue[_MAX_NODE_NUM]; //记录深度的队列
        _Info* info_p_arr[_MAX_NODE_NUM];
        int node_count = 0;
        int front = -1, rear = -1;
        node* p;
        _Info* info_p;
        char temp_str[_MAX_STR_LEN];

        //用队列统计结点的深度信息, 顺便统计每个结点数据打印时的长度
        tree_deque[++rear] = T.root;
        depth_queue[rear] = 1;
        while (front < rear)
        {
            p = tree_deque[++front];
            info_p = (_Info*)calloc(1, sizeof(_Info));
            info_p->address = p; //记录地址
            memset(temp_str, 0, _MAX_STR_LEN);
            sprintf(temp_str, elem_fmt, p->data);   // TODO: 注意
            info_p->str_len = strlen(temp_str) + 2; //计算打印后的元素长度
            info_p->depth = depth_queue[front];
            info_p_arr[node_count++] = info_p;

            if (p->left != NULL)
            {
                tree_deque[++rear] = p->left;
                depth_queue[rear] = depth_queue[front] + 1;
            }
            if (p->right != NULL)
            {
                tree_deque[++rear] = p->right;
                depth_queue[rear] = depth_queue[front] + 1;
            }
        }

        //接下来统计横坐标, 用中序遍历
        p = T.root;
        int top = -1, i;
        int horizontal_accumu_cache = 0; //横坐标累计长度
        do
        {
            while (p != NULL)
            {
                tree_deque[++top] = p;
                p = p->left;
            }
            p = tree_deque[top--];
            for (i = 0; i < node_count; ++i)
            { //这里用hash表的优势就体现出来了
                if (info_p_arr[i]->address == p)
                { //计算横坐标
                    info_p_arr[i]->left_margin = horizontal_accumu_cache;
                    horizontal_accumu_cache += info_p_arr[i]->str_len - 1; //减一可以重叠一个括号,更紧凑一点点
                }
            }
            p = p->right;
        } while (!(p == NULL && top == -1));

        //接下来开始打印
        int horiz_left_start, horiz_right_end, cursor, j, k, cur_depth = 1, end_flag = 0;
        int vert_index_arr[_MAX_NODE_NUM]; //偶数行的竖线存储数组
        i = 0;
        while (i < node_count)
        {
            k = -1;
            cursor = 0;
            while (info_p_arr[i]->depth == cur_depth)
            {
                //打印左边
                p = info_p_arr[i]->address;
                if (p->left != NULL)
                { //有左孩子说明有横线要打印
                    for (j = 0; j < node_count; ++j)
                    { //暴力查找
                        if (info_p_arr[j]->address == p->left)
                        {
                            horiz_left_start = info_p_arr[j]->left_margin + info_p_arr[j]->str_len / 2;
                            vert_index_arr[++k] = horiz_left_start;
                            break;
                        }
                    }
                    for (; cursor < horiz_left_start; ++cursor)
                        cout <<  " ";
                    for (; cursor < info_p_arr[i]->left_margin; ++cursor)
                        cout << horiz_conj_char;
                }
                else
                { //没有左孩子全打印空格即可
                    for (; cursor < info_p_arr[i]->left_margin; ++cursor)
                        cout << " ";
                }

                //打印元素
                cout << left_bracket_char;
                cout << p->data;
                cout << right_bracket_char;
                cursor += info_p_arr[i]->str_len;

                //打印右边
                if (p->right != NULL)
                {
                    for (j = 0; j < node_count; ++j)
                    {
                        if (info_p_arr[j]->address == p->right)
                        {
                            horiz_right_end = info_p_arr[j]->left_margin + info_p_arr[j]->str_len / 2;
                            vert_index_arr[++k] = horiz_right_end;
                            break;
                        }
                    }
                    for (; cursor < horiz_right_end; ++cursor)
                        cout << horiz_conj_char;
                } //右边没有else ,因为只考虑横线即可, 空白算到同层下一个元素左边

                if (++i >= node_count)
                {
                    end_flag = 1; //打印完最后一个元素, 下一行不需要竖线了,提前结束
                    break;
                }
            }
            cout << "\n";

            //打印竖线
            if (!end_flag)
            {
                cursor = 0;
                for (j = 0; j <= k; ++j)
                {
                    for (; cursor < vert_index_arr[j]; ++cursor)
                        cout << " ";
                    cout << vert_conj_char;
                    cursor++;
                }
                cout <<  "\n";
            }
            cur_depth++;
        }
        for (i = 0; i < node_count; ++i)
            free(info_p_arr[i]); //释放空间
        return;
    }

    ~AVL(){}
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main(){

    AVL b;
    int c,x;

    do{
        /* cout<<"\n1.Display\n";
        cout<<"\n2.Insert\n";
        cout<<"\n3.Delete\n";
        cout<<"\n4.Batch Insert\n";
        cout<<"\n0.Exit\n";
        cout<<"\nYour choice: "; */

        cin>>c; 

        switch (c)
        {
        case 1:
            b.show_tree(b);
            break;
                  
        case 2:
            cout<<"\nPlease enter the number to be inserted\n ";
            cin>>x;
            b.root = b.insert(b.root,x);
            break;
        
        case 3:
            cout<<"\nPlease enter the number to be deleted\n ";
            cin>>x;
            b.root = b.deleteNode(b.root,x);
            break;

        case 4:
            cout<<"\nPlease enter the numbers to be inserted, separate with spaces\n ";
            while(cin >> x){
                if (x== -1) break;
                b.root = b.insert(b.root,x);
            }
            break;
            
        case 0:
            break;
        }
        

     } while(!cin.eof());
     
  
}