#include "RISC_Classifier.h"
#include "RISC_Classifier_ANN.h"
#include "RISC_Classifier_Bayes.h"

#include "RISC_Classifier_Tree.h";
#include "RISC_Classifier_KNN.h";
#include "RISC_Classifier_JC1.h";
#include "RISC_Classifier_JC2.h";
#include "RISC_Classifier_AdTree.h"
#include "RISC_Classifier_SVM.h"

/************************************************************************/
/* ����ӿ�                                                             */
/************************************************************************/
RISC_Classifier_ANN * ann;
RISC_Classifier_Bayes *bayes;

RISC_Classifier_Tree *tree;

RISC_Classifier_KNN *knn;
RISC_Classifier_JC1 *jc1;
RISC_Classifier_JC2 *jc2;
RISC_Classifier_AdTree *adTree;
RISC_Classifier_SVM *svm;


GLC_EXP RISC_Classifier_ANN * NewClassifier_ANN();
GLC_EXP RISC_Classifier_Bayes * NewClassifier_Bayes();
GLC_EXP RISC_Classifier_Tree *NewClassifier_Tree();
GLC_EXP RISC_Classifier_KNN  *NewClassifier_KNN();
GLC_EXP RISC_Classifier_SVM  *NewClassifier_SVM();
GLC_EXP RISC_Classifier_JC1  *NewClassifier_JC1();
GLC_EXP RISC_Classifier_JC2  *NewClassifier_JC2();
GLC_EXP RISC_Classifier_AdTree  *NewClassifier_AdTree();


GLC_EXP int DestroyModel(RISC_Classifier * classifier);

/************************************************************************/
/* �������۽ӿ�                                                         */
/************************************************************************/
GLC_Class_Accuracy * accuracy;
int nClassifer=0;//�������ĸ���
int maxNClassifier=0;//������������
int nClass=0;//���ĸ���
int testDataRow=0;//����������ݵĸ���
short **testIDS=NULL;//�洢���Է�����
int *testTags=NULL;

GLC_EXP void SetClassfyTag(int *testTag,int nTest/*�������ݵĸ���*/,int classifierCount/*���÷�������������*/);                      //���÷��������ǩ
GLC_EXP void AddClassifiedResult(short *classifiedID);     //��ӷ������ķ�����
GLC_EXP int	CreateAccuracy();                              
GLC_EXP int GetClassCount();                               //������ĸ���
GLC_EXP int GetClassifierCount();                          //��÷������ĸ���
GLC_EXP void GetClassifierCofusionMatrix_1D(int classifierIndex/*����������*/,int* confusionMatrix_1d);//�������������Ļ�������ת����һά���鵼��Ԫ�ظ���ΪnClass*nClass
GLC_EXP double GetKappa();//���㿨��ϵ��
GLC_EXP GLC_Class_Accuracy * GetAccuracyModel();//��ȡԭʼ��ģ��

GLC_EXP int DestroyAccuracy();

void AccuracyAccessment(string testTagsPath, string testIDSPath, int nClass, int datarow);