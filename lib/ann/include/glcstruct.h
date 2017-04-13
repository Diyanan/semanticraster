#include "ClassifiedData.h";
#include <vector>
using std::vector;
//struct ClassUnique1//用于记录属性值及其在样本数据中的行索引
//{  
//
//	int classID;        //记录类标号属性
//	int cunt;
//	vector<int> dataIndex;//记录该类在类标签数组中的索引
//
//	ClassUnique1 * next;
//};
//struct AttValuesTab//用于记录属性值及其在样本数据中的行索引
//{  
//	double AttValue;
//	int classID;        //记录类标号属性
//	int AttValueIndex;
//};
//struct ConfusionMatrix//用于记录混淆矩阵
//{  
//	int classifierID;
//	int nClass;     //类别个数
//	int **matrix; //混淆矩阵
//	float **acryMatrix;//分类器识别性能矩阵
//};


//typedef struct AttIndexTab  //用于记录属性索引及其对应的熵值，信息增益，信息增益率
//{ 
//	int attIndex;    
//	double attInf;
//	double attGain;
//	double attGainRatio;
//	double splittPoint;
//	int D1,D2;         
//
//};
//typedef struct InfoAttTab
//{ 
//	double Info;        
//	double Weight;
//
//};
//typedef struct ClassUnique
//{  
//
//	int classID;        
//	int cunt;
//	ClassUnique * next;
//};
//typedef struct AttValuesTab
//{  
//	double AttValue;
//	int classID;       
//	int AttValueIndex;
//};
//typedef struct statisticsTab
//{
//	int valueID;
//	int valueCount;
//	double Proportion;
//};
//typedef struct ClassifiedResutEveryTime  //存储分类后元组信息，可以存在于叶子节点上
//{ 
//
//	int tuplesClassID;   //元组的类别	
//	double pre;//预测权重
//	bool isvotied;
//
//};
//typedef struct ResutEveryTime  //存储分类后元组信息，可以存在于叶子节点上
//{ 
//	int tuplesClassID;   //元组的类别	
//	double pre;//预测权重
//	bool isProseed;
//};