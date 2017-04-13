//#include <vector>
//using std::vector;
#pragma once
#include "GLC_Classifier.h"

//struct ClassUnique1//用于记录属性值及其在样本数据中的行索引
//{  
//
//	int classID;        //记录类标号属性
//	int cunt;
//	vector<int> dataIndex;//记录该类在类标签数组中的索引
//	
//	ClassUnique1 * next;
//};
// struct AttValuesTab//用于记录属性值及其在样本数据中的行索引
//{  
//	double AttValue;
//	int classID;        //记录类标号属性
//	int AttValueIndex;
//};
// struct ConfusionMatrix//用于记录混淆矩阵
//{  
//	int classifierID;
//	int nClass;     //类别个数
//	int **matrix; //混淆矩阵
//	float **acryMatrix;//分类器识别性能矩阵
//};
class GLC_Class_Accuracy
{
public:
	
	GLC_Class_Accuracy(void);
	~GLC_Class_Accuracy(void);
	void Acry_Set_ClassAcryMatrix(int*data_tag/*类标签*/,int data_row/*数据行数*/,short **class_id/*分类之后的类标号*/,int& class_num/*类个数*/,int classifier_num=3/*分类器个数*/);
	void  Acry_Set_ClassConfusionMatrix(int*data_tag/*类标签*/,int data_row/*数据行数*/,short **class_id/*分类之后的类标号*/,int& class_num/*类个数*/,int classifier_num=3/*分类器个数*/); //混淆矩阵
	float**  GLC_Class_Accuracy::Acry_Get_ClassAcryMatrix();//获得类别精度
	void  Acry_Delete_ClassAcryMatrix();
	int ** Acry_Get_ClassErrMatrix();
	int *   Acry_Get_ClassNumMatrix();
	ConfusionMatrix *  Acry_Get_ClassConfusionMatrix();//获得混淆矩阵
	ConfusionMatrix *   Acry_Get_ClassConfusionAcfyMatrix();//获得分类器识别性能矩阵（混淆矩阵精度）
	
	ConfusionMatrix* Arcy_Get_ClassConfidenceMatrix(void);// 获得信任度矩阵
private:
	ClassUnique1 *Acry_Get_ClassUnique(AttValuesTab * attValues, int attNum) ;
	ClassUnique1 *  Acry_Get_ClassUnique(AttValuesTab * attValues, int attNum,short**classfiedID,int nClassifier);  //注意最后还要编写销毁的函数
	void Acry_Delete_ClassUnique(ClassUnique1 *classLick); //清除唯一值链表	
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

