#pragma once
#include "GLCTreeClassifier.h"


class  GLC_Tree_Classifier
{
public:
	GLC_Tree_Classifier(void);
	~GLC_Tree_Classifier(void);
	// ִ��ѵ��
	bool GLCTree_Execute_Train(double** data, int data_row, int data_line,int treeHight=7,int adboostingK=10,int lifesize=1);
	bool GLCTree_Execute_Train(double* data, int data_row, int data_line,int treeHight=7,int adboostingK=10,int lifesize=1);
private:
	// ����������
	GLCTreeClassifier m_Gtree;
public:
	// ��ʼ�����ӻ�����
	//bool GLCTree_Set_Visual(CTreeCtrl* TreeCtrl,CString* attsname, int attsnum,CString* classname,int classnum);
private:
	// ���ĸ���
	int m_treeNum;
public:
	// ִ�з���
	short* GLCTree_Excute_Classify(double** data, int data_row, int data_line);
	short* GLCTree_Excute_Classify(double* data, int data_row, int data_line,int &progressi);
	short  GLCTree_Excute_Classify(double* data, int data_line);
	// �������
	bool GLCTree_Clear(void);
private:
	// ��¼����ID�ĵ�ַ
	short* m_classid;


	TreeSelection* m_treeSelection;
public:
	TreeSelection* GLCTree_Get_TreeSelection(void);
	int GLC_Set_TreeModle(TreeSelection* treeSelection=NULL);
};

