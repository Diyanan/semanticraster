#ifdef CLASSTOR_EXPORTS
#define CLASSTOR_API __declspec(dllexport)
#else
#define CLASSTOR_API __declspec(dllimport)
#endif

#include "File.h"
#include <iostream>
#include "windows.h"

class CLASSTOR_API CClasstor {
public:
	void Classifier(const char* pathTrian,const char* pathTest,const char* pathOutput ,int method);
	void getchangeDTCfile(const char* m_testfile_path,const char* m_trainfile_path,const char* m_trainupdatafile_path,int m_num,int testNUM);
};

static void print_string_stdout_temp(const char *s)
{
	fputs(s,stdout);
	fflush(stdout);
}
void (*changeDEC_print_string) (const char *) = &print_string_stdout_temp;
#if 1
static void info_changeDec(const char *fmt,...)
{
	char buf[BUFSIZ];
	va_list ap;
	va_start(ap,fmt);
	vsprintf(buf,fmt,ap);
	va_end(ap);
	(*changeDEC_print_string)(buf);
}
#else
static void info_changeDec(const char *fmt,...) {}
#endif

extern CLASSTOR_API int nClasstor;

CLASSTOR_API int fnClasstor(void);
CLASSTOR_API double test_ANNClassify(const char* pathTrian,const char* pathTest,const char* pathOutput,int hidnum);
CLASSTOR_API void test_BayesClassify(const char* pathTrian,const char* pathTest,const char* pathOutput);
CLASSTOR_API void test_AdTreeClassify(const char* pathTrian,const char* pathTest,const char* pathOutput,int adK);
CLASSTOR_API void test_SVMClassify(const char* pathTrian,const char* pathTest,const char* pathOutput);
CLASSTOR_API void test_KNNClassify(const char* pathTrian,const char* pathTest,const char* pathOutput);
CLASSTOR_API void test_TreeClassify(const char* pathTrian,const char* pathTest,const char* pathOutput);
CLASSTOR_API void testJC1(const char* pathTrian,const char* pathTest,const char* pathOutput);
CLASSTOR_API void testJC2(const char* pathTrian,const char* pathTest,const char* pathOutput);