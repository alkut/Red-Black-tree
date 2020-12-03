#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <set>
#include <ctime>

using std::cout;
using std::cin;
using std::endl;

int H = 0;

int Leak=0;
int alloc=0;

template<class T, class cmp>
class Overflow
{
public:
    Overflow()
    {
        cout<<"run out"<<endl;
    }
};

template<class T, class cmp>
class Dump_core
{
public:
    Dump_core()
    {
        cout<<"null pointer"<<endl;
    }
};

template<class T,class cmp>
class Bad_right_rot
{
public:
    Bad_right_rot()
    {
        cout<<"can't rotate right!"<<endl;
    }
};

template<class T,class cmp>
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
    bool operator () (const T& x,const T& y) const
    {
        return x<y;
    }
};

template<class T, class cmp=less<T> >
class RB_tree
{
public:
    
    //fields:
private:
    class node
    {
    public:
        T key;
        bool col=0; //0-?a?iue, 1-e?aniue
        node *left = nullptr, *right = nullptr, *parent = nullptr;
        int num=1;
        node(T key=T(),bool col=0,node *left=nullptr, node *right=nullptr, node *parent=nullptr):key(key),col(col), left(left),right(right),parent(parent) {};
    };
    node *root;
    int sizee=0;
    cmp fun;
public:
    
    /*methods:
    public:
    RB_tree(cmp);
    int size() const;
    bool empty() const;
    iterator begin() const;
    iterator end() const;
    iterator rbegin() const;
    iterator rend() const;
    T get_min() const;
    T get_max() const;
    iterator find(const T&) const;
    bool insert(T);
    bool check();
    bool erase(const T&);
    void print();
    void clear();
    bool operator <(const RB_tree<T,cmp>& other);
    bool operator <=(const RB_tree<T,cmp>& other);
    bool operator >(const RB_tree<T,cmp>& other);
    bool operator >=(const RB_tree<T,cmp>& other);
    bool operator ==(const RB_tree<T,cmp>& other);
    bool operator !=(const RB_tree<T,cmp>& other);
    int how_many_less(const T&);
    int how_many_more(const T&);
    iterator first_more(const T&);
    iterator last_less(const T&);
    int how_many_between(const T&, const T&);
    T operator [](int);
    RB_tree(const RB_tree<T,cmp>&);
    RB_tree(RB_tree<T,cmp>&&);
    RB_tree<T,cmp>& operator = (const RB_tree<T,cmp>&);
    RB_tree<T,cmp>& operator = (RB_tree<T,cmp>&&);
    RB_tree<T,cmp>* interseption(const RB_tree<T,cmp> &, const RB_tree<T,cmp> &);
    RB_tree<T,cmp>* unio(const RB_tree<T,cmp> &, const RB_tree<T,cmp> &);
    void merge(const RB_tree<T,cmp> &);
    void cut(const T&, RB_tree<T,cmp>& , RB_tree<T,cmp>& );
    ~RB_tree();
    
    private:
                                 
                                 node* find_place(const T&) const;
                                 node *min(node*) const;
                                node *max(node*) const;
                                void left_rotation(node*);
                                void right_rotation(node*);
                                void balance(node*);
                                void inscrease_num(node*);
                                void decrease_num(node*);
                                void Shift_up(node*);
                                bool is_balanced();
                                void start();
                                void black_leaves_matter(node*);
                                void print(node*,int);
                                void clearr(node*);
                                int how_many_less(node*);
                                int how_many_more(node*);
                                void copy(node *p1, node *p2);
                                int find_black_height();
                                swap(RB_tree<T,cmp>&);
                                clever_merge(RB_tree<T,cmp>&,const T&);
                                eazy_merge(node*,const T&);
    */
    class iterator
    {
    public:
        node *ptr;
        /*
        bool operator !=(const iterator&) const;
        bool operator ==(const iterator&) const;
        node *min(node*);
        node *max(node*);
        iterator(node*);
        iterator& operator ++();
        iterator operator ++(int);
        iterator& operator --();
        iterator operator --(int);
        iterator& operator +(int);
        iterator& operator -(int);
        */
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
        T operator *()
        {
            if (ptr==nullptr) throw Dump_core<T,cmp>();
            return ptr->key;
        }
        iterator& operator ++()
        {
            if (ptr==nullptr) throw Overflow<T,cmp>();
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
            if (ptr==nullptr) throw Overflow<T,cmp>();
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
            if (ptr==nullptr) throw Overflow<T,cmp>();
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
    RB_tree(cmp fun=cmp()):fun(fun)
    {
        root = nullptr;
        sizee = 0;
    }
    int size() const
    {
        return sizee;
    }
    bool empty() const
    {
        return root==nullptr;
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
    T get_min() const
    {
        if (root == nullptr) throw Dump_core<T,cmp>();
        return min(root)->key;
    }
    T get_max() const
    {
        if (root == nullptr) throw Dump_core<T,cmp>();
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
    bool insert(T x)//aiaaaeaiea eae a iau?iia aa?aai iienea, oieuei i?e yoii ii?ai ia?ooeou naaeaine?iaaiiinou aa?aaa, iiyoiio eni?aaeyai
    {
        if (root == nullptr)
        {
            root = new node(x);
            alloc++;
            Leak++;
            sizee++;
            root->num=1;
            root -> left = nullptr;
            root -> right = nullptr;
            return true;
        }
        node  *place = find_place(x);
        if (place->key==x) return false;
        node *n = new node(x);
        Leak++;
        alloc++;
        n->col=1;
        n->num=1;
        if (fun(place->key, x))
        {
            place->right = n;
            n->parent = place;
            increase_num(place);
            balance(n);
            sizee++;
            return true;
        }
        place->left = n;
        n->parent = place;
        increase_num(place);
        balance(n);
        sizee++;
        return true;
    }
    bool check()
    {
        if (root==nullptr) return true;
        start();
        return is_balanced(root, 0);
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
            if (t.ptr->col==1) //e?aniue eeno ia ei?aiu
            {
                if (t.ptr->parent->left==t.ptr)
                    t.ptr->parent->left=nullptr;
                else t.ptr->parent->right=nullptr;
                decrease_num(t.ptr->parent);
                delete t.ptr;
                Leak--;
                sizee--;
                return true;
            }
            black_leaves_matter(t.ptr);
            sizee--;
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
                    Leak--;
                    sizee--;
                    return true;
                }
                t.ptr->left->col=0;
                t.ptr->left->parent=nullptr;
                root=t.ptr->left;
                delete t.ptr;
                Leak--;
                sizee--;
                return true;
            }
            if (t.ptr->left==nullptr)
            {
                node *a=t.ptr->right;
                a -> parent = t.ptr ->parent;
                a->col=0;
                if (t.ptr->parent->left==t.ptr)
                    t.ptr->parent->left=a;
                else
                    t.ptr->parent->right=a;
                sizee--;
                decrease_num(a->parent);
                delete t.ptr;
                Leak--;
                return true;
            }
            node *a=t.ptr->left;
            a -> parent = t.ptr -> parent;
            a->col=0;
            if (t.ptr->parent->left==t.ptr)
                t.ptr->parent->left=a;
            else
                t.ptr->parent->right=a;
            sizee--;
            decrease_num(a->parent);
            delete t.ptr;
            Leak--;
            return true;
        }
        //oaia?u anou iaa ?aaaiea, ei?aiu ia ianoa

        node *next=min(t.ptr->right); //next==t.pr???

        if (next->col==1)
        {
            t.ptr->key=next->key;
            if (next->parent==t.ptr)
            {
                t.ptr->right=nullptr;
                decrease_num(t.ptr);
                delete next;
                Leak--;
                sizee--;
                return true;
            }
            next->parent->left=nullptr;
            decrease_num(next->parent);
            delete next;
            Leak--;
            sizee--;
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
                decrease_num(t.ptr);
                delete next;
                Leak--;
                sizee--;
                return true;
            }
            next->parent->left=next->right;
            next->right->parent=next->parent;
            next->right->col=0;
            decrease_num(next->parent);
            delete next;
            Leak--;
            sizee--;
            return true;
        }
        //oaia?u next- ?a?iue eeno
        t.ptr->key=next->key;
        sizee--;
        black_leaves_matter(next);
        return true;
    }
    void print()
    {
        print(root,0);
    }
    bool operator <(const RB_tree<T,cmp>& other)
    {
        auto it1=begin();
        auto it2=other.begin();
        for (; it1!=end() && it2!=other.end(); ++it1, ++it2)
        {
            if (fun(*it1, *it2)) return true;
            if (fun(*it2, *it1)) return false;
        }
        if (it1==end() && it2!=other.end()) return true;
        return false;
    }
    bool operator <=(const RB_tree<T,cmp>& other)
    {
        auto it1=begin();
        auto it2=other.begin();
        for (; it1!=end() && it2!=other.end(); ++it1, ++it2)
        {
            if (fun(*it1, *it2)) return true;
            if (fun(*it2, *it1)) return false;
        }
        if (it1==end()) return true;
        return false;
    }
    bool operator >(const RB_tree<T,cmp>& other)
    {
        auto it1=begin();
        auto it2=other.begin();
        for (; it1!=end() && it2!=other.end(); ++it1, ++it2)
        {
            if (fun(*it1, *it2)) return false;
            if (fun(*it2, *it1)) return true;
        }
        if (it2==other.end() && it1!=end()) return true;
        return false;
    }
    bool operator >=(const RB_tree<T,cmp>& other)
    {
        auto it1=begin();
        auto it2=other.begin();
        for (; it1!=end() && it2!=other.end(); ++it1, ++it2)
        {
            if (fun(*it1, *it2)) return false;
            if (fun(*it2, *it1)) return true;
        }
        if (it2==end()) return true;
        return false;
    }
    bool operator ==(const RB_tree<T,cmp>& other)
    {
        auto it1=begin();
        auto it2=other.begin();
        for (; it1!=end() && it2!=other.end(); ++it1, ++it2)
        {
            if (*it1!=*it2) return false;
        }
        if (it1==end() && it2==other.end()) return true;
        return false;
    }
    bool operator !=(const RB_tree<T,cmp>& other)
    {
        return !(*this==other);
    }
    int how_many_less(const T& x)
    {
        int res=0;
        node *p=root;
        while (p!=nullptr)
        {
            if (fun(x,p->key))
            {
                p=p->left;
            }
            else
            {
                if (x==p->key)
                {
                    return res+((p->left!=nullptr)?p->left->num:0);
                }
                {
                    res+=((p->left!=nullptr)?p->left->num:0)+1;
                    p=p->right;
                }
            }
        }
        return res;
    }
    int how_many_more(const T& x)
    {
        if (find(x)!=end()) return size()-how_many_less(x)-1;
        return size()-how_many_less(x);
    }
    iterator first_more(const T& x)
    {
        iterator res;
        node *p=root;
        bool les=true;
        while (true)
        {
            if (fun(x,p->key))
            {
                if (p->left==nullptr)
                {
                    les=true;
                    break;
                }
                p=p->left;
            }
            else
            {
                if (x==p->key)
                {
                    res.ptr=p;
                    ++res;
                    return res;
                }
                {
                    if (p->right==nullptr)
                    {
                        les=false;
                        break;
                    }
                    p=p->right;
                    les=false;
                }
            }

        }
        if (les)
        {
            res.ptr=p;
            return res;
        }
        res.ptr=p;
        ++res;
        return res;
    }
    iterator last_less(const T& x)
    {
        iterator res;
        node *p=root;
        bool les=true;
        while (true)
        {
            if (fun(x,p->key))
            {
                if (p->left==nullptr)
                {
                    les=true;
                    break;
                }
                p=p->left;
            }
            else
            {
                if (x==p->key)
                {
                    res.ptr=p;
                    --res;
                    return res;
                }
                {
                    if (p->right==nullptr)
                    {
                        les=false;
                        break;
                    }
                    p=p->right;
                    les=false;
                }
            }

        }
        if (!les)
        {
            res.ptr=p;
            return res;
        }
        res.ptr=p;
        --res;
        return res;
    }
    int how_many_between(const T& x, const T& y) //тут нестрого
    {
        return sizee-how_many_less(x)-how_many_more(y);
    }
    iterator operator [](int n)
    {
        if (root==nullptr)
        {
            throw Dump_core<T,cmp>();
        }
        if (n < 0 || n > root->num-1)
        {
            throw Overflow<T,cmp>();
        }
        node *tmp=root;
        int l=(tmp->left==nullptr)?0:tmp->left->num;
        n++;
        while (n!=0)
        {
            if (n==l+1)
            {
                iterator res;
                res.ptr=tmp;
                return res;
            }
            if (n>l+1)
            {
                n-=l+1;
                tmp=tmp->right;
                l=(tmp->left==nullptr)?0:tmp->left->num;
            }
            else
            {
                tmp=tmp->left;
                l=(tmp->left==nullptr)?0:tmp->left->num;
            }
        }
        iterator res;
        res.ptr=tmp;
        return res;
    }
    RB_tree(const RB_tree<T,cmp>& other)
    {
        if (other.root==nullptr)
        {
            root=nullptr;
            return;
        }
        root=new node();
        Leak++;
        alloc++;
        copy(root, other.root);
        sizee=other.sizee;
        fun=other.fun;
    }
    RB_tree(RB_tree<T,cmp>&& other)
    {
        root=other.root;
        other.root=nullptr;
        sizee=other.sizee;
        fun=other.fun;
    }
    RB_tree<T,cmp>& operator =(const RB_tree<T,cmp>& other)
    {
        if (this==&other) return *this;
        clear();
        if (other.root==nullptr)
        {
            return *this;
        }
        root=new node();
        Leak++;
        alloc++;
        copy(root, other.root);
        sizee=other.sizee;
        fun=other.fun;
        return *this;
    }
    RB_tree<T,cmp>& operator=(RB_tree<T,cmp>&& other)
    {
        if (this==&other) return *this;
        clear();
        root=other.root;
        other.root=nullptr;
        return *this;
    }
    RB_tree<T,cmp>* interseption(const RB_tree<T,cmp> &x, const RB_tree<T,cmp> &y)
    {
        int s1=x.size(), s2=y.size();
        RB_tree<T,cmp> *res=new RB_tree<T,cmp>(x.fun);
        Leak++;
        alloc++;
        if (s1<s2)
        {
            auto it=x.begin();
            for (; it!=x.end(); ++it)
            {
                if (y.find(*it)!=y.end())
                    res->insert(*it);
            }
            return res;
        }
        auto it=y.begin();
        for (; it!=y.end(); ++it)
        {
            if (x.find(*it)!=x.end())
                res->insert(*it);
        }
        return res;

    }
    RB_tree<T,cmp>* unio(const RB_tree<T,cmp> &x, const RB_tree<T,cmp> &y)
    {
        int s1=x.size(), s2=y.size();
        RB_tree<T,cmp> *res=new RB_tree<T,cmp>(x.fun);
        Leak++;
        alloc++;
        if (s1<s2)
        {
            *res=y;
            res->merge(x);
            return res;
        }
        *res=x;
        res->merge(y);
        return res;
    }
    void merge(const RB_tree<T,cmp> &x)
    {
        
        for (auto it:x)
            insert(it);
    }
    void swap(RB_tree<T,cmp>& x)
    {
        if (this==&x) return;
        std::swap(root, x.root);
        std::swap(x.sizee,sizee);
    }
    void clever_merge(RB_tree<T,cmp> &x,const T& t)
    {
        if (x.root==nullptr)
        {
            insert(t);
            return;
        }
        if (root==nullptr)
        {
            x.insert(t);
            swap(x);
            return;
        }

        int b_h=find_black_height(), xb_h=x.find_black_height();
        node *tmp=root;
        if (b_h==xb_h)
        {
            eazy_merge(x.root,t);
            x.root=nullptr;
            return;
        }
        if (b_h<xb_h)
        {
            swap(x);
            std::swap(b_h,xb_h);
            tmp=root;
        }
        if (fun(root->key, x.root->key))
        {
            for (int i=1; i<b_h-xb_h+1; )
            {
                tmp=tmp->right;
                if (tmp->col==0) ++i;
            }
            node *par=tmp->parent;
            node *ro=new node(t);
            Leak++;
            alloc++;
            ro->parent=par;
            ro->col=1;
            ro->num=tmp->num+x.root->num+1;
            ro->left=tmp;
            tmp->parent=ro;
            ro->right=x.root;
            x.root->parent=ro;
            node *ttmp=par;
            while(ttmp!=nullptr)
            {
                ttmp->num+=x.root->num+1;
                ttmp=ttmp->parent;
            }
            par->right=ro;
            if (par->col==1) balance(ro);
            x.root=nullptr;
            return;
        }
        for (int i=0; i<b_h-xb_h; )
        {
            tmp=tmp->left;
            if (tmp->col==0) ++i;
        }
        node *par=tmp->parent;
        node *ro=new node(t);
        Leak++;
        alloc++;
        ro->parent=par;
        ro->col=1;
        ro->num=tmp->num+x.root->num+1;
        ro->right=tmp;
        tmp->parent=ro;
        ro->left=x.root;
        x.root->parent=ro;
        node *ttmp=par;
        while(ttmp!=nullptr)
        {
            ttmp->num+=x.root->num+1;
            ttmp=ttmp->parent;
        }
        par->left=ro;
        if (par->col==1) balance(ro);
        x.root=nullptr;
        return;
    }
    void clever_merge(RB_tree<T,cmp> &x)
    {
        if (x.root==nullptr)
        {
            return;
        }
        if (root==nullptr)
        {
            swap(x);
            return;
        }
        if (x.sizee==1)
        {
            insert(x.root->key);
            return;
        }
        if (sizee==1)
        {
            x.insert(root->key);
            swap(x);
            return;
        }
        if (fun(root->key,x.root->key))
        {
            T tmp=*rbegin();
            erase(tmp);
            clever_merge(x,tmp);
            return;
        }
        T tmp=*begin();
            erase(tmp);
            clever_merge(x,tmp);
            return;
    }
    void cut(const T& t, RB_tree<T,cmp>& less, RB_tree<T,cmp>& more)
    {
        less.clear();
        more.clear();
        node *ptr=root;
        while (ptr!=nullptr && ptr->key!=t)
        {
            if (fun(ptr->key, t))
            {
                if (ptr->left!=nullptr)
                {
                    RB_tree<T,cmp> tmp(fun);
                    tmp.root=ptr->left;
                    tmp.root->col=0;
                    tmp.root->parent=nullptr;
                    tmp.sizee=ptr->left->num;
                    less.clever_merge(tmp);
                    less.insert(ptr->key);
                }
                else 
                {
                    less.insert(ptr->key);
                }
                node *ppp=ptr;
                ptr=ptr->right;
                delete ppp;Leak--;
                continue;
            }
            if (ptr->right!=nullptr)
            {
                RB_tree<T,cmp> tmp(fun);
                tmp.root=ptr->right;
                tmp.root->parent=nullptr;
                tmp.root->col=0;
                tmp.sizee=ptr->right->num;
                more.clever_merge(tmp);
                more.insert(ptr->key);
            }
            else 
            {
                more.insert(ptr->key);
            }
            node *p=ptr;
            ptr=ptr->left;
            delete p;Leak--;
        }
        if (ptr!=nullptr)
        {
            if (ptr->left!=nullptr)
            {
                RB_tree<T,cmp> tmp(fun);
                tmp.root=ptr->left;
                tmp.root->parent=nullptr;
                tmp.root->col=0;
                tmp.sizee=ptr->left->num;
                less.clever_merge(tmp);
                less.insert(ptr->key);
            }
            else
            {
                less.insert(ptr->key);
            }
            if (ptr->right!=nullptr)
            {
                RB_tree<T,cmp> tmp(fun);
                tmp.root=ptr->right;
                tmp.root->parent=nullptr;
                tmp.root->col=0;
                tmp.sizee=ptr->right->num;
                more.clever_merge(tmp);
            }
            delete ptr;Leak--;
        }
        root=nullptr;
    }
    ~RB_tree()
    {
        clearr(root);
    }
    void clear()
    {
        clearr(root);
        root = nullptr;
        sizee=0;
    }
    
    private:
    node* find_place(const T& x) const
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
    void left_rotation(node* a)// aa?ai iiaaa?aai n ei?iai a e iaioeaaui eaaui ?aaaieii b, iioii aaeaai ei?iai b, o iaai i?aaue ?aaaiie- a
    {
        if (a->left == nullptr)
        {
            throw Bad_left_rot<T,cmp>();
        }
        node* par = a->parent, *b = a->left, *A = b->left, *B = b->right; //A<b<B<a<C ooo e aacaa iaeaiueea aoeau- aa?oeiu, aieuoea- iiaaa?aauy a aaiiuo aa?oeiao
        b->right = a;
        a->left = B;
        if (par != nullptr) if (par->right==a)
                par->right = b;
            else par->left = b;
        if (B != nullptr) B->parent = a;
        a->parent = b;
        b->parent = par;
        if (a == root) root = b;
        b->num+=((a->right!=nullptr)?a->right->num:0)+1;
        a->num-=((A!=nullptr)?A->num:0)+1;
    }
    void right_rotation(node* a)
    {
        if (a->right == nullptr)
        {
            throw Bad_right_rot<T,cmp>();
        }
        node* par = a->parent, *b = a->right, *A = a->left, *B = b->left;

        a->right = B;
        if (B != nullptr) B->parent = a;
        b->left = a;
        a->parent = b;
        if (par != nullptr) if (par->left==a)
            {
                par->left = b;
            }
            else
            {
                par->right = b;
            }
        b->parent = par;
        if (a == root) root = b;
        b->num+=((A!=nullptr)?A->num:0)+1;
        a->num-=((b->right!=nullptr)?b->right->num:0)+1;
    }
    void balance(node* x)
    {
        if (x==nullptr)
        {
            cout<<"troubles with balance"<<endl;
            throw Dump_core<T,cmp>();
        }
        if (x->parent==nullptr)
        {
            x->col=0;
            return;// true;
        }
        if (x->parent->col==0) return;
        if (x->parent->parent==nullptr)
        {
            x->parent->col=0;
            return;// true;
        }
        //aaa anou
        bool pl=(x==x->parent->left), dl=(x->parent==x->parent->parent->left);
        node *par=x->parent, *ded=par->parent, *uncle=(dl)?ded->right:ded->left;
        if (pl!=dl)  //par!=nullptr
        {
            if (pl)
            {
                left_rotation(par);
                //return
                balance(par);
                return;
            }
            right_rotation(par);
            //return
            balance(par);
            return;
        }
        if (uncle==nullptr||uncle->col==0)
        {
            if (pl)
            {
                left_rotation(ded);
                ded->col=1;
                par->col=0;
                return;// true;
            }
            right_rotation(ded);
            ded->col=1;
            par->col=0;
            return;// true;
        }
        //oaia?u ayay-e?aniue
        if (ded->parent==nullptr)
        {
            ded->col=0;
            par->col=0;
            uncle->col=0;
            return; // true;
        }
        ded->col=1;
        par->col=0;
        uncle->col=0;
        //return
        balance(ded); //aica?auaao false, oieuei, anee aaa-e?aniue
    }
    void increase_num(node *x)
    {
        while (x!=nullptr)
        {
            x->num++;
            x=x->parent;
        }
    }
    void decrease_num(node *x)
    {
        while (x!=nullptr)
        {
            x->num--;
            x=x->parent;
        }
    }
    void Shift_up(node *x) //iiauai ?a?iie iaoee aaa?o
    {
        if (x==root)
        {
            return;
        }
        bool l=(x->parent->left==x);
        node *par=x->parent, *bro=(l)?par->right:par->left, *c=(l)? bro->right: bro->left, *d = (l)? bro ->left: bro->right;
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
        //oaia?u ioao-?a?iue
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
                if (par->right->col==1)
                {
                    node * n = par->right;
                    balance(n);
                }
            }
            else if (par->left->col==1)
            {
                node * n = par->left;
                balance(n);
            }
            return;
        }
        //oaia?u ioao e a?ao ?a?iua
        if (d->col==0)
        {
            if (l) right_rotation(par);
            else left_rotation(par);
            par->col=1;
            Shift_up(bro);
            return;
        }
        d->col=0;
        if (l)
        {
            left_rotation(bro);
            right_rotation(par);
            return;
        }
        right_rotation(bro);
        left_rotation(par);
        return;

    }
    bool is_balanced(node * x, int h)
    {
        if (x == nullptr)
            return true;
        if (x == root)
        {
            return (x->col == 0) && is_balanced(x->left, 1) && is_balanced(x->right, 1);
        }

        if (x->col == 1 && x->parent->col == 1)
        {
            cout << "Two red" << endl;
            return false;
        }
        if (x->left != nullptr && fun(x->key, x->left->key))
        {
            cout << "Left is greater then parent" << endl;
            return false;
        }
        if (x->right != nullptr && fun(x->right->key, x->key))
        {
            cout << "Right is lower then parent" << endl;
            return false;
        }
        if (x->left == nullptr && x->right == nullptr)
        {
            if (h + ((x->col == 0) ? 1 : 0) != H)
            {
                cout << endl << x->key << " h " << ( h + ((x->col == 0) ? 1 : 0)) << " H " << H << endl;
                return false;
            }
            if (x->num!=1)
            {
                cout<<endl<<x->key<<" leaf has num "<<x->num <<endl;
                return false;
            }
        }
        if (x->num!=(1+((x->right!=nullptr)?x->right->num:0)+((x->left!=nullptr)?x->left->num:0)))
        {
            cout<<endl<<x->key<<" node has num "<<x->num <<" left has num "<<((x->left!=nullptr)?x->left->num:0)<<" right has num "<<((x->right!=nullptr)?x->right->num:0)<<endl;
            return false;
        }
        return (is_balanced(x->left, h + ((x->col == 0) ? 1 : 0))) && (is_balanced(x->right, h + ((x->col == 0) ? 1 : 0)));
    }
    void start()
    {
        H = 0;
        node * x = root;
        while((x -> left != nullptr) || (x -> right != nullptr))
        {
            if(x -> left != nullptr)
            {
                H = H + ((x -> col == 0)? 1 : 0);
                x = x -> left;
            }
            else
            {
                H = H + ((x -> col == 0)? 1 : 0);
                x = x -> right;
            }
        }
        H = H + ((x -> col == 0)? 1 : 0);
    }
    void black_leaves_matter(node *x)  //oaaeaiea ?a?iiai eenoa
    {
        if (x->parent==nullptr)
        {
            root=nullptr;
            delete x;
            Leak--;
            return;
        }
        node *par=x->parent, *bro;
        bool l=(par->left==x);
        if (l) bro=par->right;
        else bro=par->left;
        //bro!=nullptr ec-ca naaeaine?iaaiiinoe
        node *c=(l)?bro->right:bro->left, *d=(l)?bro->left:bro->right;
        if (bro->col==1 ) //c oae oi ?a?iue
        {
            bool left_d = (d->left != nullptr), right_d = (d->right != nullptr);
            if (left_d && right_d)
            {
                if(l)
                {
                    left_rotation(bro);
                    right_rotation(par);
                    bro-> left->col=0;
                    par->left = nullptr;
                    decrease_num(par);
                    delete x;
                    Leak--;
                    return;
                }
                right_rotation(bro);
                left_rotation(par);
                bro->right->col=0;
                par->left = nullptr;
                decrease_num(par);
                delete x;
                Leak--;
                return;

            }
            if (left_d)
            {
                if(l)
                {
                    right_rotation(par);
                    par -> left = nullptr;
                    decrease_num(par);
                    delete x;
                    Leak--;
                    bro -> col = 0;
                    c -> col = 0;
                    d -> col = 1;
                    node * n = d -> left;
                    balance(n);
                    return;
                }
                left_rotation(par);
                par -> right = nullptr;
                decrease_num(par);
                delete x;
                Leak--;
                bro -> col = 0;
                c -> col = 0;
                d -> col = 1;
                node * n = d -> left;
                balance(n);
                return;
            }
            if (right_d)
            {
                if(l)
                {
                    right_rotation(par);
                    par -> left = nullptr;
                    decrease_num(par);
                    delete x;
                    Leak--;
                    bro -> col = 0;
                    c -> col = 0;
                    d -> col = 1;
                    node * n = d -> right;
                    balance(n);
                    return;
                }
                left_rotation(par);
                par -> right = nullptr;
                decrease_num(par);
                delete x;
                Leak--;
                bro -> col = 0;
                c -> col = 0;
                d -> col = 1;
                node * n = d -> right;
                balance(n);
                return;
            }
            if (l)
            {
                right_rotation(par);
                bro->col=0;
                d->col=1;
                c->col=0;
                delete x;
                Leak--;
                par->left=nullptr;
                decrease_num(par);
                return;
            }
            left_rotation(par);
            bro->col=0;
            d->col=1;
            c->col=0;
            delete x;
            Leak--;
            par->right=nullptr;
            decrease_num(par);
            return;
        }//
        // d-?a?iue, n-ionoie- iaaicii?ai ec-ca naaeaine?iaaiiinoe

        //oaia?u a?ao-?a?iue
        if (par->col==1 && c!=nullptr)
        {
            if (l)
            {
                right_rotation(par);
                par->col=0;
                bro->col=1;
                c->col=0;
                par->left=nullptr;
                decrease_num(par);
                delete x;
                Leak--;
                return;
            }
            left_rotation(par);
            par->col=0;
            bro->col=1;
            c->col=0;
            if (d!=nullptr) d->col=1;
            par->right=nullptr;
            decrease_num(par);
            delete x;
            Leak--;
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
                    decrease_num(par);
                    delete x;
                    Leak--;
                    return;
                }
                left_rotation(par);
                par->right=nullptr;
                decrease_num(par);
                delete x;
                Leak--;
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
                decrease_num(par);
                delete x;
                Leak--;
                return;
            }
            right_rotation(bro);
            left_rotation(par);
            d->col=0;
            par->col=1;
            bro->col=1;
            par->right=nullptr;
            decrease_num(par);
            delete x;
            Leak--;
            return;
        }
        //oaia?u ioao e a?ao ?a?iua
        if (c!=nullptr)
        {
            if (l)
            {
                right_rotation(par);
                c->col=0;
                par->left=nullptr;
                decrease_num(par);
                delete x;
                Leak--;
                return;
            }
            left_rotation(par);
            c->col=0;
            par->right=nullptr;
            decrease_num(par);
            delete x;
            Leak--;
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
                decrease_num(par);
                delete x;
                Leak--;
                return;
            }
            right_rotation(bro);
            left_rotation(par);
            d->col=0;
            par->right=nullptr;
            decrease_num(par);
            delete x;
            Leak--;
            return;
        }
        //oaia?u ioao e a?ao ?a?iua, ieaiyiieee- ionoua
        if (l) par->left=nullptr;
        else par->right=nullptr;
        decrease_num(par);
        delete x;
        Leak--;
        bro->col=1;
        Shift_up(par);
        return;
    }
    void print(node *x, int k)
    {
        if(x == nullptr) return;
        print(x->right,k+1);
        for(int i=0; i<k; i++)
            cout<<"      ";
        cout<<x->key<<((x->col==0)?'b':'r')<<x->num<<endl << endl;
        print(x->left, k+1);
    }
    void clearr(node *x)
    {
        if (x==nullptr) return;
        clearr(x->left);
        clearr(x->right);
        delete x;
        Leak--;
    }
    int how_many_less(node *x)
    {
        if (x==nullptr) return 0;
        int res=(x->left!=nullptr)?x->left->num:0;
        node *tmp=x, *next=x->parent;
        while (next!=nullptr)
        {
            if (next->left==tmp)
            {
                next=next->parent;
                tmp=tmp->parent;
            }
            else
            {
                res+=(next->left!=nullptr)?next->left->num:0;
                res++;
                next=next->parent;
                tmp=tmp->parent;
            }
        }
        return res;
    }
    int how_many_more(node *x)
    {
        if (x==nullptr) return 0;
        int res=(x->right!=nullptr)?x->right->num:0;
        node *tmp=x, *next=x->parent;
        while (next!=nullptr)
        {
            if (next->right==tmp)
            {
                next=next->parent;
                tmp=tmp->parent;
            }
            else
            {
                res+=(next->right!=nullptr)?next->right->num:0;
                res++;
                next=next->parent;
                tmp=tmp->parent;
            }
        }
        return res;
    }
    void copy(node *p1, node *p2)
    {
        if (p2==nullptr) return;
        p1->key=p2->key;
        p1->num=p2->num;
        p1->col=p2->col;
        node *l=(p2->left!=nullptr)?new node:nullptr, *r=(p2->right!=nullptr)?new node:nullptr;
        if (l!=nullptr)
        {
            p1->left=l;
            l->parent=p1;
            copy(l,p2->left);
            Leak++;
            alloc++;
        }
        if (r!=nullptr)
        {
            p1->right=r;
            r->parent=p1;
            copy(r,p2->right);
            Leak++;
            alloc++;
        }
    }
    int find_black_height()
    {
        int b_h=0;
        node *tmp=root;
        while (tmp!=nullptr)
        {
            if (tmp->col==0) ++b_h;
            if (tmp->right!=nullptr)
                tmp=tmp->right;
            else tmp=tmp->left;
        }
        return b_h;
    }
    void eazy_merge(node *x,const T& t)
    {
        node *new_root=new node(t);
        Leak++;
        alloc++;
        if (fun(root->key, x->key))
        {
            new_root->left=root;
            root->parent=new_root;
            new_root->right=x;
            x->parent=new_root;
            new_root->col=0;
            new_root->parent=nullptr;
            new_root->num=root->num+x->num+1;
            root=new_root;
            sizee=root->num;
            return;
        }
        new_root->right=root;
        root->parent=new_root;
        new_root->left=x;
        x->parent=new_root;
        new_root->col=0;
        new_root->parent=nullptr;
        new_root->num=root->num+x->num+1;
        root=new_root;
        sizee=root->num;
        return;
    }
};

template<class T>
int replace_copy(std::vector<T>& a, int n)
{
    int p1=0, p2=1, k=a.size();
    for (; p2<k; ++p2)
    {
        if (a[p2]!=a[p1])
        {
            p1++;
            a[p1]=a[p2];
        }
    }
    a.resize(p1+1);
    return p1;
}

bool check_merge(int n)
{
    RB_tree<int> A,B;
    std::vector<int> less,more;
    for (int i=0; i<n; ++i)
    {
        int tmp=rand()%n;
        A.insert(tmp);
        less.push_back(tmp);
    }
    int coef=rand()%100+1;
    for (int i=0; i<coef*n; ++i)
    {
        int tmp=rand()%(coef*n)+n+1;
        B.insert(tmp);
        more.push_back(tmp);
    }
    sort(less.begin(),less.end());
    sort(more.begin(), more.end());
    replace_copy(less, less.size());
    replace_copy(more, more.size());
    A.clever_merge(B,n);
    if (!A.check()) return false;
    auto it=A.begin();
    for (auto& i:less)
    {
        if (*it!=i) return false;
        ++it;
    }
    if (*it!=n) return false;
    ++it;
    for (auto& i:more)
    {
        if (*it!=i) return false;
        ++it;
    }
    if (it!=A.end()) return false;
    return true;
}
bool check_cut(int n)
{
    RB_tree<int> A,A2,B,C,l,m;
    std::vector<int> res(n);
    for (int i=0; i<n; ++i)
    { 
    int tmp=rand()%(2*n);
    A.insert(tmp);
    res[i]=tmp;
    }
    A2=A;
    int tmp=rand()%(2*n);
    A.cut(tmp,B,C);
    l=B;
    m=C;
    return (A2==B)&&A2.check()&&B.check()&&m.check()&&l.check();
}
bool check(int n)
{
    auto cmp=[](const long long& x, const long& y)
    {
        return x<y;
    };
    RB_tree<long long,std::function<bool(const long long& x, const long& y)> > A(cmp),B(cmp),C(cmp);
    std::vector<long long> a(n);
    std::vector<long long> b(n);
    for (int i=0; i<n; i++)
    {
        a[i]=rand()%n;
        b[i]=a[i];
        A.insert(a[i]);
    }
    for (int i=0; i<n; ++i)
    {
        int tmp=rand()%n;
        A.cut(tmp,B,C);
        B.clever_merge(C);
        A=std::move(B);
    }
    std::sort(a.begin(), a.end());
    int new_size=replace_copy(a,n);
    new_size++;
    int i=0;
    for (auto it=A.begin(); it!=A.end(); ++it, ++i)
        if (*(A[i])!=a[i])
        {
            cout<<"it doesn't work in insert"<<endl;
            return false;
        }
    i=0;
    for (auto it=A.begin(); it!=A.end(); ++it, ++i)
    {
        if (A.how_many_less(*it)!=i || A.how_many_more(*it)!=A.size()-i-1)
        {
            return false;
        }
        if (i>0 && i<A.size()-1 && (*A.first_more(a[i])!=a[i+1] || *A.last_less(a[i])!=a[i-1]))
        {
            return false;
        }
    }
    if (!A.check())
    {
        cout<<"check after insert"<<endl;
        return false;
    }
    for (int i=0; i<new_size/2; i++)
    {
        A.erase(a[i]);
    }

    auto iit=A.begin();

    for (int i=new_size/2; i<new_size; ++i, ++iit)
    {
        if (*(A[i-new_size/2])!=a[i])
        {
            cout<<"it doesn't work in erase"<<endl;
            return false;
        }
    }
    if (!A.check()) 
    {
        cout<<"check after erase"<<endl;
        return false;
    }
    return true;
}
int main()
{
    srand(time(NULL));
    double t=clock();
    srand(time(NULL));
    int n=0;
    while (check(10000) && n<10)
    {
        n++;
    }
    cout<<"n= "<<n<<endl;
    cout<<((clock()-t)/CLOCKS_PER_SEC)<<endl;
    cout<<"Allocated is "<<alloc<<endl;
    cout<<"Leak is "<<Leak<<endl;
    return 0;
}
