#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <time.h>

using std::cout;
using std::cin;
using std::endl;

template<class T>
class Overflow
{
public:
    Overflow()
    {
        cout<<"run out"<<endl;
    }
};

template<class T>
class Dump_core
{
public:
    Dump_core()
    {
        cout<<"null pointer"<<endl;
    }
};

template<class T>
class Bad_right_rot
{
public:
    Bad_right_rot()
    {
        cout<<"can't rotate right!"<<endl;
    }
};

template<class T>
class Bad_left_rot
{
public:
    Bad_left_rot()
    {
        cout<<"can't left right!"<<endl;
    }
};

template<class T>
class less
{
public:
    constexpr bool operator () (const T& x,const T& y) const
    {
        return x<y;
    }
};

template<class T, class cmp=less<T>>
class RB_tree
{
public:

    /* тут прототипы всех функций
    bool is_in(T x);
    T get_min();
    T get_max();
    bool insert(T x);
    bool erase(T x);

    avl_tree();
    node<T> *create_node(T x);
    node<T> *min(node<T> *x); нахождение минимума в поддереве
    node<T> *max(node<T> *x); нахождение максимума в поддереве
    int find_h(node<T> *x); нахождение высоты дерева x при условии, что высоты поддеревьев посчитаны правильно
    void fix_h(node<T> *x); исправляет высоты всех родителей x до корня
    node<T> *find_place(const T& x);
    void left_rotation(node<T> *x);
    void right_rotation(node<T> *x);
    void balance(node<T> *x); балансирует дерево x если все поддеревья сбалансированы и разность высот у x не больше 2
    void print(node<T> *x);
    ~avl_tree();
    */

    class node
    {
    public:
        T key;
        bool col=0; //0-черный, 1-красный
        node *left, *right, *parent;
        node(T key,bool col=0,node *letf=nullptr, node *right=nullptr, node *parent=nullptr):key(key),col(col),left(left),right(right),parent(parent) {};
    };

    class iterator
    {
    public:
        node *ptr;
        bool operator !=(const iterator& other) const
        {
            return ptr!=other.ptr;
        }
        bool operator ==(const iterator& other) const
        {
            return ptr==other.ptr;
        }
        node *min(node *x) const
        {
            if (x == nullptr) return nullptr;
            while (x->left != nullptr)
            {
                x = x->left;
            }
            return x;
        }
        node *max(node *x) const
        {
            if (x == nullptr) return nullptr;
            while (x->right != nullptr)
            {
                x = x->right;
            }
            return x;
        }
        iterator(node *ptr=nullptr):ptr(ptr) {};
        node& operator *()
        {
            if (ptr==nullptr) throw Dump_core<T>();
            return *ptr;
        }
        iterator& operator ++()
        {
            if (ptr==nullptr) throw Overflow<T>();
            node *tmp=ptr, *next=ptr;
            if (tmp->right==nullptr)
            {
                next=ptr->parent;
                while (next!=nullptr && next->right==tmp)
                {
                    tmp=tmp->parent;
                    next=next->parent;
                }
                if (next==nullptr)
                {
                    ptr=nullptr;
                    return *this;
                }
                ptr=next;
                return *this;
            }
            ptr=min(tmp->right);
            return *this;
        }
        iterator& operator --()
        {
            if (ptr==nullptr) throw Overflow<T>();
            node *tmp=ptr, *next=ptr;
            if (tmp->left==nullptr)
            {
                next=ptr->parent;
                while (next!=nullptr && next->left==tmp)
                {
                    tmp=tmp->parent;
                    next=next->parent;
                }
                if (next==nullptr)
                {
                    ptr=nullptr;
                    return *this;
                }
                ptr=next;
                return *this;
            }
            ptr=max(ptr->left);
            return *this;
        }
        iterator operator ++(int)
        {
            iterator res;
            res.ptr=ptr;
            if (ptr==nullptr) throw Overflow<T>();
            node *tmp=ptr, *next=ptr;
            if (tmp->right==nullptr)
            {
                next=ptr->parent;
                while (next!=nullptr && next->right==tmp)
                {
                    tmp=tmp->parent;
                    next=next->parent;
                }
                if (next==nullptr)
                {
                    ptr=nullptr;
                    return res;
                }
                ptr=next;
                return res;
            }
            ptr=min(tmp->right);
            return res;
        }
        iterator operator --(int)
        {
            iterator res;
            res.ptr=ptr;
            node *tmp=ptr, *next=ptr;
            if (tmp->left==nullptr)
            {
                next=ptr->parent;
                while (next!=nullptr && next->left==tmp)
                {
                    tmp=tmp->parent;
                    next=next->parent;
                }
                if (next==nullptr)
                {
                    ptr=nullptr;
                    return res;
                }
                ptr=next;
                return res;
            }
            ptr=max(tmp->left);
            return res;
        }
        iterator& operator +(int k)
        {
            for (int i=0; i<k; i++, (*this)++)
                ;
            return *this;
        }
        iterator& operator -(int k)
        {
            for (int i=0; i<k; i++, (*this)--)
                ;
            return *this;
        }

    };

    node *root;
    int size;
    cmp fun;
    RB_tree(cmp fun=cmp()):fun(fun)
    {
        root = nullptr;
        size = 0;
    }
    iterator begin() const
    {
        iterator res;
        res.ptr=min(root);
        return res;
    }
    iterator rbegin() const
    {
        iterator res;
        res.ptr=max(root);
        return res;
    }
    iterator end() const
    {
        iterator res;
        res.ptr=nullptr;
        return res;
    }
    iterator rend() const
    {
        iterator res;
        res.ptr=nullptr;
        return res;
    }
    node *min(node *x) const
    {
        if (x == nullptr) return nullptr;
        while (x->left != nullptr)
        {
            x = x->left;
        }
        return x;
    }
    T get_min() const
    {
        if (root == nullptr) throw Dump_core<T>();
        return min(root)->key;
    }
    node *max(node *x) const
    {
        if (x == nullptr) return nullptr;
        while (x->right != nullptr)
        {
            x = x->right;
        }
        return x;
    }
    T get_max() const
    {
        if (root == nullptr) throw Dump_core<T>();
        return max(root)->key;
    }
    iterator find(const T& x) const
    {
        node* ptr = root;
        if (ptr == nullptr) return iterator();
        while (ptr != nullptr && x != ptr->key)
        {
            if (fun(x, ptr->key))
                ptr = ptr->left;
            else ptr = ptr->right;
        }
        iterator res;
        res.ptr=ptr;
        return res;
    }
    node* find_place(const T& x)
    {
        node* ptr = root;
        while (ptr != nullptr && x != ptr->key)
        {
            if (fun(x, ptr->key))
                if (ptr->left == nullptr) return ptr;
                else ptr = ptr->left;
            else if (ptr->right == nullptr) return ptr;
            else ptr = ptr->right;
        }
        return ptr;

    }
    void left_rotation(node* a)// берем поддерево с корнем а и ненулевым левым ребенком b, потом делаем корнем b, у него правый ребенок- а
    {
        if (a->left == nullptr)
        {
            throw Bad_left_rot<T>();
        }
        node* par = a->parent, *b = a->left, *A = b->left, *B = b->right; //A<b<B<a<C тут и везде маленькие буквы- вершины, большие- поддеревья в данных вершинах
        b->right = a;
        a->left = B;
        if (par != nullptr) if (fun(par->key, a->key))
                par->right = b;
            else par->left = b;
        if (B != nullptr) B->parent = a;
        a->parent = b;
        b->parent = par;
        if (a == root) root = b;
    }
    void right_rotation(node* a)
    {
        if (a->right == nullptr)
        {
            throw Bad_right_rot<T>();
        }
        node* par = a->parent, *b = a->right, *A = a->left, *B = b->left;
        a->right = B;
        if (B != nullptr) B->parent = a;
        b->left = a;
        a->parent = b;
        if (par != nullptr) if (fun(a->key, par->key))
            {
                par->left = b;
            }
            else
            {
                par->right = b;
            }
        b->parent = par;
        if (a == root) root = b;
    }

    bool balance(node* x)
    {
        cout<<endl<<endl;
        if (x==nullptr)
        {
            cout<<"troubles with balance"<<endl;
            throw Dump_core<T>();
        }
        if (x->parent==nullptr)
        {
            x->col=0;
            return true;
        }
        if ( x->parent->col==0) return true;
        if (x->parent->parent==nullptr)
        {
            x->parent->col=0;
            return true;
        }
        //дед есть
        bool pl=fun(x->key, x->parent->key), dl=fun(x->parent->key, x->parent->parent->key);
        node *par=x->parent, *ded=par->parent, *uncle=(dl)?ded->right:ded->left;
        if (pl!=dl)  //par!=nullptr
        {
            if (pl)
            {
                left_rotation(par);
                return balance(par);
            }
            right_rotation(par);
            return balance(par);
        }
        if (uncle==nullptr||uncle->col==0)
        {
            if (pl)
            {
                left_rotation(ded);
                ded->col=1;
                par->col=0;
                return true;
            }
            right_rotation(ded);
            ded->col=1;
            par->col=0;
            return true;
        }
        //теперь дядя-красный
        if (ded->parent==nullptr)
        {
            ded->col=0;
            par->col=0;
            uncle->col=0;
            return true;
        }
        ded->col=1;
        par->col=0;
        uncle->col=0;
        return false; //возвращает false, только, если дед-красный
    }




    bool insert(T x)//добавление как в обычное дерево поиска, только при этом можем нарушить сбалансированность дерева, поэтому исправляем
    {
        if (root == nullptr)
        {
            root = new node(x);
            size++;
            return true;
        }
        node *n = new node(x), *place = find_place(x);
        if (place->key==x) return false;
        n->col=1;
        if (fun(place->key, x))
        {
            place->right = n;
            n->parent = place;
            while (!balance(n))
            {
                n=n->parent->parent;
            }
            size++;
            return true;
        }
        place->left = n;
        n->parent = place;
        while (!balance(n))
        {
            n=n->parent->parent;
        }
        size++;
        return true;
    }
    void Shift_up(node *x) //подьем черной метки вверх
    {
        if (x==root)
        {
            return;
        }
        bool l=(x->parent->left==x);
        node *par=x->parent, *bro=(l)?par->right:par->left, *c=(l)? bro->right: bro->left;
        if (par->col==1 && d->col==0)
        {
            if (l) right_rotation(par);
            else left_rotation(par);
            return;
        }
        if (par->col==1 && d->col==1)
        {
            if (l)
            {
                left_rotation(bro);
                right_rotation(par);
            }
            else
            {
                right_rotation(bro);
                left_rotation(par);
            }
            d->col=0;
            bro->col=1;
            if (c->col==1)
                balance(c);
            return;
        }
        //теперь отец-черный
        if (bro->col==1)
        {
            if (l)
            {
                right_rotation(par);
                right_rotation(par);
            }
            else
            {
                left_rotation(par);
                left_rotation(par);
            }
            bro->col=0;
            par->col=1;
            if (l)
            {
                if (d->left->col==1) balance(d->left);
            }
            else if (d->right->col==1) balance(d->right);
            return;
        }
        //теперь отец и брат черные
        if (d->col==0)
        {
            if (l) right_rotation(par);
            else left_rotation(par);
            par->col=1;
            Shift_up(bro);
            return;
        }


    }
    void black_leave_matter(node *x)  //удаление черного листа
    {
        if (x->parent==nullptr)
        {
            root=nullptr;
            delete x;
            size--;
            return;
        }
        node *par=x->parent, *bro;
        bool l=(par->left==x);
        if (l) bro=par->right;
        else bro=par->left;
        //bro!=nullptr из-за сбалансированности
        node *c=(l)?bro->right:bro->left, *d=(l)?bro->left:bro->right;
        if (bro->col==1 && c!=nullptr)
        {
            if (l)
            {
                right_rotation(par);
                bro->col=0;
                if (d!=nullptr) d->col=1;
                c->col=0;
                delete x;
                par->left=nullptr;
                return;
            }
            left_rotation(par);
            bro->col=0;
            if (d!=nullptr) d->col=1;
            c->col=0;
            delete x;
            par->right=nullptr;
            return;
        }
        // d-черный, с-пустой- невозможен из-за сбалансированности

        //теперь брат-черный
        if (par->col==1 && c!=nullptr)
        {
            if (l)
            {
                right_rotation(par);
                par->col=0;
                bro->col=1;
                c->col=0;
                par->left=nullptr;
                delete x;
                return;
            }
            left_rotation(par);
            par->col=0;
            bro->col=1;
            c->col=0;
            if (d!=nullptr) d->col=1;
            par->right=nullptr;
            delete x;
            return;
        }
        if (par->col==1 && c==nullptr)
        {
            if (d==nullptr)
            {
                if (l)
                {
                    right_rotation(par);
                    par->left=nullptr;
                    delete x;
                    return;
                }
                left_rotation(par);
                par->right=nullptr;
                delete x;
                return;
            }
            if (l)
            {
                left_rotation(bro);
                right_rotation(par);
                d->col=0;
                par->col=1;
                bro->col=1;
                par->left=nullptr;
                delete x;
                return;
            }
            right_rotation(bro);
            left_rotation(par);
            d->col=0;
            par->col=1;
            bro->col=1;
            par->right=nullptr;
            delete x;
            return;
        }
        //теперь отец и брат черные
        if (c!=nullptr)
        {
            if (l)
            {
                right_rotation(par);
                c->col=0;
                par->left=nullptr;
                delete x;
                return;
            }
            left_rotation(par);
            c->col=0;
            par->right=nullptr;
            delete x;
            return;
        }
        if (d!=nullptr)
        {
            if (l)
            {
                left_rotation(bro);
                right_rotation(par);
                d->col=0;
                par->left=nullptr;
                delete x;
                return;
            }
            right_rotation(bro);
            left_rotation(par);
            d->col=0;
            par->right=nullptr;
            delete x;
            return;
        }
        //теперь отец и брат черные, племянники- пустые
        if (l) par->left=nullptr;
        else par->right=nullptr;
        delete x;
        bro->col=1;
        Shift_up(par);
        return;
    }

    bool erase(const T x)
    {
        iterator t=find(x);
        if (t==end())
        {
            return false;
        }
        bool is_root=(t.ptr->parent==nullptr);
        if (t.ptr->left==nullptr && t.ptr->right==nullptr)
        {
            if (t.ptr->col==1) //красный лист не корень
            {
                if (t.ptr->parent->left==t.ptr)
                    t.ptr->parent->left=nullptr;
                else t.ptr->parent->right=nullptr;
                delete t.ptr;
                size--;
                return true;
            }
            black_leave_matter(t.ptr);
            size--;
            return true;
        }
        if ((t.ptr->left==nullptr) != (t.ptr->right==nullptr))
        {
            if (is_root)
            {
                if (t.ptr->left==nullptr)
                {
                    t.ptr->right->col=0;
                    t.ptr->right->parent=nullptr;
                    root=t.ptr->right;
                    delete t.ptr;
                    size--;
                    return true;
                }
                t.ptr->left->col=0;
                t.ptr->left->parent=nullptr;
                root=t.ptr->left;
                delete t.ptr;
                size--;
                return true;
            }
            if (t.ptr->left==nullptr)
            {
                node *a=t.ptr->right;
                a->col=0;
                if (t.ptr->parent->left==t.ptr)
                    t.ptr->parent->left=a;
                else
                    t.ptr->parent->right=a;
                size--;
                return true;
            }
            node *a=t.ptr->left;
            a->col=0;
            if (t.ptr->parent->left==t.ptr)
                t.ptr->parent->left=a;
            else
                t.ptr->parent->right=a;
            size--;
            return true;
        }
        //теперь есть оба ребенка, корень на месте

        node *next=min(t.ptr->right); //next==t.pr???

        if (next->col==1)
        {
            t.ptr->key=next->key;
            if (next->parent==t.ptr)
            {
                t.ptr->right=nullptr;
                delete next;
                size--;
                return true;
            }
            next->parent->left=nullptr;
            delete next;
            size--;
            return true;
        }
        if (next->right!=nullptr)
        {
            t.ptr->key=next->key;
            if (next->parent==t.ptr)
            {
                t.ptr->right=next->right;
                next->right->col=0;
                next->right->parent=t.ptr;
                delete next;
                size--;
                return true;
            }
            next->parent->left=next->right;
            next->right->parent=next->parent;
            delete next;
            size--;
            return true;
        }
        //теперь next- черный лист
        t.ptr->key=next->key;
        size--;

        cout<<"tututututututuutut"<<endl;


        black_leave_matter(next);
        return true;
    }

    void print(node *x, int k)
    {
        if(x == nullptr) return;
        print(x->right,k+1);
        for(int i=0; i<k; i++)
            cout<<"    ";
        cout<<x->key<<((x->col==0)?'b':'r')<<endl;
        print(x->left, k+1);
    }
    void print()
    {
        print(root,0);
    }
    void clear(node *x)
    {
        if (x==nullptr) return;
        clear(x->left);
        clear(x->right);
        delete x;
    }
    ~RB_tree()
    {
        clear(root);
    }
};




int main()
{
    srand(time(NULL));
    RB_tree<int> A;
    for (int i=0; i<7; i++)
        A.insert(i);
    A.print();
    cout<<endl<<endl<<endl;
    /*A.insert(3);
    A.print();
    cout<<endl<<endl<<endl;
    A.erase(2);
    A.print();
    cout<<endl<<endl<<endl;
    A.insert(4);
    A.print();
    cout<<endl<<endl<<endl;*/
    A.erase(1);
    A.print();
    cout<<endl<<endl<<endl;
    /*A.insert(5);
    A.print();
    cout<<endl<<endl<<endl;
    A.insert(6);
    A.print();
    cout<<endl<<endl<<endl;
    A.erase(4);
    A.print();*/
    return 0;
}