//#include <vector>
//using std::vector;
#pragma once
#include "GLC_Classifier.h"

//struct ClassUnique1//���ڼ�¼����ֵ���������������е�������
//{  
//
//	int classID;        //��¼��������
//	int cunt;
//	vector<int> dataIndex;//��¼���������ǩ�����е�����
//	
//	ClassUnique1 * next;
//};
// struct AttValuesTab//���ڼ�¼����ֵ���������������е�������
//{  
//	double AttValue;
//	int classID;        //��¼��������
//	int AttValueIndex;
//};
// struct ConfusionMatrix//���ڼ�¼��������
//{  
//	int classifierID;
//	int nClass;     //������
//	int **matrix; //��������
//	float **acryMatrix;//������ʶ�����ܾ���
//};
class GLC_Class_Accuracy
{
public:
	
	GLC_Class_Accuracy(void);
	~GLC_Class_Accuracy(void);
	void Acry_Set_ClassAcryMatrix(int*data_tag/*���ǩ*/,int data_row/*��������*/,short **class_id/*����֮�������*/,int& class_num/*�����*/,int classifier_num=3/*����������*/);
	void  Acry_Set_ClassConfusionMatrix(int*data_tag/*���ǩ*/,int data_row/*��������*/,short **class_id/*����֮�������*/,int& class_num/*�����*/,int classifier_num=3/*����������*/); //��������
	float**  GLC_Class_Accuracy::Acry_Get_ClassAcryMatrix();//�����𾫶�
	void  Acry_Delete_ClassAcryMatrix();
	int ** Acry_Get_ClassErrMatrix();
	int *   Acry_Get_ClassNumMatrix();
	ConfusionMatrix *  Acry_Get_ClassConfusionMatrix();//��û�������
	ConfusionMatrix *   Acry_Get_ClassConfusionAcfyMatrix();//��÷�����ʶ�����ܾ��󣨻������󾫶ȣ�
	
	ConfusionMatrix* Arcy_Get_ClassConfidenceMatrix(void);// ������ζȾ���
private:
	ClassUnique1 *Acry_Get_ClassUnique(AttValuesTab * attValues, int attNum) ;
	ClassUnique1 *  Acry_Get_ClassUnique(AttValuesTab * attValues, int attNum,short**classfiedID,int nClassifier);  //ע�����Ҫ��д���ٵĺ���
	void Acry_Delete_ClassUnique(ClassUnique1 *classLick); //���Ψһֵ����	
private:
    float ** m_class_accuracy_matrix;
	int m_class_num;
	int m_classifier_num;
	int **m_errornum_in_evryclass;
	int *m_num_in_evryclass;
	ConfusionMatrix * m_confusionMatrix;
public:
	bool Arcy_Clear(void);
	
};

