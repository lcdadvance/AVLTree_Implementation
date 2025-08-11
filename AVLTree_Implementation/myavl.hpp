/* **************************************************
 * Copyright © 2025, Wuxi Xinje Electric Co., Ltd.
 *
* File Name: myavl.cpp
* Description:  实现avl树并兼容标准容器操作
* Others:
* Version:
* Author: lcd
* Date: 2025-08-07
 * **************************************************/
#pragma once
#include<iostream>
#include<queue>
#include<stack>
#include <algorithm>

namespace xjcad
{
namespace lcd
{
// AVL树 = BST树 + 节点平衡操作
template<class T>
class AvlTree1
{
public:

	AvlTree1() {
		root_ = nullptr;
		size_ = 0;
	}
	// 拷贝构造 (深拷贝,复制整颗树)
	AvlTree1(const AvlTree1& other) :
		root_(copyTree(other.root_)), size_(other.size_) {
	}  // 调用递归复制函数

	~AvlTree1() {
		clear();
	}

	//赋值操作，operator= 等号运算符重载，也可以防止浅拷贝问题
	AvlTree1& operator=(const AvlTree1& avl);
	// 交换函数
	void swap(AvlTree1& first, AvlTree1& second) noexcept;


	// []运算符重载，通过下标的方式访问树的元素，默认按中序遍历访问
	//1.递归方式访问
	/*T& operator[](const int index)
	{
		return inOrderIndex(index);
	}*/
	//2.非递归方式，利用栈
	T& operator[](const int index) const;
	T& n_inOrder(const int index, int& m) const;

	//operator== 等号运算符重载,两颗树一样返回true，否则false
	bool operator== (const AvlTree1& avl) const
	{
		if (size_ != avl.size_) {
			return false;
		}
		for (int i = 0; i < size_; i++)
		{
			if ((*this)[i] != avl[i]) {// 使用 (*this)[i] 访问当前树的第 i 个元素
				return false;
			}
		}
		return true;
	}
	//operator!= 等号运算符重载,两颗树不一样返回true，否则false
	bool operator!= (const AvlTree1& avl) const
	{
		if (size_ != avl.size_) {
			return true;
		}
		for (int i = 0; i < size_; i++)
		{
			if ((*this)[i] != avl[i]) { // 使用 (*this)[i] 访问当前树的第 i 个元素
				return true;
			}
		}
		return false;
	}

public:
	//---------结点插入/删除-------------
	//*****AVL树的插入操作
	void insert(const T& val)
	{
		root_ = insert(root_, nullptr, size_, val);
	}
	//*****AVL树的删除操作
	void remove(const T& val)
	{
		root_ = remove(root_, nullptr, size_, val); // 添加父节点参数
	}
	//删除第一个结点
	void pop_front()
	{
		if (root_) {
			T val = inOrderIndex(0);
			remove(val);
		}
	}
	//删除最后一个结点
	void pop_back()
	{
		if (!empty()) {
			T val = inOrderIndex(size_ - 1);
			remove(val);
		}
	}
	//删除迭代器指向元素（这个函数要么放到Iterator定义的下面，要么提前声明一下Iterator,不然不知道Iterator是啥）
	class Iterator;
	/* it._p是Iterator的私有成员变量，不能访问。两种解决方法
		1.不访问私有成员变量_p，通过上面方式
		2.在Iterator类里添加友元声明：friend void AvlTree1<T>::erase(Iterator it);*/
	void erase(Iterator it) {
		if (it._p) {  //如果传进来的迭代器不是指向空的，即有效的迭代器
			//std::cout << "进入" << std::endl;
			remove(*it);
		}
	}
	//void erase(Iterator start, Iterator end)
	//{
	//	//如果start不指向空且start不等于end
	//	if (start._p && start != end) {
	//		for (Iterator it = start; it != end; ++it)
	//		{
	//			remove(*it);
	//		}
	//	}
	//}


	//--------------查询-----------------
	//1.AVL树的查询操作
	bool find(const T& val) const
	{
		return find(root_, val);
	}
	//2.AVL树节点总数查询
	/*int size() const noexcept
	{
		return size(root_);
	}*/
	int size() const
	{
		return size_;
	}
	//3.AVL树层数查询
	int high() const noexcept
	{
		return high(root_);
	}
	//4.判空
	bool empty() const
	{
		return root_ == nullptr;
	}
	//5.AVL树每一层的节点数
	void levelNum() const
	{
		int h = high();
		for (int i = 0; i < h; i++)
		{
			std::cout << "第" << i + 1 << "层节点数目为：" << levelNum(root_, i) << "  ";
		}
		std::cout << std::endl;
	}
	//6.判断是否是AVL树
	bool isBalance() const
	{
		int l = 0; //递归函数的局部变量形参，用于记录每个节点的层数
		bool flag = true; //递归回溯时判断是否失衡的标志位
		isBalance(root_, l, flag);
		return flag;
	}
	//7.返回第一个和最后一个元素
	T front() const {
		if (!empty()) {
			return inOrderIndex(0);
		}
	}
	T back() const {
		if (!empty()) {
			return inOrderIndex(size() - 1);
		}
	}


	//--------------遍历-----------------
	//前序遍历VLR
	void preOrder() const
	{
		std::cout << "[前序]遍历：";
		preOrder(root_);
		std::cout << std::endl;
	}
	//中序遍历LVR
	void inOrder() const
	{
		std::cout << "[中序]遍历：";
		inOrder(root_);
		std::cout << std::endl;
	}
	//后序遍历LRV
	void postOrder() const
	{
		std::cout << "[后序]遍历：";
		postOrder(root_);
		std::cout << std::endl;
	}
	//层序遍历
	void levelOrder() const
	{
		int high1 = high(); //树高
		std::cout << "[层序]遍历：";
		for (int i = 0; i < high1; i++)
			levelOrder(root_, i); //每次只打印某一层
		std::cout << std::endl;
	}

	//--------------其余------------------
	//1.清空树 ######
	void clear()
	{
		if (root_ != nullptr) //树不为空再删（利用层序遍历思想删除BST/AVL树所有节点）
		{
			std::queue<Node*> q;
			q.push(root_);
			while (!q.empty())
			{
				Node* cur = q.front(); //取出队列头元素
				q.pop();

				if (cur->left_ != nullptr) {
					q.push(cur->left_);
				}
				if (cur->right_ != nullptr) {
					q.push(cur->right_);
				}
				delete cur;
			}
		}
		root_ = nullptr; // 置空根指针
		size_ = 0;
	}
	//2.两颗avl树交换函数  eg: tree1.swap(tree2)
	void swap(AvlTree1& other) noexcept {
		using std::swap;
		swap(root_, other.root_);
	}
	//3/返回中序遍历结果中索引为i的元素
	T& inOrderIndex(const int index) const
	{
		if (index < 0 || index >= size()) {
			throw "out of range!";
		}
		int m = 0;
		T result;
		inOrderIndex(root_, index, m, result);
		return result;
	}

	struct Node;
	class Iterator
	{
	public:
		Iterator(Node* p = nullptr, AvlTree1<T>* pavl = nullptr) :_p(p), _pavl(pavl) {} //迭代器构造函数
		//不同容器的迭代器不能进行比较运算，没有意义
		bool operator!=(const Iterator& it) const
		{
			//检查迭代器有效性: 检查两迭代器对应的不是同一个容器
			if (_pavl != it._pavl)
				throw "Iterator incompatable!";
			return _p != it._p; //判断它们指向的节点是否一样
		}
		bool operator==(const Iterator& it) const
		{
			//检查迭代器有效性: 检查两迭代器对应的不是同一个容器
			if (_pavl != it._pavl)
				throw "Iterator incompatable!";
			return _p == it._p;
		}
		bool operator<(const Iterator& it) const
		{
			//检查迭代器有效性: 检查两迭代器对应的不是同一个容器
			if (_pavl != it._pavl)
				throw "Iterator incompatable!";
			if (!_p || !it._p)  //如果任一迭代器的底层指针为空，返回false
				return false;
			else
				return _p->data_ < it._p->data_;
		}
		bool operator<=(const Iterator& it) const
		{
			//检查迭代器有效性: 检查两迭代器对应的不是同一个容器
			if (_pavl != it._pavl)
				throw "Iterator incompatable!";
			if (!_p || !it._p)  //如果任一迭代器的底层指针为空，返回false
				return false;
			else
				return _p->data_ <= it._p->data_;
		}
		bool operator>(const Iterator& it) const
		{
			//检查迭代器有效性: 检查两迭代器对应的不是同一个容器
			if (_pavl != it._pavl)
				throw "Iterator incompatable!";
			if (!_p || !it._p)  //如果任一迭代器的底层指针为空，返回false
				return false;
			else
				return _p->data_ > it._p->data_;
		}
		bool operator>=(const Iterator& it) const
		{
			//检查迭代器有效性: 检查两迭代器对应的不是同一个容器
			if (_pavl != it._pavl)
				throw "Iterator incompatable!";
			if (!_p || !it._p)  //如果任一迭代器的底层指针为空，返回false
				return false;
			else
				return _p->data_ >= it._p->data_;
		}

		//前置递增
		Iterator& operator++()
		{
			if (_p) {
				_p = nextInOrder(_p);  //应该返回其中序遍历序列中该元素下一个元素的地址
			}                          //如果迭代器指向空，什么都不做，就不引发异常了
			return *this;
		}
		//后置递增
		Iterator& operator++(int)
		{
			Iterator temp = *this;
			++(*this);
			return temp;
		}
		//前置递减
		Iterator& operator--()
		{
			if (_p) {
				_p = preInOrder(_p);
			}
			return *this;
		}
		//后置递减
		Iterator& operator--(int)
		{
			Iterator temp = *this;
			--(*this);
			return temp;
		}

		//类成员运算符重载：it += n（改变原迭代器本身，不返回新迭代器，故不支持(it+1)+3这种链式操作），默认n过大超界时指向最后一个元素
		Iterator& operator+=(const int& n) {
			if (n <= 0 || !_p) { return *this; }//n非法或迭代器指向空 

			Node* current = this->_p;
			for (int i = 0; i < n; i++) {
				Node* next = nextInOrder(current);
				if (next == nullptr) {
					break;
				}
				current = next;
			}
			this->_p = current; // 避免提前修改 _p：使用 cur 临时变量进行移动，确保在移动过程中不破坏迭代器的当前状态
			return *this;
		}
		//类成员运算符重载：it + n（返回新迭代器，支持 (it+1)+3这种链式操作）
		Iterator operator+(const int& n) const {
			Iterator temp = *this;
			temp += n;
			return temp;  //不能返回局部变量temp的引用，即Iterator&
		}
		//在类内部定义友元  n + it
		friend Iterator operator+(int n, const Iterator& it) {
			return it + n;
		}

		//it -= n  默认n过大超界时指向第一个元素（最小值）
		Iterator& operator-=(const int& n) {
			if (n <= 0) { return *this; }

			Node* current = this->_p;
			for (int i = 0; i < n; i++) {
				Node* last = preInOrder(current);
				if (last == nullptr) {
					break;
				}
				current = last;
			}
			this->_p = current; // 避免提前修改 _p：使用 cur 临时变量进行移动，确保在移动过程中不破坏迭代器的当前状态
			return *this;
		}
		//类成员运算符重载：it - n（返回新迭代器，支持 (it-1)-3这种链式操作）
		Iterator operator-(const int& n) const {
			Iterator temp = *this;
			temp -= n;
			return temp;  //不能返回局部变量temp的引用，即Iterator&
		}
		//n - it没必要了

		T& operator*() {
			//检查迭代器有效性: 检查两迭代器对应的不是同一个容器
			if (!_p) {
				throw "Iterator is nullptr,do not have the value!";
			}
			return _p->data_;  //解引用返回指针对应节点的数据域
		}
		Node* operator->() {
			return _p;
		}

	private:
		// 查找中序后继
		static Node* nextInOrder(Node* node) {
			if (!node) { return nullptr; }

			// 情况1：存在右子树
			if (node->right_) {
				return findMin(node->right_);
			}

			// 情况2：向上查找第一个左祖先
			Node* cur = node;
			Node* parent = node->parent_;
			while (parent != nullptr && cur == parent->right_) {
				cur = parent;
				parent = parent->parent_;
			}
			return parent;         //如果是右下角最大值的节点，它递增后会变为nullptr;
		}
		static Node* findMin(Node* node) {
			while (node && node->left_) {
				node = node->left_;
			}
			return node;
		}
		// 查找中序前驱
		static Node* preInOrder(Node* node) {
			if (!node) { return nullptr; }

			// 情况1：存在左子树
			if (node->left_) {
				return findMax(node->left_);
			}
			// 情况2：向上查找第一个右祖先
			Node* cur = node;
			Node* parent = node->parent_;
			while (parent != nullptr && cur == parent->left_)
			{
				cur = parent;
				parent = parent->parent_;
			}
			return parent;           //如果是左下角最小值的节点，它递减后会变为nullptr;
		}
		static Node* findMax(Node* node) {
			while (node && node->right_) {
				node = node->right_;
			}
			return node;
		}

		Node* _p; //维护一个指针，指向AVL树的当前节点
		AvlTree1<T>* _pavl; //++++++++++++++ 让迭代器知道它是属于哪颗AVL树的

		friend void AvlTree1<T>::erase(Iterator it);//友元声明
		//friend void AvlTree1<T>::erase(Iterator start, Iterator end);
	};

	Iterator begin()  //返回一个指向AVL树中序遍历中的第一个元素（也就是最小值）的迭代器
	{
		Node* cur = root_;
		while (cur != nullptr && cur->left_ != nullptr)
		{
			cur = cur->left_;
		}
		return Iterator(cur, this); //this指向当前容器对象
	}
	Iterator end()  //返回一个指向AVL树中序遍历中的最后一个元素（也就是最大值）后面位置的迭代器，所有这里end()迭代器下面的指针指向的是空
	{
		return Iterator(nullptr, this);
	}
	Iterator last() // 获取最后一个元素（中序最大）
	{
		Node* cur = root_;
		while (cur != nullptr && cur->right_ != nullptr)
		{
			cur = cur->right_;
		}
		return Iterator(cur, this);
	}

	////删除迭代器指向元素（这个函数要放到iteratr下面，不然不知道Iterator是啥）
	//void erase(Iterator it)
	//{
	//	T x = *it;
	//	remove(x);
	//}


private:
	//定义AVL树节点类型
	struct Node
	{
		Node(T data = T()) :data_(data), left_(nullptr), right_(nullptr), parent_(nullptr), height_(1) {}
		T data_;
		Node* left_;
		Node* right_;
		Node* parent_;
		int height_; //记录节点高度值
	};
	Node* root_; //指向根节点的指针
	int size_; //结点数目

	//返回节点的高度值
	int height(Node* node)
	{
		return node == nullptr ? 0 : node->height_;
	}
	//右旋转操作  以节点node为轴做右旋转操作，并把新的根节点返回
	Node* rightRotate(Node* node)
	{
		//旋转操作
		Node* child = node->left_;
		Node* grandchild = child->right_;
		node->left_ = grandchild;
		child->right_ = node;
		// 更新父指针
		child->parent_ = node->parent_;  // 新根继承原节点的父指针
		node->parent_ = child;           // node成为child的子节点
		if (grandchild) {
			grandchild->parent_ = node;  // 更新grandchild的父指针
		}
		//高度更新   node child
		node->height_ = std::max(height(node->left_), height(node->right_)) + 1; //当前节点高度值 = 左右孩子高度值较大者+1
		child->height_ = std::max(height(child->left_), height(child->right_)) + 1;
		return child; //返回旋转后的子树新的根节点地址，以回溯连接到老根节点的父节点上去
	}
	//左旋转操作  以节点node为轴做左旋转操作，并把新的根节点返回
	Node* leftRotate(Node* node)
	{
		//旋转操作
		Node* child = node->right_;
		Node* grandchild = child->left_;
		node->right_ = grandchild;
		child->left_ = node;
		// 更新父指针
		child->parent_ = node->parent_;
		node->parent_ = child;
		if (grandchild) {
			grandchild->parent_ = node;
		}
		//高度更新   node child
		node->height_ = std::max(height(node->left_), height(node->right_)) + 1;
		child->height_ = std::max(height(child->left_), height(child->right_)) + 1;
		return child;
	}
	//左平衡操作（左孩子的右子树太高了） 以节点node为轴做左-右旋转操作，并把新的根节点返回
	Node* leftBalance(Node* node)
	{
		//先左旋
		node->left_ = leftRotate(node->left_);
		if (node->left_) {
			node->left_->parent_ = node; // 更新新左孩子的父指针
		}
		//再右旋
		return rightRotate(node); //最终新的根节点就是右旋函数返回的根节点
	}
	//右平衡操作（右孩子的左子树太高了） 以节点node为轴做右-左旋转操作，并把新的根节点返回
	Node* rightBalance(Node* node)
	{
		node->right_ = rightRotate(node->right_);
		if (node->right_) {
			node->right_->parent_ = node; // 更新新右孩子的父指针
		}
		return leftRotate(node);
	}

	//-------------结点插入/删除-----------------
	//AVL树的插入操作递归实现
	Node* insert(Node* node, Node* parent, int& size_, const T& val)
	{
		if (node == nullptr) //递归结束，找到插入位置
		{
			Node* newNode = new Node(val);
			newNode->parent_ = parent;
			size_++;
			return newNode;
		}
		if (node->data_ == val) //找到相同节点不用再往下递归了，直接向上回溯
		{
			return node;
		}
		else if (node->data_ > val)
		{
			node->left_ = insert(node->left_, node, size_, val); //回溯时更新一下node的左孩子!!!
			//添加1 在递归回溯时判断节点是否失衡(上面是往当前节点node的左子树插的) node的左子树太高，node失衡了
			if (height(node->left_) - height(node->right_) > 1)
			{
				Node* child = node->left_;
				if (height(child->left_) >= height(child->right_)) //1. LL情形 左孩子的左子树太高
				{
					Node* newRoot = rightRotate(node);
					newRoot->parent_ = parent; // 设置新根的父指针
					return newRoot;
				}
				else   //2. LR情形 左孩子的右子树太高
				{
					Node* newRoot = leftBalance(node);
					newRoot->parent_ = parent; // 设置新根的父指针
					return newRoot;
				}
			}
		}
		else
		{
			node->right_ = insert(node->right_, node, size_, val);

			//添加2 在递归回溯时判断节点是否失衡
			if (height(node->right_) - height(node->left_) > 1)
			{
				Node* child = node->right_;
				if (height(child->right_) >= height(child->left_)) //1. RR情形 右孩子的右子树太高
				{
					Node* newRoot = leftRotate(node);
					newRoot->parent_ = parent; // 设置新根的父指针
					return newRoot;
				}
				else   //2. RL情形 左孩子的右子树太高
				{
					Node* newRoot = rightBalance(node);
					newRoot->parent_ = parent; // 设置新根的父指针
					return newRoot;
				}
			}
		}
		//添加3 在递归回溯时检测更新节点高度(上面也进行了节点高度的更新，但是是失衡时进行的，不失衡时没更新节点高度，但是你插入了节点，祖先节点高度变化了那必须得更新)
		node->height_ = std::max(height(node->left_), height(node->right_)) + 1;
		return node;
	}
	//AVL树的删除操作递归实现 在根节点为node的树上，找到val节点删除它，并返回val节点的孩子节点的地址，以连到被删除节点的父节点上去
	Node* remove(Node* node, Node* parent, int& size_, const T& val)
	{
		if (node == nullptr) { //没找到
			return nullptr;
		}
		if (node->data_ == val)
		{
			//情形3
			if (node->left_ != nullptr && node->right_ != nullptr)
			{
				//*****为了避免删除前驱或后继造成节点失衡，谁高删除谁
				if (height(node->left_) >= height(node->right_)) //删前驱
				{
					Node* cur = node->left_;
					while (cur->right_ != nullptr)
						cur = cur->right_;
					node->data_ = cur->data_; //2.覆盖数据并递归删除
					node->left_ = remove(node->left_, node, size_, cur->data_); //3.现在要删的是前驱节点cur了,同时更新node的左孩子域
				}
				else  //删后继
				{
					Node* cur = node->right_;
					while (cur->left_ != nullptr)
						cur = cur->left_;
					node->data_ = cur->data_;
					node->right_ = remove(node->right_, node, size_, cur->data_);
				}
			}
			else if (node->left_ == nullptr && node->right_ == nullptr) //情形1
			{
				delete node;
				size_--;
				return nullptr;
			}
			else  //情形2
			{
				if (node->left_ != nullptr)
				{
					Node* child = node->left_;
					if (child) {
						child->parent_ = parent;
					}
					delete node;
					size_--;
					return child;
				}
				else
				{
					Node* child = node->right_;
					if (child) {
						child->parent_ = parent;
					}
					delete node;
					size_--;
					return child;
				}
			}
		}
		else if (node->data_ > val)
		{
			node->left_ = remove(node->left_, node, size_, val);

			//*****左子树删除节点，可能造成右子树太高
			if (height(node->right_) - height(node->left_) > 1)
			{
				if (height(node->right_->right_) >= height(node->right_->left_))
				{
					//右孩子的右子树太高，RR  做左旋操作
					return leftRotate(node);
				}
				else //RL
				{
					return rightBalance(node);
				}
			}
		}
		else
		{
			node->right_ = remove(node->right_, node, size_, val);

			//*****右子树删除节点，可能造成左子树太高
			if (height(node->left_) - height(node->right_) > 1)
			{
				if (height(node->left_->left_) >= height(node->left_->right_))
				{
					//左孩子的左子树太高，LL  做右旋操作
					return rightRotate(node);
				}
				else //RL
				{
					return leftBalance(node);
				}
			}
		}
		//*****更新节点高度
		node->height_ = std::max(height(node->left_), height(node->right_)) + 1;

		return node; //把该句放上面两个判断里会出错。把当前节点返回父节点，更新父节点相应的地址域
	}

	//-----------------查询----------------------
	//AVL树的查询操作递归实现（存在返回true，否则返回false）
	bool find(Node* node, const T& val) const
	{
		if (node == nullptr) { //递归退出条件，此时没有查到
			return false;
		}
		if (node->data_ == val) {
			return true;
		}
		else if (node->data_ > val) {
			find(node->left_, val);
		}
		else {
			find(node->right_, val);
		}
	}
	//AVL树节点总数查询递归实现
	/*int size(Node* node) const
	{
		if (node == nullptr){
			return 0;
		}
		int left_num = size(node->left_);
		int right_num = size(node->right_);
		return left_num + right_num + 1;
	}*/
	//AVL树层数查询递归实现
	int high(Node* node) const
	{
		if (node == nullptr) {
			return 0;
		}
		int l_high = high(node->left_);
		int r_high = high(node->right_);
		return std::max(l_high, r_high) + 1;
		//return l_high > r_high ? l_high + 1 : r_high + 1;
	}
	//AVL树每一层的节点数递归实现（类似层数查询）
	int levelNum(Node* node, int i) const
	{
		if (node == nullptr) {
			return 0;
		}
		if (i == 0) {
			return 1;
		}
		int left = levelNum(node->left_, i - 1);
		int right = levelNum(node->right_, i - 1);
		return left + right;
	}
	//判断是否是AVL树        递归过程中记录节点的高度值  返回节点的高度值
	int isBalance(Node* node, int l, bool& flag) const
	{
		if (node == nullptr)
		{
			return l;  //把当前节点高度返回回去
		}

		int left = isBalance(node->left_, l + 1, flag);  //L
		if (!flag) { //已经失衡了，不用再往下继续判断了
			return left;
		}
		int right = isBalance(node->right_, l + 1, flag); //R
		if (!flag) {
			return right;
		}

		if (abs(left - right) > 1) //节点失衡
		{
			flag = false;
		}
		return std::max(left, right); //给父节点返回高度（左右子树高度较大者）
	}

	//------------------遍历--------------------
	//前序遍历VLR
	void preOrder(Node* node) const
	{
		if (node != nullptr)
		{
			std::cout << node->data_ << " "; //V
			preOrder(node->left_); //L
			preOrder(node->right_); //R
		}
	}
	//中序遍历LVR
	void inOrder(Node* node) const
	{
		if (node != nullptr)
		{
			inOrder(node->left_); //L
			std::cout << node->data_ << " ";
			inOrder(node->right_); //R
		}
	}
	//后序遍历LRV
	void postOrder(Node* node) const
	{
		if (node != nullptr)
		{
			postOrder(node->left_); //L
			postOrder(node->right_); //R
			std::cout << node->data_ << " ";
		}
	}
	//层序遍历
	void levelOrder(Node* node, int i) const
	{
		if (node == nullptr) {
			return;
		}
		if (i == 0)
		{
			std::cout << node->data_ << " ";
			return;
		}
		levelOrder(node->left_, i - 1);
		levelOrder(node->right_, i - 1);
	}

	//-----------------辅助函数-------------------
		// 递归复制子树 (核心深拷贝函数)
	Node* copyTree(Node* node) {
		if (node == nullptr) {
			return nullptr;
		}

		// 创建新节点（复制数据）
		Node* newNode = new Node(node->data_);
		newNode->height_ = node->height_;  // 复制高度值
		newNode->parent_ = node->parent_;

		// 递归复制左右子树
		newNode->left_ = copyTree(node->left_);
		newNode->right_ = copyTree(node->right_);

		return newNode;
	}
	//重载[]运算符时使用，返回中序遍历结果中索引为i的元素
	void inOrderIndex(Node* node, const int index, int& m, T& result) const
	{
		if (node != nullptr)
		{
			inOrderIndex(node->left_, index, m, result); //L
			/*std::cout << node->data_ << " ";*/
			//if (m == index)
			//{
			//	std::cout << "m: " << m << std::endl;
			//	std::cout << node->data_ << std::endl;
			//}     //V
			if (m == index)
			{
				result = node->data_;
			}
			m++;
			inOrderIndex(node->right_, index, m, result); //R
		}
	}

};

//--------------部分成员函数类外实现---------------
//1.赋值操作，= 运算符重载，也可以防止浅拷贝问题
template<class T>
AvlTree1<T>& AvlTree1<T>::operator=(const AvlTree1<T>& avl){
if (this != &avl) //防止自赋值误操作(避免"自杀式"资源释放)，如tree = tree;
{
	AvlTree1 temp(avl); // 利用拷贝构造创建临时副本
	swap(*this, temp); // 交换当前对象和副本
} // 临时对象销毁（自动清理原资源）
return *this;
}
//交换函数
template<class T>
void AvlTree1<T>::swap(AvlTree1<T>& first, AvlTree1<T>& second) noexcept {
	using std::swap;
	swap(first.root_, second.root_);
	swap(first.size_, second.size_);
}
//2.[]运算符重载，通过下标的方式访问树的元素，默认按中序遍历访问
//非递归方式，利用栈
template<class T>
T& AvlTree1<T>::operator[](const int index) const {
	if (index < 0 || index >= size()) {
		throw "out of range!";
	}
	int m = 0; //计数器标志位
	return n_inOrder(index, m);
}
//非递归中序遍历，按照输入索引返回中序遍历序列对应元素
template<class T>
T& AvlTree1<T>::n_inOrder(const int index, int& m) const {
	std::stack<Node*> s;
	//先把根节点开始的左孩子依次入栈 
	Node* cur = root_;
	while (cur != nullptr)
	{
		s.push(cur);
		cur = cur->left_;
	}

	while (!s.empty())
	{
		Node* top = s.top(); //获取栈顶元素
		s.pop();
		if (m == index) {
			return top->data_;
		}
		m++;
		//std::cout << top->data_ << " ";

		cur = top->right_; //右孩子
		while (cur != nullptr)
		{
			s.push(cur);
			cur = cur->left_; //右孩子要是左边还有节点，一直入
		}
	}
}

}
}