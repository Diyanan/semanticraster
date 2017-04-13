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
/* 分类接口                                                             */
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
/* 精度评价接口                                                         */
/************************************************************************/
GLC_Class_Accuracy * accuracy;
int nClassifer=0;//分类器的个数
int maxNClassifier=0;//分类器最大个数
int nClass=0;//类别的个数
int testDataRow=0;//参与测试数据的个数
short **testIDS=NULL;//存储测试分类结果
int *testTags=NULL;

GLC_EXP void SetClassfyTag(int *testTag,int nTest/*测试数据的个数*/,int classifierCount/*设置分类器的最多个数*/);                      //设置分类的类别标签
GLC_EXP void AddClassifiedResult(short *classifiedID);     //添加分类器的分类结果
GLC_EXP int	CreateAccuracy();                              
GLC_EXP int GetClassCount();                               //获得类别的个数
GLC_EXP int GetClassifierCount();                          //获得分类器的个数
GLC_EXP void GetClassifierCofusionMatrix_1D(int classifierIndex/*分类器索引*/,int* confusionMatrix_1d);//将当个分类器的混淆矩阵转换成一维数组导出元素个数为nClass*nClass
GLC_EXP double GetKappa();//计算卡巴系数
GLC_EXP GLC_Class_Accuracy * GetAccuracyModel();//获取原始类模型

GLC_EXP int DestroyAccuracy();

void AccuracyAccessment(string testTagsPath, string testIDSPath, int nClass, int datarow);