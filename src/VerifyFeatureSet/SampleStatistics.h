#pragma once

namespace VerifyFeatureSet
{
	struct SampleStatistics
	{
		double max;
		double min;
		double mean;
		double stddev;
		double variance;
		int number;
		//bool hasHistogram;
		bool isValid;
		int buckets;
		float* histogram;//��ŵ��ǹ�һ�����ֱ��ͼ
	};
}