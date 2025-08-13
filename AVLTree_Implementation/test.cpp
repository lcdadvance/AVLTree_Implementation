/*****************************************************************//**
 * \file   test.cpp
 * \brief  测试avl树 
 * 
 * \author lcd
 * \date   August 2025
 *********************************************************************/

#include "myavl.hpp"

///遍历树等信息查询
void show01(xjcad::lcd::AvlTree1<int>& avl) {
	//遍历
	avl.preOrder();
	avl.inOrder();
	avl.postOrder();
	avl.levelOrder();
	avl.levelNum();
	std::cout << "树的结点总数为：" << avl.size() << std::endl;
	std::cout << "树的层数为：" << avl.high() << std::endl;
	std::cout << "avl树为空吗？ 标志：" << avl.empty() << std::endl;
	std::cout << "树的第一个元素为：" << avl.front() << std::endl;
	std::cout << "树的最后一个元素为：" << avl.back() << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
}

///迭代器双向遍历
void show02(xjcad::lcd::AvlTree1<int>& avl, xjcad::lcd::AvlTree1<int>::Iterator& it) {

	std::cout << "前序遍历1：";
	for (; it != avl.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	//C++11的foreach遍历
	std::cout << "for each前向遍历：";
	for (int x : avl)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;

	//后序遍历输出
	std::cout << "后序遍历1：";
	xjcad::lcd::AvlTree1<int>::Iterator it1 = avl.last();
	for (; it1 != xjcad::lcd::AvlTree1<int>::Iterator(nullptr, &avl); it1--)
	{
		std::cout << *it1 << " ";
	}
	std::cout << std::endl;

	std::cout << "后序遍历2：";
	xjcad::lcd::AvlTree1<int>::Iterator it2 = avl.last();
	for (; it2 >= avl.begin(); it2--) //it2--到最后肯定使得it2的成员变量_p指针变为nullptr的
	{
		std::cout << *it2 << " ";
	}
	std::cout << std::endl;
	std::cout << "------------------------" << std::endl;
}

///测试插入、删除
void test01(xjcad::lcd::AvlTree1<int>& avl) {

	show01(avl);

	//删除测试
	std::cout <<"树中是否存在结点9："<< avl.find(9) << std::endl;
	std::cout << "---删除9：" << std::endl;
	avl.remove(9);
	std::cout << "树中是否存在结点9：" << avl.find(9) << std::endl;

	//std::cout << "---删除10：" << std::endl;
	//avl.remove(10);

	//std::cout << "---删除6：" << std::endl;
	//avl.remove(6);
	////
	//std::cout << "---删除1,2,3：" << std::endl;
	//avl.remove(1);
	//avl.remove(2);
	//avl.remove(3);

	show01(avl);
}

///测试拷贝与赋值
void test02(xjcad::lcd::AvlTree1<int>& avl) {
	// 深拷贝
	std::cout << "---深拷贝验证：" << std::endl;
	xjcad::lcd::AvlTree1<int> avl2(avl);
	std::cout << "拷贝构造后的树avl2为：";
	avl2.inOrder();
	std::cout << "树avl2的节点总数为：" << avl2.size() << std::endl;
	std::cout << "树avl2的层数为：" << avl2.high() << std::endl;
	std::cout << "avl2 == avl? 标志：" << (avl2 == avl) << std::endl;

	std::cout << "---深拷贝独立性验证：" << std::endl;
	avl.insert(42);  // 修改原树
	std::cout << "找到在原树avl中新插入的数了吗：" << avl.find(42) << std::endl;;
	std::cout << "拷贝树avl2找到在原树avl中新插入的数了吗：" << avl2.find(42) << std::endl;
	avl.remove(42);
	avl2.remove(5);
	std::cout << "原树avl中找到在拷贝树avl2中新删除的数了吗：" << avl.find(5) << std::endl;; 
	std::cout << "找到在拷贝树avl2中新删除的数了吗：" << avl2.find(5) << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;

	//深拷贝赋值
	std::cout << "---赋值操作验证：" << std::endl;
	xjcad::lcd::AvlTree1<int> avl3;
	avl3 = avl;
	std::cout << "拷贝构造后的树avl3为：";
	avl3.inOrder();
	std::cout << "树avl3的节点总数为：" << avl3.size() << std::endl;
	std::cout << "树avl3的层数为：" << avl3.high() << std::endl;
	std::cout << "avl3 == avl? 标志：" << (avl3 == avl) << std::endl;

	std::cout << "---赋值独立性验证：" << std::endl;
	avl.insert(42);  // 修改原树
	std::cout << "找到在原树avl中新插入的数了吗：" << avl.find(42) << std::endl;; 
	std::cout << "拷贝树avl3找到在原树avl中新插入的数了吗：" << avl3.find(42) << std::endl;  // 副本不受影响
	avl.remove(42);

	avl2.insert(99);
	avl2.remove(5);
	std::cout << "原树avl中找到在拷贝树avl3中新删除的数了吗：" << avl.find(5) << std::endl;;
	std::cout << "原树avl中找到在拷贝树avl3中新插入的数了吗：" << avl.find(99) << std::endl;;
	std::cout << "找到在拷贝树avl2中新删除的数了吗：" << avl2.find(5) << std::endl;
	std::cout << "找到在拷贝树avl2中新插入的数了吗：" << avl2.find(99) << std::endl;
	std::cout << "---------------------------------------------" << std::endl;
}

///迭代器测试
void test03(xjcad::lcd::AvlTree1<int>& avl, xjcad::lcd::AvlTree1<int>::Iterator& it) {

	//++、-- 测试（改变原迭代器）
	it = avl.begin();
	std::cout << "++、-- 测试（改变原迭代器）：" << std::endl;
	std::cout << "it: " << *it << std::endl;
	std::cout << "++it: " << *(++it) << std::endl;
	std::cout << "it++: " << *(it++) << std::endl;
	std::cout << "现在it为: " << *it << std::endl;
	std::cout << "--it: " << *(--it) << std::endl;
	std::cout << "it--: " << *(it--) << std::endl;
	std::cout << "现在it为: " << *it << std::endl;
	std::cout << "-------------------" << std::endl;

	//+=、-= 测试（改变原迭代器）
	it = avl.begin();
	std::cout << "+=、-= 测试（改变原迭代器）：" << std::endl;
	std::cout << "it: " << *it << std::endl;
	it += 3;
	std::cout << "it += 3后it为: " << *it << std::endl;
	it -= 2;
	std::cout << "it -= 2后it为: " << *it << std::endl;
	it += 20;
	std::cout << "it += 20后it为(边界限定): " << *it << std::endl;
	it -= 20;
	std::cout << "it -= 20后it为(边界限定): " << *it << std::endl;
	std::cout << "-------------------" << std::endl;

	//+、- 测试（返回新迭代器）
	it = avl.begin();
	std::cout << "+、- 测试（返回新迭代器）：" << std::endl;

	std::cout << "it: " << *it << std::endl;
	auto it1 = it + 2;
	std::cout << "it1 = it + 2, it1为: " << *it1 << std::endl;
	std::cout << "it: " << *it << std::endl;

	auto it2 = (it + 2) + 1;
	std::cout << "it2 = (it + 2) + 1, it2为: " << *it2 << std::endl;

	auto it3 = it2 - 1;
	std::cout << "it3 = it2 - 1, it3为: " << *it3 << std::endl;

	auto it4 = (it + 7) - 1;
	std::cout << "it4 =  (it + 7) - 1, it4为: " << *it4 << std::endl;
	std::cout << "it: " << *it << std::endl;

	auto it5 = 2 + it;
	std::cout << "it5 = 2 + it, it5为: " << *it5 << std::endl;

	auto it6 = (3 + it) - 1;
	std::cout << "it6 = (3 + it) - 1, it6为: " << *it6 << std::endl;

	auto it7 = it + 20;
	std::cout << "it7 = it + 20, it7为(边界限定): " << *it7 << std::endl;

	auto it8 = it6 - 20;
	std::cout << "it8 = it6 - 20, it8为(边界限定): " << *it8 << std::endl;

	std::cout << "-------------------" << std::endl;

	//删除erase测试
	std::cout << "删除迭代器指向位置测试：" << std::endl;
	std::cout << "it: " << *it << std::endl;
	avl.erase(it);
	it = avl.begin(); //删除会释放结点导致迭代器失效
	show02(avl, it);  //遍历里面有++it，最终it会变为end()，即为空

	auto itx = avl.end();
	if (it == itx) {
		std::cout << "迭代器it此时指向end()" << std::endl;
	}

	it = avl.begin(); //重置it
	avl.erase(it + 100);
	it = avl.begin(); //删除会释放结点导致迭代器失效
	show02(avl, it); //打印avl树遍历信息
}


int main()
{
	xjcad::lcd::AvlTree1<int> avl = {};

	for (int i = 1; i < 11; i++)
	{
		avl.insert(i);
	}

	test01(avl);

	//xjcad::lcd::AvlTree1<double> avl2;
	//std::cout << "树avl2为空吗？ 标志：" << avl2.empty() << std::endl;
	//std::cout << "avl2是平衡树吗：" << avl2.isBalance() << std::endl;

    //test02(avl);


	//索引访问元素
	//std::cout << "---索引访问元素：" << std::endl;
	//std::cout << avl.inOrderIndex(5) << std::endl; //调用函数访问中序遍历结果中索引为5的元素
	//std::cout << avl[5] << std::endl; //[]运算符重载访问元素
	////std::cout << avl[10] << std::endl; //超出索引引发异常

	std::cout << "-------------------------------------------------" << std::endl;



	xjcad::lcd::AvlTree1<int>::Iterator it = avl.begin();

	show02(avl, it); //打印avl树遍历信息

	//test03(avl, it);


	return 0;
}