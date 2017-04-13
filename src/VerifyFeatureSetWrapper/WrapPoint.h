#pragma once

namespace VerifyFeatureSetWrapper
{
    public value struct WrapPoint
    {
	public:
		int x;
        int y;

	public:
		WrapPoint(int m_x, int m_y)//m_x表示列索引，即第几列，m_y表示行索引，即第几行
        {
            x = m_x;
            y = m_y;//
        }
    };
}