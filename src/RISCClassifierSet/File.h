// File.h: interface for the File class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILE_H__47BA9F3C_2040_4B76_A7C8_9690698A5C70__INCLUDED_)
#define AFX_FILE_H__47BA9F3C_2040_4B76_A7C8_9690698A5C70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _declspec(dllexport) File  
{
public:
	
	File();
	float ** get_data_float();
	double** get_data_double();
	void get_data_row_line(int &row,int & line);
	File(const char* file_path);
	virtual ~File();
private:
	int m_line;
	int m_row;
	const char* m_file_path;

};

#endif // !defined(AFX_FILE_H__47BA9F3C_2040_4B76_A7C8_9690698A5C70__INCLUDED_)
