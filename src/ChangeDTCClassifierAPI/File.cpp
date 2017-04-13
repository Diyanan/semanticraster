// File.cpp: implementation of the File class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "File.h"

#include <fstream>
#include <string>
#include<iostream>
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

File::File()
{

}
File::File(const char* file_path)
{
   //	system("pause");
	try
	{
		int row = 0;
		int line = 0;
		ifstream fin(file_path);
		string str;
		fin>>str;
		int lenth=str.length();
		for (int i=0;i<lenth;i++)
		{
			if (str[i]==',')
			{
				line++;
				//system("pause");
			}
		}
		line=line+1;
		m_line=line;
		
		while( !fin.eof()) 
		{   
			
			fin>>str;
			if(str.length() > 0)
			{
				row++;
			}
		}
		m_row=row;
	    m_file_path=file_path;
	}
	catch (...)
	{
		return ;
	}

	
	

}
File::~File()
{

}

void File::get_data_row_line(int &row, int &line)
{
	row = m_row;
	line = m_line;

}

double** File::get_data_double()
{

	ifstream fin(m_file_path);
	double ** data;
	data=new double *[m_row];
	string str_temp ="";
	for (int i=0;i<m_row;i++)
	{
		data[i]=new double [m_line];
		int lineI=0;
		string str,tempstr;
		int tagI=0,cutI=0;		
		fin>>str;
		tempstr=str;
		for (int j=0;j<str.length();j++)
		{
			cutI++;
			
			if (str[j]==',')
			{				
				string str_data=str.substr(tagI,cutI-1);
				tagI=j+1;
				cutI=0;
				//str=str.erase(0,j+1);
				
				double temp= atof(str_data.c_str());						
				data[i][lineI]=temp;
				//cout<<data[i][lineI]<<" ";
				lineI++;
			}

			if (j==str.length()-1)
			{
				string str_data=str.substr(j-cutI+1,cutI+1);
				
				double temp= atof(str_data.c_str());			
				data[i][lineI]=temp;
				//cout<<data[i][lineI]<<" ";
				lineI++;
			}
			//cout<<endl;
			
		}
		//cout<<".";
		//cout<<endl;
	}
	fin.close();
return data;
}

float ** File::get_data_float()
{

	ifstream fin(m_file_path);
	float ** data;
	data=new float *[m_row];
	for (int i=0;i<m_row;i++)
	{
		data[i]=new float [m_line];
		int lineI=0;
		string str,tempstr;
		int tagI=0,cutI=0;		
		fin>>str;
		tempstr=str;
		for (int j=0;j<str.length();j++)
		{
			cutI++;
			
			if (str[j]==',')
			{				
				string str_data=str.substr(tagI,cutI-1);
				tagI=j+1;
				cutI=0;
				//str=str.erase(0,j+1);
				
				double temp= atof(str_data.c_str());						
				data[i][lineI]=temp;
				//cout<<data[i][lineI]<<" ";
				lineI++;
			}
			if (j==str.length()-1)
			{
				string str_data=str.substr(j-cutI+1,cutI+1);
				
				double temp= atof(str_data.c_str());			
				data[i][lineI]=temp;
				//cout<<data[i][lineI]<<" ";
				lineI++;
			}
			
		}

		//cout<<endl;
	}
	//cout<<endl;
	fin.close();
	return data;
}
