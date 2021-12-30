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
    int cal_balance_factor(struct node *n){
        if(n==NULL){
            return 0;
        }

        string node_type(get_node_type(n));

        if(node_type == "bchild"){
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

    // 右旋，插入在左子树的左孩子
    struct node * llrotation(struct node *n){
        struct node *tp;
        tp = n->left;
        n->left = tp->right;
        tp->right = n;

        return tp; 
    }

    // 左旋
    struct node * rrrotation(struct node *n){
        struct node *tp;
        tp = n->right;
        n->right = tp->left;
        tp->left = n;

        return tp; 
    }


    // 先右旋后左旋
    struct node * rlrotation(struct node *n){
        struct node *tp;
        n->right = llrotation(n->right);
        tp = rrrotation(n);
        return tp;
    }

    // 先左旋后右旋
    struct node * lrrotation(struct node *n){
        struct node *tp;
        n->left = rrrotation(n->left);
        tp = llrotation(n);
        return tp;
    }

    struct node* insert(struct node *r,int data){
        
        // 找到插入点后插入，初始化高度为1
        if(r==NULL){
            struct node *n;
            n = new struct node;
            n->data = data;
            r = n;
            r->left = r->right = NULL;
            r->height = 1; 
            return r;             
        }

        // 查找到插入点位置
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

        // 插入点在左子树的左孩子，右旋
        if(cal_balance_factor(r)==2 && cal_balance_factor(r->left)==1){
            r = llrotation(r);
            calheight(r);

        }
        // 左旋
        else if(cal_balance_factor(r)==-2 && cal_balance_factor(r->right)==-1){
            r = rrrotation(r);
            calheight(r);
        }
        // 先右旋再左旋
        else if(cal_balance_factor(r)==-2 && cal_balance_factor(r->right)==1){
            r = rlrotation(r);
            calheight(r);
        }
        // 先左旋再右旋
        else if(cal_balance_factor(r)==2 && cal_balance_factor(r->left)==-1){
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
                q = traverse_right(p->left);
                p->data = q->data;
                p->left=deleteNode(p->left,q->data);
            }
            else{
                q = traverse_left(p->right);
                p->data = q->data;
                p->right = deleteNode(p->right,q->data);
            }
        }

        // 删除后维护树平衡
        /* cout<<"p data: "<<p->data<<endl;
        cout<<"p factor: "<<cal_balance_factor(p)<<endl;
        cout<<"p left_factor: "<<cal_balance_factor(p->left)<<endl;
        cout<<"p right_factor: "<<cal_balance_factor(p->right)<<endl<<endl; */

        if(cal_balance_factor(p)==2 && cal_balance_factor(p->left)==1){ p = llrotation(p); calheight(p);}                  
        else if(cal_balance_factor(p)==2 && cal_balance_factor(p->left)==-1){ p = lrrotation(p); calheight(p);}
        else if(cal_balance_factor(p)==2 && cal_balance_factor(p->left)==0){ p = llrotation(p); calheight(p);}
        else if(cal_balance_factor(p)==-2 && cal_balance_factor(p->right)==-1){ p = rrrotation(p); calheight(p);}
        else if(cal_balance_factor(p)==-2 && cal_balance_factor(p->right)==1){ p = rlrotation(p); calheight(p);}
        else if(cal_balance_factor(p)==-2 && cal_balance_factor(p->right)==0){ p = llrotation(p); calheight(p);}

        calheight(p->left);
        calheight(p->right);
        calheight(p);

        
        return p;
    }

    // 遍历右子节点
    struct node* traverse_right(struct node* p){
        while(p->right!=NULL)
            p = p->right;
        return p;    
    }

    // 遍历左子节点
    struct node* traverse_left(struct node* p){
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
    void show_tree()
    {

        if (this->root == NULL)
            return;

        int _MAX_STR_LEN = 1280;    //限制str数量
        int _MAX_NODE_NUM = 10240;   //限制结点最大个数
        string horiz_conj_char ="─";   //横线
        string vert_conj_char = "│";    //做树状结构的竖线

        //实现思路：
        // 利用辅助的结构体来存储信息

        struct Node_print_info
        {
            node* node_pointer; //字典保存
            int str_len; //value长度，默认左右长度为2
            int depth;  //本节点的所在深度，因为绘制中只能层次遍历
            int left_index;  //横坐标
        } ;

        //初始化辅助信息
        node* avl_queue[_MAX_NODE_NUM];
        int dp_queue[_MAX_NODE_NUM]; //记录深度的队列
        Node_print_info* print_info_list[_MAX_NODE_NUM];
        int node_count = 0;
        int front = -1, rear = -1;
        node* p;
        Node_print_info* info_p;
        char max_str_list[_MAX_STR_LEN];

        //1. 利用队列来进行层次遍历 ,存储深度和长度
        avl_queue[++rear] = this->root;
        dp_queue[rear] = 1;
        while (front < rear)
        {
            p = avl_queue[++front];
            Node_print_info* info_p = new Node_print_info  ;
            info_p->node_pointer = p; //记录地址
            memset(max_str_list, 0, _MAX_STR_LEN);
            sprintf(max_str_list, "%d", p->data);   // 限制长度
            info_p->str_len = strlen(max_str_list) + 2; //假设两边长度是2个横线
            info_p->depth = dp_queue[front];
            print_info_list[node_count++] = info_p;

            if (p->left != NULL)
            {
                avl_queue[++rear] = p->left;
                dp_queue[rear] = dp_queue[front] + 1;
            }
            if (p->right != NULL)
            {
                avl_queue[++rear] = p->right;
                dp_queue[rear] = dp_queue[front] + 1;
            }
        }

        //2.进行中序遍历，用于计算每个节点的横坐标 
        p = this->root;
        int top = -1, i;
        int cum_x_location = 0; //累计长度
        do
        {
            while (p != NULL)
            {
                avl_queue[++top] = p;
                p = p->left;
            }
            p = avl_queue[top--];
            for (i = 0; i < node_count; ++i)
            { 
                if (print_info_list[i]->node_pointer == p)
                { 
                    print_info_list[i]->left_index = cum_x_location;
                    cum_x_location += print_info_list[i]->str_len - 1; //更新
                }
            }
            p = p->right;
        } while (!(p == NULL && top == -1));
        //cout<<cum_x_location<<endl;

        //打印
        int horiz_left_start, horiz_right_end, cursor, j, k, cur_depth = 1, end_flag = 0;
        int vert_xindex[_MAX_NODE_NUM]; //用于存储竖线信息
        i = 0;
        while (i < node_count)
        {
            k = -1;
            cursor = 0;
            while (print_info_list[i]->depth == cur_depth)
            {

                //left
                p = print_info_list[i]->node_pointer;  //节点指针
                if (p->left != NULL)
                { 
                    for (j = 0; j < node_count; ++j)
                    { //查找  ，复杂度高
                        if (print_info_list[j]->node_pointer == p->left)
                        {
                            horiz_left_start = print_info_list[j]->left_index + print_info_list[j]->str_len / 2;
                            vert_xindex[++k] = horiz_left_start;
                            break;
                        }
                    }
                    //空格
                    for (; cursor < horiz_left_start; ++cursor)
                    { 
                        cout <<  " ";
                    }

                    cout <<"┌";
                    cursor++;

                    for (; cursor < print_info_list[i]->left_index; ++cursor)
                    {
                        cout << horiz_conj_char;
                    }
                }
                else for (; cursor < print_info_list[i]->left_index; ++cursor) cout << " ";

                //打印元素
                if(p->left !=nullptr) cout << horiz_conj_char;
                else cout << " "; //没孩子就空格
                cout << p->data;

                if(p->right !=nullptr) cout << horiz_conj_char;
                else cout << " ";

                cursor += print_info_list[i]->str_len;

                //right，相同逻辑
                if (p->right != NULL)
                {
                    for (j = 0; j < node_count; ++j)
                    {
                        if (print_info_list[j]->node_pointer == p->right)
                        {
                            horiz_right_end = print_info_list[j]->left_index + print_info_list[j]->str_len / 2;
                            vert_xindex[++k] = horiz_right_end;
                            break;
                        }
                    }
                    for (; cursor < horiz_right_end; ++cursor)cout << horiz_conj_char;
                    cout <<"┐";
                    ++cursor;
                } 
                if (++i >= node_count)
                {
                    end_flag = 1; 
                    break;
                }
            }
            cout << "\n";

            //下一行，打印竖线
            if (!end_flag)
            {
                cursor = 0;
                for (j = 0; j <= k; ++j)
                {
                    for (; cursor < vert_xindex[j]; ++cursor)  cout << " "; //已经存储好竖线位置
                    cout << vert_conj_char;
                    cursor++;
                }
                cout <<  "\n";
            }
            cur_depth++;
        }
        for (i = 0; i < node_count; ++i)
            delete (print_info_list[i]); 
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
            cout<<"\n";
            b.show_tree();
            break;
                  
        case 2:
            cout<<"\nPlease enter the number to be inserted\n ";
            cin>>x;
            cout<<x<<endl;
            b.root = b.insert(b.root,x);
            break;
        
        case 3:
            cout<<"\nPlease enter the number to be deleted\n ";
            cin>>x;
            cout<<x<<endl;
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
        // 1

     } while(!cin.eof());
  
}