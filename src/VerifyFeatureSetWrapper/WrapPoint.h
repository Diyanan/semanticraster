#pragma once

namespace VerifyFeatureSetWrapper
{
    public value struct WrapPoint
    {
	public:
		int x;
        int y;

	public:
		WrapPoint(int m_x, int m_y)//m_x��ʾ�����������ڼ��У�m_y��ʾ�����������ڼ���
        {
            x = m_x;
            y = m_y;//
        }
    };
}