#pragma once
#include "GLCTreeClassifier.h"


class  GLC_Tree_Classifier
{
public:
	GLC_Tree_Classifier(void);
	~GLC_Tree_Classifier(void);
	// 执行训练
	bool GLCTree_Execute_Train(double** data, int data_row, int data_line,int treeHight=7,int adboostingK=10,int lifesize=1);
	bool GLCTree_Execute_Train(double* data, int data_row, int data_line,int treeHight=7,int adboostingK=10,int lifesize=1);
private:
	// 分类器对象
	GLCTreeClassifier m_Gtree;
public:
	// 初始化可视化窗口
	//bool GLCTree_Set_Visual(CTreeCtrl* TreeCtrl,CString* attsname, int attsnum,CString* classname,int classnum);
private:
	// 树的个数
	int m_treeNum;
public:
	// 执行分类
	short* GLCTree_Excute_Classify(double** data, int data_row, int data_line);
	short* GLCTree_Excute_Classify(double* data, int data_row, int data_line,int &progressi);
	short  GLCTree_Excute_Classify(double* data, int data_line);
	// 清除数据
	bool GLCTree_Clear(void);
private:
	// 记录分类ID的地址
	short* m_classid;


	TreeSelection* m_treeSelection;
public:
	TreeSelection* GLCTree_Get_TreeSelection(void);
	int GLC_Set_TreeModle(TreeSelection* treeSelection=NULL);
};

